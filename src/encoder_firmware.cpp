#include "../include/encoder_firmware.h"
#include <iostream>
namespace firmware {
struct encoderISRArgs {
  Encoder *obj;
  mraa::Gpio *pin;
};

uint8_t getPhase(uint8_t a, uint8_t b) {
//  assert(a < 2); // Can only be 0 or 1
//  assert(b < 2); // Can only be 0 or 1
  uint8_t ret = 0;
  
  switch (a << 1 | b) {
    case 0:
      ret = 0;
      break;
    case 1:
      ret = 1;
      break;
    case 2:
      ret = 3;
      break;
    case 3:
      ret = 2;
      break;
  }
  return ret;
}

void Encoder::resetCount() {
	count = 0;
}

void edgeISRWrapper(void *args) {
  struct encoderISRArgs *isrArgs = (encoderISRArgs *)args;
  Encoder *obj = isrArgs->obj;
  mraa::Gpio *pin = isrArgs->pin;
  obj->edgeISR(pin);
}

void Encoder::edgeISR(mraa::Gpio *pin) {
//  assert(pin == phaseAp || pin == phaseBp);
  char pinChar = pin == phaseAp ? 'A' : 'B';
  
//  printf("In ISR for pin %c\r\n", pinChar);

  uint8_t a = aState;
  uint8_t b = bState;

  uint8_t newState = pin->read();

  uint8_t prevPhase = getPhase(a,b);
//  printf("Previous Phase: %x\r\n ", prevPhase);

  uint8_t newPhase = 0xFF;
  if (pin == phaseAp) {
    aState = newState;
    newPhase = getPhase(newState, b); 
  } else if (pin == phaseBp){
    bState = newState;
    newPhase = getPhase(a, newState);
  }
  
//  printf("New Phase: %x\r\n ", newPhase);

  updateTick(prevPhase, newPhase);
}


void Encoder::updateTick(uint8_t oldPhase, uint8_t newPhase) {
  int8_t delta = newPhase - oldPhase; 
  //printf("Old: %x New: %x Delta: %d Count: %d\r\n", oldPhase, newPhase, delta, count);
  if ((delta == 1) || (delta == -3)) {
    count++;
  } else if ((delta  == -1) || (delta == 3)) {
    count--;
  } else {
    printf("Invalid Transition from %x to %x\r\n", oldPhase, newPhase);
  }
}


Encoder::Encoder(uint8_t phaseApin, uint8_t phaseBpin) {
  phaseAp = new mraa::Gpio(phaseApin);
  phaseBp = new mraa::Gpio(phaseBpin);
 
  aState = 0;
  bState = 0;

  isrArgsA = new encoderISRArgs;
  isrArgsA->obj = this;
  isrArgsA->pin = phaseAp;

  isrArgsB = new encoderISRArgs;
  isrArgsB->obj = this;
  isrArgsB->pin = phaseBp;

  phaseAp->dir(mraa::DIR_IN);
  phaseBp->dir(mraa::DIR_IN);

  phaseAp->isr(mraa::EDGE_BOTH, edgeISRWrapper, isrArgsA);
  phaseBp->isr(mraa::EDGE_BOTH, edgeISRWrapper, isrArgsB);
 
  count = 0;
}

Encoder::~Encoder() {
  delete phaseAp;
  delete phaseBp;
  delete isrArgsA;
  delete isrArgsB;
}

double Encoder::getCount() const {
  return double(count)*1.3625/8000.0;
}
}
