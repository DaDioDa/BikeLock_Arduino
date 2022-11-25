
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>


#define WIFI_SSID "ZenFone6_4475"
#define WIFI_PASSWORD "95279527"

#define API_KEY "AIzaSyCZocBwNLrAATvO0O__NzjdjkBET3NaEA4"
#define DATABASE_URL "https://bikelock-6b19b-default-rtdb.asia-southeast1.firebasedatabase.app/"

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;

//設定卡片
bool SetUserCard(String CardID)
{
  if (Firebase.RTDB.setString(&fbdo, "ParkingLot/Lot_001/UserCard", CardID)) {
    Serial.println("Set Card Success");
    return true;    
  }
  else {
    Serial.println(fbdo.errorReason());
    return false;
  }
}

//讀取卡片
String GetUserCard()
{
  if (Firebase.RTDB.getString(&fbdo, "ParkingLot/Lot_001/UserCard")) {
      Serial.println("Get Card Success");
      return fbdo.stringData();
    }
    else {
      Serial.println(fbdo.errorReason());
      return "Failed";
    }
}

//設定Wifi和資料庫連結
void SetUpWiFi()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.database_url = DATABASE_URL;
  config.api_key = API_KEY;


  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup() {
  SetUpWiFi();
}
void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    //if(SetUserCard("aa-bb-cc-dd"))
    //{
    //  Serial.println("Loop yes");
    //}
    Serial.println(GetUserCard());
  }
}
