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

#ifndef EEPROM2_h
#define EEPROM2_h

#include <inttypes.h>

uint8_t EEPROM_read_byte(int eeprom_addres);              //reads 1-byte value
void EEPROM_write_byte(int eeprom_addres, uint8_t value); //writes 1-byte value
void EEPROM_read_mem(int eeprom_addres, void* pData, int size);        //reads data block
void EEPROM_write_mem(int eeprom_addres, const void* pData, int size); //writes data block

//templates for other data types
template< typename T > void EEPROM_read(int eeprom_addres, T &data)        //reads data
{
  EEPROM_read_mem(eeprom_addres, &data, sizeof(data));
}

template< typename T > void EEPROM_write(int eeprom_addres, const T &data) //writes data
{
  EEPROM_write_mem(eeprom_addres, &data, sizeof(data));
}

#endif
