#define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
#include <PinChangeInt.h>

#define PIN_A 2  // Hall effect A
#define PIN_B 11 // Hall effect B

int count = 0;

void pinAfuncRising() {if( digitalRead(PIN_B)) count--; else count++;}
void pinAfuncFalling(){if(!digitalRead(PIN_B)) count--; else count++;}
void pinBfuncRising() {if(!digitalRead(PIN_A)) count--; else count++;}
void pinBfuncFalling(){if( digitalRead(PIN_A)) count--; else count++;}

void setup() {
  pinMode(PIN_A, INPUT); digitalWrite(PIN_A, HIGH);
  PCintPort::attachInterrupt(PIN_A, &pinAfuncRising,  RISING);
  PCintPort::attachInterrupt(PIN_A, &pinAfuncFalling, FALLING);
  pinMode(PIN_B, INPUT); digitalWrite(PIN_B, HIGH);
  PCintPort::attachInterrupt(PIN_B, &pinBfuncRising,  RISING);
  PCintPort::attachInterrupt(PIN_B, &pinBfuncFalling, FALLING);

  Serial.begin(115200);
  Serial.println("Hall Effect Encoder Test");
}

void loop() {
  Serial.println(count);
  delay(250);
}

