#include <stdio.h>
#include "client.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define TAM_ENTRADA_USUARIO 256
#define SERVER_DEFAULT_IP "127.0.0.1"
#define SERVER_DEFAULT_PORT 12000

#define DEBUG 0

void enviaRequisicao(mensagem_t requisicao){
  char req[TAM_REQ];
  strcpy(req, requisicao.operacao);
  strcat(req, " ");
  /*char *req_aux = req + strlen(req);
  memcpy(req_aux, requisicao.primeiroNumero, sizeof(int));
  strcpy(req_aux + sizeof(int), " ");
  memcpy(req + strlen(req), requisicao.segundoNumero, sizeof(int));*/
  sprintf(req + strlen(req), "%d", requisicao.primeiroNumero);
  strcat(req, " ");
  sprintf(req + strlen(req), "%d", requisicao.segundoNumero);

#if DEBUG
  int i;
  for (i = 0; i < strlen(req); i++){
    printf("%c", req[i]);
  }
#else
int i;
for (i = 0; i < strlen(req); i++){
  printf("%c", req[i]);
}
printf("\n");

  int totalBytesEnviar = strlen(req);
  int bytesEnviados = 0;
  while(bytesEnviados < totalBytesEnviar){
    char* ptrMensagem = req + bytesEnviados;
    int n = send(requisicao.socketFD, ptrMensagem, totalBytesEnviar - bytesEnviados, 0);

    if (n == -1){
      perror("Erro ao enviar");
      exit(1);
    }

    bytesEnviados += n;
  }
#endif
}

void validaEntrada(char* entrada, int tamanho){
  int quantidadeDeOperadores = 0;
  int encontrouCaracter = 0;
  int contador = 0;
  do{
    fgets(entrada, tamanho, stdin);
    encontrouCaracter = 0;
    quantidadeDeOperadores = 0;
    for (contador = 0; entrada[contador] != 0; contador++){
      if (entrada[contador] == '+' || entrada[contador] == '-' || entrada[contador] == '*' || entrada[contador] == '/'){
        quantidadeDeOperadores++;
        if (quantidadeDeOperadores > 1) {
          printf("Entrada inválida! A entrada possui mais de um operador!\n");
          printf("Modo de entrada: <primeiro numero>  <operador>  <segundo numero>\n");
          printf("Digite novamente:\n");
        }
      }
      if(isalpha(entrada[contador])){
        encontrouCaracter = 1;
        printf("Entrada inválida! A entrada possui uma letra\n");
        printf("Modo de entrada: <primeiro numero>  <operador>  <segundo numero>\n");
        printf("Digite novamente:\n");
      }
    }
  }while(quantidadeDeOperadores != 1 || encontrouCaracter != 0);
}

void lerEntradaUsuario(char* entrada, int tamanho){
  printf("Digite a String de Entrada: \n");
  validaEntrada(entrada, tamanho);
}


void iniciaConexao(int* socketFD, char* ipServidor, unsigned short int portaServidor){
  *socketFD = socket(PF_INET, SOCK_STREAM, 0);
  if(*socketFD == -1){
    perror("Erro ao alocar socket");
    exit(1);
  }

  struct sockaddr_in endereco;
  endereco.sin_family = PF_INET;
  endereco.sin_port = htons(portaServidor);
  endereco.sin_addr.s_addr = inet_addr(ipServidor);
  memset(&(endereco.sin_zero), 0, 8);


  if(connect(*socketFD, (struct sockaddr *)&endereco, sizeof(struct sockaddr_in)) == -1){
    perror("Erro ao conectar");
    exit(1);
  }
}

char *trim(char *palavra){
  char *fim;

  while(isspace((unsigned char)*palavra)) palavra++;

  if(*palavra == 0)
    return palavra;

  fim = palavra + strlen(palavra) - 1;
  while(fim > palavra && isspace((unsigned char)*fim)) fim--;

  *(fim+1) = 0;

  return palavra;
}

void getOperador(char* entrada, int* posicao, char* resultado){
  char operador;
  int contador = *posicao;

  for (; entrada[contador] != 0; contador++){
    if (entrada[contador] == '+' || entrada[contador] == '-' || entrada[contador] == '*' || entrada[contador] == '/') {
      operador = entrada[contador];
      contador++;
      break;
    }
  }

  switch (operador) {
    case '+':
      strcpy(resultado, "add");
      break;
    case '-':
      strcpy(resultado, "sub");
      break;
    case '/':
      strcpy(resultado, "div");
      break;
    case '*':
      strcpy(resultado, "mul");
      break;
    default:
      printf("Operador Inválido!");
      exit(1);
  }
  *posicao = contador;
}

int getNumeroInteiro(char* entrada, int* posicao){
  char valorTemp[TAM_ENTRADA_USUARIO];
  int contadorValorTemp = 0;
  int contador = *posicao;

  while(isdigit(entrada[contador]) == 0){
    if (entrada[contador] == 0) {
      perror("Entrada Inválida");
      exit(1);
    }
    contador++;
  }
  for(contadorValorTemp = 0; entrada[contador] != 0; contador++, contadorValorTemp++){

    if(isdigit(entrada[contador])){
      valorTemp[contadorValorTemp] = entrada[contador];
    }else{
      contadorValorTemp++;
      //contador++;
      break;
    }
  }
  valorTemp[contadorValorTemp] = 0;
  *posicao = contador;
  int resultado = atoi(valorTemp);
  return resultado;
}

void criarMensagem(char* entrada, int tamanho, int socketFD, mensagem_t* mensagem){
  entrada = trim(entrada);
  mensagem->socketFD = socketFD;
  int posicao = 0;
  mensagem->primeiroNumero = getNumeroInteiro(entrada, &posicao);
  getOperador(entrada, &posicao, mensagem->operacao);
  mensagem->segundoNumero = getNumeroInteiro(entrada, &posicao);
}

void formatarMensagem(char* mensagem, int* deuErro, int* resultado){
  char temp[2];
  temp[0] =  mensagem[0];
  temp[1] = '\0';
  *deuErro = atoi(temp);
  printf("%d\n", *deuErro);
  char resultadoTemp[TAM_REQ];
  memset(resultadoTemp, '\0', TAM_REQ);
  int contador, aux;
  for (contador = 2, aux = 0; mensagem[contador] != 0; contador++, aux++){
    resultadoTemp[aux] = mensagem[contador];
  }
  *resultado = atoi(resultadoTemp);
  printf("Aqui: %d\n", *resultado);
}

void imprimirResultado(int deuErro, int resultado){
  if (deuErro == 0){
    printf("\nNao houve nenhum erro!\n");
    printf("Resultado: %d\n", resultado);
  }else{
    printf("\nHouve um erro!\n");
    printf("Código do erro: %d\n", resultado);
  }
}

int main(int argc, char* argv[]){
  unsigned short int portaServidor;
  char *ipServidor;
  if(argc == 1){
    portaServidor = SERVER_DEFAULT_PORT;
    ipServidor = SERVER_DEFAULT_IP;
  } else if(argc == 2 || argc > 3 ){
    printf("Modo de Uso: \n");
    printf("%s <ip servidor> <porta servidor>\n",argv[0]);
  }else{
    portaServidor = atoi(argv[2]);
    ipServidor = argv[1];
  }

  mensagem_t mensagem;
  char entrada[TAM_ENTRADA_USUARIO];
  int tamanho = TAM_ENTRADA_USUARIO;
  int socketFD;
  lerEntradaUsuario(entrada, tamanho);
  iniciaConexao(&socketFD, ipServidor, portaServidor);
  criarMensagem(entrada, tamanho, socketFD, &mensagem);
  enviaRequisicao(mensagem);

  char *messageResult = malloc (TAM_REQ * sizeof(char));
  recv(mensagem.socketFD, messageResult, TAM_REQ, 0);
  printf("%s\n", messageResult);
  int deuErro, resultado;
  formatarMensagem(messageResult, &deuErro, &resultado);
  imprimirResultado(deuErro, resultado);

  return 0;
}
