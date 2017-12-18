/**
* @brief This header file contains commands and class for motor.
*
* @author Juraj Haluska
*
*/

#ifndef _ENGINE_H
#define _ENGINE_H

#include "protocol.h"

//! motor commands
#define CMD_STOP    0x01
#define CMD_MOVE    0x02
#define CMD_REQUEST 0x03

/**
 *  @brief Class that controlls elevator's motor.
 */  
class Engine {
private:
  //! private attributes
  static const int maxDataLength = 5;           /**< maximal length of dataBuffer */  
  uint8_t myAddr;                               /**< address of motor */ 
  Protocol * protocol;                          /**< pointer to global protocol */ 
  uint8_t dataBuffer[maxDataLength];            /**< data buffer */ 
  packet_t packet;                              /**< packet */ 
  double encoderCount;                          /**< encoder count */ 
  bool encoderCountValid;                       /**< if value in encoderCount is valid */ 
public:
  //! create motor
  Engine(uint8_t _myAddr, Protocol *_protocol): myAddr(_myAddr), protocol(_protocol) {
    //! initialize packet struct
    packet.peerAddr = myAddr;
    packet.data = dataBuffer;
    packet.dynamic = false;    
  };

  /**
   * @brief stop motor
   */
  void stop();

  /**
   * @brief start motor with specified speed
   * 
   * @param    speed in range <-100,100>
   */
  void move(int32_t speed);

  /**
   * @brief sends request for encoder value to motor 
   */
  void requestEncoderCount();

  /**
   * @brief returns last encoder count 
   * 
   * @return    encouder count
   */
  double getLastEncoderCount();

  /**
   * @return    boolean, if value from getLastEncoderCount is valid
   */
  bool isEncoderCountValid();

  /**
   * @brief Packet handling
   * 
   * This function checks if packet has address for engine.
   * If yes, motor will read data and set corresponing atributes.
   * This function should be called in each execution cycle.
   */
  void handlePacket(packet_t *packet);
};

#endif /* _ENGINE_H */