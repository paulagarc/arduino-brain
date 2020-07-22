//--------------------------DEFINES_WIFI-----------------------------------
#define WIFI_SSID "WLAN_8B09"
#define WIFI_PASSWORD "bfd5792f6a6e7b9874c3"
//-------------------------------------------------------------------------

//------------------------------SETUP--------------------------------------
void connectWifi()
{
  printDebug("");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }
  printDebug("Iniciado STA: ");
  printDebug(String(WIFI_SSID));
  printDebug("IP address: "); 
  printDebug(WiFi.localIP().toString()); 
}
//-------------------------------------------------------------------------
