#include "TCPDebugClass.h"

//------------------------------SETUP--------------------------------------
void setupTCP() {
  /* Starting debugging server */
  TCPDebug.begin();
  delay(100);
}
//-------------------------------------------------------------------------

//--------------------------FUNCTIONS--------------------------------------
//chek for clients to print all messages
void loopTCP() {
  if (!TCPDebug.hasClient())
    TCPDebug.checkForClient();
}

//function to send the message
void printTCP(String message) {
  TCPDebug.println(message);
}
//-------------------------------------------------------------------------
