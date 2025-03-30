#include <Keypad.h>
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
bool arrsequal(char arr1[], char arr2[]) {
  for (int j = 0 ; j < 7 ; j++){
    if (arr1[j] != arr2[j]) {
      return false;
    }
  }
  return true;
}

char currentInput[7]{'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
char correctPassword[7]{'2', '0', '8', '5', '0', '6', '\0'};
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

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  char key = keypad.getKey();

  if (key) {
    
    
    if(key == '*') {
      for (int j = 0; j < 6 ; j++) {
        currentInput[j] = '\0';
      }
      counter = 0;
      Serial.println(currentInput);
    }
    
    else {
      currentInput[counter] = key;
      Serial.println(currentInput);
      counter++;
      if (counter == 6){
      if(arrsequal(currentInput, correctPassword)){
        Serial.println("Correct password!");
        for (int j = 0; j < 7 ; j++) {
          currentInput[j] = '\0';
        }
        counter = 0;
      }
      else{
        Serial.println(("Wrong password! Try again!"));
        for (int j = 0; j < 7 ; j++) {
          currentInput[j] = '\0';
        }
        counter = 0;
        Serial.println(currentInput);

      }
    }
    }
    
    
  }
  
}