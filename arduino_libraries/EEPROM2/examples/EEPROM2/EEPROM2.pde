#include <EEPROM2.h>
//  Sample for EEPROM library v2
//  see http://freeduino.ru for details

void setup() {
  // --- Part 1 of sample ---
  // Here we are going to count every 'reset' of Arduino board.
  // We need counter, it has type 'unsigned long',
  // and takes 4 bytes of memory. We will store it in EEPROM at address 8  -
  // it will occupy 4 bytes with addressed 8, 9, 10, 11.
  // As we know, setup() is called after each Arduino reset. We will read
  // counter from EEPROM, increment it, and write back to EEPROM.
  // And finally, we will print counter to serial port.
  long counter;
  // long value can be read from EEPROM this way:
  EEPROM_read_mem(8, &counter, sizeof(counter));
  //              ^  ^         ^- size of data (4 bytes for type 'long' )
  //              |  \- addres of data in RAM
  //              \-EEPROM address
  // Or, we can do the same with a bit simpler code:
  EEPROM_read(8, counter);
  // increment counter:
  counter++;
  // write counter to EEPROM. Here we could use EEPROM_write_mem with
  // syntax like EEPROM_read_mem, but I like EEPROM_write more. :)
  EEPROM_write(8, counter);
  // print counter to serial port:
  Serial.begin(9600);
  Serial.print("It's reset #"); Serial.println(counter, DEC);

  // --- Part 2 of sample ---
  // Here we will show, that EEPROM_write and EEPROM_read are able
  // to work with any types of data, including arrays.
  // First of all, we will write array of 8 'int' numbers to EEPROM to
  // address 32. It will occupy 16 bytes (because 'int' has a size of 2 bytes)
  // from 32 to 47.
  int my_array[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  EEPROM_write(32, my_array);
  // Next, we will fill array contents in RAM with zeroes
  int i;
  for (i=0; i<8; i++) {
    my_array[i] = 0;
  }
  // and print it to serial port. We will see zeroes:
  for (i=0; i<8; i++) {
    Serial.print(my_array[i], DEC); Serial.print(" ");
  }
  Serial.println();
  // Next, we will read array back from EEPROM:
  EEPROM_read(32, my_array);
  // And print it again. We will see our numbers:
  for (i=0; i<8; i++) {
    Serial.print(my_array[i], DEC); Serial.print(" ");
  }
  Serial.println();
  // after running this sketch you can remove 'EEPROM_write(32, my_array);' line,
  // and see, that contents of EEPROM is saved after reset or power loss.
}

void loop() {
}
