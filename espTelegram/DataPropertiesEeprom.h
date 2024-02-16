#ifndef DATA_PROPERTIES_EEPROM_H
#define DATA_PROPERTIES_EEPROM_H

#include <EEPROM.h>

class DataPropertiesEeprom{

    private:
    int tempPumpON;
    int tempPumpOFF;
    int timePumpON;
    int tempMin;
    int tempMax;

    int tempPumpON_2;
    int tempPumpOFF_2;
    int timePumpON_2;
    int tempMin_2;
    int tempMax_2;    

public:

    
void saveToEeprom(){
  if(isDifferentParametrSave()){
    EEPROM.write(0, tempPumpON);
    EEPROM.write(1, tempPumpOFF);
    EEPROM.write(2, timePumpON);
    EEPROM.write(3, tempMin);
    EEPROM.write(4, tempMax);

    EEPROM.write(5, tempPumpON_2);
    EEPROM.write(6, tempPumpOFF_2);
    EEPROM.write(7, timePumpON_2);
    EEPROM.write(8, tempMin_2);
    EEPROM.write(9, tempMax_2);

    EEPROM.commit();
  }
}


void readFromEeprom(){
  tempPumpON = EEPROM.read(0);
  tempPumpOFF = EEPROM.read(1);
  timePumpON = EEPROM.read(2);
  tempMin = EEPROM.read(3);
  tempMax = EEPROM.read(4);

  tempPumpON_2 = EEPROM.read(5);
  tempPumpOFF_2 = EEPROM.read(6);
  timePumpON_2 = EEPROM.read(7);
  tempMin_2 = EEPROM.read(8);
  tempMax_2 = EEPROM.read(9);

}

bool isDifferentParametrSave(){

    if(tempPumpON != EEPROM.read(0)){
      return true;
    }
    if(tempPumpOFF != EEPROM.read(1)){
      return true;
    }
    if(timePumpON != EEPROM.read(2)){
      return true;
    }
    if(tempMin != EEPROM.read(3)){
      return true;
    }
    if(tempMax != EEPROM.read(4)){
      return true;
    }

    if(tempPumpON_2 != EEPROM.read(5)){
      return true;
    }
    if(tempPumpOFF_2 != EEPROM.read(6)){
      return true;
    }
    if(timePumpON_2 != EEPROM.read(7)){
      return true;
    }
    if(tempMin_2 != EEPROM.read(8)){
      return true;
    }
    if(tempMax_2 != EEPROM.read(9)){
      return true;
    }

    return false;
  }


    void setTempPumpON(int temp){
        tempPumpON = temp;
    }
    void setTempPumpON_2(int temp){
        tempPumpON_2 = temp;
    }

    void setTempPumpOFF(int temp){
        tempPumpOFF = temp;
    }
    void setTempPumpOFF_2(int temp){
        tempPumpOFF_2 = temp;
    }

    void setTimePumpON(int timeM){
        timePumpON = timeM;
    }
    void setTimePumpON_2(int timeM){
        timePumpON_2 = timeM;
    }

    void setTempMin(int temp){
        tempMin = temp;
    }
    void setTempMin_2(int temp){
        tempMin_2 = temp;
    }

    void setTempMax(int temp){
        tempMax = temp;
    }
    void setTempMax_2(int temp){
        tempMax_2 = temp;
    }

    int getTempPumpON(){return tempPumpON;}
    int getTempPumpON_2(){return tempPumpON_2;}
    int getTempPumpOFF(){return tempPumpOFF;}
    int getTempPumpOFF_2(){return tempPumpOFF_2;}
    int getTimePumpON(){return timePumpON;}
    int getTimePumpON_2(){return timePumpON_2;}
    int getTempMin(){return tempMin;}
    int getTempMin_2(){return tempMin_2;}
    int getTempMax(){return tempMax;}
    int getTempMax_2(){return tempMax_2;}

    String getSTempPumpON(){return (String)tempPumpON;}
    String getSTempPumpON_2(){return (String)tempPumpON_2;}
    String getSTempPumpOFF(){return (String)tempPumpOFF;}
    String getSTempPumpOFF_2(){return (String)tempPumpOFF_2;}
    String getSTimePumpON(){return (String)timePumpON;}
    String getSTimePumpON_2(){return (String)timePumpON_2;}
    String getSTempMin(){return (String)tempMin;}
    String getSTempMin_2(){return (String)tempMin_2;}
    String getSTempMax(){return (String)tempMax;}
    String getSTempMax_2(){return (String)tempMax_2;}

};

#endif