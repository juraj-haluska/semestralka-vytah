#include "mbed.h"
#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"
#include "engine.h"
#include "elevator.h"
#include <MMA8451Q.h>

#define MMA8451_I2C_ADDRESS (0x1D << 1)

#define BAUDRATE    57600
#define MY_ADDR     0x00

Serial help(PTC4, PTC3, 115200);

DigitalOut myled(LED_GREEN);
Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR);

// free fall detection
MMA8451Q accel(PTE25, PTE24, MMA8451_I2C_ADDRESS);
InterruptIn freeFall(PTA15);

Display display(DISPLAY, &protocol);  
LedPanel ledPanelA(LED_PANEL_A, &protocol);
LedPanel ledPanelB(LED_PANEL_B, &protocol);
Cabin cabin(0xF0, 0x0F, &protocol, &freeFall);
Engine engine(0xF1, &protocol);

Elevator elevator(&display, &ledPanelA, &ledPanelB, &cabin, &engine);

int main()
{

  // enable free fall interrupt
  accel.enableFFD();

  help.printf("debuging works\r\n");
  
  // cyclic executive
  while (true) {
    // check incoming data
    osEvent evt = protocol.getPacketMailbox().get(0);
    if (evt.status == osEventMail) {
        packet_t *packet = (packet_t*) evt.value.p;
        // actualize data in objects
        elevator.checkButtons(packet);
        elevator.checkProximity(packet);
        engine.handlePacket(packet);

        protocol.getDataPool().free((uint8_t (*)[PACKET_DATA_LEN])packet->data);
        protocol.getPacketMailbox().free(packet);
    }

    elevator.execute();
  }
}