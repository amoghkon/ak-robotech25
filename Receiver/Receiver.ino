#include <esp_now.h>
#include <WiFi.h>
#include <Stepper.h>
#include <Wire.h>
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17
String success;
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);
typedef struct struct_message {
    int data;
} struct_message;
struct_message readings;
struct_message incomingReadings;
uint8_t broadcastAddress[] = {----, ----, ----, ----, ----, ----}; // mac address of receiver
int testData = 12;
int uncData;
 // change this to fit the number of steps per revolution
esp_now_peer_info_t peerInfo;

// Callback when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//   if (status ==0){
//     success = "Delivery Success :)";
//   }
//   else{
//     success = "Delivery Fail :(";
//   }
// }



// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  uncData = incomingReadings.data;
}

void setup() {
  myStepper.setSpeed(10);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  //esp_now_register_send_cb(OnDataSent);
  
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
int mno = 0;
void loop() {
  
  readings.data = testData;
  //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &readings, sizeof(readings));
   
  // if (result == ESP_OK) {
  //   //Serial.println("Sent with success");
  //   Serial.println(uncData);
  // }
  // else {
  //   //Serial.println("Error sending the data");
  // }
  // delay(1000);
  //Serial.println(uncData);
  if(uncData){
    Serial.println("Door open");
    uncData = 0;
    Serial.println(mno);
    mno++;
    Serial.println("clockwise");
    myStepper.step(2*stepsPerRevolution);
    delay(10000);
    myStepper.step(-2*stepsPerRevolution);
  }

}
