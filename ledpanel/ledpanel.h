/**
* @brief This header file contains commands and class for ledpanel.
*
* @author Juraj Haluska
*
*/

#ifndef _LED_PANEL
#define _LED_PANEL

#include "protocol.h"

//! led panel commands
#define LED_OFF 0x00
#define LED_ON  0x01

/**
 *  @brief Class that controlls led panels.
 */  
class LedPanel {
private:
  //! private attributes
  static const int maxDataLength = 1;         /**< maximal length of dataBuffer */ 
  uint8_t myBaseAddr;                         /**< base address of led panel */ 
  Protocol *protocol;                         /**< pointer to global protocol */ 
  uint8_t dataBuffer[maxDataLength];          /**< data buffer */ 
  packet_t packet;                            /**< packet */ 
public:
  //! create led panel
  LedPanel(uint8_t _myBaseAddr, Protocol *_protocol): myBaseAddr(_myBaseAddr), protocol(_protocol) {
    //! initialize packet struct
    packet.data = dataBuffer;
    packet.dynamic = false;
    packet.dataLength = maxDataLength;
  };

  /**
   * @brief Turn LED on
   * 
   * @param    led number
   */
  void setLed(int led);

  /**
   * @brief Turn LED off
   * 
   * @param    led number
   */
  void unsetLed(int led);
};


#endif /* _LED_PANEL */