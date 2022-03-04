#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

const int COUNT_ZONES = 3;
const int MIN_IMPULS = 6;
const int TIME_CHECK = 5000;
const int TIME_AGAIN = 60000;
const int TIME_DELAY_ZONA1 = 30000;

class InterruptHandler{

private:
    long lastTurn;
    long timeCheck;
    long timeZona1;
    bool firstTime;
    int zonaInt[COUNT_ZONES];
    bool zona[COUNT_ZONES];
    bool needSend;
    
    bool alarmPinChange;
    bool alarmPinState;
    int alarmPinCount;
    bool zona1Delay;  

public:
    InterruptHandler(){        
        firstTime = false;
        needSend = false;       
        alarmPinChange = false;
        alarmPinCount = 0;
        alarmPinState = false;
        zona1Delay = false;
    }

    void alarmPinCheck(){

        if(!digitalRead(ALARM_PIN) && !alarmPinChange){
            alarmPinCount++;
            if(alarmPinCount > 50){
                alarmPinChange = true;
                alarmPinCount = 0;
                alarmPinState = true;
            }
        }

        if(digitalRead(ALARM_PIN) && alarmPinChange){
            alarmPinCount++;
            if(alarmPinCount > 50){
                alarmPinChange = false;
                alarmPinCount = 0;
                alarmPinState = true;
            }
        }
        if(millis() < 15000){
            alarmPinState = false;
        }
    }    

    void runTest(){        
        try{
            if(!needSend && isSendSms()){
                if(!firstTime){                    
                    timeZona1 = millis();                    
                    timeCheck = millis();
                    firstTime = true;                    
                } 
            }
            if(needSend){
                if(zona[0] && !zona[1] && !zona[2]){
                    firstTime = true;
                    timeCheck = millis();
                }
            }
        }
		catch (...)
		{			
		}
    }

    void isZona1Delay(){

        if(zona[1] || zona[2]){
            zona1Delay = true;
        }
        if(zona[0]){
            if(millis() - timeZona1 > TIME_DELAY_ZONA1){
                zona1Delay = true;
            }
        }
    }

    bool isSendSms(){
        isZona1Delay();
        if(millis() - lastTurn > TIME_AGAIN){                               
            return true;
        }
        if(millis() < TIME_AGAIN){
            return true;
        }      
        return false;
    }    

    bool impulsAlarm(){
        bool impulsEnough = false;

        try{        
            for (int i = 0; i < COUNT_ZONES; i++)
            {
                if(zonaInt[i] > MIN_IMPULS){
                    impulsEnough = true;
                    break;
                }
            }
        }
		catch (...)
		{
			std::cout << "InterruptHandler::impulsAlarm()    --------***********------------***********     " << "\n";
		}
        return impulsEnough;
    }

    void reset(){

        try{            
            for (int i = 0; i < COUNT_ZONES; i++)
            {
                zona[i] = false;
                zonaInt[i] = 0;           
            }
            std::cout << "InterruptHandler::isSendSms()  reset()  ------------***********     " << "\n";
        }
		catch (...)
		{
			std::cout << "InterruptHandler::reset()    --------***********------------***********     " << "\n";
		}
        firstTime = false; 
        needSend = false;
        lastTurn = millis();      
    }

    void readPins(){
        
        try{
            if(digitalRead(ZONA_1)){
                zonaInt[0] += 1;
                if(zonaInt[0] > MIN_IMPULS){                    
                    zona[0] = true;
                }
            }
            if(digitalRead(ZONA_2)){
                zonaInt[1] += 1;
                if(zonaInt[1] > MIN_IMPULS){
                    zona[1] = true;
                }
            }
            if(digitalRead(ZONA_3)){
                zonaInt[2] += 1;
                if(zonaInt[2] > MIN_IMPULS){
                    zona[2] = true;
                }
            }   
            if(millis() - timeCheck > TIME_CHECK && !needSend){
                firstTime = false;
                if(impulsAlarm()){                
                    needSend = true;                
                } else{
                    reset();
                    lastTurn = millis() - TIME_AGAIN;
                } 
            }            
        }
        catch (...)
        {
            std::cout << "InterruptHandler::readPins()    --------***********------------***********     " << "\n";
        }
    }

    void setAlarmPinStateReset(){
        alarmPinState = false;
    }

    void resetZona1Delay(){
        zona1Delay = false;
    }
    
    bool getFirstTime(){ return firstTime;}
    bool getNeedSend(){ return needSend;} 
    bool getAlarmPinChange(){ return alarmPinChange;}
    bool getAlarmPinState(){ return alarmPinState;}
    bool getZona1Delay(){ return zona1Delay;}

    bool getZona0(){ return zona[0];}//--
    long gettimeZona1(){ return timeZona1;}//--

    bool getBoolZones(bool* arr){

        bool trigger = false;
        try{
            for (size_t i = 0; i < COUNT_ZONES; i++)
            {                
                arr[i] = zona[i];////
                if(arr[i]){
                    trigger = true;
                }
            }
            if(trigger){
                needSend = true;
            }
            return trigger;
        }
        catch (...)
        {
            std::cout << "InterruptHandler::getBoolZones()    --------***********------------***********     " << "\n";
        }
        return trigger;
    }
    
};

#endif