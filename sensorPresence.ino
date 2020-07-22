//-------------------STATES OF THE PRESENCE SENSOR-------------------------
#define ENTER_TO_THE_SHOWER           0
#define PERSON_IN_THE_SHOWER          1
#define OUT_OR_FALLEN_IN_THE_SHOWER   2
#define WAIT_OUT_THE_SHOWER           3
//-------------------------------------------------------------------------

//--------------------------VARIABLES--------------------------------------
boolean hasFallen = false;
int pinUp;
int pinDown;
int valUp = 0;
int valDown = 0;
int down = 0;
int up = 0;
int stateFallen = ENTER_TO_THE_SHOWER;
boolean emergencyFallen = false;
//-------------------------------------------------------------------------

//--------------------------SETUP------------------------------------------
void setupPresence(int pin1, int pin2) {
  pinUp = pin1;
  pinDown = pin2;
  pinMode(pinUp, INPUT);
  pinMode(pinDown, INPUT);
}
//-------------------------------------------------------------------------

int motionUp() {
  valUp = digitalRead(pinUp);  // read input value
}

int motionDown() {
  valDown = digitalRead(pinDown);  // read input value
}

boolean getEmergFallen() {
  return emergencyFallen;
}

void setEmergencyFallenFalse() {
  emergencyFallen = false;
}


void motorFallen() {

  //read the value of the pins
  up = motionUp();
  down = motionDown();

  switch (stateFallen) {
    case ENTER_TO_THE_SHOWER:
      //if both pins are 1 then there is a detection so a person has enter
      if (up == 1 && down == 1) {
        printDebug("SHOWER");
        stateFallen = PERSON_IN_THE_SHOWER;
      }
      break;

    case PERSON_IN_THE_SHOWER:
      //if there is no motion then means the person is still in the shower
      if (up == 0 && down == 0) {
        stateFallen = OUT_OR_FALLEN_IN_THE_SHOWER;
      }
      break;

    case OUT_OR_FALLEN_IN_THE_SHOWER:
      //if both pins are 1 then there is a detection so the person has gone out the shower
      if (up == 1 && down == 1) {
        printDebug("SHOWER OUT");
        stateFallen = WAIT_OUT_THE_SHOWER;
      }
      //if just the the down pin has detection then the person has fallen
      else if (down == 0 && up == 1) {
        printDebug("FALLEN");
        emergencyFallen = true;
        stateFallen = WAIT_OUT_THE_SHOWER;
      }
      break;

    case WAIT_OUT_THE_SHOWER:
      //wait until another person enters the shower
      if (up == 0 && down == 0) {
        stateFallen = ENTER_TO_THE_SHOWER;
      }
      break;
  }
}
