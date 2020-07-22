//--------------------------VARIABLES--------------------------------------
int pinEmergency;
boolean buttonIsEmergency = false;
//-------------------------------------------------------------------------

//--------------------------SETUP------------------------------------------
void setupButtonEmerg(int emerg) {
  pinEmergency = emerg;
  pinMode(pinEmergency, INPUT);
}
//-------------------------------------------------------------------------

void setEmergencyButtonFalse() {
  buttonIsEmergency = false;
}

boolean getEmergButton() {
  return buttonIsEmergency;
}

void isEmergency() {
  if (digitalRead(pinEmergency) == 1) {
    buttonIsEmergency = true;
  }
}
