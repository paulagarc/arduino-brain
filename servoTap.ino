#define PERCENTAGE_POS 25
#define MAXLOOP 4
//-----------------------STATES OF THE SERVO TAP---------------------------
#define STATE_GET_FIREBASE      6
#define MOVEMENT_FROM_FIREBASE  5
#define MOVEMENT_FROM_PULSE     0
#define TURN_TAP_RIGHT          1
#define TURN_TAP_LEFT           2
#define STATE_TAP_EMERGENCY     4
#define SAVE_ACTUAL_POS_TAP     3
//-------------------------------------------------------------------------

//--------------------------VARIABLES--------------------------------------
int pinTap;
int pulsRight;
int pulsLeft;
unsigned long prevTimeTap;
unsigned long actualTime;
int stateTap = MOVEMENT_FROM_PULSE;
Servo servoTap;
int numLoop;
boolean emergencyTap = false;
int posActual;
boolean saveActualInfo = false;
boolean in = false;
boolean infoFirebase = false;
boolean tapLeft = false;
boolean tapRight = false;
boolean firebaseMovement = false;
//-------------------------------------------------------------------------

//--------------------------SETUP------------------------------------------
void setupTap(int tap, int right, int left) {
  pinTap = tap;
  pulsRight = right;
  pulsLeft = left;
  pinMode(pulsRight, INPUT);
  pinMode(pulsLeft, INPUT);
  servoTap.attach(tap);
  servoTap.write(90);
}
//-------------------------------------------------------------------------

boolean getEmergTap() {
  return emergencyTap;
}

boolean getSaveActualInfoTap() {
  return saveActualInfo;
}

int getPosTap() {
  return posActual;
}

void setSaveActualInfoFalse() {
  saveActualInfo = false;
}

void setEmergencyTapFalse() {
  emergencyTap = false;
}

void setEmergencyTapTrue() {
  emergencyTap = true;
}

boolean getInfoFirebase() {
  return infoFirebase;
}

void setInfoFirebase(int posActual) {
  numLoop = posActual / PERCENTAGE_POS;
  infoFirebase = false;
}

void moveTapLeftFromFirebase(int posActual) {
  tapLeft = true;
  tapRight = false;
  numLoop = posActual / PERCENTAGE_POS;
  firebaseMovement = true;
}

void moveTapRightFromFirebase(int posActual) {
  tapRight = true;
  tapLeft = false;
  numLoop = posActual / PERCENTAGE_POS;
  firebaseMovement = true;
}


void motorTap() {
  switch (stateTap) {
    case MOVEMENT_FROM_PULSE:

      //set the servo without movement
      servoTap.write(90);

      //pressed right button
      if (digitalRead(pulsRight) == 1) {
        infoFirebase = true;
        stateTap = STATE_GET_FIREBASE;
        tapRight = true;
      }
      //pressed left button
      else if (digitalRead(pulsLeft) == 1) {
        infoFirebase = true;
        tapLeft = true;
        stateTap = STATE_GET_FIREBASE;
      }
      //pressed the emergency button
      else if (getEmergTap()) {
        infoFirebase = true;
        stateTap = STATE_GET_FIREBASE;
      }
      else {
        stateTap = MOVEMENT_FROM_FIREBASE;
      }
      break;

    case STATE_GET_FIREBASE:
      //get info from firebase to start rotation
      if (infoFirebase == false) {
        if (tapRight == true) {
          actualTime = millis();
          stateTap = TURN_TAP_RIGHT;
          tapRight = false;
          printDebug("RIGHT");
        }
        else if (tapLeft == true) {
          actualTime = millis();
          tapLeft = false;
          stateTap = TURN_TAP_LEFT;
          printDebug("LEFT");
        }
        else if (getEmergTap()) {
          actualTime = millis();
          stateTap = STATE_TAP_EMERGENCY;
          if (in == false) {
            printDebug("EMERGENCY");
            in = true;
          }
        }
      }
      break;

    case TURN_TAP_RIGHT:
      //when MAXLOOP then there can't be more right loops
      if (numLoop >= MAXLOOP) {
        stateTap = MOVEMENT_FROM_PULSE;
        in = false;
      }
      else {
        //turn right (opening the tap), one loop means 2 seconds
        if (actualTime + 2000 < millis()) {
          numLoop++;
          stateTap = SAVE_ACTUAL_POS_TAP;
        }
        else {
          //put the servo with right movement
          servoTap.write(135);
        }
      }
      break;

    case TURN_TAP_LEFT:
      //when 0 then there can't be more left loops
      if (numLoop <= 0) {
        stateTap = MOVEMENT_FROM_PULSE;
      }
      else {
        //turn left (closing the tap), one loop means 2 seconds
        if (actualTime + 2000 < millis()) {
          numLoop--;
          stateTap = SAVE_ACTUAL_POS_TAP;
        }
        else {
          //put the servo with left movement
          servoTap.write(45);
        }
      }
      break;

    case SAVE_ACTUAL_POS_TAP:
      //save actual position to change in firebase
      posActual = numLoop * PERCENTAGE_POS;
      saveActualInfo = true;

      if (emergencyTap == true) {
        stateTap = STATE_TAP_EMERGENCY;
      }
      else {
        stateTap = MOVEMENT_FROM_PULSE;
      }

      stateTap = MOVEMENT_FROM_PULSE;
      break;

    case STATE_TAP_EMERGENCY:
      //when 0 then the tap is closed
      if (numLoop <= 0) {
        setEmergencyTapFalse();
        stateTap = SAVE_ACTUAL_POS_TAP;
      }
      else {
        //turn left (closing the tap)
        if (actualTime + 2000 < millis()) {
          numLoop--;
          stateTap = SAVE_ACTUAL_POS_TAP;
        }
        else {
          //put the servo with left movement (closing)
          servoTap.write(45);
        }
      }
      break;

    case MOVEMENT_FROM_FIREBASE:
      //see if there is movement from firebase
      if (firebaseMovement && tapLeft) {
        //android left button pressed
        stateTap = TURN_TAP_LEFT;
        firebaseMovement = false;
        actualTime = millis();
      }
      else if (firebaseMovement && tapRight) {
        //android right button pressed
        stateTap = TURN_TAP_RIGHT;
        firebaseMovement = false;
        actualTime = millis();
      }
      else {
        stateTap = MOVEMENT_FROM_PULSE;
      }
      break;
  }
}
