// We use 4-character tabstops, so IN VIM:  <esc>:set ts=4
// ...that's: ESCAPE key, colon key, then "s-e-t SPACE key t-s-=-4"
//
//-------- define these in your sketch, if applicable ----------------------------------------------------------
//-------- This must go ahead of the #include statement --------------------------------------------------------
// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
// #define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// You can reduce the code size by 20-50 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variables PCintPort::pinState and/or
// PCintPort::arduinoPin are set and made available to your interrupt routine.
// #define NO_PIN_STATE        // to indicate that you don't need the pinState
// #define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
// if there is only one PCInt vector in use the code can be inlined
// reducing latency and code size
// define DISABLE_PCINT_MULTI_SERVICE below to limit the handler to servicing a single interrupt per invocation.
// #define       DISABLE_PCINT_MULTI_SERVICE
// The following is intended for testing purposes.  If defined, then a variable PCintPort::pinmode can be read
// in your interrupt subroutine.  It is not defined by default:
// #define PINMODE
//-------- define the above in your sketch, if applicable ------------------------------------------------------

/*
	PinChangeInt.h
	---- VERSIONS ----------------------------------------------------------------------------
	Version 1.1 Sat Dec  3 00:06:03 CST 2011
	...updated to fix the "delPin" function as per "pekka"'s bug report.  Thanks!

	Version 1.2 Sat Dec  3 Sat Dec  3 09:15:52 CST 2011
	Modified Thu Sep  8 07:33:17 CDT 2011 by GreyGnome.  Fixes a bug with the initial port
	value.  Now it sets the initial value to be the state of the port at the time of
	attachInterrupt().  The line is port.PCintLast=port.portInputReg; in attachInterrupt().
	See GreyGnome comment, below.

	Added the "arduinoPin" variable, so the user's function will know exactly which pin on
	the Arduino was triggered.

	Version 1.3 Sat Dec  3 22:56:20 CST 2011
	Significant internal changes:
	Tested and modified to work with Arduino 1.0.

	Modified to use the new() operator and symbolic links instead of creating a pre-populated
	PCintPins[].  Renamed some variables to simplify or make their meaning more obvious (IMHO anyway).
	Modified the PCintPort::PCint() code (ie, the interrupt code) to loop over a linked-list.  For
	those who love arrays, I have left some code in there that should work to loop over an array
	instead.  But it is commented out in the release version.

	For Arduino versions prior to 1.0: The new() operator requires the cppfix.h library, which is
	included with this package.  For Arduino 1.0 and above: new.h comes with the distribution, and
	that is #included.

	Version 1.4 Tue Jan 10 09:41:14 CST 2012
	All the code has been moved into this .h file, so as to allow #define's to work from the user's
	sketch.  Thanks to Paul Stoffregen from pjrc.com for the inspiration! (Check out his website for
	some nice [lots more memory] Arduino-like boards at really good prices.  ...This has been an unsolicited
	plug. Now back to our regular programming.  ...Hehe, "programming", get it?)

	As a result, we no longer use the PinChangeIntConfig.h file.  The user must #define things in his/her
	sketch.  Which is better anyway.

	Removed the pcIntPorts[] array, which created all the ports by default no matter what.  Now, only
	those ports (PCintPort objects) that you need will get created if you use the NO_PORTx_PINCHANGES #defines.
   	This saves flash memory, and actually we get a bit of a memory savings anyway even if all the ports are
	left enabled.

	The attachInterrupt and detachInterrupt routines were modified to handle the new PCintPort objects.

	Version 1.5 Thu Feb  2 18:09:49 CST 2012
	Added the PCintPort::pinState static variable to allow the programmer to query the state of the pin
	at the time of interrupt.
	Added two new #defines, NO_PIN_STATE and NO_PIN_NUMBER so as to reduce the code size by 20-50 bytes,
	and to speed up the interrupt routine slightly by declaring that you don't care if the static variables
	PCintPort::pinState and/or PCintPort::arduinoPin are set and made available to your interrupt routine.
	// #define NO_PIN_STATE        // to indicate that you don't need the pinState
	// #define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin

	Version 1.51 Sun Feb  5 23:28:02 CST 2012
	Crap, a bug!  Changed line 392 from this:
	PCintPort::pinState=curr & changedPins ? HIGH : LOW;
	to this:
	PCintPort::pinState=curr & p->mask ? HIGH : LOW;
	Also added a few lines of (commented-out) debug code.

	Version 1.6beta Fri Feb 10 08:48:35 CST 2012
	Set the value of the current register settings, first thing in each ISR; e.g.,
	ISR(PCINT0_vect) {
		PCintPort::curr = portB.portInputReg; // version 1.6
		...
	...instead of at the beginning of the PCintPort::PCint() static method.  This means that the port is read
	closer to the beginning of the interrupt, and may be slightly more accurate- only by a couple of microseconds,
	really, but it's a cheap win.

	Fixed a bug- a BUG!- in the attachInterrupt() and detachInterrupt() methods.  I didn't have breaks in my
	switch statements!  Augh!  What am I, a (UNIX) shell programmer?  ...Uh, generally, yes...

	Added the PINMODE define and the PCintPort::pinmode variable.
	---- VERSIONS ----------------------------------------------------------------------------

	See google code project for latest, bugs and info http://code.google.com/p/arduino-pinchangeint/
	For more information Refer to avr-gcc header files, arduino source and atmega datasheet.

	This library was inspired by and derived from "johnboiles" (it seems) 
	PCInt Arduino Playground example here: http://www.arduino.cc/playground/Main/PcInt
	If you are the original author, please let us know at the google code page
	
	It provides an extension to the interrupt support for arduino by
	adding pin change interrupts, giving a way for users to have
	interrupts drive off of any pin.

	Please make any configuration changes in the accompanying PinChangeIntConfig.h file.
	This will help avoid having to reset your config in the event of changes to the 
	library code (just don't replace that file when you update).

	Modified Thu Sep  8 07:33:17 CDT 2011 by GreyGnome.  See GreyGnome comment, below.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/

#ifndef PinChangeInt_h
#define	PinChangeInt_h

#include "stddef.h"

// Thanks to Maurice Beelen, nms277, Akesson Karlpetter, and Orly Andico for these fixes.
#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
  #include <new.h>
#else
  #include <pins_arduino.h>
  #include "../cppfix/cppfix.h"
  #include "WProgram.h"
#endif

#undef DEBUG

/*
* Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
* The PCINT corresponding to the pin must be enabled and masked, and
* an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
* must use some logic to actually implement a per-pin interrupt service.
*/

/* Pin to interrupt map:
* D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
* D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
* A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
*/

#define	INLINE_PCINT
#if ((defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES)) || \
			(defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTD_PINCHANGES) && defined(NO_PORTB_PINCHANGES)))
#undef INLINE_PCINT
#define	INLINE_PCINT inline
#endif

// Provide drop in compatibility with johnboiles PCInt project at
// http://www.arduino.cc/playground/Main/PcInt
#define	PCdetachInterrupt(pin)	PCintPort::detachInterrupt(pin)
#define	PCattachInterrupt(pin,userFunc,mode) PCintPort::attachInterrupt(pin, userFunc,mode)
#define PCgetArduinoPin() PCintPort::getArduinoPin()


typedef void (*PCIntvoidFuncPtr)(void);

class PCintPort {
public:
	PCintPort(int index,volatile uint8_t& maskReg) :
	portInputReg(*portInputRegister(index + 2)),
	portPCMask(maskReg),
	PCICRbit(1 << index),
	firstPin(NULL)
	{
		#ifdef FLASH
		ledsetup();
		#endif
	}
	volatile	uint8_t&		portInputReg;
	static		void attachInterrupt(uint8_t pin, PCIntvoidFuncPtr userFunc, int mode);
	static		void detachInterrupt(uint8_t pin);
	INLINE_PCINT void PCint();
	static 	uint8_t curr;
	#ifndef NO_PIN_NUMBER
	static	uint8_t	arduinoPin;
	#endif
	#ifndef NO_PIN_STATE
	static	uint8_t	pinState;
	#endif
	#ifdef PINMODE
	static uint8_t pinmode;
	#endif
	#ifdef FLASH
	static void ledsetup(void);
	#endif

protected:
	class PCintPin {
	public:
		PCintPin() :
		PCintFunc((PCIntvoidFuncPtr)NULL),
		mode(0) {}
		PCIntvoidFuncPtr PCintFunc;
		uint8_t 	mode;
		uint8_t		mask;
		uint8_t arduinoPin;
		PCintPin* next;
	};
	void		addPin(uint8_t arduinoPin,uint8_t mode,PCIntvoidFuncPtr userFunc);
	void		delPin(uint8_t mask);
	volatile	uint8_t&		portPCMask;
	const		uint8_t			PCICRbit;
	uint8_t		lastPinView;
	PCintPin*	createPin(uint8_t arduinoPin, uint8_t mode);
	PCintPin*	firstPin;
};

uint8_t PCintPort::curr=0;
#ifndef NO_PIN_NUMBER
uint8_t PCintPort::arduinoPin=0;
#endif
#ifndef NO_PIN_STATE
uint8_t PCintPort::pinState=0;
#endif
#ifdef PINMODE
uint8_t PCintPort::pinmode=0;
#endif

#ifdef FLASH
#define PINLED 13
volatile uint8_t *led_port;
uint8_t led_mask;
uint8_t not_led_mask;
boolean ledsetup_run=false;
void PCintPort::ledsetup(void) {
	if (! ledsetup_run) {
		led_port=portOutputRegister(digitalPinToPort(PINLED));
		led_mask=digitalPinToBitMask(PINLED);
		not_led_mask=led_mask^0xFF;
		pinMode(PINLED, OUTPUT); digitalWrite(PINLED, LOW);
		ledsetup_run=true;
	}
};
#endif


#ifndef NO_PORTB_PINCHANGES
PCintPort portB=PCintPort(0,PCMSK0); // port PB==2  (from Arduino.h, Arduino version 1.0)
#endif

#ifndef NO_PORTC_PINCHANGES
PCintPort portC=PCintPort(1,PCMSK1); // port PC==3  (also in pins_arduino.c, Arduino version 022)
#endif

#ifndef NO_PORTD_PINCHANGES
PCintPort portD=PCintPort(2,PCMSK2); // port PD==4
#endif

PCintPort::PCintPin* PCintPort::createPin(uint8_t arduinoPin, uint8_t mode) {

	// Create pin p:  fill in the data
	PCintPin* p=new PCintPin;
	p->arduinoPin=arduinoPin;
	p->mode = mode;
	p->next=NULL;
	portPCMask |= p->mask = digitalPinToBitMask(arduinoPin); // the mask

#ifdef DEBUG
	Serial.print("createPin. pin given: "); Serial.print(arduinoPin, DEC);
	int addr = (int) p;
	Serial.print(" instance addr: "); Serial.println(addr, HEX);
#endif
	PCICR |= PCICRbit;
	return(p);
}

void PCintPort::addPin(uint8_t arduinoPin, uint8_t mode,PCIntvoidFuncPtr userFunc)
{
	PCintPin* p=createPin(arduinoPin, mode);

	if (p == NULL) return;
	// Add to linked list, starting with firstPin
	if (firstPin == NULL) firstPin=p;
	else {
		PCintPin* tmp=firstPin;
		while (tmp->next != NULL) {
				tmp=tmp->next;
		};
		tmp->next=p;
	}
	p->PCintFunc=userFunc;
#ifdef DEBUG
	Serial.print("addPin. pin given: "); Serial.print(arduinoPin, DEC), Serial.print (" pin stored: ");
	int addr = (int) p;
	Serial.print(" instance addr: "); Serial.println(addr, HEX);
#endif
}

void PCintPort::delPin(uint8_t mask)
{
	PCintPin* current=firstPin;
	PCintPin* prev=NULL;
	while (current) {
		if (current->mask == mask) { // found the target
			uint8_t oldSREG = SREG;
			cli(); // disable interrupts
			portPCMask &= ~mask; // disable the mask entry.
			if (portPCMask == 0) PCICR &= ~(PCICRbit);
			// Link the previous' next to the found next. Then remove the found.
			if (prev != NULL) prev->next=current->next; // linked list skips over current.
			else firstPin=current->next; // at the first pin; save the new first pin
			delete current;
			SREG = oldSREG; // Restore register; reenables interrupts
			return;
		}
		prev=current;
		current=current->next;
	}
}

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 */
void PCintPort::attachInterrupt(uint8_t arduinoPin, PCIntvoidFuncPtr userFunc, int mode)
{
	PCintPort *port;
	uint8_t portNum = digitalPinToPort(arduinoPin);
	if ((portNum == NOT_A_PORT) || (userFunc == NULL)) return;

	switch (portNum) {
#ifndef NO_PORTB_PINCHANGES
	case 2:
		port=&portB;
		break;
#endif
#ifndef NO_PORTC_PINCHANGES
	case 3:
		port=&portC;
		break;
#endif
#ifndef NO_PORTD_PINCHANGES
	case 4:
		port=&portD;
		break;
#endif
	}
	// Added by GreyGnome... must set the initial value of lastPinView for it to be correct on the 1st interrupt.
	// ...but even then, how do you define "correct"?  Ultimately, the user must specify (not provisioned for yet).
	port->lastPinView=port->portInputReg;

	// map pin to PCIR register
	port->addPin(arduinoPin,mode,userFunc);
#ifdef DEBUG
	Serial.print("attachInterrupt FUNC: "); Serial.println(arduinoPin, DEC);
#endif
}

void PCintPort::detachInterrupt(uint8_t arduinoPin)
{
	PCintPort *port;
#ifdef DEBUG
	Serial.print("detachInterrupt: "); Serial.println(arduinoPin, DEC);
#endif
	uint8_t portNum = digitalPinToPort(arduinoPin);
	if (portNum == NOT_A_PORT) {
		return;
	} 
	switch (portNum) {
#ifndef NO_PORTB_PINCHANGES
	case 2:
		port=&portB;
		break;
#endif
#ifndef NO_PORTC_PINCHANGES
	case 3:
		port=&portC;
		break;
#endif
#ifndef NO_PORTD_PINCHANGES
	case 4:
		port=&portD;
		break;
#endif
	}
	port->delPin(digitalPinToBitMask(arduinoPin));
}

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
void PCintPort::PCint() {
	#ifdef FLASH
	if (*led_port & led_mask) *led_port&=not_led_mask;
	else *led_port|=led_mask;
    #endif
	#ifndef DISABLE_PCINT_MULTI_SERVICE
	uint8_t pcifr;
	do {
	#endif
		// get the pin states for the indicated port.
		uint8_t changedPins = PCintPort::curr ^ lastPinView;
		changedPins &= portPCMask;
		lastPinView = PCintPort::curr;

		PCintPin* p = firstPin;
		while (p) {
			if (p->mask & changedPins) { // a changed bit
				// Trigger interrupt if mode is CHANGE, or if mode is RISING and
				// the bit is currently high, or if mode is FALLING and bit is low.
				if (     p->mode == CHANGE
					|| ((p->mode == RISING)  &&  (curr & p->mask))
					|| ((p->mode == FALLING) && !(curr & p->mask)) ) {
					#ifndef NO_PIN_STATE
					PCintPort::pinState=PCintPort::curr & p->mask ? HIGH : LOW; // version 1.51, updated 1.6
					#endif
					#ifndef NO_PIN_NUMBER
					PCintPort::arduinoPin=p->arduinoPin;
					#endif
					#ifdef PINMODE
					PCintPort::pinmode=p->mode;
					#endif
					p->PCintFunc();
				}
			}
			p=p->next;
		}
	#ifndef DISABLE_PCINT_MULTI_SERVICE
		pcifr = PCIFR & PCICRbit;
		PCIFR = pcifr;	// clear the interrupt if we will process it (no effect if bit is zero)
	} while(pcifr);
	#endif
}

#ifndef NO_PORTB_PINCHANGES
ISR(PCINT0_vect) {
	PCintPort::curr = portB.portInputReg; // version 1.6
	portB.PCint();
}
#endif

#ifndef NO_PORTC_PINCHANGES
ISR(PCINT1_vect) {
	PCintPort::curr = portC.portInputReg; // version 1.6
	portC.PCint();
}
#endif

#ifndef NO_PORTD_PINCHANGES
ISR(PCINT2_vect){ 
	PCintPort::curr = portD.portInputReg; // version 1.6
	portD.PCint();
}
#endif

#endif
