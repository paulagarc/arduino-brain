//---------------------DEFINES_FIREBASE-----------------------------------
#define firebase_HOST "e-healt-androidapp.firebaseio.com"
#define firebase_AUTH "lubPlWBIOglWBuFIvA7NGeW9q1iDdfXsD7mcl43Q"
#define FIREBASE_FCM_SERVER_KEY "AAAAn84FwXk:APA91bGRVfoPmIqTPqq7RDqoMS8AatrAVWtKNzrjXIaiQROtZZdVMuDIqoqSYVwZNsMu_GCxup6_lMjyjYxy8GYdFY6vTI4xiBze9dDjOS5v0w1mIzaGRIfuXZBt36B-wA5KMxS9tL0R"
//-------------------------------------------------------------------------

//------------------------------SETUP--------------------------------------
void fire_base() {

  Firebase.begin(firebase_HOST, firebase_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  firebaseData.fcm.begin(FIREBASE_FCM_SERVER_KEY);
  firebaseData.fcm.setPriority("high");
  firebaseData.fcm.setTimeToLive(1000);
}
//-------------------------------------------------------------------------

String getFirebaseToken() {
  //Get the correct token of the firebase to connect for cloud messaging 
  String path = "ControlParameters/";
  path += HARDWARE_NUM;
  path += "/token";

  Firebase.getString(firebaseData, path);
  return firebaseData.stringData();
}

void sendMessageEmergency(String typeMessage, String messageToSend) {
  //Send message for cloud messaging
  printDebug("Send Firebase Cloud Messaging...");

  String token = getFirebaseToken();
  firebaseData.fcm.clearDeviceToken();
  firebaseData.fcm.addDeviceToken(token);
  
  //the type of message will be this one: 
  firebaseData.fcm.setNotifyMessage(typeMessage, messageToSend);
  firebaseData.fcm.setDataMessage("{\"myData\":\"myValue\"}");

  //send message to recipient index 0
  if (Firebase.sendMessage(firebaseData, 0)) {
    printDebug("PASSED");
    printDebug(firebaseData.fcm.getSendResult());
  }
  else {
    printDebug("FAILED");
    printDebug("REASON: " + firebaseData.errorReason());
  }
}


void setFirebaseTemp(float valorT) {
  //set temperature in firebase
  
  //create path to insert to firebase
  String path = "ControlParameters/";
  path += HARDWARE_NUM;
  path += "/temperature";

  Firebase.setFloat(firebaseData, path, valorT);
}

void setFirebasePresence() {
  //set in firebase that the person has fallen in the shower
  String path = "ControlParameters/";
  path += HARDWARE_NUM;
  path += "/fall";
  
  Firebase.setString(firebaseData, path, "true");
}

void setFirebaseTap(int valorTap) {
  //set in firebase the tap position
  String path = "ControlParameters/";
  path += HARDWARE_NUM;
  path += "/waterAmount";
  
  Firebase.setInt(firebaseData, path, valorTap);
}

int getFirebaseTap() {
  //get tap position from firebase
  String path = "ControlParameters/";
  path += HARDWARE_NUM;
  path += "/waterAmount";

  Firebase.getInt(firebaseData, path);
  return firebaseData.intData();
}
