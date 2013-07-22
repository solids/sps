#include <Wire.h>
#include <stdio.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(2, OUTPUT); 
  digitalWrite(2, LOW);
  delay(100);
  // Turn the first node in the chain on  
  digitalWrite(2, HIGH);
  delay(100);
  Serial.println("Locating nodes..");
}

uint8_t find_nodes = 1;
int currentNode = 1;
int bufloc = 0;
char val[255];

void rec() {
  bufloc=0;
  while(Wire.available()) {   
    byte i = Wire.read();

    val[bufloc] = i;
    bufloc++; 
  }
  val[bufloc] = 0;
}

void print_rec() {
  char buf[4];
  int i=0;
  for (i; i<bufloc; i++) {
    sprintf(buf,"0x%02x", val[i]); 
    Serial.print(buf);
  }
}

void loop() {
  if (find_nodes) {
    Wire.beginTransmission(0x00);  // Give the node at #1 new addy
    Wire.write(currentNode);  // new address
    int res = Wire.endTransmission();     // stop transmitting
    delay(70);
    Wire.requestFrom(currentNode, 1, 1);
    rec();   
    Serial.println(val);
    if (res > 0 || val[0] != currentNode) {
      Serial.print("found ");
      Serial.print(currentNode-1);
      Serial.println(" nodes ");
      find_nodes = 0;
    } else {
      Serial.print("found node @ ");
      Serial.print(currentNode);
      Serial.print(" verify=");
      print_rec();
      currentNode++;
    }
  }
  // TODO: parser for controlling different buttons and collecting data from sensors
}
