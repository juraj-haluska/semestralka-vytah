#include "mbed.h"
#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"
#include "engine.h"

#define BAUDRATE    57600
#define MY_ADDR     0x00

Serial help(PTC4, PTC3, 115200);

DigitalOut myled(LED_GREEN);
Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR);

Display display(0x30, &protocol);  
LedPanel panel1(0x20, &protocol);
Cabin cabin(0xF0, 0x0F, &protocol);
Engine engine(0xF1, &protocol);

int main()
{
  help.printf("debuging works\r\n");
  protocol.start();
    
    
  while (true) {
    display.setDirectionUp(4);
    wait(0.5);
    display.setDirectionDown(5);
    wait(0.5);
    //cabin.lock();
//    wait(1);
//    engine.move(100);
//    wait(5);
//    cabin.lock();
//    wait(1);
//    engine.move(-100);
//    wait(5);
        //help.printf("waiting...\r\n");
//        osEvent evt = protocol.getInMailbox().get();
//        if (evt.status == osEventMail) {
//            packet_t *packet = (packet_t*)evt.value.p;
//            help.printf("packet\r\n");
//            protocol.getInMailbox().free(packet);
//        }
  }
}