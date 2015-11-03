#include <b9SoftwareSerial.h>
#include <PinChangeInt.h>

// Projector's RS-232 Port I/O pins
#define PROJECTOR_RX 12
#define PROJECTOR_TX 13
bool bIsMirrored = false;
// Projector RS-232 interface & status variables
b9SoftwareSerial projectorSerial = b9SoftwareSerial(PROJECTOR_RX, PROJECTOR_TX);

void setup() {

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
HandleProjectorComm_Vivitek_D535(); 
delay(200);
}

void projectorRxChange() {cli(); projectorSerial.do_interrupt(); sei();}

void serialEvent() {

    if(Serial.available()) {
        char cInChar;
        int i;
        
        // get the new byte:
        cInChar = (char)Serial.read(); 
        switch (cInChar)  {
    
           
      case 'p':     // Projector Power & Mode commands
      case 'P':
        i = SerialReadInt();
        if(i==0){
          Serial.println("Command: Turning Projector OFF"); 
          projectorSerial.write("~PF\r");
        }
        else if(i==1){
          Serial.println("Command: Turning Projector ON"); 
          projectorSerial.write("~PN\r");
        }
        
        else if(i==2){
          Serial.println("Command: Reseting Projector"); 
          projectorSerial.write("~sB100\r");  // Vivitek D535 set brightness 100%
          projectorSerial.write("~sC100\r");  // Vivitek D535 set contrast 100%
          projectorSerial.write("~sR50\r");  // Vivitek D535 set Color 50%
          projectorSerial.write("~sN50\r");  // Vivitek D535 set Tint 50%
          projectorSerial.write("~sT1\r");  // Vivitek D535 set color Temperature Normal  ( 0:Cold 1:Normal 2:Warm )         
          projectorSerial.write("~sA4\r");  // Vivitek D535 set aspect (Scaling) to Native ( 0:Fill, 1:4to3, 2:16to9, 3:Letter Box, 4:Native )
          if(bIsMirrored)
            projectorSerial.write("~sJ1\r");  // Vivitek D535 set Projetion Mode to Back ( 0:Front 1:Rear 2:Rear+Ceiling 3:Ceiling )
            else
            projectorSerial.write("~sJ0\r");  // Vivitek D535 set Projetion Mode to Front ( 0:Front 1:Rear 2:Rear+Ceiling 3:Ceiling )            
        }
        else if(i==3){
          Serial.println("Command: Set Projection Mode: Front"); 
          projectorSerial.write("~sJ0\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = false;
        }
        else if(i==4){
          Serial.println("Command: Set Projection Mode: Rear"); 
          projectorSerial.write("~sJ1\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = true;
        }
        else if(i==5){
          Serial.println("Command: Set Projection Mode: Ceiling"); 
          projectorSerial.write("~sJ2\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = false;          
        }
        else if(i==6){
          Serial.println("Command: Set Projection Mode: Rear + Ceiling"); 
          projectorSerial.write("~sJ3\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = true;          
        }
        else if(i==7){
          Serial.println("Command: Projector Factory Reset"); 
          projectorReset();
        }
        else if(i==11){
          Serial.println("Open Menu");
          projectorSerial.write("~rM\r");   //MENU
          delay(100); 
        }
        
        else if(i==13){
          Serial.println("Command: Lamp Hour"); 
          projectorSerial.write("~qL\r"); //lamp query
          delay(100);
        }
      break; 
        
      default:
      break;     
         
         
         
         
         
          
        }//switch
    }//serial.available
}//serialEvent



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
  String sData;
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


///////////////////////////////////////////////////////
//
// pull and return an int off the serial stream
//
int SerialReadInt() {
  // The string we read from Serial will be stored here:
  char str[32];
  str[0] = '\0';
  int i=0;
  while(true) {
    // See if we have serial data available:
    if (Serial.available() > 0) {
      // Yes, we have!
      // Store it at next position in string:
      str[i] = Serial.read();
      
      // If it is newline or we find a variable separator, then terminate the string
      // and leave the otherwise infinite loop:
      if (str[i] == '\n' || str[i] == '\0' || i==31) {
        str[i] = '\0';
        break;
      }
      // Ok, we were not at the end of the string, go on with next:
      else
        i++;
    }
  }
  // Convert the string to int and return:
  return(atoi(str));
} 

//////////////////////////////////////////////////////////////
