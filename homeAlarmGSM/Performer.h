#ifndef PERFORMER_H
#define PERFORMER_H

#include "WebControl.h"
#include <string>

class Performer
{
private:    
    const int DELAY_MESSAGE = 5000;
    bool m_allowDelay;
    bool m_allowPerformer;
    int m_allowNextActionAt;

    std::string m_ussdEeprom;

    static bool s_onAlarm;      
    static bool s_callBack;
    static bool s_smsBack;
    static bool s_smsUssd;   
    static bool s_relayUser;
    static int s_alarmOperation;

    unsigned long m_timeMess;
    
    std::string sendMessage = "send message check 2021 ";  

public:    

    void resetAll(){
        m_allowDelay = false;
        m_allowPerformer = false;
        m_allowNextActionAt = 0;
    }

    void saveTime(){
        m_timeMess = millis();
        m_allowPerformer = true;
    }

    bool checkTime(){
        if(m_timeMess > millis()){
            m_timeMess = millis();
        }
        if(millis() - m_timeMess > DELAY_MESSAGE && m_allowPerformer){
            m_allowDelay = true;
        }
        return m_allowDelay;
    }

    //-----------------------------------------------------------------------------
    void checkOwners(std::string* array, HandlerMessage &handlerMessage, WebContlol &webContlol){ 

        bool isPresent = false;
        readMessegge(handlerMessage, webContlol);

        try{

            for (size_t i = 0; i < 6; i++)
            {            
                if(handlerMessage.getTelephone() == array[i] && handlerMessage.getTelephone().size() > 10){
                    //std::cout << "have such telephone    " << "\n";
                    choiceAction(handlerMessage, webContlol);
                    isPresent = true;
                    break;
                }
            } 
            if(!isPresent){
                handlerMessage.resetAll();
            }  
        }
        catch (...)
		{
		}
        
    }
    //-----------------------------------------------------------------------------
     void readMessegge(HandlerMessage &handlerMessage, WebContlol &webContlol){     
        
         if(handlerMessage.getTypeMessage() == INFO){ 
             SerialAT.println("AT+CMGL=\"REC UNREAD\"");          
         }        
    }
    //-----------------------------------------------------------------------------
    void choiceAction(HandlerMessage &handlerMessage, WebContlol &webContlol){     
        bool deleteSms = true;;
        try{
            if(handlerMessage.getNoCarrier()){
                switch (handlerMessage.getRing())
                {
                case 2:
                    //std::cout << "RING2 "<< "\n";
                    break;
                case 3:
                    //std::cout << "RING3 "<< "\n";
                    break;
                case 4:
                    //std::cout << "RING4 "<< "\n";
                    break;
                default:
                    break;
                }
            }        

            if(handlerMessage.getTypeMessage() == INFO){
                SerialAT.println("AT+CMGL=\"REC UNREAD\"");
                delay(500);
            }        

            if(handlerMessage.getMessage().rfind(webContlol.getPassword()) == 
                handlerMessage.getMessage().length() - webContlol.getPassword().length() - 1){

                if(handlerMessage.getMessage().find(std::string("alarmon")) != -1){
                    s_onAlarm = true;
                    s_callBack = true;
                    EEPROM.write(EEPROM_ON_ALARM, 255);
                    EEPROM.commit();
                    //std::cout << "ON ALARM "<< "\n";
                }

                if(handlerMessage.getMessage().find(std::string("alarmoff")) != -1){
                    s_onAlarm = false;
                    s_callBack = true;
                    s_alarmOperation = 2;
                    EEPROM.write(EEPROM_ON_ALARM, 0);
                    EEPROM.commit();
                    //std::cout << "OFF ALARM "<< "\n";
                }                

                if(handlerMessage.getMessage().find(std::string("relay2on")) != -1){
                    s_relayUser = true;
                    EEPROM.write(EEPROM_ON_RNDRELE, 255);
                    EEPROM.commit();
                    //std::cout << "ON RELAY2 "<< "\n";
                }

                if(handlerMessage.getMessage().find(std::string("relay2off")) != -1){
                    s_relayUser = false;
                    EEPROM.write(EEPROM_ON_RNDRELE, 0);
                    EEPROM.commit();
                    //std::cout << "OFF RELAY2 "<< "\n";
                }

                if(handlerMessage.getMessage().find(std::string("status")) != -1){
                    s_smsBack = true;                 
                    //std::cout << "STATUS "<< "\n";
                }

                if(handlerMessage.getMessage().find(std::string("ussd")) != -1){                    
                    std::string send = "AT+CUSD=1,\"";
                    send = send.append(m_ussdEeprom);
                    send = send.append("\"");

                    SerialAT.println(send.c_str());
                    delay(500);
                    deleteSms = false;
                    s_smsUssd = true;
                    //std::cout << "USSD 101 "<< "\n";
                }

                if(handlerMessage.getMessage().find(std::string("time")) != -1){

                    std::string mess = handlerMessage.getMessage().substr(4);
                    int lenMess = mess.length();
                    int lenPass = webContlol.getPassword().length();
                    mess = mess.substr(0, lenMess - lenPass);

                    for (int i = 0; i < mess.size(); i++)
                    {
                        if (mess[i] == '/' || mess[i] == '-') {			
                            mess.replace(i, 1, 1, ':');
                        }
                    }
                    //std::cout << "TIME SET "<< mess <<"\n";
                    setSystemTime(mess); 
                }
                if(deleteSms){
                    SerialAT.println("AT+CMGDA=\"DEL READ\"");
                    delay(1000);
                    SerialAT.println("AT+CMGDA=\"DEL SENT\"");
                    delay(1000);
                }
            }

            handlerMessage.resetAll(); 
        }
        catch (...)
		{
		}            
    }

    void setSystemTime(std::string mess){
        
        HandlerMessage handlerMessage = HandlerMessage();
        handlerMessage.split(mess);
        handlerMessage.printObjMessage();

        std::string arr[6];
        handlerMessage.getFragment(arr);
        int yearS = 0;
        int mounmS = 0;
        int dayS = 0;
        int hourS = 0;
        int minutS = 0;
        
        try
        {
            yearS = (int)atoi(arr[0].c_str());
            mounmS = (int)atoi(arr[1].c_str());
            dayS = (int)atoi(arr[2].c_str());
            hourS = (int)atoi(arr[3].c_str());
            minutS = (int)atoi(arr[4].c_str());
        
        if(yearS > 2021 && yearS < 2100){ 
            if(mounmS > 0 && mounmS < 13){
                if(dayS > 0 && dayS < 32){
                    if(hourS > -1 && hourS < 24){
                        if(minutS > -1 && minutS < 60){                                                        
                            dateTime.setDateTime(yearS, mounmS, dayS, hourS, minutS);
                        }
                    }
                }
            }
        } 
        }
        catch (...) {
        }
        
    }

    void setUssdEeprom(std::string kod){
        m_ussdEeprom = kod;
    }

    static void setOnAlarm(bool state){
        s_onAlarm = state;
    }    
    
    static void setCallBack(bool state){
        s_callBack = state;
    }

    static void setSmsBack(bool state){
        s_smsBack = state;
    }

    static void setSmsUssd(bool state){
        s_smsUssd = state;
    }  

    static void setRelayUser(bool state){
        s_relayUser = state;
    }

    static void setAlarmOperation(int state){
        s_alarmOperation = state;
    }

    bool getAllowPerformer(){return m_allowPerformer;}
    static bool getOnAlarm(){return s_onAlarm;}
    static bool getCallBack(){return s_callBack;}
    static bool getSmsBack(){return s_smsBack;}
    static bool getSmsUssd(){return s_smsUssd;}
    static bool getRelayUser(){return s_relayUser;}
    static int getAlarmOperation(){return s_alarmOperation;}
    
};

bool Performer::s_onAlarm = false;
bool Performer::s_callBack = false;
bool Performer::s_smsBack = false;
bool Performer::s_smsUssd = false;
bool Performer::s_relayUser = false;
int Performer::s_alarmOperation = 0;

#endif