#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Access Point"
#define WIFI_PASSWORD "83137309"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDlyGxi6E6zV-xy78HTRMZONCo3-m_Hpq4"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "fasty-8e90c-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//some importent variables
String sValue, sValue2;
bool signupOK = false;
const int ENA = D5;
const int IN1 = D8;
const int IN2 = D7;
const int IN3 = D4;
const int IN4 = D3;
const int ENB = D6;


void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  auth.user.email = "car@car.com";
  auth.user.password = "carcar";
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK ) {
    if (Firebase.RTDB.getString(&fbdo, "/command")) {
      if (fbdo.dataType() == "string") {
        sValue = fbdo.stringData();
        int a = sValue.toInt();
        Serial.println(a);
        if (a == 1){
          // Forward
      analogWrite(ENA, 1023); // Full speed
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENB, 1023); // Full speed
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

        } 
         if (a == -1){
      // Backward
      analogWrite(ENA, 1023); // Full speed
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENB, 1023); // Full speed
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);


        } 
         if (a == 3){
        // Right
      analogWrite(ENA, 1023); // Full speed
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENB, 1023); // Full speed
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

        }  
        if (a == 2){
      // Left
      analogWrite(ENA, 1023); // Full speed
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENB, 1023); // Full speed
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);


        } 
         if (a == 0){
      // Stop
      analogWrite(ENA, 0); // Stop motor A
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENB, 0); // Stop motor B
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
}
}
