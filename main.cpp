#include "mbed.h"
#include "protocol.h"
#include "display.h"
#include "ledpanel.h"

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
    
  Display display(0x30, &protocol);  
  LedPanel panel1(0x20, &protocol);
    
  int led = 0;
    
  while (true) {
    panel1.setLed(led);
    wait(0.5f);
    panel1.unsetLed(led++);
    wait(0.5f);
    if (led > 5) led = 0;
    
//        help.printf("waiting...\r\n");
//        osEvent evt = protocol.getInMailbox().get();
//        if (evt.status == osEventMail) {
//            packet_t *packet = (packet_t*)evt.value.p;
//            help.printf("packet\r\n");
//            protocol.getInMailbox().free(packet);
//        }
  }
}

