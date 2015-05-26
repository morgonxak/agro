/*
  EEPROM2.h - EEPROM library v2
  Copyright (c) A.Nesterovsky, http://freeduino.ru.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#include <avr/eeprom.h>
#include "EEPROM2.h"

uint8_t EEPROM_read_byte(int eeprom_addres)               //reads 1-byte value
{
	return eeprom_read_byte((unsigned char *)eeprom_addres);
}

void EEPROM_write_byte(int eeprom_addres, uint8_t value)  //writes 1-byte value
{
	eeprom_write_byte((unsigned char *)eeprom_addres, value);
}

void EEPROM_read_mem(int eeprom_addres, void *pData, int size)        //reads data block
{
  //code is a bit ugly, but, as far, as I see, it leads to smaller binary
  uint8_t *pData2 = (uint8_t*)pData;
  for (; size>0; size--) {
    *pData2++ = eeprom_read_byte((unsigned char *)(eeprom_addres++));
  }
}

void EEPROM_write_mem(int eeprom_addres, const void *pData, int size) //writes data block
{
  //code is a bit ugly, but, as far, as I see, it leads to smaller binary
  uint8_t *pData2 = (uint8_t*)pData;
  for (; size>0; size--) {
    eeprom_write_byte((unsigned char *)(eeprom_addres++), *pData2++);
  }
}
