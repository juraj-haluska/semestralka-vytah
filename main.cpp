#include "mbed.h"
#include "protocol.h"

#define BAUDRATE    57600
#define MY_ADDR     0x00

Serial help(PTC4, PTC3, 115200);

// void onPacketReceived(Packet * packet) {
//     help.printf("packet received from %x:\r\n", packet->getPeerAddr());
//     for (int i = 0; i < packet->getData().size(); i++) {
//         help.printf("\t%x\r\n", packet->getData()[i]);
//     }
//     delete packet;
// }

Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR);

DigitalOut myled(LED_GREEN);
//Thread debuger;
Thread sender_th;

void debuger_th(){
    while(true) {
        wait(0.5f);
        myled = !myled; 
    }
}

int main()
{
    //debuger.start(debuger_th);
    sender_th.start(callback(&protocol, &Protocol::senderTh));
    help.printf("debuging works\r\n");

    //unsigned char data [] = {0x41,0x42, 0x43, '\n'};

    while (true) {

        help.printf("waiting...\r\n");
         osEvent evt = protocol.getInQueue().get();
         if (evt.status == osEventMessage) {
             packet_t *packet = (packet_t*)evt.value.p;
             help.printf("packet\r\n");

             delete [] packet->data;
             delete packet;
         }
         wait(1.0f);

         packet_t *packet = new packet_t;
         packet->peerAddr = 0xD0;
         packet->dataLength = 2;
         packet->data = new uint8_t [packet->dataLength];
         packet->data[0] = 0x41;
         packet->data[1] = '\n';

         protocol.getOutQueue().put(packet);
    }
}

