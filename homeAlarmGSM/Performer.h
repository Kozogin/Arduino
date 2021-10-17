#ifndef PERFORMER_H
#define PERFORMER_H

#include <string>

class Performer
{
private:    
    const int DELAY_MESSAGE = 5000;
    bool m_allowDelay;
    bool m_allowPerformer;

    bool m_onAlarm;
    bool m_offAlarm;
    bool m_ring2;
    bool m_ring3;
    bool m_ring4;
    bool m_callBack;
    bool m_smsBack;
    bool m_relay1;
    bool m_relay2;
    unsigned long m_timeMess; 

    std::string m_password = "my";
    std::string m_telephone = "+380676759301";
    std::string sendMessage = "send message check 2021 ";  

public:

    void resetAll(){
        m_allowDelay = false;
        m_allowPerformer = false;

        m_onAlarm = false;
        m_offAlarm = true;
        m_ring2 = false;
        m_ring3 = false;
        m_ring4 = false;
        m_callBack = false;
        m_smsBack = false;
        m_relay1 = false;
        m_relay2 = false;
    }

    void saveTime(){
        m_timeMess = millis();
        m_allowPerformer = true;
        std::cout << "save TIME "<< m_timeMess << "\n";
    }

    bool checkTime(){
        if(millis() - m_timeMess > DELAY_MESSAGE && m_allowPerformer){
            m_allowDelay = true;
            std::cout << "delta TIME "<< millis() - m_timeMess << "\n";
        }
        return m_allowDelay;
    }

    void choiceAction(HandlerMessage &handlerMessage){
        if(handlerMessage.getNoCarrier()){
            switch (handlerMessage.getRing())
            {
            case 2:
                m_ring2 = true;
                m_ring3 = false;
                m_ring4 = false;
                std::cout << "RING2 "<< "\n";                    
                   // SerialAT.println("ATD+380676759301;");

                        SerialAT.print("ATD");
                        SerialAT.print(m_telephone.c_str());
                        SerialAT.print(";\n");
                        


                    delay(5000);
                    m_ring2 = false;
                break;
            case 3:
                m_ring2 = false;
                m_ring3 = true;
                m_ring4 = false;
                 std::cout << "RING3 "<< "\n";
                break;
            case 4:
                m_ring2 = false;
                m_ring3 = false;
                m_ring4 = true;
                 std::cout << "RING4 "<< "\n";

                 //SerialAT.print("AT+CMGS=\"+380676759301\"\r");

                    SerialAT.print("AT+CMGS=\"");
                    SerialAT.print(m_telephone.c_str());
                    SerialAT.print("\"\r");                    
                    SerialAT.print(sendMessage.c_str());
                    delay(500);
                    SerialAT.write((char)26);
                    delay(5000);

                break;

            default:
                break;
            }
        }

         if(handlerMessage.getTypeMessage() == INFO){
            SerialAT.println("AT+CMGL=\"REC UNREAD\"");
            delay(500);
         }

        if(handlerMessage.getMessage().find(std::string("alarmon").append(m_password)) != -1 && handlerMessage.getTelephone() == m_telephone){
            m_onAlarm = true;
            m_offAlarm = false;
            std::cout << "ON ALARM "<< "\n";
        }

        if(handlerMessage.getMessage().find(std::string("alarmoff").append(m_password)) != -1 && handlerMessage.getTelephone() == m_telephone){
            m_onAlarm = false;
            m_offAlarm = true;
            std::cout << "OFF ALARM "<< "\n";
        }

        if(handlerMessage.getMessage().find(std::string("relay1on").append(m_password)) != -1 && handlerMessage.getTelephone() == m_telephone){
            m_relay1 = true;
            std::cout << "ON RELAY1 "<< "\n";
            digitalWrite(LED_GPIO, LED_ON);
        }

        if(handlerMessage.getMessage().find(std::string("relay1off").append(m_password)) != -1 && handlerMessage.getTelephone() == m_telephone){
            m_relay1 = false;
            std::cout << "OFF RELAY1 "<< "\n";
            digitalWrite(LED_GPIO, LED_OFF);
        }

        if(handlerMessage.getMessage().find(std::string("relay2on").append(m_password)) != -1 && handlerMessage.getTelephone() == m_telephone){
            m_relay2 = true;
            std::cout << "ON RELAY2 "<< "\n";
        }

        if(handlerMessage.getMessage().find(std::string("relay2off").append(m_password)) != -1 && handlerMessage.getTelephone() == m_telephone){
            m_relay2 = false;
            std::cout << "OFF RELAY2 "<< "\n";
        }
        
    }

    void setOnAlarm(bool state){
        m_onAlarm = state;
        saveTime();
    }

    void setOffAlarm(bool state){
        m_offAlarm = state;
        saveTime();
    }

    void setRing2(bool state){
        m_ring2 = state;
        saveTime();
    }

    void setRing3(bool state){
        m_ring3 = state;
        saveTime();
    }

    void setRing4(bool state){
        m_ring4 = state;
        saveTime();
    }

    void setCallBack(bool state){
        m_callBack = state;
        saveTime();
    }

    void setSmsBack(bool state){
        m_smsBack = state;
        saveTime();
    }

    void setRelay1(bool state){
        m_relay1 = state;
        saveTime();
    }

    void setRelay2(bool state){
        m_relay2 = state;
        saveTime();
    }

    bool getAllowPerformer(){return m_allowPerformer;}
    bool getOnAlarm(){return m_onAlarm;}
    bool getOffAlarm(){return m_offAlarm;}
    bool getRing2(){return m_ring2;}
    bool getRing3(){return m_ring3;}
    bool getRing4(){return m_ring4;}
    bool getCallBack(){return m_callBack;}
    bool getSmsBack(){return m_smsBack;}
    bool getRelay1(){return m_relay1;}
    bool getRelay2(){return m_relay2;}
};

#endif