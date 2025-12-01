#include <Arduino.h>
#include <DHT.h>
#include <EmonLib.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define pin_tensao A0
#define pin_fumaca 13
#define pin_magnetico 7
#define pin_ventoinha 10

EnergyMonitor emon1;
DHT dht(DHTPIN, DHTTYPE);

char codigo[] = "00000"; // codigo [temperatura, umidade, tensao, fumaça, magnetico] retorna 1 se algum sensor estive fora

double ler_temperatura();
double ler_umidade();
double ler_tensao();
int ler_fumaca();
int ler_magnetico();
void controle_ventoinha(int comando);

void setup() {
  Serial.begin(9600);
  dht.begin();
  emon1.voltage(pin_tensao, 270.0, 60.0);
  pinMode(DHTPIN, INPUT);
  pinMode(pin_tensao, INPUT);
  pinMode(pin_fumaca, INPUT);
  pinMode(pin_magnetico, INPUT);
  pinMode(pin_ventoinha, OUTPUT);
}

void loop() {
  //usaremos a função loop para ser a verificadora dos sensores
  double temp = ler_temperatura(); 
  double umidade = ler_umidade();
  double tensao = ler_tensao();
  int fumaca = ler_fumaca();
  int porta = ler_magnetico();

  if(temp > 30){
    codigo[0] = '1';
    controle_ventoinha(1);
  }else{
    codigo[0] = '0';
    controle_ventoinha(0);
  }

  if(umidade > 70.0 || umidade < 40){
    codigo[1] = '1';
  }else{
    codigo[1] = '0';
  }

  if(tensao<100.0 || tensao>140.0){
    codigo[2] = '1';
  }else{
    codigo[2] = '0';
  }

  if(fumaca){
    codigo[3] = '0';
  }else{
    codigo[3] = '1';
  }

  if(porta){
    codigo[4] = '0';
  }else{
    codigo[4] = '1';
  }

char temp_str[10];
char umidade_str[10];
char tensao_str[10];

dtostrf(temp, 4, 2, temp_str);
dtostrf(umidade, 4, 2, umidade_str);
dtostrf(tensao, 4, 2, tensao_str);

char json_code[100];
snprintf(
    json_code,
    sizeof(json_code),
    "\"{'code': %s, 'temperatura': %s, 'umidade': %s, 'tensao': %s, 'fumaca': %d, 'porta': %d }\"",
    codigo, 
    temp_str, 
    umidade_str, 
    tensao_str, 
    fumaca, 
    porta);
 
Serial.println(json_code);

delay(5000);
}

double ler_temperatura(){
  //lê o valor do sensor de temperatura
  //retorna o valor em graus Celsius
  //usar a biblioteca DHT
  double t = dht.readTemperature(false);
  return t;
}

double ler_umidade(){
  //lê o valor do sensor de umidade
  //retorna o valor em porcentagem
  //usar a biblioteca DHT
  double u = dht.readHumidity();
  return u;
}

double ler_tensao(){
  //lê o valor do sensor de tensão
  //retorna o valor em RMS
  //usar a biblioteca EmonLib
  emon1.calcVI(20, 2000); 
  double Vrms = emon1.Vrms;
  return Vrms;
}

int ler_fumaca(){
  //lê o valor do sensor de fumaça
  // se retornar HIGH (1), não há fumaça
  // se retornar LOW (0), há fumaça
  return digitalRead(pin_fumaca);
}

int ler_magnetico(){
  //lê o valor do sensor magnético
  // se retornar HIGH (1), a porta está fechada
  // se retornar LOW (0), a porta está aberta
  return digitalRead(pin_magnetico);
}


void controle_ventoinha(int comando){
  // controla a ventoinha de resfriamento
  // comando 1: ligar ventoinha
  // comando 0: desligar ventoinha
  if(comando == 1){
    digitalWrite(pin_ventoinha, HIGH);
  }else if(comando == 0){  
    digitalWrite(pin_ventoinha, LOW);
  }
}