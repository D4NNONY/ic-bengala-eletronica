//Para o funcionamento do projeto é necessário instalar as bibliotecas abaixo
#include <Wire.h>
#include <VL53L0X.h>
#include <avdweb_VirtualDelay.h>

#define pinoBuzzerAlto 5 //saída
#define pinoVibracaoAlto 4 //saída
#define pinoVibracaoBaixo 6 //saída
#define pinoBuzzerBaixo 7 //saída
#define distanciaLimite 1300 //distância máxima de medição em cm
// pinos XSHUT
#define xshut1 2 //sensoralto
#define xshut2 3 //sensorbaixo

VL53L0X sensoralto, sensorbaixo; //Cria uma instância para cada sensor

// Varíaveis auxiliares de medição (para o filtro)
int cm1atual, cm1antigo = 0;
int cm2atual, cm2antigo = 0;
unsigned long tempo1 = 0, tempo2 = 0;

VirtualDelay contadorPausaAlto, contadorDuracaoAlto; //contador de tempo para as pausas entre os beeps do buzzer alto
VirtualDelay contadorPausaBaixo, contadorDuracaoBaixo; //contador de tempo para as pauas entre os beeps do buzzer baixo

//tempos de pausa entre os beeps (milisegundos) de acordo com a distância
const int pausa1 = 100; //para a distância1 
const int pausa2 = 300; //para a distância2
const int pausa3 = 600; //para a distância3
const int pausa4 = 800; //para a distância4
const int pausa5 = 1000; //para a distância5

//frequências produzidas de acordo com a distância
const int frequencia1 = 1200; //para a distância1 
const int frequencia2 = 800; //para a distância2 
const int frequencia3 = 600; //para a distância3 
const int frequencia4 = 400; //para a distância4
const int frequencia5 = 200; //para a distância5

//duração do beep de acordo com a distância
const int duracao1 = 50; //para a distância1
const int duracao2 = 100; //para a distância2
const int duracao3 = 200; //para a distância3
const int duracao4 = 400; //para a distância4
const int duracao5 = 600; //para a distância5

void setup()
{
  pinMode(pinoVibracaoAlto, OUTPUT);
  pinMode(pinoBuzzerAlto, OUTPUT);
  pinMode(pinoVibracaoBaixo, OUTPUT);
  pinMode(pinoBuzzerBaixo, OUTPUT);
  
  Serial.begin(9600);
  Wire.begin();

  //O sensor VL53L0X utiliza o barramento I2C para comunicação e ambos os sensores inciam com o mesmo endereço I2C, impedindo a comunicação
  //com o Arduino, portanto é necessário reprogramar o endereço de um deles, enquanto o outro está desligado
  
  // Define o sensor 2 como entrada para fazer o pino SHUT_2 ficar em nível alto
  pinMode(xshut1, OUTPUT); //sensoralto
  pinMode(xshut2, INPUT); //sensorbaixo

  // "Desliga" o sensor alto
  digitalWrite(xshut1, LOW);
  delay(2);

  // Altera o endereço do sensor baixo
  sensorbaixo.setAddress(0x32);

  // Religa o sensor 1 definindo ele como entrada
  pinMode(xshut1, INPUT);

  // Inicializa os sensores
  sensoralto.init();
  sensorbaixo.init();
  sensoralto.setTimeout(500);
  sensorbaixo.setTimeout(500);

  // Sensores definidos para modo de longo alcance
  sensoralto.setSignalRateLimit(0.1);
  sensoralto.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensoralto.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensorbaixo.setSignalRateLimit(0.1);
  sensorbaixo.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensorbaixo.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
}


void loop()
{
  // Faz a medição da distância e retorna um valor em milímetros
  int cm1atual = sensoralto.readRangeSingleMillimeters();
  int cm2atual = sensorbaixo.readRangeSingleMillimeters();

  filtro();
 
  // Imprime no monitor serial
  Serial.print(cm1atual);
  Serial.print(" - ");
  Serial.println(cm2atual); 

  // Funções de alerta para cada sensor
  if (cm1atual < distanciaLimite){
    alertaAlto(cm1atual);
  }
  if (cm2atual < distanciaLimite){
    alertaBaixo(cm2atual);
  }
  
}

void filtro(void) // Esta função realiza uma filtragem de valores "nulos" retornados pelo sensor
{
  // Se o sensor retornar um repetidas vezes "valor de distância nula" (8140) por um período menor que 1,5s, esse valor é descartado
  // e se considera o último valor de distância válido lido. 
  if((cm1atual > 8000) && ((millis()-tempo1) < 1500)){
    cm1atual = cm1antigo;
  }
  // Se nenhuma dessas condições é atendida, o último valor lido se torna o atual
  else{
    cm1antigo = cm1atual;
    tempo1 = millis();
  }

  // Mesma lógica para o segundo sensor
  if((cm2atual > 8000) && ((millis()-tempo2) < 1500)){
    cm2atual = cm2antigo;
  }
  else{
    cm2antigo = cm2atual;
    tempo2 = millis();
  }
}

void alertaAlto(int cm1atual){ //função que ativa os 'beeps', recebe o retorno da função calcularDistanciaAlto

  int cm = cm1atual;
  
  if (cm >= 0 && cm<= 300){ //distância1: 0-30cm
    contadorPausaAlto.start(pausa1); //inicia o contador da pausa
    if (contadorPausaAlto.elapsed()){ //quando o contador da pausa termina:
      contadorDuracaoAlto.start(duracao1); //o contador da duração inicia
      tone(pinoBuzzerAlto, frequencia1); //o buzzer é ativado em determinadio frequência
      digitalWrite(pinoVibracaoAlto, HIGH); //o motor de vibração é acionado
      if(contadorDuracaoAlto.elapsed()) { //quando contador da duração termina:
        noTone(pinoBuzzerAlto); //o buzzer é desativado
        digitalWrite(pinoVibracaoAlto, LOW); //o motor de vibração é desativado
      }
    }
  }
  
  if (cm > 300 && cm<= 600){ //distância2: 30-60cm
    contadorPausaAlto.start(pausa2);
    if(contadorPausaAlto.elapsed()){
      contadorDuracaoAlto.start(duracao2); 
      tone(pinoBuzzerAlto, frequencia2);
      digitalWrite(pinoVibracaoAlto, HIGH); 
      if(contadorDuracaoAlto.elapsed()) {
        noTone(pinoBuzzerAlto);
        digitalWrite(pinoVibracaoAlto, LOW); 
      }
    }
  }

  if (cm > 600 && cm<= 900){ //distância3: 60-90cm
    contadorPausaAlto.start(pausa3);
    if(contadorPausaAlto.elapsed()){
      contadorDuracaoAlto.start(duracao3); 
      tone(pinoBuzzerAlto, frequencia3);
      digitalWrite(pinoVibracaoAlto, HIGH); 
      if(contadorDuracaoAlto.elapsed()) {
        noTone(pinoBuzzerAlto);
        digitalWrite(pinoVibracaoAlto, LOW); 
      }
    }
  }
  
  if (cm > 900 && cm<= 1100){ //distância4: 90-110cm
    contadorPausaAlto.start(pausa4);
    if(contadorPausaAlto.elapsed()){
      contadorDuracaoAlto.start(duracao4); 
      tone(pinoBuzzerAlto, frequencia4);
      digitalWrite(pinoVibracaoAlto, HIGH); 
      if(contadorDuracaoAlto.elapsed()) {
        noTone(pinoBuzzerAlto);
        digitalWrite(pinoVibracaoAlto, LOW); 
      }
    }
  }
  if (cm > 1100 && cm< distanciaLimite){ //distância5: 110-150cm
    contadorPausaAlto.start(pausa5);
    if(contadorPausaAlto.elapsed()){
      contadorDuracaoAlto.start(duracao5); 
      tone(pinoBuzzerAlto, frequencia5);
      digitalWrite(pinoVibracaoAlto, HIGH); 
      if(contadorDuracaoAlto.elapsed()) {
        noTone(pinoBuzzerAlto);
        digitalWrite(pinoVibracaoAlto, LOW);
      }
    }
  }
}

void alertaBaixo(int cm2atual){

  int cm2 = cm2atual;
  
  if (cm2 >= 0 && cm2<= 300){ //distância1: 0-30cm
    contadorPausaBaixo.start(pausa1);
    if (contadorPausaBaixo.elapsed()){
      contadorDuracaoBaixo.start(duracao1);
      tone(pinoBuzzerBaixo, frequencia1);
      digitalWrite(pinoVibracaoBaixo, HIGH);
      if(contadorDuracaoBaixo.elapsed()){
        noTone(pinoBuzzerBaixo);
        digitalWrite(pinoVibracaoBaixo, LOW);
      }
    }
  }
  
  if (cm2 > 300 && cm2<= 600){ //distância2: 30-60cm
    contadorPausaBaixo.start(pausa2);
    if (contadorPausaBaixo.elapsed()){
      contadorDuracaoBaixo.start(duracao2);
      tone(pinoBuzzerBaixo, frequencia2);
      digitalWrite(pinoVibracaoBaixo, HIGH);
      if(contadorDuracaoBaixo.elapsed()){
        noTone(pinoBuzzerBaixo);
        digitalWrite(pinoVibracaoBaixo, LOW);
      }
    }
  }

  if (cm2 > 600 && cm2<= 800){ //distância3: 60-90cm
    contadorPausaBaixo.start(pausa3);
    if (contadorPausaBaixo.elapsed()){
      contadorDuracaoBaixo.start(duracao3);
      tone(pinoBuzzerBaixo, frequencia3);
      digitalWrite(pinoVibracaoBaixo, HIGH);
      if(contadorDuracaoBaixo.elapsed()){
        noTone(pinoBuzzerBaixo);
        digitalWrite(pinoVibracaoBaixo, LOW);
      }
    }
  }
  if (cm2 > 900 && cm2<= 1100){ //distância4: 90-110cm
    contadorPausaBaixo.start(pausa4);
    if (contadorPausaBaixo.elapsed()){
      contadorDuracaoBaixo.start(duracao4);
      tone(pinoBuzzerBaixo, frequencia4);
      digitalWrite(pinoVibracaoBaixo, HIGH);
      if(contadorDuracaoBaixo.elapsed()){
        noTone(pinoBuzzerBaixo);
        digitalWrite(pinoVibracaoBaixo, LOW);
      }
    }
  }
  if (cm2 > 1100 && cm2< distanciaLimite){ //distância5: 100-150cm
    contadorPausaBaixo.start(pausa5);
    if (contadorPausaBaixo.elapsed()){
      contadorDuracaoBaixo.start(duracao5);
      tone(pinoBuzzerBaixo, frequencia5);
      digitalWrite(pinoVibracaoBaixo, HIGH);
      if(contadorDuracaoBaixo.elapsed()){
        noTone(pinoBuzzerBaixo);
        digitalWrite(pinoVibracaoBaixo, LOW);
      }
    }
  }
}
