nclude "mraa.hpp"

#include <cassert>
#include <csignal>
#include <iostream>
#include <vector>

#define SHIELD_I2C_ADDR 0x40

int running = 1;

std::vector<uint8_t> registers = {
  6,   // output 0
  10,  // output 1
  14,  // output 2
  18,  // output 3
  22,  // output 4
  26,  // output 5
  30,  // output 6
  34,  // output 7
  38,  // output 8
  42,  // output 9
  46,  // output 10
  50,  // output 11
  54,  // output 12
  58,  // output 13
  62,  // output 14
  66   // output 15
};
  

void sig_handler(int signo)
{
  if (signo == SIGINT) {
    printf("closing spi nicely\n");
    running = 0;
  }
}

void writePWM(mraa::I2c* i2c, int index, double duty) {
  assert(0.0 <= duty && duty <= 1.0);
  assert(0 <= index && index < 16);
  double on = 4096.0 * duty;
  uint16_t onRounded = (uint16_t) on;
  uint16_t offRounded = 4096 - onRounded;

  char writeBuf[2];

  // ON_L
  writeBuf[0] = registers[index];
  writeBuf[1] = onRounded & 0xff;
  i2c->address(SHIELD_I2C_ADDR);
  i2c->write(writeBuf, 2);

  // ON_H
  writeBuf[0] = registers[index] + 1;
  writeBuf[1] = (onRounded >> 8) & 0xff;
  i2c->address(SHIELD_I2C_ADDR);
  i2c->write(writeBuf, 2);

  // OFF_L
  writeBuf[0] = registers[index] + 2;
  writeBuf[1] = offRounded & 0xff;
  i2c->address(SHIELD_I2C_ADDR);
  i2c->write(writeBuf, 2);

  // OFF_H
  writeBuf[0] = registers[index] + 3;
  writeBuf[1] = (offRounded >> 8) & 0xff;
  i2c->address(SHIELD_I2C_ADDR);
  i2c->write(writeBuf, 2);
}

int main()
{
  // Handle Ctrl-C quit
  signal(SIGINT, sig_handler);

  // Edison i2c bus is 6
  mraa::I2c* i2c = new mraa::I2c(6);
  assert(i2c != nullptr);

  while (running) {
    // Alternate two locations with 2-sec delay
    writePWM(i2c, 12, 0.2);
    sleep(2.0);
    writePWM(i2c, 12, 0.8);
    sleep(2.0);
  }
}
