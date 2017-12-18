/**
* @brief This header file is declares communication protocol.
*
* @author Juraj Haluska
*
*/

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "mbed.h"

//! packet related constants
#define START_BYTE      0xA0
#define ZERO 0x00

//! positions of fileds in packet counting from 1
#define PACKET_SB_POS   1
#define PACKET_RA_POS   2
#define PACKET_TA_POS   3
#define PACKET_DL_POS   4

//! length of packet
#define MAX_DATA_LENGTH 24

//! length of data field in packet
#define PACKET_DATA_LEN 16

//! receive buffer capacity
#define RECEIVE_SLOTS   16

//! ACK timeout in ms
#define ACK_TIMEOUT     250

//! flag indicating that ACK packet arrived
#define EVENT_ACK       0x01

//! ACK byte sequence
const uint8_t ACK[] = {
  START_BYTE, 0x00, 0x00, 0x00, 0x00
};

//! CRC8 table
const unsigned char CRC8_TAB[] = {
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
  157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
  190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
  219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
  101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
  202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
  87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53    
};

/**
 *  @brief Packet structure.
 */  
typedef struct {
  uint8_t peerAddr;       /**< address of receiver or transmitter depending on the way of packet */  
  uint8_t dataLength;     /**< length of data pointed by data pointer */  
  uint8_t * data;         /**< actual data */  
  bool dynamic;           /**< states if packet was allocated dynamically */  
} packet_t;

/**
 *  @brief Class that implements communication protocol.
 */  
class Protocol {
private:
  Serial& serial;                  /**< initialised uart */  
  EventFlags event;                /**< event flag */  
  uint8_t myAddr;                  /**< my address - sender address */  

  uint8_t dataLength;                     /**< iterator for recvBuffer */  
  uint8_t recvBuffer[MAX_DATA_LENGTH];    /**< buffer for incoming data */
  uint8_t crc;                            /**< running crc sum */

  Mail<packet_t, RECEIVE_SLOTS> recvMailbox;                    /**< mailbox for received packets */
  MemoryPool<uint8_t[PACKET_DATA_LEN], RECEIVE_SLOTS> dataPool; /**< memory pool for allocation data in received packets */

  //! private functions
  void newPacket();
  void onByteReceived();
public:
  //! create protocol
  Protocol(Serial& _serial, uint8_t _myAddr);

  /**
   * @brief Send packet 
   * 
   * This function is synchronous. It waits until ACK for transmitted
   * packet is received. 
   * 
   * @param   packet which encapsulates all needed info for transfer
   */
  void sendPacket(packet_t * packet);

  /**
   * @brief Get mailbox of received packets
   * 
   * @return  mailbox
   */
  Mail<packet_t, RECEIVE_SLOTS> & getPacketMailbox();

   /**
   * @brief Get memory pool of packet data filed
   * 
   * @return  memory pool
   */
  MemoryPool<uint8_t [PACKET_DATA_LEN], RECEIVE_SLOTS> & getDataPool();
};

#endif /* PROTOCOL_H_ */