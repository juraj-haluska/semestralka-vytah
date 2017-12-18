/**
* @brief This header file contains display commands and class for display.
*
* @author Juraj Haluska
*
*/

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "protocol.h"

//! display commands
#define DIRECT_UP    0x01
#define DIRECT_DOWN  0x02
#define DIRECT_NONE  0x03

/**
 *  @brief Class that controlls elevator's display.
 */  
class Display {
private:
  //! private attributes
  static const int maxDataLength = 2;             /**< maximal length of dataBuffer */  
  uint8_t myAddr;                                 /**< address of display */ 
  Protocol *protocol;                             /**< pointer to global protocol */ 
  int lastFloor;                                  /**< keeps info about last floor */ 
  uint8_t dataBuffer[maxDataLength];              /**< data buffer */ 
  packet_t packet;                                /**< packet */ 

public:
  //! create display
  Display(uint8_t _myAddress, Protocol *_protocol): myAddr(_myAddress), protocol(_protocol) {
    //! initialize packet struct
    packet.peerAddr = myAddr;
    packet.data = dataBuffer;
    packet.dataLength = maxDataLength;
    packet.dynamic = false;    
  };

  /**
   * @brief set up direction and floor on display
   * 
   * @param   floor to show on display
   */
  void setDirectionUp(int floor);

  /**
   * @brief set down direction and floor on display
   * 
   * @param   floor to show on display
   */
  void setDirectionDown(int floor);
  /**
   * @brief hide direction infromation from display
   * 
   * @param   floor to show on display
   */
  void setDirectionNone(int floor);
  /**
   * @return    floor which was set last time
   */
  int getLastFloor();
};

#endif /* _DISPLAY_H */