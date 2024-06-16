#ifndef SERVICE_TELEGRAM_H
#define SERVICE_TELEGRAM_H

#include "TelegramHandler.h"
#include "DataPropertiesEeprom.h"
#include "LogicRelayTelegram.h"
#include "WebContlol.h"

class ServiceTelegram {

private:
  DataPropertiesEeprom dataPropertiesEeprom = DataPropertiesEeprom();
  TelegramHandler telegramHandler = TelegramHandler();
  LogicRelayTelegram logicRelayTelegram = LogicRelayTelegram();
  WebContlol webContlol = WebContlol();

public:

  ServiceTelegram() {

    logicRelayTelegram.setDataPropertiesEeprom(&dataPropertiesEeprom);
    logicRelayTelegram.setTelegramHandler(&telegramHandler);
    telegramHandler.setDataPropertiesEeprom(&dataPropertiesEeprom);
    webContlol.setDataPropertiesEeprom(&dataPropertiesEeprom);

    telegramHandler.setPtrTempContuor_2(logicRelayTelegram.getPtrTempContuor_2());
    telegramHandler.setPtrTempContuor_3(logicRelayTelegram.getPtrTempContuor_3());
    telegramHandler.setPtrTempContuor_4(logicRelayTelegram.getPtrTempContuor_4());
    telegramHandler.setPtrTempContuor_5(logicRelayTelegram.getPtrTempContuor_5());
    telegramHandler.setPtrTempContuor_6(logicRelayTelegram.getPtrTempContuor_6());

    telegramHandler.setPtrTempOut(logicRelayTelegram.getPtrTempOut());
    telegramHandler.setIsPumpOn(logicRelayTelegram.getIsPumpOn());

    telegramHandler.setSendMessage(logicRelayTelegram.getSendMessage());
    dataPropertiesEeprom.setPtrOnSendMessage(logicRelayTelegram.getSendMessage());

    webContlol.setPtrTempContuor_2(logicRelayTelegram.getPtrTempContuor_2());
    webContlol.setPtrTempContuor_3(logicRelayTelegram.getPtrTempContuor_3());
    webContlol.setPtrTempContuor_4(logicRelayTelegram.getPtrTempContuor_4());
    webContlol.setPtrTempContuor_5(logicRelayTelegram.getPtrTempContuor_5());
    webContlol.setPtrTempContuor_6(logicRelayTelegram.getPtrTempContuor_6());

    webContlol.setPtrTempOut(logicRelayTelegram.getPtrTempOut());

    webContlol.setPtrStabilArr(logicRelayTelegram.getPtrStabilChange());
  }

  String getPage() {
    return webContlol.getPage();
  }
  String getPageC2() {
    return webContlol.getPageC2();
  }
  String getPageC3() {
    return webContlol.getPageC3();
  }
  String getPageC4() {
    return webContlol.getPageC4();
  }
  String getPageC5() {
    return webContlol.getPageC5();
  }
  String getPageC6() {
    return webContlol.getPageC6();
  }
  String getPageOption() {
    return webContlol.getPageOption();
  }

  void onOffPoolRadiator() {
    logicRelayTelegram.onOffPoolRadiator();
  }

  void getTimeTelegram() {
    logicRelayTelegram.onPump();
  }

  void startTelegram() {
    bot.inlineMenu("Sauna collector", telegramHandler.printMainMenu());
  }

  void readFromEeprom() {
    telegramHandler.readFromEeprom();
    // setTelegramSsidT();
    // setTelegramPasswordT();
    // setTelegramTokenT();
    // setTelegramChatIdT();
  }

  void saveToEeprom() {
    dataPropertiesEeprom.saveToEeprom();
  }

  void newMsg(FB_msg &msg) {
    telegramHandler.newMsg(msg);
  }

  void logicRelay() {
    logicRelayTelegram.logicRelay();
  }

  void setTempStabil_2(char *arrDate) {
    dataPropertiesEeprom.setTempStabil_2(((String)arrDate).toInt());
  }
  void setTempStabil_3(char *arrDate) {
    dataPropertiesEeprom.setTempStabil_3(((String)arrDate).toInt());
  }
  void setTempStabil_4(char *arrDate) {
    dataPropertiesEeprom.setTempStabil_4(((String)arrDate).toInt());
  }
  void setTempStabil_5(char *arrDate) {
    dataPropertiesEeprom.setTempStabil_5(((String)arrDate).toInt());
  }
  void setTempStabil_6(char *arrDate) {
    dataPropertiesEeprom.setTempStabil_6(((String)arrDate).toInt());
  }

  void setCurve_2(char *arrDate) {
    dataPropertiesEeprom.setCurve_2(((String)arrDate).toInt());
  }
  void setCurve_3(char *arrDate) {
    dataPropertiesEeprom.setCurve_3(((String)arrDate).toInt());
  }
  void setCurve_4(char *arrDate) {
    dataPropertiesEeprom.setCurve_4(((String)arrDate).toInt());
  }
  void setCurve_5(char *arrDate) {
    dataPropertiesEeprom.setCurve_5(((String)arrDate).toInt());
  }
  void setCurve_6(char *arrDate) {
    dataPropertiesEeprom.setCurve_6(((String)arrDate).toInt());
  }

  void changeOnRadiatorPool(bool onRadiator) {
    dataPropertiesEeprom.changeOnRadiatorPool(onRadiator);
  }
  void setStatus_2(bool onManual) {
    dataPropertiesEeprom.setStatus_2(onManual);
  }
  void setStatus_3(bool onManual) {
    dataPropertiesEeprom.setStatus_3(onManual);
  }
  void setStatus_4(bool onManual) {
    dataPropertiesEeprom.setStatus_4(onManual);
  }
  void setStatus_5(bool onManual) {
    dataPropertiesEeprom.setStatus_5(onManual);
  }
  void setStatus_6(bool onManual) {
    dataPropertiesEeprom.setStatus_6(onManual);
  }

  void setStartTime(char *arrDate) {
    dataPropertiesEeprom.setStartTime(((String)arrDate).toInt());
  }
  void setEndTime(char *arrDate) {
    dataPropertiesEeprom.setEndTime(((String)arrDate).toInt());
  }
  void setCorrectX(char *arrDate) {
    dataPropertiesEeprom.setCorrectX(((String)arrDate).toInt());
  }
  void setCorrectA(char *arrDate) {
    dataPropertiesEeprom.setCorrectA(((String)arrDate).toInt());
  }
  void setOutLower(char *arrDate) {
    dataPropertiesEeprom.setOutLower(((String)arrDate).toInt());
  }
  void setContuorLower(char *arrDate) {
    dataPropertiesEeprom.setContuorLower(((String)arrDate).toInt());
  }
  void setNumberContuor(char *arrDate) {
    dataPropertiesEeprom.setNumberContuor(((String)arrDate).toInt());
  }
  void setCountMessage(char *arrDate) {
    dataPropertiesEeprom.setCountMessage(((String)arrDate).toInt());
  }

  void setTypeServoNO_2(bool typeNO) {
    dataPropertiesEeprom.setTypeServoNO_2(typeNO);
  }
  void setTypeServoNO_3(bool typeNO) {
    dataPropertiesEeprom.setTypeServoNO_3(typeNO);
  }
  void setTypeServoNO_4(bool typeNO) {
    dataPropertiesEeprom.setTypeServoNO_4(typeNO);
  }
  void setTypeServoNO_5(bool typeNO) {
    dataPropertiesEeprom.setTypeServoNO_5(typeNO);
  }
  void setTypeServoNO_6(bool typeNO) {
    dataPropertiesEeprom.setTypeServoNO_6(typeNO);
  }

  void setSsidT(char *arrDate) {
    dataPropertiesEeprom.setSsidT((String)arrDate);
  }
  void setPasswordT(char *arrDate) {
    dataPropertiesEeprom.setPasswordT((String)arrDate);
  }
  void setTokenT(char *arrDate) {
    dataPropertiesEeprom.setTokenT((String)arrDate);
  }
  void setChatIdT(char *arrDate) {
    dataPropertiesEeprom.setChatIdT((String)arrDate);
  }

  // void setTelegramSsidT() {
  //   telegramHandler.setTelegramSsidT(dataPropertiesEeprom.getSSsidT());
  // }
  // void setTelegramPasswordT() {
  //   telegramHandler.setTelegramPasswordT(dataPropertiesEeprom.getSPasswordT());
  // }
  // void setTelegramTokenT() {
  //   telegramHandler.setTelegramTokenT(dataPropertiesEeprom.getSTokenT());
  // }
  // void setTelegramChatIdT() {
  //   telegramHandler.setTelegramChatIdT(dataPropertiesEeprom.getSChatIdT());
  // }
  
};

#endif