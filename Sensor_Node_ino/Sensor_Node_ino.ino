#include <SPI.h>
#include <digitalWriteFast.h>
#include "iBoardRF24.h"
#include <avr/sleep.h>
#include <avr/wdt.h>


// описание параметров модуля
#define SID 500                        // идентификатор датчика Внешний 1
#define CNT 30 // количество циклов по 8 секунд между "посылками" (30 = 4 минуты между посылками)
volatile boolean wdt_tripped=1;
int count; 
int tests=0;
//const
#define ID 2
#define name 2
//nrf
iBoardRF24 radio(8,7,11,12,13,2);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL,0xF0F0F0F0D2LL};

//
// создаём структуру для передачи значений
typedef struct{         
  int ParamID;        // id
  int ParamNAME;
  float ParamT;         // ТЕмпература
  float ParamP;    // влажность почвы
  bool vcc;
  
}
Message;

#define LED 9
#define BUTTON 4

Message sensor;  //создаем структуру
///

//режим сна для МК
void system_sleep() {
  delay(2);                            // Wait for serial traffic
  _SFR_BYTE(ADCSRA) &= ~_BV(ADEN);     // Switch ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();                        // System sleeps here
  sleep_disable();
  _SFR_BYTE(ADCSRA) |= _BV(ADEN);      // Switch ADC on
}

void wdt_interrupt_mode() {
  wdt_reset();
  WDTCSR |= _BV(WDIE); // Restore WDT interrupt mode
}

ISR(WDT_vect) {
  wdt_tripped=1;  // set global volatile variable
}

////


void setup(void)
{

  wdt_disable();
  wdt_reset();
  wdt_enable(WDTO_8S);   //пробуждение каждые 8 сек

  
    // светик 
  pinMode(LED, OUTPUT);
  //nrt
  radio.begin();
  radio.setDataRate(RF24_1MBPS); //выбор скорости
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(100); //тут установка канала
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15); //надо
  radio.openWritingPipe(pipes[1]); // Открываем канал передачи
  radio.openReadingPipe(1,pipes[0]); // Открываем канал приема  
  //
  
  radio.stopListening();
}

int mode =1;

void loop(void)
{ 

  //тут можно увеличить интервал времени между отправками данных по RF24 за счёт счётчика циклов
  wdt_interrupt_mode();
  
  if (wdt_tripped) {
    count++;
    wdt_tripped = 0;
    
    // отправим данные, если уже "пора" 
    if (count == ((mode==1) ? (count) : (CNT))) {
      
      // зажжем светодиод
      if (mode == 1) {
        digitalWrite(LED, HIGH);
      }
      
      radio.powerUp();  //подаём питание на NRF24
      delay(20);
        sendSensorMessage();
        
      radio.powerDown(); // отключаем питание RF24
      delay(20);
  
      count = 0;
      // погасим светодиод
      if (mode == 1) {
        digitalWrite(LED, LOW);
      }
    }
  }
  
  if(tests<10) {
    mode = 1;
    tests++;
  }
  else {
    mode = 0;
  }
  
  // спать!
  system_sleep();   //МК засыпает

}
  

// отправить сообщение (идентификатор параметра)
void sendSensorMessage() 
{
  float vcc=((float) readVcc()/1000.0);
  //подготовим данные в структуру для передачи
  sensor.ParamID=ID;
  sensor.ParamNAME=name;        
  sensor.ParamT = (((float)analogRead(A2) * vcc / 1024.0) - 0.5)/0.01;       
  sensor.ParamP= (((float)analogRead(A0) * vcc / 1024.0) - 0.5)/0.01;  
  sensor.vcc = ( (vcc > 2.67) ? 1 : 0 ); 
  

  //отправляем данные по RF24
  bool ok = radio.write( &sensor, sizeof(sensor) ); 

  delay (20); 
  return;
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(75); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}



