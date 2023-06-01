{
#include <Servo.h>
#include "Ultrasonic.h"

// Déclaration des servo-moteurs :
Servo midrota;
Servo eyelid;

// Valeur capteur :
Ultrasonic ultrasonic_droit(2, 3);
Ultrasonic ultrasonic_milieu(4, 5);
Ultrasonic ultrasonic_gauche(7, 6);

// Distance avant détéction :
const int cm = 100;
int offmode = 1;
int where;
int angleS;
boolean keepOnGoing = true;

void setup() {
  // Pin des servo-moteurs :
  midrota.attach(A1);
  eyelid.attach(A2);
  
  // Définir l'angle de base :  
  midrota.write(90);
  eyelid.write(0);
}

void loop() {
  // Mode veille -> Réveil
  do {
    if (whereisit() != 0) {
      where = whereisit();
      angleS = calibrage();
      eyelid.write(90);
      for (int i=0; i<=3 && keepOnGoing; i++) {
        if (whereisit() == 0) {
          keepOnGoing = false;
        }
        delay(1000);
      }
      if (keepOnGoing) {
        midrota.write(angleS);
        offmode = 0;
      } else {
        eyelid.write(0);
      }
    }
    keepOnGoing = true;
    delay(100);
  } while (offmode == 1);
  do {
    where = whereisit();
    while (whereisit() == 0) {
      for (int i=0; i<=80; i++) {
        delay(100);
        if (i == 80) {
          offmode = 1;
        }
      }
    }
    angleS = calibrage();
    midrota.write(angleS);
    delay(100);
  } while (offmode == 0);
  delay(100);
}

int whereisit() {
  int distance1 = ultrasonic_droit.Ranging(CM);
  int distance2 = ultrasonic_milieu.Ranging(CM);
  int distance3 = ultrasonic_gauche.Ranging(CM);

  if ((distance1 <= cm) && (distance2 > cm) && (distance3 > cm)) {
    return 1;
  } else if ((distance1 > cm) && (distance2 <= cm) && (distance3 > cm)) {
    return 2;
  } else if ((distance1 > cm) && (distance2 > cm) && (distance3 <= cm)) {
    return 3;
  } else if ((distance1 <= cm) && (distance2 <= cm) && (distance3 > cm)) {
    return 4;
  } else if ((distance1 > cm) && (distance2 <= cm) && (distance3 <= cm)) {  
    return 5;
  } else if ((distance1 > cm) && (distance2 > cm) && (distance3 > cm)) {
    offmode = 1;
    return 0;
  }
}

int calibrage() {
  if (where == 1) {
    return 30;
  } else if (where == 2 || where == 0) {
    return 90;
  } else if (where == 3) {
    return 150;
  } else if (where == 4) {
    return 60;
  } else if (where == 5) {
    return 120;
  }
}
}
