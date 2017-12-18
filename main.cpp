#include "mbed.h"
#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"
#include "engine.h"
#include "elevator.h"
#include "watchdog.h"
#include <MMA8451Q.h>

//! I2C address of accelerometer
#define MMA8451_I2C_ADDRESS (0x1D << 1)

//! protocol baud rate
#define BAUDRATE    57600

//! address of this device
#define MY_ADDR     0x00

//! create debugging serial output
Serial help(PTC4, PTC3, 115200);

//! create serial for protocol
Serial pc(USBTX, USBRX, BAUDRATE);

//! create protocol
Protocol protocol(pc, MY_ADDR);

//! free fall detection
MMA8451Q accel(PTE25, PTE24, MMA8451_I2C_ADDRESS);
InterruptIn freeFall(PTA15);

//! creation of elevator components
Display display(DISPLAY, &protocol);  
LedPanel ledPanelA(LED_PANEL_A, &protocol);
LedPanel ledPanelB(LED_PANEL_B, &protocol);
Cabin cabin(CABIN, EMERGENCY, &protocol, &freeFall);
Engine engine(MOTOR, &protocol);
Watchdog watchdog(WATCHDOG, &protocol);

//! create elevator itelf
Elevator elevator(&display, &ledPanelA, &ledPanelB, &cabin, &engine, &watchdog);

int main()
{
  //! enable free fall interrupt
  accel.enableFFD();

  help.printf("debuging works\r\n");
  
  //! cyclic executive
  while (true) {
    //! check for incoming data
    osEvent evt = protocol.getPacketMailbox().get(0);
    if (evt.status == osEventMail) {
        packet_t *packet = (packet_t*) evt.value.p;

        //! new data are in mail - actualize data in objects
        elevator.checkButtons(packet);
        elevator.checkProximity(packet);
        engine.handlePacket(packet);

        //! free memory
        protocol.getDataPool().free((uint8_t (*)[PACKET_DATA_LEN])packet->data);
        protocol.getPacketMailbox().free(packet);
    }

    //! execute state machine of elevator
    elevator.execute();
  }
}