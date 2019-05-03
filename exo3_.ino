#include <interruptions.h>
/* 2.3 Commande asservie
 *  /!\ Programme incomplet car tops non configurés
 */


//outputs
  //roue gauche
#define mav_g 10
#define mar_g 9

  //roue droite
#define mav_d 3
#define mar_d 5

//interrupts
#define int_g 8
#define int_d 9

int c=0;
int d=0;

/* ---- mov ---- */
void rot_g(){
  analogWrite(mar_g,200);
  analogWrite(mav_d,200); 
}


void rot_d(){
  analogWrite(mav_g,200);
  analogWrite(mar_d,200); 
}

void av(){
  analogWrite(mav_g,199);
  analogWrite(mav_d,200);  
}


void ar(){
  analogWrite(mar_g,200);
  analogWrite(mar_d,200);  
}


void stop_mv(){
  analogWrite(mav_g,0);
  analogWrite(mar_g,0);  
  analogWrite(mav_d,0);
  analogWrite(mar_d,0);
}


/* ---- counters ---- */
void counter_g(){
  c++;
}

void counter_d(){
  d++;

}

void raz(){
  c=0;
  d=0;
}

void setup() {
  Serial.begin(9600);
  pinMode(mav_g,OUTPUT);
  pinMode(mar_g,OUTPUT);
  pinMode(mav_d,OUTPUT);
  pinMode(mar_d,OUTPUT);

  PCattachInterrupt(A0,counter_g,CHANGE);
  PCattachInterrupt(A1,counter_d,CHANGE);
}
/*

*/

void loop() {
  // avancer
  av();
  while(c<375 && d<375){    
    counter_g();
    counter_d();
  delay(10);
  }
  raz();
  stop_mv();
    delay(1000);

  // reculer
  ar();
  while(c<375 && d<375){  
    counter_g();
    counter_d();
    delay(10);
  }
  raz();

  stop_mv();
  delay(1000);

  rot_d();
  // rotation droite
  while(c<100 && d<100){
    
    counter_g();
    counter_d();
    delay(10);
  }
  raz();

  stop_mv();
  delay(1000);
  rot_g();
  // rotation gauche
  while(c<150 && d<150){
    
    counter_g();
    counter_d();
    delay(10);
  }
  stop_mv();
  raz();
  delay(1000);

}

