//-----------------------STATES OF THE BRAIN-------------------------------
#define CHECK_ANDROID_CHANGE_TAP    10
#define BRAIN_TEMPERATURE           0
#define EMERGENCY_FOR_TEMPERATURE   1
#define THERE_IS_AN_EMERGENCY       5
#define WAIT_UNTIL_EMERGENCY_ENDS   6
#define BRAIN_FALLEN_IN_THE_SHOWER  2
#define BRAIN_BUTTON_EMERGENCY      3
#define BRAIN_FIREBASE_POS_TAP      4
#define INIT_BRAIN                  7
//-------------------------------------------------------------------------

//--------------------------VARIABLES--------------------------------------
int stateBrain = INIT_BRAIN;
boolean emergencyBrain = false;
float tempBrain;
int posActualTap;
int newPosTap;
int valor;
//-------------------------------------------------------------------------


void printDebug(String msn) {
  TCPDebug(msn);
  SerialDebug(msn);
}

void brain() {
  switch (stateBrain) {
    case INIT_BRAIN:
      //read from firebase which is the actual position of the tap
      posActualTap = getFirebaseTap();
      newPosTap = getFirebaseTap();
      stateBrain = BRAIN_TEMPERATURE;
      break;

    case BRAIN_TEMPERATURE:
      //get temperature when time has passed
      if (getTempReady()) {
        tempBrain = getTemp();
        setFirebaseTemp(tempBrain);
        setTempReadyFalse();
      }
      stateBrain = EMERGENCY_FOR_TEMPERATURE;
      break;

    case EMERGENCY_FOR_TEMPERATURE:
      //if the temperature is too high then it's emergency, otherwise continue
      if (getEmergTemp()) {
        emergencyBrain = true;
        //send a message to the android app
        sendMessageEmergency("Temperature too high", String(tempBrain) + " ºC");
        stateBrain = THERE_IS_AN_EMERGENCY;
      }
      else {
        stateBrain = BRAIN_FALLEN_IN_THE_SHOWER;
      }
      break;

    case BRAIN_FALLEN_IN_THE_SHOWER:
      //check if the person has fallen in the shower so there is an emergency, otherwise continue
      if (getEmergFallen()) {
        setFirebasePresence();
        emergencyBrain = true;
        //send to the andorid app a message
        sendMessageEmergency("Shower", "Has fallen");
        stateBrain = THERE_IS_AN_EMERGENCY;
      }
      else {
        stateBrain = BRAIN_BUTTON_EMERGENCY;
      }
      break;

    case BRAIN_BUTTON_EMERGENCY:
      //if we have pressed the emergency button then there is an emergency, otherwise continue
      if (getEmergButton()) {
        emergencyBrain = true;
        //send to the android app a message
        sendMessageEmergency("Emergency", "Button emergency pressed");
        stateBrain = THERE_IS_AN_EMERGENCY;
      }
      else {
        stateBrain = BRAIN_FIREBASE_POS_TAP;
      }
      break;

    case BRAIN_FIREBASE_POS_TAP:
      //check if it is time to take the tap position from firebase
      if (getInfoFirebase()) {
        posActualTap = getFirebaseTap();
        setInfoFirebase(posActualTap);
      }

      //change the position of the tap to the firebase
      if (getSaveActualInfoTap()) {
        posActualTap = getPosTap();
        setFirebaseTap(posActualTap);
        setSaveActualInfoFalse();
      }
      if (emergencyBrain) {
        //if there is any kind of emergency then we go to emergency
        stateBrain = THERE_IS_AN_EMERGENCY;
      }
      else {
        //there is no emergency, continue
        stateBrain = CHECK_ANDROID_CHANGE_TAP;
      }
      break;

    case CHECK_ANDROID_CHANGE_TAP:
      //if the person presses the android tap button then the tap has to move (equals as normal buttons)
      //value from firebase is different from the local value
      newPosTap = getFirebaseTap();
      if (posActualTap > newPosTap) {
        //Move tap left
        moveTapLeftFromFirebase(posActualTap);
      }
      else if (posActualTap < newPosTap) {
        //Move tap right
        moveTapRightFromFirebase(posActualTap);
      }
      //refresh info
      posActualTap = newPosTap;
      stateBrain = BRAIN_TEMPERATURE;
      break;

    case THERE_IS_AN_EMERGENCY:
      setEmergencyTapTrue();
      stateBrain = WAIT_UNTIL_EMERGENCY_ENDS;
      break;

    case WAIT_UNTIL_EMERGENCY_ENDS:
      //when the emergency has stopped, change all the variables to false
      if (!getEmergTap()) {
        setEmergencyButtonFalse();
        setEmergencyTempFalse();
        setEmergencyFallenFalse();
        emergencyBrain = false;
      }
      //save the position of the tap because in any emergency --> close the tap
      stateBrain = BRAIN_FIREBASE_POS_TAP;
      break;

  }
}
