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

float ler_temperatura();
float ler_umidade();
float ler_tensao();
bool ler_fumaca();
bool ler_magnetico();
void controle_ventoinha(char* comando);

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
  float temp = ler_temperatura(); 
  float umidade = ler_umidade();
  float tensao = ler_tensao();
  bool fumaca = ler_fumaca();
  bool porta = ler_magnetico();

  if(temp > 30){
    codigo[0] = '1';
    controle_ventoinha("on");
  }else{
    codigo[0] = '0';
    controle_ventoinha("off");
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
  snprintf(json_code, sizeof(json_code),"{'code': %s, 'temperatura': %d, 'umidade': %d, 'tensao': %d,'fumaca': %d,'porta': %d}", codigo, temp, umidade, tensao, fumaca, porta);
  Serial.println(codigo);
}

float ler_temperatura(){
  float temperatura = dht.readTemperature(false);
  return temperatura;
}

float ler_umidade(){
  float umidade = dht.readHumidity();
  return umidade;
}
float tensao(){
  int leitura = analogRead(pin_tensao);
  float tensao = (leitura * 5.0) / 1023.0; // converte para tensão em volts
  return tensao;
}

bool ler_fumaca(){
    int leitura = analogRead(pin_fumaca);
    float tensao = leitura * (5.0 / 1023.0);

    if(tensao>0 && tensao<2.75){
      return false;
    }else{
      return true;  
    }
}

bool ler_magnetico(){
  //lê se a porta está aberta
  int estado = digitalRead(pin_magnetico);

  if(estado== HIGH){
    return true;
  }else{
    return false;
  }
}


void controle_ventoinha(char* comando){
  //implementar
  
}