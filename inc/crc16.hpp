/*
 * crc16.h
 *
 *  Created on: 18/03/2014
 *      Author: Renato Coral Sampaio
 */

#ifndef CRC16_HPP
#define CRC16_HPP

unsigned short CRC16(unsigned short crc, char data);
unsigned short calcula_CRC(unsigned char *commands, int size);

#endif /* CRC16_HPP */
