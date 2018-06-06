#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ENTRADAS        2
#define SAIDAS          2
#define NR_AMOSTRAS     20
#define NR_NEURON_O     6
#define EPOCAS          100000
#define TX_APRENDIZADO  1

													//  esq 	 dir	 esq 	dir
double cj_treinamento[NR_AMOSTRAS][ENTRADAS+SAIDAS]= {	{315,	315,	255,	255},
                                                        {18,	315,	210,	10},
                                                        {315,	18,		10,		210},
                                                        {315,	265,	80,		230},
                                                        {265,	315,	230,	80},
                                                        {130,	130,	100,	150},
                                                        {25,	315,	170,	15},
                                                        {315,	25,		15,		170},
                                                        {150,	315,	255,	100},
                                                        {315,	150,	100,	255},
                                                        {245,	265,	180,	150},
                                                        {265,	245,	150,	180},
                                                        {15,  15,   0,    255},
                                                        {35,  15,   20,   255},
                                                        {75,  50,   15,   240},
                                                        {10,  50,   230,  76},
                                                        {25,  60,   200,  50},
                                                        {15,  50,   255,  70},
                                                        {50,  150,  200,  30},
                                                        {140, 130,  100,  150}};


double a;
double b;
double w_e_o[ENTRADAS+1][NR_NEURON_O];
double w_o_s[NR_NEURON_O+1][SAIDAS];
double saida_o[NR_NEURON_O];
double saida_s[SAIDAS];
double delta_saida[SAIDAS];
double gradiente_oculta[NR_NEURON_O];
double delta_oculta[NR_NEURON_O];
double media_erro = 0.0;


void inicializa_sinapses();
int gera_nr_aleatorios();
void mostrar_sinapses();
double f_sigmoid(double net);
void calcular_saidas(double entradas[ENTRADAS]);
void treinar_RNA();
double calcular_erro(double desejado, double saida);
void menu();
void calcular_delta_saida(double desejado, double desejado2);
void calcular_delta_oculta();
void calcular_gradiente_oculta();
void ajustar_pesos_sinapticos(double entradas[ENTRADAS]);
void gravar_pesos_sinapticos();
void restaurar_pesos_sinapticos();


int main()
{
  srand(time(NULL));

  while (1) {

    menu();

  }

  return 0;
}

void inicializa_sinapses()
{
  int i, j;

  for (i = 0; i < ENTRADAS+1; i++) {
    for (j =0; j < NR_NEURON_O; j++) {

     //w_e_o[i][j] = 0.0;
     w_e_o[i][j] = gera_nr_aleatorios();
    }
  }


  for (i = 0; i < NR_NEURON_O+1; i++) {
    for (j =0; j < SAIDAS; j++) {

      //w_o_s[i][j] = 0.0;
      w_o_s[i][j] = gera_nr_aleatorios();
    }
  }

}

int gera_nr_aleatorios()
{
  int numeros[2] = {-1, 1};
  // Retorna -1 ou 1
  return (numeros[rand() % 2]);
}

void mostrar_sinapses()
{
  int i, j;
  FILE *f=fopen("file.txt","w");

  if(f == NULL){
    printf("Error opening file\n");
    return;
  }

  // Inicializa pesos sin�pticos da entrada para a camada oculta
  for (i = 0; i < ENTRADAS+1; i++) {
    for (j =0; j < NR_NEURON_O; j++) {
      printf("w_e_o[%d][%d] = %f; ", i, j, w_e_o[i][j]);
      fprintf(f, "w_e_o[%d][%d] = %f; \n", i, j, w_e_o[i][j]);
    }
    printf("\n");
  }

  // Inicializa pesos sin�pticos da camada oculta para a sa�da
  for (i = 0; i < NR_NEURON_O+1; i++) {
    for (j =0; j < SAIDAS; j++) {
      printf("w_o_s[%d][%d] = %f; ", i, j, w_o_s[i][j]);
      fprintf(f, "w_o_s[%d][%d] = %f; \n", i, j, w_o_s[i][j]);
    }
    printf("\n");

  }
  fclose(f);
}

double f_sigmoid(double net)
{
  return 1 / (1 + exp(-net));
}

void calcular_saidas(double entradas[ENTRADAS])
{
  int i, j;

  // Calcular os nets da entrada para a camada oculta
  for (i = 0; i < NR_NEURON_O; i++) {
    saida_o[i] = 0.0;
    saida_o[i] += w_e_o[0][i] * 1;  // Calcula saida para bias

    for (j = 1; j < ENTRADAS+1; j++) {
     saida_o[i] += w_e_o[j][i] * entradas[j-1];
    }

    // Calcular a saida de saida_o[i]
    saida_o[i] = f_sigmoid(saida_o[i]);
  }

  // Calcular os nets da camada oculta para a sa�da
  for (i = 0; i < SAIDAS; i++) {
    saida_s[i] = 0.0;
    saida_s[i] += w_o_s[0][i] * 1;  // Calcula saida para bias

    for (j = 1; j < NR_NEURON_O+1; j++) {
      saida_s[i] += w_o_s[j][i] * saida_o[j-1];
    }

    saida_s[i] = f_sigmoid(saida_s[i]);
  }

}

void treinar_RNA()
{
  int i, j;
  double entradas[ENTRADAS];

  for (i = 1; i <= EPOCAS; i++) {

    for (j = 0; j < NR_AMOSTRAS; j++) {
      entradas[0] = cj_treinamento[j][0]/1000;
      entradas[1] = cj_treinamento[j][1]/1000;

      // Feedforward
      calcular_saidas(entradas);

      // Backward (backpropagation)
      calcular_delta_saida(cj_treinamento[j][2]/1000,cj_treinamento[j][3]/1000);
      calcular_gradiente_oculta();
      calcular_delta_oculta();
      ajustar_pesos_sinapticos(entradas);
    }

  }

  // Mostra media dos erros
  printf("RNA TREINADA - Media dos erros: %lf\n", media_erro);
}

double calcular_erro(double desejado, double saida)
{
  return desejado - saida;
}

void menu()
{
  int opcao;
  double entradas[ENTRADAS];

  printf("Rede Neural Perceptron de Multiplas Camadas\n");
  printf("Problema do Carrinho\n");
  printf("*******************************************\n\n");
  printf("1.Treinar a rede\n");
  printf("2.Usar a rede\n");
  printf("3.Ver pesos sinapticos\n");
  printf("4.Sair\n");
  printf("Opcao? ");
  scanf("%d", &opcao);

  switch (opcao) {

    case 1: inicializa_sinapses();
            treinar_RNA();
            break;

    case 2: printf("Entrada 1: ");
            scanf("%lf", &a);
            entradas[0] = a/1000;
            printf("Entrada 2: ");
            scanf("%lf", &b);
            entradas[1] = b/1000;
            calcular_saidas(entradas);
            printf("\nResposta da RNA: %lf\n\n\n", saida_s[0]*1000);
            printf("\nResposta da RNA: %lf\n\n\n", saida_s[1]*1000);
            break;

    case 3: mostrar_sinapses();
            break;

    case 4: exit(0);

  }
}

void calcular_delta_saida(double desejado, double desejado2)
{

    delta_saida[0] = calcular_erro(desejado, saida_s[0]) * (1 - saida_s[0] * saida_s[0]);
    delta_saida[1] = calcular_erro(desejado2, saida_s[1]) * (1 - saida_s[1] * saida_s[1]);
}

void calcular_gradiente_oculta()
{
  int i, j;

  for (i = 0; i < SAIDAS; i++) {
    for (j = 1; j < NR_NEURON_O+1; j++) {
      gradiente_oculta[j-1] = delta_saida[i] * w_o_s[j][i];
    }
  }
}

void calcular_delta_oculta()
{
  int i;
  for (i = 0; i < NR_NEURON_O; i++)
    delta_oculta[i] = gradiente_oculta[i] * saida_o[i] * (1 - saida_o[i]);
}

void ajustar_pesos_sinapticos(double entradas[ENTRADAS])
{
  int i, j;

  // Ajusta os pesos sin�pticos da camada intermedi�ria para a camada de sa�da
  for (i = 0; i < SAIDAS; i++) {
    w_o_s[0][i] = w_o_s[0][i] + TX_APRENDIZADO * delta_saida[i] * 1;

    for (j = 1; j < NR_NEURON_O+1; j++) {
      w_o_s[j][i] = w_o_s[j][i] + TX_APRENDIZADO * delta_saida[i] * saida_o[j-1];
    }

  }

  // Ajusta os pesos sin�pticos da camada de saida para a camada intermedi�ria
  for (i = 0; i < NR_NEURON_O; i++) {
    w_e_o[0][i] = w_e_o[0][i] + TX_APRENDIZADO * delta_oculta[i] * 1;

    for (j = 1; j < ENTRADAS+1; j++) {
     w_e_o[j][i] = w_e_o[j][i] + TX_APRENDIZADO * delta_oculta[i] * entradas[j-1];
    }
  }

}
