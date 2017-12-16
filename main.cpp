#include "mbed.h"
#include "protocol.h"
#include "display.h"

#define BAUDRATE    57600
#define MY_ADDR     0x00

Serial help(PTC4, PTC3, 115200);

DigitalOut myled(LED_GREEN);
Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR);

int main()
{
  help.printf("debuging works\r\n");

  protocol.start();
    
  char ch = 'a';
    
  Display display(0x30, &protocol);  
    
  while (true) {
    display.setDirectionUp(2);
    wait(1.0f);
    display.setDirectionDown(3);
    wait(1.0f);
    display.setDirectionNone(0);
    wait(1.0f);
//        help.printf("waiting...\r\n");
//        osEvent evt = protocol.getInMailbox().get();
//        if (evt.status == osEventMail) {
//            packet_t *packet = (packet_t*)evt.value.p;
//            help.printf("packet\r\n");
//            protocol.getInMailbox().free(packet);
//        }
  }
}

