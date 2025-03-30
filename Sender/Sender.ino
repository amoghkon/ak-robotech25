#define BLYNK_TEMPLATE_ID  "-----------"
#define BLYNK_TEMPLATE_NAME  "-------"
#include <esp_now.h>
#include <WiFi.h>
#include <WifiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Keypad.h>
#include "SPIFFS.h"
#include <LiquidCrystal.h>
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns

LiquidCrystal lcd(32, 33, 25, 26, 27, 14);

bool arrsequal(char arr1[], char arr2[]) {
  for (int j = 0 ; j < 6 ; j++){
    if (arr1[j] != arr2[j]) {
      return false;
    }
  }
  return true;
}

char authToken[33]{"-----------------------"};
char ssid[12]{"----------"};
char pass[9]{"-------"};




char currentInput[6]{'\0', '\0', '\0', '\0', '\0', '\0'};
char correctPassword[6]{'2', '0', '8', '5', '0', '\0'};
int counter = 0;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

#include <Wire.h>
String success;
typedef struct struct_message {
    int data;
} struct_message;
struct_message readings;
struct_message incomingReadings;
uint8_t broadcastAddress[] = {----, ----, ----, ----, ----, ----}; // mac address of receiver
int testData = 24;
int uncData;

esp_now_peer_info_t peerInfo;

BLYNK_WRITE(V0)
{
  String pinValue = param.asString(); // assigning incoming value from pin V1 to a variable

  // process received value
  pinValue.toCharArray(correctPassword, 6);
  Serial.println(correctPassword);
  lcd.clear();
  lcd.print("Password changed!");
  
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}



// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  uncData = incomingReadings.data;
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Enter passcode, * to clear: ");
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  Blynk.begin(authToken, ssid, pass);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  Blynk.run();
  char key = keypad.getKey();

  if (key) {
    
    
    if(key == '*') {
      for (int j = 0; j < 5 ; j++) {
        currentInput[j] = '\0';
      }
      counter = 0;
      Serial.println(currentInput);
      lcd.clear();
      lcd.print("Enter passcode: * to clear");
    }
    else {
      currentInput[counter] = key;
      Serial.println(currentInput);
      lcd.clear();
      lcd.print(currentInput);
      counter++;
      if (counter == 5){
        if(arrsequal(currentInput, correctPassword)){
          Serial.println("Correct password!");
          lcd.clear();
          lcd.print("Correct Password!");
          // Insert code to send data
          readings.data = testData;
          esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &readings, sizeof(readings));
          
          if (result == ESP_OK) {
            Serial.println("Sent with success");
            Serial.println(uncData);
            lcd.clear();
            lcd.print("Unlocking Door!");
            Blynk.logEvent("door_unlock");
          }
          else {
            Serial.println("Error sending the data");
            lcd.clear();
            lcd.print("Failure unlocking Door!");
          }
           
          for (int j = 0; j < 6 ; j++) {
            currentInput[j] = '\0';
          }
          counter = 0;
        }
        else{
          Serial.println(("Wrong password! Try again!"));
          lcd.clear();
          lcd.print("Wrong password! Try Again.");
          for (int j = 0; j < 6 ; j++) {
            currentInput[j] = '\0';
          }
          counter = 0;
          Serial.println(currentInput);

        }
      }
    }
  }
  // ***********************************************
  // **********************************************
  


}