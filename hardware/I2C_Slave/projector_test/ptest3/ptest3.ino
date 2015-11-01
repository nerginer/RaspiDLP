#include <b9SoftwareSerial.h>
#include <PinChangeInt.h>

#include <Wire.h>
#define SLAVE_ADDRESS 0x04


// Projector's RS-232 Port I/O pins
#define PROJECTOR_RX 12
#define PROJECTOR_TX 13
bool bIsMirrored = false;
// Projector RS-232 interface & status variables
b9SoftwareSerial projectorSerial = b9SoftwareSerial(PROJECTOR_RX, PROJECTOR_TX);
String sData;
String command="";

void setup() {
pinMode(13, OUTPUT);
//I2C
Wire.begin(SLAVE_ADDRESS);
// define callbacks for i2c communication
Wire.onReceive(receiveData);
Wire.onRequest(sendData);

//I2C END


// Projector RS-232 serial I/O
pinMode(PROJECTOR_RX, INPUT);
PCintPort::attachInterrupt(PROJECTOR_RX, &projectorRxChange, CHANGE);
pinMode(PROJECTOR_TX, OUTPUT); 


Serial.begin(115200); 

// set the data rate for the b9SoftwareSerial port
projectorSerial.begin(9600);

Serial.println("Gnexlab DLP Controller v0.1");  
  
  
}

void loop() { // run over and over
//HandleProjectorComm_Vivitek_D535(); 
delay(100);
}

void projectorRxChange() {cli(); projectorSerial.do_interrupt(); sei();}


void projectorOFF(){
  Serial.println("Command: Turning Projector OFF"); 
  projectorSerial.write("~PF\r");
  delay(100);
}

void projectorON(){
  Serial.println("Command: Turning Projector ON"); 
  projectorSerial.write("~PN\r");
  delay(100);
}


void projectorReset(){
    projectorSerial.write("~rM\r");   //MENU
    delay(100);
    projectorSerial.write("~rL\r");   //LEFT
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN1
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN2
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN3
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN4
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN5
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN6
    delay(100);
    projectorSerial.write("~rD\r");   //DOWN7
    delay(100);
    projectorSerial.write("~rR\r");   //RIGHT 
    delay(1000);
    projectorSerial.write("~sB100\r");  // Vivitek D535 set brightness 100%
    projectorSerial.write("~sC100\r");  // Vivitek D535 set contrast 100%
    projectorSerial.write("~sR50\r");   // Vivitek D535 set Color 50%
    projectorSerial.write("~sN50\r");   // Vivitek D535 set Tint 50%
    projectorSerial.write("~sT1\r");    // Vivitek D535 set color Temperature Normal  ( 0:Cold 1:Normal 2:Warm )         
    projectorSerial.write("~sA4\r");    // Vivitek D535 set aspect (Scaling) to Native ( 0:Fill, 1:4to3, 2:16to9, 3:Letter Box, 4:Native )
    if(bIsMirrored)
      projectorSerial.write("~sJ1\r");  // Vivitek D535 set Projetion Mode to Back ( 0:Front 1:Rear 2:Rear+Ceiling 3:Ceiling )
      else
      projectorSerial.write("~sJ0\r");  // Vivitek D535 set Projetion Mode to Front ( 0:Front 1:Rear 2:Rear+Ceiling 3:Ceiling )            
  
}

void HandleProjectorComm_Vivitek_D535() {
    
  // Handle the projector's transmission
  
  char c;
  sData ="";
  while(projectorSerial.available() > 0) {
    // Yes, we have data available
    c = projectorSerial.read();
    // If it is newline or we find a variable separator, then terminate
    // and leave the otherwise infinite loop, otherwise add it to sData
    if (c == '\r' || c == '\n' || c == '\0') break; else sData += String(c);
  }
   
  if (sData!=""){
    Serial.println("Data:" + sData);
  }
    
  }


//I2C




// callback for received data
void receiveData(int byteCount){

  
  while( Wire.available()){
  command += (char)Wire.read();
  
  }

  
  Serial.print("I2C received: ");
  Serial.println(command);

  if (command == "~PF\r"){
    projectorOFF();
  }
  
  if (command == "~PN\r"){
    projectorON();
  }
  

}

// callback for sending data
void sendData(){
 HandleProjectorComm_Vivitek_D535();  
 Wire.write(sData.c_str());
}



