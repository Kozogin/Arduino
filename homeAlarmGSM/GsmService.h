#ifndef GSM_SERVICE_H
#define GSM_SERVICE_H

#include "HandlerMessage.h"
#include "Buffer.h"
#include "Performer.h"
#include "WebControl.h"

class GsmService{

private:
    Buffer buffer = Buffer();
    HandlerMessage handlerMessage = HandlerMessage();
    Performer performer = Performer();
    WebContlol webContlol = WebContlol();

public:
    GsmService(){}

    void refreshUssd(){
        webContlol.refreshData();
        performer.setUssdEeprom(webContlol.getUssAc());
    }

    void setWebcontrol(WebContlol &webContlolGsm){        
        webContlol = webContlolGsm;
    }    

    void bufferRead(){
        buffer.bufferRead();
    }

    void split(std::string message){
        handlerMessage.split(message);
    }

    void setHandlerMessage(){
        handlerMessage.setHandlerMessage();
    }

    void resetMessageGsm(){
        buffer.resetMessageGsm();
    }

    void resetAll(){
        performer.resetAll();
    }

    void saveTime(){
        performer.saveTime();
    }

    bool checkTime(){
        return performer.checkTime();
    }    

    void checkOwners(){
        std::string array[6];

        try{
            webContlol.getOwnerTelephones(array);
            performer.checkOwners(array, handlerMessage, webContlol);
        }
		catch (...)
		{
		} 
        
    }

    bool getAllowPerformer(){       
        return performer.getAllowPerformer();
    }

    bool checkTelephone(std::string telephone){
        return handlerMessage.checkTelephone(telephone);
    }

    bool getOnAlarm(){
        return Performer::getOnAlarm();
    }

    bool getRelayUser(){
        return Performer::getRelayUser();
    }

    bool getCallBack(){
        return Performer::getCallBack();
    }

    bool getSmsBack(){
        return Performer::getSmsBack();
    }

    bool getSmsUssd(){
        return Performer::getSmsUssd();
    }

    int getAlarmOperation(){
        return Performer::getAlarmOperation();
    }    

    std::string getTelephone(){
        return handlerMessage.getTelephone();
    }

    std::string getMessage(){
        return handlerMessage.getMessage();
    }

    std::string getMessageSave(){
        return handlerMessage.getMessageSave();
    }

    void setOnAlarm(bool state){
        Performer::setOnAlarm(state);
        performer.saveTime();
    }

    void setRelayUser(bool state){
        Performer::setRelayUser(state);
        performer.saveTime();
    }

    void setCallBack(bool state){
        Performer::setCallBack(state);
        performer.saveTime();
    }

    void setSmsBack(bool state){
        Performer::setSmsBack(state);
        performer.saveTime();
    }

    void setSmsUssd(bool state){
        Performer::setSmsUssd(state);
        performer.saveTime();
    }

    void setAlarmOperation(int state){
        Performer::setAlarmOperation(state);
        performer.saveTime();
    }
    
    void runService(){

        bufferRead();
        std::string messageGsm = buffer.getMessageGsm();

    try{
        if(messageGsm != ""){
            split(messageGsm);
            setHandlerMessage();       
            resetMessageGsm();

            saveTime();
        }

        if(getAllowPerformer()){
            if (checkTime())
            {                
                webContlol.deleteAll();
                webContlol.refreshData();                
                checkOwners();
                resetAll();
            }
        } 
        }
		catch (...)
		{}      

    }

    void setIndexUnsolicitedMessage(int indexUnsolicitedMessage){
        handlerMessage.setIndexUnsolicitedMessage(indexUnsolicitedMessage);
    }    

    int getIndexUnsolicitedMessage(){
        return handlerMessage.getIndexUnsolicitedMessage();
    }    

};

#endif