#ifndef GSM_SERVICE_H
#define GSM_SERVICE_H

#include "HandlerMessage.h"
#include "Buffer.h"
#include "Performer.h"

class GsmService{

private:
    Buffer buffer = Buffer();
    HandlerMessage handlerMessage = HandlerMessage();
    Performer performer = Performer();

public:
    GsmService(){}

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

    void choiceAction(HandlerMessage &handlerMessage){
        performer.choiceAction(handlerMessage);
    }

    bool getAllowPerformer(){
        return performer.getAllowPerformer();
    }    


    void runService(){

        bufferRead();
        std::string messageGsm = buffer.getMessageGsm();

        if(messageGsm != ""){
            split(messageGsm);
            setHandlerMessage();        
            resetMessageGsm();

            saveTime();
        }

        if(getAllowPerformer()){
            if (checkTime())
            {
                std::cout << "CHECK TIME "<< "\n";

                choiceAction(handlerMessage);

                resetAll();
            }
        }



        	
    }
    

};

#endif