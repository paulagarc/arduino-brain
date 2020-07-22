#define MAXTEMP 40

//-----------------------STATES OF THE TEMPERATURE-------------------------
#define STATE_GET_TEMPERTATURE      0
#define COMPARE_TO_SEE_EMERGENCY    1
//-------------------------------------------------------------------------

//--------------------------VARIABLES--------------------------------------
int pinTEMP;
unsigned long prevTimeTEMP;
float temperatureC = 35;
float temperatureOld;
const int oneWireBus = 4;
int stateTemp = STATE_GET_TEMPERTATURE;
boolean emergencyTemp = false;
boolean tempReady = false;
int counter = 0;
//-------------------------------------------------------------------------

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinTEMP);
DallasTemperature sensors(&oneWireObjeto);

//--------------------------SETUP------------------------------------------
void setupTEMP(int pinT) {
  pinTEMP = pinT;
  pinMode(pinTEMP, INPUT);
  sensors.begin();
  prevTimeTEMP = millis();
}
//-------------------------------------------------------------------------

void setTempReadyFalse() {
  tempReady = false;
}

void setEmergencyTempFalse() {
  emergencyTemp = false;
}

boolean getEmergTemp() {
  return emergencyTemp;
}

boolean getTempReady() {
  return tempReady;
}

float getTemp() {
  return temperatureC;
}


void calculaTEMP() {
  switch (stateTemp) {

    case STATE_GET_TEMPERTATURE:
      //the 30sec has passed
      if (prevTimeTEMP + 30000 < millis()) {
        prevTimeTEMP = millis();
        //get the temperature from the sensor
        sensors.requestTemperatures();
        //save the temperature to compare later
        temperatureOld = temperatureC;
        //        temperatureC = sensors.getTempCByIndex(0);

        temperatureC++;
        if (temperatureC == 43) {
          temperatureC == 35;
        }

        printDebug(String(temperatureC) + " ºC");
        stateTemp = COMPARE_TO_SEE_EMERGENCY;
      }
      break;

    case COMPARE_TO_SEE_EMERGENCY:
      //if the temperature has changed then we have to save it on the firebase
      if (temperatureC != temperatureOld) {

        tempReady = true;

        if (temperatureC >= MAXTEMP) {
          emergencyTemp = true;
        }
      }
      stateTemp = STATE_GET_TEMPERTATURE;
      break;
  }
}
