#include <math.h>
#define ENTRADAS 2
#define SAIDAS 2
#define NR_NEURON_O 6
const int motor_1_a   = 3;
const int motor_2_b   = 9;
const int sensor1      = 13;
const int sensor2       = 12;
const int SENSOR_MAX  = 300;
const int SENSOR_MIN  = 10;


 double w_e_o[ENTRADAS + 1][NR_NEURON_O] = {{-3.504251,-7.872451,4.008148,-2.902241, -7.872451, 9.299938},
                                            {-13.114737, 16.781754, -1.924973, -0.543827,16.781754, -17.724171 },
                                            {11.238211, 5.449191,-19.038139, 2.076796, 5.449191, -19.158763 }};
double w_o_s[NR_NEURON_O+1][SAIDAS] = {{-4.104383,-5.232040},
                                       { -2.632021, -12.939634}, 
                                       {-0.118213,7.010443},
                                       {-1.687591, -4.802265},
                                       {35.181601,-1.416810},
                                       {-0.118213,7.010443},
                                       {0.676688,7.551531}};

int vel1 = 0;
int vel2 = 0;
double saida_s[SAIDAS];
double entradas[ENTRADAS];
double sensor[ENTRADAS];
double saida_o;
double w_o_o;
int distancia(int sensor);
double f_sigmoid(double net);
void calcar_saidas(double entradas[ENTRADAS]);

void setup()
{
  Serial.begin(9600);
  pinMode(motor_1_a, OUTPUT);
  pinMode(motor_2_b, OUTPUT);
  
}

void loop()
{
  sensor[0] = distancia(sensor1);
  sensor[1] = distancia(sensor2);
  Serial.print("Distancia s1: ");
  Serial.print(sensor[0]);
  Serial.print("  -  Distancia s2: ");
  Serial.print(sensor[1]);
  calcular_saidas(sensor);
  vel1 = saida_s[0];
  vel2 = saida_s[1];
  Serial.print("Velocidade m1: ");
  Serial.print(vel1);
  Serial.print("  -  Velocidade m2: ");
  Serial.print(vel2);
  acelerar_motor(vel1, vel2);
  delay(300);
 /* if (distancia() > 200) acelerar_motor_1(200);
  else if (distancia() <= SENSOR_MIN) parar_motor_1();
    */

}

void calcular_saidas(double entradas[ENTRADAS])
{
  int i, j;

  for (i = 0; i < NR_NEURON_O; i++)
  {
    saida_o[i][0] = 0.0;
    saida_o[i][0] += w_e_o[0][i] * 1;

    for (j = 1; j < ENTRADAS + 1; j++)
      saida_o[i][0] += w_e_o[j][i] * entradas[j - 1];

    saida_o[i][0] = f_sigmoid(saida_o[i][0]);
  }

  for (i = 0; i < NR_NEURON_O; i++)
  {
    saida_o[i][1] = 0.0;
    saida_o[i][1] += w_o_o[0][i] * 1;

    for (j = 1; j < NR_NEURON_O + 1; j++)
      saida_o[i][1] += w_o_o[j][i] * saida_o[j - 1][0];

    saida_o[i][1] = f_sigmoid(saida_o[i][1]);
  }

  for (i = 0; i < SAIDAS; i++)
  {
    saida_s[i] = 0.0;
    saida_s[i] += w_o_s[0][i] * 1;

    for (j = 1; j < NR_NEURON_O + 1; j++)
      saida_s[i] += w_o_s[j][i] * saida_o[j - 1][1];

    saida_s[i] = f_sigmoid(saida_s[i]);
  }
}

double f_sigmoid(double net)
{
  return 1 / (1 + exp(-net));
}

int distancia(int sensor) 
{
  int tempo;
  
  // Emite sinal
  pinMode(sensor, OUTPUT);
  digitalWrite(sensor, HIGH);
  delayMicroseconds(100);
  digitalWrite(sensor, LOW);
  // Configura sensor para receber sinal
  pinMode(sensor, INPUT);
  tempo = pulseIn(sensor, HIGH);
  return tempo / 2 / 29.4;
}

void parar_motor_1()
{
  digitalWrite(motor_1_a, LOW);
  digitalWrite(motor_2_b, LOW);  
}

void acelerar_motor(int vel1, int vel2)
{
  analogWrite(motor_1_a, vel1);
  analogWrite(motor_2_b, vel2);
}