#include <Arduino.h>
#include <math.h>

//sinカーブとかを描く用
unsigned long startmillis = 0;

#define OPEN 1
#define CLOSE 0

#define LONG 1
#define SHORT 0

#define INNER_ARM_LENGTH 230
#define OUTER_ARM_LENGTH 225

double p_max = 410.0;
double p_min = 50.0;

double l_target = 0;
double l_now = 0;

double tau_p_long = 60.0;
double tau_n_long = 60.0;

double tau_p_short = 60.0;
double tau_n_short = 60.0;


//内圧が増えていくときの過渡応答グラフから，あるpのときの対応フェーズを返す
double phase_p(double p, bool isLongMuscle) {
  double temp = log((p_max - p_min)/(p_max - p));
  if(isLongMuscle) return tau_p_long * temp;
  return tau_p_short * temp;
}
//内圧が減っていくときの過渡応答グラフから，あるpのときの対応フェーズを返す
double phase_n(double p, bool isLongMuscle) {
  double temp = log((p_max - p_min)/(p - p_min));
  if(isLongMuscle) return tau_n_long * temp;
  return tau_n_short * temp;
}

void setup() {
  //このプログラムは基本的にUNO向けなので注意
  DDRD = DDRD | B11111100;
  DDRB = DDRB | B00000011;

  //人工筋内圧
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  //リンク内角
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  Serial.begin(115200);
  delay(1000);
}

int openspan = 0;
double setPressure(int p_target) {

  int p_now = map(constrain(analogRead(A4),   0, 234),   0, 234, 50, 400);
  
  if(p_now < p_target) {
    //吸気
    openspan = phase_p(p_target, LONG) - phase_p(p_now, LONG);
    
  } else {
    //排気
    openspan = phase_n(p_target, LONG) - phase_n(p_now, LONG);
    
  }
}

void loop() {
  setPressure(100);
}
