
#include <Servo.h>


/*** ****************************************************** ***/
/***   Global variables and function definition  ***/
/*** ****************************************************** ***/
Servo rightServo;
Servo leftServo;

const int _PinRightIR = 3;
const int _PinLeftIR = 2;

const int _PinRightServo = 9;
const int _PinLeftServo = 6;

const int _WHITE = 1;
const int _BLACK = 0;

const int _STRAIGHT_TIME = 1000;

int previousMoveServoMillis = 0;
int currentMoveServoMillis  = 0;
int previousStraightMillis  = 0;
int currentStraightMillis   = 0;

int rightIR = -1;
int leftIR = -1;

int turnTo = -1;  // right:0; left:1; none:-1;

/*** ****************************************************** ***/
/***   Setup  ***/
/*** ****************************************************** ***/
void setup() {
  
  Serial.begin(9600);

  // Servos
  leftServo.attach(_PinLeftServo);
  rightServo.attach(_PinRightServo);
  leftServo.write(90);
  rightServo.write(90);
  // Infrared Sensor
  
  attachInterrupt(0,leftInterrupt,FALLING);   // Pin 2: _PinLeftIR
  //pinMode(_PinLeftIR, INPUT);
  attachInterrupt(1,rightInterrupt,FALLING);  // Pin 3: _PinRightIR
  //pinMode(_PinRightIR, INPUT);

  previousMoveServoMillis = millis();
    
}


/*** ****************************************************** ***/
/***   Loop  ***/
/*** ****************************************************** ***/
void loop() {
  
  rightIR = digitalRead(_PinRightIR);
  leftIR = digitalRead(_PinLeftIR);
  
  Serial.print("turnTo ..: ");  Serial.print(turnTo);   Serial.print("        "); 
  Serial.print("rightIR ..: "); Serial.print(rightIR);  Serial.print("        ");
  Serial.print("leftIR ..: ");  Serial.print(leftIR);   Serial.println("        ");
    Serial.flush();
  checkServos();
  


  // Los dos sobre blanco, avanza rápido
  if (rightIR == _WHITE && leftIR == _WHITE ) {

    // los IR están sobre blanco.
    // 1. Estan en una recta?
    // 2. Se ha salido de la pista por la derecha
    // 3. Se ha salido de la pista por la izquierda.
      /*
    if( (turnTo == -1)
        || (turnTo == 0 && leftIR == 0) 
        || (turnTo == 1 && rightIR == 0) ) {*/
      
        rightServo.write(180);   
        leftServo.write(0);
        //turnTo = -1;
    //}
      
    /*currentStraightMillis = millis();
    if(currentStraightMillis - previousStraightMillis > _STRAIGHT_TIME) {
      turnTo = -1;
    }
    previousStraightMillis = currentStraightMillis;      */
  }

  // Los sobre negro, ojo es una curva tan cerrada que los dos IR están sobre el negro.
  // Si es curva cerrada a izquieda, entonces el izquierdo va delante.
  // Si es curva cerrada a derecha, entonces el derecho va delante.
  // Debemos continuar haciendo la curva hacia el lado inicial de la curva.
  else if (rightIR == _BLACK && leftIR == _BLACK ) {
/*
      switch(privilegedIR) {
        case -1:
          // Error, nunca deberia producirse
          rightServo.write(90);   
          leftServo.write(90);
          break;

        case 0:          
          rightServo.write(90);   
          leftServo.write(45);
          break;

        case 1:          
          rightServo.write(90);   
          leftServo.write(45);
          break;
          
        default:
          // Error, nunca debería producirse
          rightServo.write(90);   
          leftServo.write(90);
          break;          
      }  */    
      // turnTo = turnTo; // No to el turnTo para que mantenga la curva en su sentido original
      rightServo.write(90); // delete line  
      leftServo.write(90);  // delete line
      return;
  }
  

  /*else if(turnTo == 0 && leftIR == _WHITE) {
      
    rightServo.write(90);   
    leftServo.write(0);
        
  }*/ else if (rightIR == _WHITE) {    
      rightServo.write(180); 
      leftServo.write(90); 
      
  }/* else if(turnTo == 1 && rightIR == _WHITE) {
      
    rightServo.write(180);   
    leftServo.write(90);
        
  }*/ else if (leftIR == _WHITE) {      
      rightServo.write(90);
      leftServo.write(0);
  }

}

/* ************************************************************* *
 *  Name ........: checkServos
 *  Descrition ..: evitamos esperas bloqueantes (delay) 
 *                comprobando si desde que se movio el ultimo servo
 *                han pasado los 10ms necesarios para que no 
 *                tropiece
 *  Arguments ...:
 * ************************************************************* */
void checkServos() {
  
  currentMoveServoMillis = millis();
  if ( currentMoveServoMillis - previousMoveServoMillis < 10 )
    delay(currentMoveServoMillis - previousMoveServoMillis);
  previousMoveServoMillis = currentMoveServoMillis;
}

/* ************************************************************* *
 *  Name ........: rightInterrupt
 *  Descrition ..: cuando el valor del IR  pasa de 0 a 1 RISSING
 *  Arguments ...:
 * ************************************************************* */
void rightInterrupt() { // FALLING 1 to 0 --> White to Black, car is out of line

  if(turnTo == -1)
    turnTo = 0; // turn to right

    Serial.print("rightInt...:");
    Serial.println(turnTo);
    Serial.flush();
  
}

/* ************************************************************* *
 *  Name ........: leftInterrupt
 *  Descrition ..: cuando el valor del IR  pasa de 0 a 1 RISSING
 *  Arguments ...:
 * ************************************************************* */
void leftInterrupt() { // FALLING 1 to 0 --> White to Black, car is out of line
        
    
  if(turnTo == -1)
    turnTo = 1; // turn to left
            
    Serial.print("leftInt...:");
    Serial.println(turnTo);
    Serial.flush();
}


