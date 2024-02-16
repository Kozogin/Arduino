#ifndef LOGIC_RELAY_TELEGRAM_H
#define LOGIC_RELAY_TELEGRAM_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#define TEMP_SENSOR_WOOD 2
#define TEMP_SENSOR_CAPACITY 0
#define PUMP_WOOD 14
#define PUMP_CAPACITY 12
#define ALARM_PIN 15

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWireWood(TEMP_SENSOR_WOOD);
OneWire oneWireCapacity(TEMP_SENSOR_CAPACITY);
DallasTemperature sensorWood(&oneWireWood);
DallasTemperature sensorCapacity(&oneWireCapacity);

class LogicRelayTelegram{

private:

  DataPropertiesEeprom* ptrDataProperties;
  TelegramHandler* ptrTelegramHandler;

  unsigned long millisOn;
  unsigned long millisOn_2;
  unsigned long attemptOnOff;
  unsigned long attemptOnOff_2;

  unsigned long timer10;
  int coefSendAlarm = 6;

  int tempSensor;
  int tempSensor_2;
  bool isPumpWork;
  bool isPumpWork_2;

  bool allowOffPump;
  bool allowOffPump_2;
    

public:

void setDataPropertiesEeprom(DataPropertiesEeprom *dataPro){
  ptrDataProperties = dataPro;
}

void setTelegramHandler(TelegramHandler *telegr){
  ptrTelegramHandler = telegr;
}

void readSensor() {

  int iter = 0;
  int iter_2 = 0;
  sensorWood.requestTemperatures();
  sensorCapacity.requestTemperatures();
  tempSensor = sensorWood.getTempCByIndex(0);
  tempSensor_2 = sensorCapacity.getTempCByIndex(0);

  while(tempSensor < -100 && iter < 3){
    sensorWood.requestTemperatures();
    tempSensor = sensorWood.getTempCByIndex(0);
    Serial.println();
    Serial.print("iter = ");
    Serial.print(iter);
    Serial.print(" , t = ");
    Serial.print(tempSensor);
    Serial.print(" *C    ");

    iter++;
    delay(50);
  }  

  while(tempSensor < -100 && iter_2 < 3){
    sensorCapacity.requestTemperatures();
    tempSensor_2 = sensorCapacity.getTempCByIndex(0);
    iter_2++;
    delay(50);
  } 
}

void showDisplay(){

  lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print((int)tempSensor);
    lcd.print("*C pump 1 ");

    if(isPumpWork){
      digitalWrite(PUMP_WOOD, LOW);
      lcd.print("ON ");
    } else {
      digitalWrite(PUMP_WOOD, HIGH);
      lcd.print("OFF");
    }

    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print((int)tempSensor_2);
    lcd.print("*C pump 2 ");    

    if(isPumpWork_2){
      digitalWrite(PUMP_CAPACITY, LOW);
      lcd.print("ON ");
    } else {
      digitalWrite(PUMP_CAPACITY, HIGH);
      lcd.print("OFF");
    }      

}

void operationRelay(){

  if(isPumpWork){
      digitalWrite(PUMP_WOOD, LOW);
    } else {
      digitalWrite(PUMP_WOOD, HIGH);
    }
    if(isPumpWork_2){
      digitalWrite(PUMP_CAPACITY, LOW);
    } else {
      digitalWrite(PUMP_CAPACITY, HIGH);
    } 
}

int koefDowntime(int tSensor, int tOn, int tOff){
  return 5 - 5 * (tSensor - tOff) / (tOn - tOff);
}
   
void logicRelay(){   

  if(millis() - timer10 > 10000){

    readSensor();
    operationRelay();
    showDisplay();

    if(tempSensor > ptrDataProperties->getTempPumpON() && !isPumpWork){
      isPumpWork = true;
      ptrTelegramHandler->changeStateSend(true);
      allowOffPump = false;
      attemptOnOff = 0; 
      millisOn = millis();
    }
    if(tempSensor_2 > ptrDataProperties->getTempPumpON_2() && !isPumpWork_2){
      isPumpWork_2 = true;
      ptrTelegramHandler->changeStateSend(true);
      allowOffPump_2 = false;
      attemptOnOff_2 = 0; 
      millisOn_2 = millis();
    }    

    if(tempSensor < ptrDataProperties->getTempPumpOFF() && isPumpWork){
      if(millis() - millisOn > 60000 * ptrDataProperties->getTimePumpON()){        
        isPumpWork = false;
        ptrTelegramHandler->changeStateSend(true);
      }
    }
    if(tempSensor_2 < ptrDataProperties->getTempPumpOFF_2() && isPumpWork_2){
      if(millis() - millisOn_2 > 60000 * ptrDataProperties->getTimePumpON_2()){
        isPumpWork_2 = false;
        ptrTelegramHandler->changeStateSend(true);
      }
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int downTown;
    if(isPumpWork){
      downTown = 1;
    } else {
      downTown = koefDowntime(tempSensor, ptrDataProperties->getTempPumpON(), ptrDataProperties->getTempPumpOFF());
    }

    if(allowOffPump){
      if(tempSensor > ptrDataProperties->getTempPumpOFF() && tempSensor < ptrDataProperties->getTempPumpON()){
        if(attemptOnOff > downTown * 6 * ptrDataProperties->getTimePumpON()){
          attemptOnOff = 0;         
          isPumpWork = !isPumpWork;
          ptrTelegramHandler->changeStateSend(false);
        }
        attemptOnOff++;
      }
    }

    if(isPumpWork_2){
      downTown = 1;
    } else {
      downTown = koefDowntime(tempSensor_2, ptrDataProperties->getTempPumpON_2(), ptrDataProperties->getTempPumpOFF_2());
    }

    if(allowOffPump_2){
      if(tempSensor_2 > ptrDataProperties->getTempPumpOFF_2() && tempSensor_2 < ptrDataProperties->getTempPumpON_2()){
        if(attemptOnOff_2 > downTown * 6 * ptrDataProperties->getTimePumpON_2()){
          attemptOnOff_2 = 0;          
          isPumpWork_2 = !isPumpWork_2;
          ptrTelegramHandler->changeStateSend(false);
        }

        attemptOnOff_2++;
      }
    }

    if(tempSensor < (ptrDataProperties->getTempPumpOFF() + ptrDataProperties->getTempPumpON()) / 2){
      allowOffPump = true;
    }
    if(tempSensor_2 < (ptrDataProperties->getTempPumpOFF_2() + ptrDataProperties->getTempPumpON_2()) / 2){
      allowOffPump_2 = true;
    }

    if(tempSensor < -120){
      isPumpWork = true;
    }
    if(tempSensor_2 < -120){
      isPumpWork_2 = true;
    }


    String noticeAlarm = "ALARM, ";
    bool isSendAlarm = false;

    if(tempSensor < ptrDataProperties->getTempMin()){
      isSendAlarm = true;
      noticeAlarm += "temperature is low --> " + (String)tempSensor + "*C \n";
    }
    if(tempSensor > ptrDataProperties->getTempMax()){
      isSendAlarm = true;
      noticeAlarm += "temperature is high --> " + (String)tempSensor + "*C \n";
    }
    if(tempSensor_2 < ptrDataProperties->getTempMin_2()){
      isSendAlarm = true;
      noticeAlarm += "temperature 2 is low --> " + (String)tempSensor_2 + "*C \n";
    }
    if(tempSensor_2 > ptrDataProperties->getTempMax_2()){
      isSendAlarm = true;
      noticeAlarm += "temperature 2 is high --> " + (String)tempSensor_2 + "*C \n";
    }

    if(isSendAlarm){
      if(coefSendAlarm > 5){
        ptrTelegramHandler->sendAlarm(noticeAlarm);
        coefSendAlarm = 0;
      }
      coefSendAlarm++;
    } else {
      coefSendAlarm = 6;
    }

    timer10 = millis();
  }
}

bool *getPtrIsPumpWork(){
  return &isPumpWork;
}
bool *getPtrIsPumpWork_2(){
  return &isPumpWork_2;
}
int *getPtrTempSensor(){
  return &tempSensor;
}
int *getPtrTempSensor_2(){
  return &tempSensor_2;
}

};


#endif