#include <SPI.h>
#include <digitalWriteFast.h>
#include "iBoardRF24.h"
#include <EEPROM2.h>

//nrf
iBoardRF24 radio(3,8,5,6,7,2);
//настройки 
int Setting[256][2];
unsigned short int SettingCount;

//
#define id=21 //id сервера

//paramname -1-rele
//          -2-SensorNode


// структура для приема данных с sensor Node
typedef struct{         
  int ParamID;        // id
  int ParamNAME;
  float ParamT;         // ТЕмпература
  float ParamP;    // влажность почвы
  bool vcc;
  
}
SensorNode_OUT;

SensorNode_OUT SN; //созжаем структуру


//структура для отправки на miniBord
typedef struct
{
    int RB_id; //id получателя
    int id_OUT; // id servera
    bool Srele_OUT; //состояния реле
}
miniRB_OUT;
miniRB_OUT RB_IN; 

void setup(void)
{
 
  
  Serial.begin(9600);
  //send_Setting();//вызвать один раз для настройки 
  //считываем настройки из EEPROM
  EEPROM_read(32, Setting); //
  EEPROM_read(8, SettingCount);
  //nrf
  radio.begin();
  radio.setDataRate(RF24_1MBPS); //выбор скорости 
  //radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(100); //тут установка канала
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15); //надо
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.openReadingPipe(1,0xF0F0F0F0D2LL);
  radio.startListening();
  //
  RB_IN.id_OUT  id;//задаем id для отправки клиенту
  
  
}
void add_setting(int ser_id, int rele_id, int sensor_id) 
{
  
   Setting[SettingCount+1][0]=ser_id;
   Setting[SettingCount+1][1]=rele_id;
   Setting[SettingCount+1][2]=sensor_id; 
   SettingCount++;
   EEPROM_write(32, Setting); //записать
   EEPROM_write(8, SettingCount); //записать количество элементов
}
void send_Setting()//предварительные настройки
{
  SettingCount=0;
  
  Setting[0][0] id;//id server
  Setting[0][1]=1;//id Rele
  Setting[0][2]=2;//id Sensor Node
  EEPROM_write(8, SettingCount);
  EEPROM_write(32, Setting);
  
  Serial.println("//////");
  Serial.println(SettingCount);
  Serial.println("//////");
  Serial.println();
  for(int i=0;i<=SettingCount;i++)
  {
    for(int j=0;j<=2;j++)
    {
      Serial.print(Setting[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
      
}

void RBord_IN(bool Srele,int RB_id_client)
{
  radio.stopListening();
  
  RB_IN.Srele_OUT=Srele;
  RB_IN.RB_id=RB_id_client;
  bool ok = radio.write( &RB_IN, sizeof(RB_IN)); //Отправка данных структуры
  radio.startListening();
}

void VievEnternet()
{
  
  
};

bool authentication()//проверка от кого пришли данные
{
  if (radio.available()) 
  {
    radio.read( &SN, sizeof(SN)); 
    for(int i=0;i<=SettingCount;i++)
      if (SN.ParamID==Setting[i][SN.ParamNAME])
      {
         Serial.println( SN.ParamID);
         Serial.println( SN.ParamNAME);
         Serial.println( SN.ParamT);
         Serial.println( SN.ParamP);
         Serial.println( SN.vcc);
      }
  }
  
}

void loop(void)
{

  if (Serial.available())
  {
    bool Srele=Serial.read()-48;
    RBord_IN(Srele,Setting[SettingCount][1]);
  
  }
  bool ok = authentication();
  
 /*
 if (radio.available()) 
   {
     radio.read( &SN, sizeof(SN)); 
     Serial.println( SN.ParamID);
     Serial.println( SN.ParamNAME);
     Serial.println( SN.ParamT);
     Serial.println( SN.ParamP);
     Serial.println( SN.vcc);
     Serial.write("yes\n");
     
   }
    //Serial.write("no\n");
  
   //delay(300);
*/
  }

  


