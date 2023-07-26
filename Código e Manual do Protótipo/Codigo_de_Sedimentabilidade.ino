/*
  Projeto: Sedimentabilidade
  Curso: Tecnologia em Saneamento Ambiental
  Desenvolvedores: Antônio Vinícius da Silva Sousa e Anna Patrícia de Lima
  Orientador: Heraldo Antunes Silva Filho
  Data: 25/04/2023
*/

// Inclusão de Bibliotecas.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definições e Criação de Objetos.
#define porta4 4                 // Definição do primeiro Laser na entrada digital 4
#define porta7 7                 // Definição do segundo Laser na entrada digital 7
#define porta8 8                 // Definição do terceiro Laser na entrada digital 8
float LDR1 = A0;                 // Definição do primeiro LDR na entrada analógica A0
float LDR2 = A1;                 // Definição do segundo LDR na entrada analógica A1
float LDR3 = A2;                 // Definição do terceiro LDR na entrada analógica A2

int contador = 0;                                        // Variável para controle do pressionamento do botão
float valorLDR1 = 0, valorLDR2 = 0, valorLDR3 = 0;       // Variáveis das leituras dos dados enviados pelo Laser para o LDR
float tensaoLDR1, tensaoLDR2, tensaoLDR3;                // Variáveis das tensões de ambos os LDR's
int passagemConfirmada1, passagemConfirmada2, passagemConfirmada3; // Variáveis lógicas para o controle do sistema de Lasers
unsigned long sensor1 = 0, sensor2 = 0, sensor3 = 0;     // Variáveis para o armazenamento do tempo transcorrido no período de sedimentabilidade
unsigned char criterio1 = 0, criterio2 = 0, criterio3 = 0;         // Variáveis lógicas para o controle do sistema de Lasers
float tempo1 = 0, tempo2 = 0, tempo3 = 0;                // Variáveis para armazenamento do tempo transcorrido em milissegundos
float intervalo1 = 0, intervalo2 = 0, intervalo3 = 0;    // Variáveis para armazenamento do tempo transcorrido em segundos
float velocidade1 = 0, velocidade2 = 0, velocidade3 = 0; // Variáveis para armazenamento da velocidade de ambos os intervalos em centímetros por segundo

#define valorReferencia1 350     // Definição do valor de referência para o primeiro conjunto do protótipo
#define valorReferencia2 380     // Definição do valor de referência para o segundo conjunto do protótipo
#define valorReferencia3 200     // Definição do valor de referência para o terceiro conjunto do protótipo
#define distancia1 4.5           // Definição do valor de referência para a distância entre o 1º e o 2º conjunto do protótipo
#define distancia2 4.5           // Definição do valor de referência para a distância entre o 2º e o 3º conjunto do protótipo
#define distancia3 9.0           // Definição do valor de referência para a distância entre o 1º e o 3º conjunto do protótipo
#define tempoEspera 0            // Definição de um delay de um segundo para apresentar os valores calculados e obtidos
#define tempoBotao 500           // Definição de um delay de meio segundo para controle do processo de pressionamento do botão
#define botao 2                  // Definição do botão do protótipo para a porta digital 2

// Instanciando Objetos.
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);                           // Inicialização do Monitor Serial
  lcd.init();                                   // Inicia a comunicação com o Display LCD
  lcd.backlight();                              // Liga a iluminação do Display LCD
  lcd.clear();                                  // Limpa o Display LCD
  pinMode (LDR1, INPUT);                        // Definição do LDR1 como entrada do sinal
  pinMode (LDR2, INPUT);                        // Definição do LDR2 como entrada do sinal
  pinMode (LDR3, INPUT);                        // Definição do LDR3 como entrada do sinal
  pinMode (porta4, OUTPUT);                     // Definição da Porta Digital 4 como saída do sinal
  pinMode (porta7, OUTPUT);                     // Definição da Porta Digital 7 como saída do sinal
  pinMode (porta8, OUTPUT);                     // Definição da Porta Digital 8 como saída do sinal
  pinMode(botao, INPUT_PULLUP);                 // Definição do botão como entrada do sinal
}

void loop() {
// Impressão no Display LCD.
  lcd.setCursor(0, 0);                          // Posiciona o cursor na primeira coluna da linha 1
  lcd.print(" TESTE SEDIMENTACAO");
  lcd.setCursor(0, 1);                          // Posiciona o cursor na primeira coluna da linha 2
  lcd.print("Aperte o botao para");
  lcd.setCursor(0, 2);                          // Posiciona o cursor na primeira coluna da linha 3
  lcd.print("proseguir!");
  lcd.setCursor(0, 3);                          // Posiciona o cursor na primeira coluna da linha 4
  lcd.print("       Aguardando...");
   
  if(!digitalRead(botao)){
    contador++;                                 // Armazenamento da quantidade de vezes que o botão foi pressionado
    delay(tempoBotao);                          // Criação de um intervalo de tempo de meio segundo
    lcd.clear();                                // Limpa o Display LCD 
    
    while(contador == 1){  
// Acionamento dos Lasers.
      digitalWrite(porta4, HIGH);
      digitalWrite(porta7, HIGH);
      digitalWrite(porta8, HIGH);
    
// Leituras dos Dados Enviados pelos Lasers para Ambos os LDR's.
      valorLDR1 = analogRead(LDR1);
      valorLDR2 = analogRead(LDR2);
      valorLDR3 = analogRead(LDR3);
      
// Estrutura de Controle do Período de Sedimentabilidade - Verificação da Passagem de Luz do Laser até o LDR.
      if (valorLDR1 < valorReferencia1){
        passagemConfirmada1 = 1;
      }
      if (valorLDR2 < valorReferencia2){
        passagemConfirmada2 = 1;
      }
      if (valorLDR3 < valorReferencia3){
        passagemConfirmada3 = 1;
      }
           
// Estrutura de Controle do Período de Sedimentabilidade - Acionamento dos Temporizadores e Cálculo da Velocidade dos Intervalos.
      if (passagemConfirmada1 == 1 && criterio1 == 0){
        sensor1 = millis();
        criterio1 = 1;
      }
      if (passagemConfirmada2 == 1 && passagemConfirmada1 == 1 && criterio2 == 0){
        sensor2 = millis();
        criterio2 = 1;
      }
      if (passagemConfirmada3 == 1 && passagemConfirmada2 == 1 && passagemConfirmada1 == 1 && criterio3 == 0){
        sensor3 = millis();
        criterio3 = 1;
      }
      
      if (criterio1 == 1 && criterio2 == 1 && criterio3 == 1 && passagemConfirmada1 == 1 && passagemConfirmada2 == 1 && passagemConfirmada3 == 1){        
// Cálculo do Intervalo de Tempo e da Velocidade do 1º sensor ao 2º sensor.
        tempo1 = (sensor2 - sensor1);           // Intervalo de tempo em milissegundos
        intervalo1 = tempo1 / 1000;             // Conversão de milissegundos para segundos
        velocidade1 = distancia1 / intervalo1;  // Cálculo da velocidade do deslocamento da partícula em centímetros por segundo
        
// Cálculo do Intervalo de Tempo e da Velocidade do 2º sensor ao 3º sensor.
        tempo2 = (sensor3 - sensor2);           // Intervalo de tempo em milissegundos
        intervalo2 = tempo2 / 1000;             // Conversão de milissegundos para segundos
        velocidade2 = distancia2 / intervalo2;  // Cálculo da velocidade do deslocamento da partícula em centímetros por segundo
        
// Cálculo do Intervalo de Tempo e da Velocidade do 1º sensor ao 3º sensor.
        tempo3 = (sensor3 - sensor1);           // Intervalo de tempo em milissegundos
        intervalo3 = tempo3 / 1000;             // Conversão de milissegundos para segundos
        velocidade3 = distancia3 / intervalo3;  // Cálculo da velocidade do deslocamento da partícula em centímetros por segundo
    
// Impressão no Monitor Serial.
        Serial.print("Velocidade 1: ");
        Serial.print(velocidade1, 5);           // ', 5' é usado para mostrar mais casas decimais no monitor serial
        Serial.println(" cm/s");
        Serial.print(intervalo1);
        Serial.println(" segundos");
        Serial.print("Velocidade 2: ");
        Serial.print(velocidade2, 5);           // ', 5' é usado para mostrar mais casas decimais no monitor serial
        Serial.println(" cm/s");
        Serial.print(intervalo2);
        Serial.println(" segundos");
        Serial.print("Velocidade 3: ");
        Serial.print(velocidade3, 5);           // ', 5' é usado para mostrar mais casas decimais no monitor serial
        Serial.println(" cm/s");
        Serial.print(intervalo3);
        Serial.println(" segundos");         
      }
      
// Impressão no Monitor Serial.
      Serial.print("Valor do primeiro laser:");
      Serial.println(valorLDR1);
      Serial.print("Valor do segundo laser:");
      Serial.println(valorLDR2);
      Serial.print("Valor do terceiro laser:");
      Serial.println(valorLDR3);
      Serial.println("-----------------------");
      delay(tempoEspera);                       // Criação de um intervalo de tempo de um segundo
          
// Impressão no Display LCD.      
      lcd.setCursor(0, 0);                      // Posiciona o cursor na primeira coluna da linha 1
      lcd.print("AVALIACAO DOS DADOS");
      lcd.setCursor(0, 1);                      // Posiciona o cursor na primeira coluna da linha 2
      lcd.print("INTERV.=");
      lcd.setCursor(8, 1);                      // Posiciona o cursor na nona coluna da linha 2
      lcd.print("|PRI|");
      lcd.setCursor(13, 1);                     // Posiciona o cursor na décima quarta coluna da linha 2
      lcd.print("SEG|");
      lcd.setCursor(17, 1);                     // Posiciona o cursor na décima oitava coluna da linha 2
      lcd.print("TER");

      lcd.setCursor(0, 2);                      // Posiciona o cursor na primeira coluna da linha 3
      lcd.print("T(s)=");
      lcd.setCursor(8, 2);                      // Posiciona o cursor na nona coluna da linha 3
      lcd.print("|");
      lcd.setCursor(9, 2);                      // Posiciona o cursor na décima coluna da linha 3
      lcd.print(intervalo1, 1);                 // ', 1' é usado para mostrar casas decimais no monitor serial
      lcd.setCursor(12, 2);                     // Posiciona o cursor na décima terceira coluna da linha 3
      lcd.print("|");
      lcd.setCursor(13, 2);                     // Posiciona o cursor na décima quarta coluna da linha 3
      lcd.print(intervalo2, 1);                 // ', 1' é usado para mostrar casas decimais no monitor serial
      lcd.setCursor(16, 2);                     // Posiciona o cursor na décima sétima coluna da linha 3
      lcd.print("|");
      lcd.setCursor(17, 2);                     // Posiciona o cursor na décima oitava coluna da linha 3
      lcd.print(intervalo3, 1);                 // ', 1' é usado para mostrar casas decimais no monitor serial

      lcd.setCursor(0, 3);                      // Posiciona o cursor na décima terceira coluna da linha 4
      lcd.print("V(cm/s)=");
      lcd.setCursor(8, 3);                      // Posiciona o cursor na oitava coluna da linha 4
      lcd.print("|");
      lcd.setCursor(9, 3);                      // Posiciona o cursor na décima coluna da linha 4
      lcd.print(velocidade1, 1);                // ', 1' é usado para mostrar casas decimais no monitor serial
      lcd.setCursor(12, 3);                     // Posiciona o cursor na décima terceira coluna da linha 4
      lcd.print("|");
      lcd.setCursor(13, 3);                     // Posiciona o cursor na décima quarta coluna da linha 4
      lcd.print(velocidade2, 1);                // ', 1' é usado para mostrar casas decimais no monitor serial
      lcd.setCursor(16, 3);                     // Posiciona o cursor na décima sétima coluna da linha 4
      lcd.print("|");     
      lcd.setCursor(17, 3);                     // Posiciona o cursor na décima oitava coluna da linha 4
      lcd.print(velocidade3, 1);                // ', 1' é usado para mostrar casas decimais no monitor serial
      delay(500);                               // Delay de meio segundo
      
      if(!digitalRead(botao)){
        contador++;                             // Armazenamento da quantidade de vezes que o botão foi pressionado
        lcd.clear();                            // Limpa o Display LCD
        delay(tempoBotao);                      // Criação de um intervalo de tempo de meio segundo

// Impressão no Display LCD.
        lcd.setCursor(0, 0);                    // Posiciona o cursor na primeira coluna da linha 1
        lcd.print("Aperte novamente pa-");
        lcd.setCursor(0, 1);                    // Posiciona o cursor na primeira coluna da linha 2
        lcd.print("ra reiniciar!");
        lcd.setCursor(0, 3);                    // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("       Aguardando...");
      }  
    }

    while(contador == 2){
// Desligamento dos Lasers.
      digitalWrite(porta4, LOW);
      digitalWrite(porta7, LOW);
      digitalWrite(porta8, LOW);

// Reinicialização das Variáveis.
      criterio2 = 0;
      criterio1 = 0;
      criterio3 = 0;
      passagemConfirmada1 = 0;
      passagemConfirmada2 = 0;
      passagemConfirmada3 = 0;
      velocidade1 = 0;
      velocidade2 = 0;
      velocidade3 = 0;
      tempo1 = 0;
      tempo2 = 0;
      tempo3 = 0;
      intervalo1 = 0; 
      intervalo2 = 0; 
      intervalo3 = 0;
      
      if(!digitalRead(botao)){
        contador++;                             // Armazenamento da quantidade de vezes que o botão foi pressionado
        delay(tempoBotao);                      // Criação de um intervalo de tempo de meio segundo
        contador = 0;                           // Zera a contagem das vezes que o botão foi pressionado
        lcd.clear();                            // Limpa o Display LCD
      }
    }
  }
}
