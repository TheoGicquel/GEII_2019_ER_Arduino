/**************************************************
 **                                              **
 ** Avancer en ligne droite a l'aide des codeurs **
 ** D'une certaine distance                      **
 **************************************************/

//----------------------------------------------| Bibliothèques |
#include <interruptions.h>

//----------------------------------------------| Définition E/S |
#define interrupt_pin_g A0
#define interrupt_pin_d A1

//outputs
  //roue gauche
#define mg_av 10
#define mg_ar 9

  //roue droite
#define md_av 3
#define md_ar 5

//interrupts
#define interrupt_g 8
#define interrupt_d 9

//----------------------------------------------| Variables & Constantes |
int acc_g=0;
int acc_d=0;

int dist_g=0;
int dist_d=0;

int desync;
int mg_vit=200;
int md_vit=200;
const int ajust=12;

//----------------------------------------------| Fonctions - Encodeur |


void codeur_g(){
  acc_g++;
  dist_g++;
}

void codeur_d(){
  acc_d++;
  dist_g++;
}

void raz_dist(){
  dist_g=0;
  dist_d=0;
}


void sync(){
  desync=acc_g-acc_d;

  
  if(desync!=0){ //-------- decalage present  v
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
  analogWrite(mg_ar,mg_vit);
  analogWrite(md_av,md_vit); 
}


void rot_d(){
  analogWrite(mg_av,mg_vit);
  analogWrite(md_ar,md_vit); 
}

void av(){
  analogWrite(mg_av,mg_vit);
  analogWrite(md_av,md_vit);
  sync();
}


void ar(){
  analogWrite(mg_ar,mg_vit);
  analogWrite(md_ar,md_vit);
  sync();  
}


void stop_mv(){
  analogWrite(mg_av,0);
  analogWrite(mg_ar,0);  
  analogWrite(md_av,0);
  analogWrite(md_ar,0);
}

//----------------------------------------------| Initialisation |
void setup(){
  pinMode(mg_av,OUTPUT);
  pinMode(mg_ar,OUTPUT);
  pinMode(md_av,OUTPUT);
  pinMode(md_ar,OUTPUT);

  PCattachInterrupt(A0,codeur_g,CHANGE);
  PCattachInterrupt(A1,codeur_d,CHANGE);



  
}

//==============================================| Loop |==============================================//
void loop() {

  // 60tops pour un tour complet / 10.5cm
  // 571 pour faire 1m
  while(dist_g<571 && dist_d<571){
  av();
  }
  stop_mv();
  raz_dist();
  delay(2000);

    while(dist_g<571 && dist_d<571){
  ar();
  }
  raz_dist();
  stop_mv();
  delay(2000);
}
