//Perceptron é uma rede neural de uma camada com duas separacao, soma e transferência, cada neuronio pode ter N entradas mas apenas uma saída
//Regra de propagacao FeedForward
//ALgoritmo de treinamento do perceptron é a regra delta
//w(t+1) = w(t) + erro*taxa_aprendizado*entrada
//erro = desejado - saída
// w[0] = bias
// w[1] = x1
// w[2] = x2

#include <stdio.h>
#include <stdlib.h>

#define EPOCAS		10000
#define L 			4
#define C 			3
#define TX_APR 		0.7
#define BIAS 			1
#define ENTRADA 		2
#define SAIDA 			1
#define QTD_AMOSTRAS 	4

int dados_treinamento[L][C] = {{1, 1, 1},
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 0}};

float w[ENTRADA+1];
int entradas[ENTRADA];

//Cabeçalho de funções auxiliares
int erro(int desejado, int saida);
int f_degrau(float net);
int menu();
void mostrar_w();
float calcular_net();
void gerar_valores_iniciais_w();
void treinar_rna();
void ajustar_peso_sinaptico(int erro);

//Função Principal
int main()
{
	int opc;
	srand(time(NULL));
	gerar_valores_iniciais_w();

	do {
		opc = menu();

		switch(opc)
		{
			case 1: printf("Treinando RNA . . .\n");
				treinar_rna();
				printf("RNA treinada . . . \n\n");
				break;

			case 2: printf("Entrada 1: ");
				scanf("%d", &entradas[0]);
				printf("Entrada 2: ");
				scanf("%d", &entradas[1]);
				printf("\n***************\n");
				printf("Saida da RNA: %i\n", f_degrau(calcular_net()));
				printf("***************\n\n\n");
				break;

			case 3: mostrar_w();
				break;

			case 4: break;
		}

	} while(opc != 5);

	return 0;
}
//
int menu()
{
    int opc;
	printf("1 - Treinar a RNA\n");
	printf("2 - Usar a RNA\n");
	printf("3 - Mostrar pesos sinapticos\n");
	printf("4 - Salvar pesos sinapticos em arquivo .txt\n");
	printf("5 - Sair\n");
	printf("Opçao? ");
	scanf("%d", &opc);

	return opc;
}


//Implementação de funções auxiliares

void mostrar_w()
{
	int i;
	for (i = 0; i < ENTRADA+1; i++)
		printf("w[%d]: %.2f\n", i, w[i]);
}

float calcular_net()
{
	int i;
	float net = 0.0;
	//Calcular bias
	net += w[0] * BIAS;

	//Calcular para as entradas

	for ( i = 1; i < ENTRADA+1; i++){
		net += w[i] * entradas[i-1];
	}

	return net;
}

int erro(int desejado, int saida)
{
	return desejado - saida;
}

int f_degrau(float net)
{
	if(net > 0) return 1;
	return 0;
}


void gerar_valores_iniciais_w()
{
	int i;
	for ( i = 0; i < ENTRADA+1; i++){
		w[i] = (float)(rand() % 10) / 100;
	}
}

void treinar_rna()
{
	int epoca = 1, i, resposta, saida_erro;

	while ( epoca <= EPOCAS) {
		for( i = 0; i < QTD_AMOSTRAS; i++){
			entradas[0] = dados_treinamento[i][0];
			entradas[1] = dados_treinamento[i][1];
			resposta = f_degrau(calcular_net());
			saida_erro = erro(dados_treinamento[i][2], resposta);
			printf("Erro: %d\n", saida_erro);
			if (saida_erro != 0){
				ajustar_peso_sinaptico(saida_erro);
			}
		}
		epoca++;
	}
}

void ajustar_peso_sinaptico(int erro)
{
	int i;

	//Ajusta peso de bias

	w[0] = w[0] + TX_APR * erro * BIAS;

	for ( i = 0; i < ENTRADA; i++){
		//Regra delta para as demais conexões sinapticas
		w[i+1] = w[i+1] + TX_APR * erro * entradas[i];
	}
}
