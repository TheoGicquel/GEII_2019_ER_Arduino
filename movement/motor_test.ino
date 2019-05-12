/**************************************************
 **                                              **
 ** Avancer en ligne droite a l'aide des codeurs **
 ** D'une certaine distance                      **
 **************************************************/

//----------------------------------------------| Bibliothèques |
#include <interruptions.h>
//----------------------------------------------| Définition E/S |
#define interrupt_g A0
#define interrupt_d A1

//outputs
  //roue gauche
#define mg_av 10
#define mg_ar 9

  //roue droite
#define md_av 3
#define md_ar 5

//interrupts
/**
#define interrupt_g 8
#define interrupt_d 9
**/
//----------------------------------------------| Variables & Constantes |
int acc_g=0;
int acc_d=0;

int dist_g=0;
int dist_d=0;

int desync;
int mg_vit=200;
int md_vit=200;
const int ajust=10;

//----------------------------------------------| Fonctions - Encodeur |


void codeur_g(){ // incrementation codeur gauche
  acc_g++;
  dist_g++;
}

void codeur_d(){ // incrementation codeur droite
  acc_d++;
  dist_d++;
}

void raz_dist(){ // reset de la distance parcourue
  dist_g=0;
  dist_d=0;
}


void sync(){ // correctif de trajectoire
  desync=acc_g-acc_d;

  
  if(desync!=0){ //-------- verif decalage present
    acc_g=0;
    acc_d=0;
    
    if(desync<0){ //-------------- roue gauche en avance ---------------

      if(mg_vit>=255){ //------------------- ralentissement roue gauche
        mg_vit=mg_vit-ajust;
      }
      
      else if(md_vit<=255){ //--------------- accelerer roue droite
        md_vit=md_vit+ajust;
       }
       
    }

    if(desync>0){ //-------------- roue droite en avance ---------------

      if(md_vit>=255){ //------------------- ralentissement roue droite
        md_vit=md_vit-ajust;
      }
      else if(mg_vit<=255){ //--------------- accelerer roue gauche
        mg_vit=mg_vit+ajust;
      }
    }
  } // decalage present
delay(1);
} // fin sync



//----------------------------------------------| Fonctions - Mouvement |
void rot_g(){
  sync();
  analogWrite(mg_ar,mg_vit);
  analogWrite(md_av,md_vit); 
}


void rot_d(){
  sync();
  analogWrite(mg_av,mg_vit);
  analogWrite(md_ar,md_vit); 
  
}

void av(){
  sync();
  analogWrite(mg_av,mg_vit);
  analogWrite(md_av,md_vit);
}


void ar(){
  sync();
  analogWrite(mg_ar,mg_vit);
  analogWrite(md_ar,md_vit);  
}


void raz_vit(){
  mg_vit=200;
  md_vit=200;
}



void stop_mv(){
  analogWrite(mg_av,0);
  analogWrite(mg_ar,0);  
  analogWrite(md_av,0);
  analogWrite(md_ar,0);
}
//----------------------------------------------| Serie |
void dist_debug(){
  Serial.print("\ng: ");
  Serial.print(dist_g);
  Serial.print("  d:");
  Serial.print(dist_d);
}
//----------------------------------------------| Initialisation |
void setup(){
  pinMode(mg_av,OUTPUT);
  pinMode(mg_ar,OUTPUT);
  pinMode(md_av,OUTPUT);
  pinMode(md_ar,OUTPUT);

  PCattachInterrupt(interrupt_g,codeur_g,CHANGE);
  PCattachInterrupt(interrupt_d,codeur_d,CHANGE);
  
}

//==============================================| Loop |==============================================//
void loop() {
  // 30 tops pour un tour complet / 12cm
  // 8*30 = 240 tops pour faire ~100cm
  
  //------------------------------
  while(dist_g<240 && dist_d<240){
    av();
    dist_debug();
  }
  
  stop_mv();
  delay(2000);
  raz_dist();
  
  //------------------------------
  
  while(dist_g<240 && dist_d<240){
    ar();
    dist_debug();
  }
  
  stop_mv();
  delay(2000);
  raz_dist();
  raz_vit();
  //------------------------------

  while(dist_g<25 && dist_d<25){
    rot_g();
    dist_debug();

  }
  stop_mv();
  delay(2000);
  raz_dist();
  raz_vit();

  //------------------------------

  while(dist_g<25 && dist_d<25){
    rot_d();
    dist_debug();

  }
  stop_mv();
  delay(2000);
  raz_dist();
  raz_vit();


}
