#include "mbed.h"
#include "protocol.h"
#include "packet.h"

#define BAUDRATE 57600
#define MY_ADDR 0x00
#define START_BYTE 0xA0

void onPacketReceived(const Packet * packet);

DigitalOut myled(LED_GREEN);
Serial pc(USBTX, USBRX, BAUDRATE);
Protocol protocol(pc, MY_ADDR, &onPacketReceived);

Serial help(PTC4, PTC3, 115200);

void sendPacket(unsigned char recAddr, unsigned char * data, unsigned char length);

void onPacketReceived(const Packet * packet) {
    help.printf("ack received\r\n");
}

void sendPacket(unsigned char recAddr, unsigned char * data, unsigned char length) {
    // init crc
    unsigned char crc = 0;
    crc = CRC8_TAB[crc ^ recAddr];
    crc = CRC8_TAB[crc ^ MY_ADDR];
    
    // send "header"
    pc.putc(START_BYTE);
    pc.putc(recAddr); 
    pc.putc(MY_ADDR); 
    pc.putc(length);
    
    // send data and compute crc
    for (int i = 0; i < length; i++) {
        crc = CRC8_TAB[crc ^ data[i]];
        pc.putc(data[i]);
    }
    
    // send crc
    pc.putc(crc);
}
int it = 0;
uint8_t buffer[100];

void onReceived() {
    protocol.onReceived();
    // //uint8_t received = pc.getc();
    // buffer[it++] = pc.getc();
    // if (it > 20) {
    // //help.putc(received);
    //     for (int i = 0; i < it; i++) { 
    //         help.printf("%d: %x\r\n",i, buffer[i]);
    //     }
    // }
}

int main()
{
    help.printf("debuging works\r\n");

    pc.attach(&onReceived, Serial::RxIrq);
    //unsigned char data [] = {0x41,0x42, 0x43, '\n'};
    
    //start whole process
    //onPacketReceived(NULL);

    // unsigned char data [] = {0x41,0x42, 0x43, '\n'};
    // sendPacket(0xd0, data, sizeof(data));

    // unsigned char data [] = {0x41,0x42, 0x43, '\n'};
    // sendPacket(0xd0, data, sizeof(data));

    // unsigned char data [] = {0x41,0x42, 0x43, '\n'};
    // sendPacket(0xd0, data, sizeof(data));

    unsigned char data [] = {0x41,0x42, 0x43, '\n'};

    while (true) {
        wait(1.0f);
        //onReceived();
        sendPacket(0xd0, data, sizeof(data));
    }
}

