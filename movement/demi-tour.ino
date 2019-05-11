/**************************************************
 **  Avancer et effectuer une rotation selon     **
 **  un angle et une vitesse                     **
 **                                              **
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
// correction de trajectoire
int acc_g=0;
int acc_d=0;
const int ajust=12;
int desync;


// mesure de distance parcourue
int dist_g=0;
int dist_d=0;

// distance a parcourir
int dist=0;

// vitesse de deplacement
int mg_vit=200;
int md_vit=200;

// rotation angulaire
int degre=0;
int angle=0;

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

void stop_mv(){
  analogWrite(mg_av,0);
  analogWrite(mg_ar,0);  
  analogWrite(md_av,0);
  analogWrite(md_ar,0);
}


//----------------------------------------------| Fonctions - Calcul angulaire |


/**
 *  180°  100 tops
 *  90°   50 tops
 *  45°   25 tops
 *  9°    5 tops
 */

//----------------------------------------------| Fonctions - Calcul de distance a parcourir |
  // 60 tops pour un tour complet = 10.5cm de distance
  // 571 tops pour faire 100cm
  // 6 tops pour faire ~1cm



//==============================================| Initialisation |
void setup(){
  pinMode(mg_av,OUTPUT);
  pinMode(mg_ar,OUTPUT);
  pinMode(md_av,OUTPUT);
  pinMode(md_ar,OUTPUT);

  PCattachInterrupt(interrupt_pin_g,codeur_g,CHANGE);
  PCattachInterrupt(interrupt_pin_d,codeur_d,CHANGE); 

}

//==============================================| Loop |==============================================//
void loop() {
delay(1000);


  dist=10;// parcourir 10 cm
  dist=dist*6; // calcul de distance

  // avancer tant que distance non atteinte
  while(dist_g<dist && dist_d<dist){av();}
  
  stop_mv();
  delay(2000);
  raz_dist();

  angle=100;
  // tourner tant que angle calcule non atteint
  while(dist_g<angle && dist_d<angle){
    
    rot_d();
 
    }
  
  stop_mv();
  delay(2000);
  raz_dist();
}









