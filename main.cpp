#include "mbed.h"
#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"
#include "engine.h"
#include "elevator.h"

#define BAUDRATE    57600
#define MY_ADDR     0x00

Serial help(PTC4, PTC3, 115200);

DigitalOut myled(LED_GREEN);
Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR);

Display display(DISPLAY, &protocol);  
LedPanel ledPanelA(LED_PANEL_A, &protocol);
LedPanel ledPanelB(LED_PANEL_B, &protocol);
Cabin cabin(0xF0, 0x0F, &protocol);
Engine engine(0xF1, &protocol);

Elevator elevator(&display, &ledPanelA, &ledPanelB, &cabin, &engine);

int main()
{
  help.printf("debuging works\r\n");
  
  // cyclic executive
  while (true) {
    // check incoming data
    osEvent evt = protocol.getPacketMailbox().get(0);
    if (evt.status == osEventMail) {
        packet_t *packet = (packet_t*) evt.value.p;
        // check for button clicks
        elevator.checkButtons(packet);

        protocol.getDataPool().free((uint8_t (*)[PACKET_DATA_LEN])packet->data);
        protocol.getPacketMailbox().free(packet);
    }
    wait(0.2);
    help.printf("going to floor:%d\r\n", elevator.getNext());
    // cabin.lock();
    // engine.move(-100);
    // wait(2);
    // engine.stop();
    // cabin.unlock();
    // cabin.lock();
    // engine.move(+100);
    // wait(2);
    // engine.stop();
    // cabin.unlock();
  }
}