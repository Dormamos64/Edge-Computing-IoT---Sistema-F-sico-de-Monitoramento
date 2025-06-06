#include "DHT.h"                     // Biblioteca para o sensor de umidade DHT22
#include <LiquidCrystal_I2C.h>      // Biblioteca para o display LCD com comunicação I2C

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inicializa o LCD no endereço I2C 0x27, com 16 colunas e 2 linhas

#define dhtPin 10                   // Define o pino do Arduino conectado ao DHT22
#define dhtType DHT22              // Define o tipo do sensor como DHT22
#define trigger 9                  // Pino de trigger do sensor ultrassônico
#define echo 8                     // Pino de echo do sensor ultrassônico
#define led1 11                    // Pino do LED verde 
#define led2 7                     // Pino do LED amarelo 
#define led3 6                     // Pino do LED vermelho 
#define buzzer 4                   // Pino do buzzer

DHT dht(dhtPin, dhtType);          // Cria o objeto do sensor DHT

float umid = 0;                    // Variável para armazenar o valor da umidade
int dist = 0;                      // Variável para armazenar a distância medida

void setup() {
  Serial.begin(9600);              // Inicializa a comunicação serial a 9600 bps
  dht.begin();                     // Inicia o sensor DHT
  pinMode(trigger, OUTPUT);       // Define o pino do trigger como saída
  pinMode(echo, INPUT);           // Define o pino do echo como entrada
  pinMode(led1, OUTPUT);          // Define o pino do LED verde como saída
  pinMode(led2, OUTPUT);          // Define o pino do LED amarelo como saída
  pinMode(led3, OUTPUT);          // Define o pino do LED vermelho como saída
  pinMode(buzzer, OUTPUT);        // Define o pino do buzzer como saída

  lcd.init();                      // Inicializa o LCD
  lcd.backlight();                 // Liga a luz de fundo do LCD
}

void loop() {
  // Leitura da umidade
  umid = dht.readHumidity();                                  // Lê a umidade do ar
  Serial.println("Umidade: " + String(umid) + "%");           // Exibe a umidade no monitor serial

  // Leitura da distância usando o sensor ultrassônico
  digitalWrite(trigger, LOW);                                 
  delayMicroseconds(2);                                       // Pequeno atraso
  digitalWrite(trigger, HIGH);                                
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);                                 // Finaliza o pulso

  dist = pulseIn(echo, HIGH) / 58;                            // Calcula a distância em cm
  Serial.println("Distancia: " + String(dist) + " cm");       // Exibe a distância no monitor serial

  // Exibição das informações no LCD
  lcd.clear();                                                // Limpa o LCD
  lcd.setCursor(0, 0);                                        // Define o cursor na primeira linha
  lcd.print("Umid: " + String(umid) + "%");                   // Mostra a umidade no LCD
  lcd.setCursor(0, 1);                                        // Define o cursor na segunda linha
  lcd.print("Dist: " + String(dist) + "cm ");                 // Mostra a distância no LCD

  // Verificação do nível de risco com base na distância
  if (dist > 100) {
    // Nível seguro
    digitalWrite(led1, HIGH);                                 // Liga o LED verde
    digitalWrite(led2, LOW);                                  // Desliga os outros LEDs
    digitalWrite(led3, LOW);
    digitalWrite(buzzer, LOW);                                // Buzzer desligado
    lcd.print("OK");                                          // Mensagem de status
  } else if (dist > 50) {
    // Nível de atenção
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);                                 // Liga o LED amarelo
    digitalWrite(led3, LOW);
    digitalWrite(buzzer, LOW);                                // Buzzer desligado
    lcd.print("ATENCAO");                                     // Mensagem de status
  } else {
    // Nível crítico
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);                                 // Liga o LED vermelho
    digitalWrite(buzzer, HIGH);                               // Ativa o buzzer
    lcd.print("PERIGO");                                      // Mensagem de status
  }

  delay(2000);                                                // Aguarda 2 segundos antes da próxima leitura
}