#include <avdweb_VirtualDelay.h>
#include <Ultrasonic.h>

#define pinoVibracaoAlto 4 //saída
#define pinoBuzzerAlto 5 //saída
#define pinoEchoAlto 6 //entrada
#define pinoTrigAlto 7 //saída
#define pinoVibracaoBaixo 11 //saída
#define pinoBuzzerBaixo 10 //saída
#define pinoTrigBaixo 9 //saída
#define pinoEchoBaixo 8 //entrada
#define distanciaLimite 330 //distância máxima de medição em cm

Ultrasonic sensoralto(pinoTrigAlto,pinoEchoAlto); //trigger, echo
Ultrasonic sensorbaixo(pinoTrigBaixo,pinoEchoBaixo);

VirtualDelay contadorAlto; //contador de tempo para as pausas entre os beeps do buzzer alto
VirtualDelay contadorBaixo; //contador de tempo para as pauas entre os beeps do buzzer baixo

//tempos de pausa entre os beeps (milisegundos) de acordo com a distância
const int pausa1 = 400; //para a distância1 
const int pausa2 = 600; //para a distância2
const int pausa3 = 800; //para a distância3
const int pausa4 = 1200; //para a distância4

//frequências produzidas de acordo com a distância
const int frequencia1 = 1200; //para a distância1 
const int frequencia2 = 800; //para a distância2 
const int frequencia3 = 600; //para a distância3 
const int frequencia4 = 400; //para a distância4

//duração do beep de acordo com a distância
const int duracao1 = 200; //para a distância1
const int duracao2 = 300; //para a distância2
const int duracao3 = 400; //para a distância3
const int duracao4 = 500; //para a distância4

void setup() {
  //declaração de cada pino como saída ou entrada, dependendo de seu uso
  pinMode(pinoVibracaoAlto,OUTPUT); 
  pinMode(pinoBuzzerAlto,OUTPUT); 
  pinMode(pinoEchoAlto,INPUT); 
  pinMode(pinoTrigAlto,OUTPUT); 
  pinMode(pinoEchoBaixo,INPUT); 
  pinMode(pinoTrigBaixo,OUTPUT);
  pinMode(pinoBuzzerBaixo,OUTPUT);
  pinMode(pinoVibracaoBaixo,OUTPUT);
}

void loop() {
  
  float cm = calcularDistanciaAlto(); //calcula a distância para o sensor alto
  float cm2 = calcularDistanciaBaixo(); //o mesmo para o sensor baixo
  
  if (cm < distanciaLimite) {
    sensorAlto(cm); //se a distância atual for menor que a limite, executa os beeps para objetos detectados no alto
  }
  if (cm2 < distanciaLimite) {
    sensorBaixo(cm2); //o mesmo para o baixo
  }
  
}

float calcularDistanciaAlto(){ //função que calcula a distância entre o sensor e o obstáculo
  
  long microsec = sensoralto.timing(); //calcula o tempo de retorno do sinal em mircrosegunos
  float cm = sensoralto.convert(microsec, Ultrasonic::CM); //converte o tempo de retorno (microsegundos) em distância (cm)

  return cm; //retorna o valor float da distância em cm
}

float calcularDistanciaBaixo(){ //função que calcula a distância
  
  long microsec2 = sensorbaixo.timing(); 
  float cm2 = sensorbaixo.convert(microsec2, Ultrasonic::CM);

  return cm2;
}

void sensorAlto(float cm){ //função que ativa os 'beeps', recebe o retorno da função calcularDistanciaAlto
  
  if (cm >= 0 && cm<= 30){ //distância1: 0-30cm
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
  
  if (cm >= 30 && cm<= 60){ //distância2: 30-60cm
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

  if (cm >= 60 && cm<= 90){ //distância3: 60-90cm
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
  
  if (cm >= 90 && cm<= 110){ //distância4: 90-120cm
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
}

void sensorBaixo(float cm2){
  
  if (cm2 >= 0 && cm2<= 30){ //distância1: 0-30cm
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
  
  if (cm2 >= 30 && cm2<= 60){ //distância2: 30-60cm
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

  if (cm2 >= 60 && cm2<= 90){ //distância3: 60-90cm
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
  if (cm2 >= 90 && cm2<= 110){ //distância4: 90-110cm
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
}
