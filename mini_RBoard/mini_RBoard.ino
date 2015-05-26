#include <SPI.h>
#include <digitalWriteFast.h>
#include "iBoardRF24.h"

#define rele 4 //Пин для управления реле
#define id 1 //id устройства
#define name 1 //Имя устройства
int Server_id[]={21}; //id серверов от которых можно принимать данные

#define countof (sizeof(Server_id)/sizeof(Server_id[0]))

// Set up nRF24L01 radio on iBoard
iBoardRF24 radio(9,10,11,12,13,2);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL,0xF0F0F0F0D2LL};


//Структура для принятия

typedef struct
{
    int RB_id; //id получателя 
    int id_OUT; //id servera
    bool Srele_OUT; //состояния реле
}
miniRB_OUT;
miniRB_OUT RB_OUT; 


void setup(void)
{
 //Для NRF
  radio.begin();
  radio.setDataRate(RF24_1MBPS); //выбор скорости
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(100); //тут установка канала
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15); //надо
  radio.openWritingPipe(pipes[1]); // Открываем канал передачи
  radio.openReadingPipe(1,pipes[0]); // Открываем канал приема  
  
// Для реле
  pinMode(rele,OUTPUT);
//начальные настройки  
 // RB_IN.Srele_IN=LOW;
  RB_OUT.id_OUT=id;
//  RB_IN.name_IN=name;
//  digitalWrite(rele,RB_IN.Srele_IN);
  
  radio.startListening(); //начинаем слушать
}

void HRele(void)//применить состояния на реле
{
  for(int i=0;i<=countof;i++)
    if (RB_OUT.id_OUT ==Server_id[i])
      digitalWrite(rele,RB_OUT.Srele_OUT);
}



void loop(void)
{
   
   if (radio.available()) 
   {
     radio.read( &RB_OUT, sizeof(RB_OUT)); 
     if (RB_OUT.RB_id==id)
       HRele(); //выстовить реле
     //OUT(); //отослать ответ //пока что не работает и нафиг не нужна
   } 

}
  


