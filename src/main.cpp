#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11


char codigo[] = "00000"; // codigo [temperatura, umidade, tensao, fumaça, magnetico] retorna 1 se algum sensor estive fora
const float BETA = 3950;
const int pin_temperatura = DHTPIN;
const int pin_umidade = DHTPIN;
const int pin_tensao = 0;
const int pin_fumaca = 0;
const int pin_magnetico = 0;

DHT dht(DHTPIN, DHTTYPE);

double ler_temperatura();
double ler_umidade();
double ler_tensao();
int ler_fumaca();
int ler_magnetico();
void controle_ventoinha(int comando);

void setup() {
  Serial.begin(9600);
  pinMode(pin_temperatura, INPUT);
  pinMode(pin_umidade, INPUT);
  pinMode(pin_tensao, INPUT);
  pinMode(pin_fumaca, INPUT);
  pinMode(pin_magnetico, INPUT_PULLUP);
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

  if(tensao<110.0 || tensao>130.0){
    codigo[2] = '1';
  }else{
    codigo[2] = '0';
  }

  if(fumaca){
    codigo[3] = '1';
  }else{
    codigo[3] = '0';
  }

  if(porta){
    codigo[4] = '1';
  }else{
    codigo[4] = '0';
  }
  char json_code[100];
  snprintf(json_code, sizeof(json_code),"{'code': %s, 'temperatura': %lf, 'umidade': %lf, 'tensao': %lf, 'fumaca': %d, 'porta': %d}", codigo, temp, umidade, tensao, fumaca, porta);
  Serial.println(codigo);
}

double ler_temperatura(){
  double temperatura = dht.readTemperature(false);
  return temperatura;
}

double ler_umidade(){
  double umidade = dht.readHumidity();
  return umidade;
}
double ler_tensao(){
  int leitura = analogRead(pin_tensao);
  double tensao = (leitura * 5.0) / 1023.0; // converte para tensão em volts
  return tensao;
}

int ler_fumaca(){
    int leitura = analogRead(pin_fumaca);
    double tensao = leitura * (5.0 / 1023.0);

    if(tensao>0 && tensao<2.75){
      return 0;
    }else{
      return 1;  
    }
}

int ler_magnetico(){
  //lê se a porta está aberta
  int estado = digitalRead(pin_magnetico);

  if(estado== HIGH){
    return 1;
  }else{
    return 0;
  }
}


void controle_ventoinha(int comando){

  if(comando == 1){
    Serial.println("Ligar ventoinha");
  }else if(comando == 0){
    Serial.println("Desligar ventoinha");  
  }
  //implementar
  
}