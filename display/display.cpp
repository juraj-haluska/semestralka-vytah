#include "display.h"
#include "protocol.h"

void Display::setDirectionUp(int floor) {  
  lastFloor = floor;
  dataBuffer[0] = DIRECT_UP;
  dataBuffer[1] = floor + '0';
  (*protocol).sendPacket(&packet);  
}

void Display::setDirectionDown(int floor) {
  lastFloor = floor;
  dataBuffer[0] = DIRECT_DOWN;
  dataBuffer[1] = floor + '0';
  (*protocol).sendPacket(&packet);  
}

void Display::setDirectionNone(int floor) {
  lastFloor = floor;
  dataBuffer[0] = DIRECT_NONE;
  dataBuffer[1] = floor + '0';
  (*protocol).sendPacket(&packet);   
}

int Display::getLastFloor() {
    return lastFloor;    
}
