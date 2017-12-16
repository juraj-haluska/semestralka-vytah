#include "mbed.h"
#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"

#define BAUDRATE    57600
#define MY_ADDR     0x00

Serial help(PTC4, PTC3, 115200);

DigitalOut myled(LED_GREEN);
Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR);

Display display(0x30, &protocol);  
LedPanel panel1(0x20, &protocol);
Cabin cabin(0xF0, &protocol);

int main()
{
  help.printf("debuging works\r\n");
  
  protocol.start();
        
  int led = 0;
  bool lock = false;  
    
  while (true) {
    panel1.setLed(led);
    wait(0.5f);
    if (lock) {
        cabin.lock();    
    } else {
        cabin.unlock();    
    }
    lock = !lock;
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