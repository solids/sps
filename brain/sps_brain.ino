#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(2, OUTPUT); 
  digitalWrite(2, LOW);
  
  // Setup TWI
  // Prepare to start assigning addresses to nodes

  // Turn the first node in the chain on  

  digitalWrite(2, HIGH);
  delay(100);
}

uint8_t find_nodes = 1, currentNode = 1;
void loop() {
  if (find_nodes) {
    Serial.println("finding nodes");
    uint8_t bytesRead = Wire.requestFrom(0, 1, 1);
    Serial.print(bytesRead);
    Serial.println(" from TWI bus");
    
    // Looks like there is a device on the network
    // without an address, give it an address
    if (bytesRead > 0) {
      
      Serial.print("giving next node in the chain address #");
      Serial.println(currentNode);
      
      Wire.beginTransmission(0);  // Give the node at #0 new addy
      Wire.write(currentNode);  // new address
      Wire.endTransmission();     // stop transmitting
    
      Serial.println("confirming address");
      // Confirm the new address took
      delay(1);
      bytesRead = Wire.requestFrom((int)currentNode, 1, 1);
      if (bytesRead > 0) {      
        Serial.println("address confirmed");
        currentNode++;
      }
    } else {
      find_nodes = 0;
      // TODO: enable hot plugging
      Serial.print("done detecting nodes, found ");
      Serial.println(currentNode-1);
    }
  }
  
  // TODO: parser for controlling different buttons and collecting data from sensors
  
}
