#include <stdio.h>
#include "cliente.h"
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
#define TAM_REQ 256
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

void lerEntradaUsuario(char* entrada, int tamanho){
  printf("Digite a String de Entrada: \n");
  fgets(entrada, tamanho, stdin);
}

void iniciaConexao(int* socketFD, char* ipServidor, unsigned short int portaServidor){
  *socketFD = socket(PF_INET, SOCK_STREAM, 0);
  if(*socketFD == -1){
    perror("Erro ao alocar socket");
    exit(1);
  }

  struct sockaddr_in endereco;
  endereco.sin_family = PF_INET;
  endereco.sin_port = portaServidor;
  endereco.sin_addr.s_addr = inet_addr(ipServidor);
  memset(&(endereco.sin_zero), 0, 8);



  if(connect(*socketFD, (struct sockaddr *)&endereco, sizeof(struct sockaddr_in)) == -1){
    perror("Erro ao conectar");
    exit(1);
  }
}

char *trim(char *str){
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  *(end+1) = 0;

  return str;
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
    default:
      strcpy(resultado, "mul");
  }
  *posicao = contador;
}

int getNumeroInteiro(char* entrada, int* posicao){
  char valorTemp[TAM_ENTRADA_USUARIO];
  int contadorValorTemp = 0;
  int contador = *posicao;

  while(!isdigit(entrada[contador])){
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
      contador++;
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
  iniciaConexao(&socketFD, ipServidor, portaServidor);
  lerEntradaUsuario(entrada, tamanho);
  criarMensagem(entrada, tamanho, socketFD, &mensagem);
  enviaRequisicao(mensagem);

  return 0;
}
