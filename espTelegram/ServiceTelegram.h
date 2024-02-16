#ifndef SERVICE_TELEGRAM_H
#define SERVICE_TELEGRAM_H

#include "TelegramHandler.h"
#include "DataPropertiesEeprom.h"
#include "LogicRelayTelegram.h"

class ServiceTelegram{

  private:
    DataPropertiesEeprom dataPropertiesEeprom = DataPropertiesEeprom();
    TelegramHandler telegramHandler = TelegramHandler();
    LogicRelayTelegram logicRelayTelegram = LogicRelayTelegram();    

  public:

  ServiceTelegram(){
    
    logicRelayTelegram.setDataPropertiesEeprom(&dataPropertiesEeprom);
    logicRelayTelegram.setTelegramHandler(&telegramHandler);
    telegramHandler.setDataPropertiesEeprom(&dataPropertiesEeprom);

    telegramHandler.setPtrIsPumpWork(logicRelayTelegram.getPtrIsPumpWork());
    telegramHandler.setPtrIsPumpWork_2(logicRelayTelegram.getPtrIsPumpWork_2());

    telegramHandler.setPtrTempSensor(logicRelayTelegram.getPtrTempSensor());
    telegramHandler.setPtrTempSensor_2(logicRelayTelegram.getPtrTempSensor_2());
  }

  void readFromEeprom(){
    telegramHandler.readFromEeprom();
  }

  void newMsg(FB_msg& msg) {
    telegramHandler.newMsg(msg);
  }

  void logicRelay(){
    logicRelayTelegram.logicRelay();
  }



};

#endif