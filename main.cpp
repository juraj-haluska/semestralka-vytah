#include "mbed.h"
#include "protocol.h"

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
    
    while (true) {

//        help.printf("waiting...\r\n");
//        osEvent evt = protocol.getInMailbox().get();
//        if (evt.status == osEventMail) {
//            packet_t *packet = (packet_t*)evt.value.p;
//            help.printf("packet\r\n");
//            protocol.getInMailbox().free(packet);
//        }
        wait(0.8f);

        protocol.getOutMailbox();
        packet_t *packet = protocol.getOutMailbox().alloc();
        packet->peerAddr = 0xD0;
        packet->data[0] = ch;
        packet->data[1] = '\n';

        packet->dataLength = 2;

        protocol.getOutMailbox().put(packet);
        
        if (ch < 'z') {
            ch++;    
        } else {
            ch = 'a';
        }
    }
}

