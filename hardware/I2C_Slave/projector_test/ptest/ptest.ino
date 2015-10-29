#include <b9SoftwareSerial.h>
#include <PinChangeInt.h>

// Projector's RS-232 Port I/O pins
#define PROJECTOR_RX 12
#define PROJECTOR_TX 13

// Persistent Calibration and setup settings
#define XNATIVEDEFAULT 1024
#define YNATIVEDEFAULT 768
#define XYPSIZEDEFAULT 100
#define HALFLIFEDEFAULT 2000 // Hours before projector lamp output is reduced to 1/2 original value

int iNativeX = 0;
int iNativeY = 0;
int iXYPixelSize = 0;
int iHalfLife = 2000;


// Projector RS-232 interface & status variables
b9SoftwareSerial projectorSerial = b9SoftwareSerial(PROJECTOR_RX, PROJECTOR_TX);
unsigned long ulLastProjectorQueryTime = millis();
unsigned long ulLastProjectorMsgRcvdTime = millis();
bool bFirstProjectorQuery = true;
// We can only do one factory reset once the projector is turned on because it leaves us in an
// unknown menu state and we can not repeat the key stroke sequence again.
// So we only allow for one reset per power on command.
// This reset can be commanded manually with P7 or automaticaly with the B (base) command
bool bProjectorNeedsReset = false; 
bool bIsMirrored = false;
int iProjectorPwr = -1; // 0 = OFF, 1 = ON, -1 = ?
int iLampHours    = -1; // reported lamp hours, -1 = ?
unsigned long ulLastCmdReceivedTime = millis(); // reset every time we get a cmd from the host
bool bVerbose = true; // Send comments to Serial stream

void BC_C(const __FlashStringHelper* s, String sVariable = "");
void BC_String(const __FlashStringHelper* s, String sVariable = "");

///////////////////////////////////////////////////////
//
// Program Setup
//
void setup() {

  // Projector RS-232 serial I/O
  pinMode(PROJECTOR_RX, INPUT);
  PCintPort::attachInterrupt(PROJECTOR_RX, &projectorRxChange, CHANGE);
  pinMode(PROJECTOR_TX, OUTPUT); 

  
  Serial.begin(115200); 
  
  // set the data rate for the b9SoftwareSerial port
  projectorSerial.begin(9600);  
  ulLastCmdReceivedTime = millis(); // initilized
  ulLastProjectorMsgRcvdTime = millis();

  // Hello World....
  BC_V();
  bVerbose = false;

  

  BC_K();  // Lamp Half Life
 

  
  BC_D();  // Projector's X Resolution
  BC_E();  // Projector's Y Resolution
  BC_H();  // Calibrated XY Pixel Size in Microns  


  }

void loop() {

 
    HandleProjectorComm_Vivitek_D535();
    delay(100);

}

void serialEvent() {

if(Serial.available()) {
    char cInChar;
    int i, iDiff;
    
    // get the new byte:
    cInChar = (char)Serial.read(); 
    switch (cInChar)  {

    case '$':        // Reset Projector's Half Life value
        i = SerialReadInt();
        if (i>0) {
          iHalfLife = i;
          //storeHalfLife();  // store new value in persistant EEPROM
        }
        BC_C(F("Command: Lamp Half Life Set to: "), String(iHalfLife));
        break;    

    case 'u':        // Reset the projectors calibrated xy pixel size in microns
    case 'U':
      i = SerialReadInt();
      if(i>=0)
      {
        iXYPixelSize = i;
        //storeRefXYPixelSize();  // store new value in persistant EEPROM
     }
      BC_C(F("Command: Calibrated XY Pixel Size Set To "), String(iXYPixelSize));
      break;     


    case 'h':        // Reset the projectors native X Resolution
    case 'H':
      i = SerialReadInt();
      if(i>=0)
      {
        iNativeX = i;
        //storeRefNativeX();  // store new value in persistant EEPROM
      }
      BC_C(F("Command: Native X Resolution Set To "), String(iNativeX));
      break;     
       
    case 'i':        // Reset the projectors native Y Resolution
    case 'I':
      i = SerialReadInt();
      if(i>=0)
      {
        iNativeY = i;
       // storeRefNativeY();  // store new value in persistant EEPROM
      }
      BC_C(F("Command: Native Y Resolution Set To "), String(iNativeY));
      break;     


    case 'p':     // Projector Power & Mode commands
      case 'P':
        i = SerialReadInt();
        if(i==0){
          BC_C(F("Command: Turning Projector OFF")); 
          projectorSerial.write("~PF\r");
        }
        else if(i==1){
          BC_C(F("Command: Turning Projector ON")); 
          projectorSerial.write("~PN\r");
        }
        else if(i==12){
          projectorSerial.write("~rM\r");   //MENU
          delay(100);
        
        }
        
        else if(i==13){
           projectorSerial.write("~qL\r"); //lamp query
          delay(100);
        
        }
           
        else if(i==2){
          BC_C(F("Command: Reseting Projector")); 
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
          BC_C(F("Command: Set Projection Mode: Front")); 
          projectorSerial.write("~sJ0\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = false;
        }
        else if(i==4){
          BC_C(F("Command: Set Projection Mode: Rear")); 
          projectorSerial.write("~sJ1\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = true;
        }
        else if(i==5){
          BC_C(F("Command: Set Projection Mode: Ceiling")); 
          projectorSerial.write("~sJ2\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = false;          
        }
        else if(i==6){
          BC_C(F("Command: Set Projection Mode: Rear + Ceiling")); 
          projectorSerial.write("~sJ3\r");  // Vivitek D535 set Projetion Mode to Front (0:Front 1:Rear 2:Ceiling 3:Rear+Ceiling )
          bIsMirrored = true;          
        }
        else if(i==7){
          BC_C(F("Command: Projector Factory Reset")); 
          projectorReset();
        }

        break; 
           default:
        break;  
} 
}
}





//////////////////////////////////////////////////////////////
//
// Handle the automated projectors communications
//  Keep iProjectorPwr and iLampHours updated 
void HandleProjectorComm_Vivitek_D535() {
    
  // Handle the projector's transmission
  String sData;
  char c;
  while(projectorSerial.available() > 0) {
    // Yes, we have data available
    c = projectorSerial.read();
    // If it is newline or we find a variable separator, then terminate
    // and leave the otherwise infinite loop, otherwise add it to sData
    if (c == '\r' || c == '\n' || c == '\0') break; else sData += String(c);
  }
 /* if(sData=="Off") {
    iProjectorPwr = 0;
    bProjectorNeedsReset = true;
  }
  else if(sData=="On") {
    iProjectorPwr = 1; 
    projectorSerial.write("~qL\r"); // Transmit Lamp hours query
  }  
  else if(sData=="p" || sData=="P") {
    sData=""; //ignore the "P" response
  }
  else {*/
   
     BC_String("Data:" + sData);
    char str[65];
    sData.toCharArray(str,64);
    int iH = atoi(str);
    if(iH>=0 && iH<100000 && iH >= iLampHours) iLampHours = iH;
   // BC_C(F(iLampHours));
//   BC_String(F("WB9C1"));
    
  //}
}

void projectorReset(){
  if(bProjectorNeedsReset){
    bProjectorNeedsReset = false;
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
}  

void projectorRxChange() {cli(); projectorSerial.do_interrupt(); sei();}


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
//////////////////////////////////////////////////////////////


void BC_K() {
  BC_String(F("K"), String(iHalfLife));
  BC_C(F("Estimated hours before lamp output is 1/2 of original value: "), String(iHalfLife));
}

void BC_D() {
  BC_String(F("D"), String(iNativeX));
  BC_C(F("Projector's Native X Resolution: "), String(iNativeX));
}

//////////////////////////////////////////////////////////////
void BC_E() {
  BC_String(F("E"), String(iNativeY));
  BC_C(F("Projector's Native Y Resolution: "), String(iNativeY));
}

//////////////////////////////////////////////////////////////
void BC_H() {
  BC_String(F("H"), String(iXYPixelSize));
  BC_C(F("Calibrated XY Pixel Size in Microns: "), String(iXYPixelSize));
}

void BC_V() {
    BC_String(F("V1 1 2"));
    BC_String(F("WB9C1"));
    BC_C(F("B9Creator(tm) Firmware version 1.1.2 running on a B9Creator Model 1"));
    BC_C(F("Copyright 2012, 2013 B9Creations, LLC"));
    BC_C(F("B9Creations(tm) and B9Creator(tm) are trademarks of B9Creations, LLC"));
    BC_C(F("This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License."));
    BC_C(F("To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/"));
    BC_C(F("For updates and to download the lastest version, visit http://b9creator.com"));
}
//////////////////////////////////////////////////////////////
void BC_C(String sComment) {
  if(bVerbose){
    BC_String("C" + sComment);
  }
}
void BC_C(const __FlashStringHelper* s, String sVariable) {
  if(bVerbose){
    Serial.print("C");
    Serial.print(s);
    Serial.println(sVariable);
  }
}


//////////////////////////////////////////////////////////////
void BC_String(String sString) {
  Serial.println(sString);
}

//////////////////////////////////////////////////////////////
void BC_String(const __FlashStringHelper* s, String sVariable) {
    Serial.print(s);
    Serial.println(sVariable);
}
