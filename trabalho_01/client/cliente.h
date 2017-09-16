
#ifndef CLIENTE_H
#define CLIENTE_H
#define OPP_LEN 3

typedef struct {
  char operacao[OPP_LEN];
  int primeiroNumero;
  int segundoNumero;
  int socketFD;
} mensagem_t;

void enviaRequisicao(mensagem_t requisicao);
void lerEntradaUsuario(char* entrada, int tamanho);
void iniciaConexao(int* socketFD, char* ipServidor, unsigned short int portaServidor);
void criarMensagem(char* entrada, int tamanho, int socketFD, mensagem_t* mensagem);

#endif //CLIENTE_H
