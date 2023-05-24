#ifndef lED1602_CARDEVICE_H
#define lED1602_CARDEVICE_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

enum TypeScreen{
  ALL_DISPLAY, GPS_SPEEDOMETER, DATE_TIME_ALL, TOTAL_TRACK, COURSE, ALTITUDE, VOLTAGE, TEMPERATURE, FREQUENCY, POSITION_GPS, SATELLITES
};

class led1602CarDevice
{
  private:
    TypeScreen typeScr;    
    bool m_newData;

    void bringFormat(int value){
      if(abs(value) < 100){
        lcd.print(" ");
        if(abs(value) < 10){
          lcd.print(" ");
        }
      }
      lcd.print(value);
    }

    void bringFormat1000(int value1000){
      if(abs(value1000) < 1000){
          lcd.print(" ");
          bringFormat(value1000);
      } else {
        lcd.print(value1000);
      }      
    }

    void bringFormatMinus(float valueMinus){
      if(valueMinus < 0){          
        } else {
          lcd.print(" ");
        }
    }
 
    void printDateTimePrivate(int yearN, byte month, byte dayN, byte hour, byte minute){
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(dayN);
      lcd.print("/");
      lcd.print(month);
      lcd.print("/");
      lcd.print(yearN);

      lcd.setCursor(10,0);
      bringFormat(hour);      
      lcd.print(":");
      if(minute < 10){
        lcd.print("0");
      }
      lcd.print(minute);

      lcd.setCursor(0,1);
      lcd.print("TOTAL");
      lcd.setCursor(10,1);
      bringFormat(millis()/3600000); 
      lcd.print(":");
      if(getMinuteTotal() < 10){
        lcd.print("0");
      }
      lcd.print(getMinuteTotal());
    }

    int getMinuteTotal(){
      return (millis() / 60000) % 60;
    }

    String addPoint(long positionL){
      String strPosition = (String)(positionL);
      return strPosition.substring(0, 2) + "." + strPosition.substring(2, 8);
    }

  String meterKilometer(int track) {

    String s = "";
    String spaceInsert = "     ";

    if (track < 10) {
      s = "0.00" + (String)(track);
    }
    else if (track < 100) {
      s = "0.0" + (String)(track);
    }
    else if (track < 1000) {
      s = "0." + (String)(track);
    }
    else {
      s = (String)(track);
      s = s.substring(0, s.length() - 3) + "." + s.substring(s.length() - 3, 3);      
    }	

    return spaceInsert.substring(0, 8 - s.length()) + s;
  }
    
  public:

  led1602CarDevice(){ }

  void printAllDisplay(int speed, int frecuencyNow, byte hour, byte minute, float voltageVal, int track, int outTemp){
    if(typeScr != ALL_DISPLAY){    
      lcd.clear();      
      typeScr = ALL_DISPLAY;
    } 
      lcd.setCursor(0,0);
      if(m_newData){
          bringFormat(speed); 
        } else {
          lcd.print("---"); 
        }     
      
    lcd.setCursor(5,0);
    bringFormat1000(frecuencyNow);     
    
    lcd.setCursor(10,0);
    bringFormat(hour);      
    lcd.print(":");
    if(minute < 10){
      lcd.print("0");
    }
    lcd.print(minute);

    lcd.setCursor(0,1);
    lcd.print(voltageVal);
    lcd.setCursor(4,1);
    lcd.print(meterKilometer(track));
    lcd.setCursor(12,1);
    bringFormatMinus(outTemp);    
    bringFormat(outTemp);    
  }
  
  void printSpeed(float speedFloat, float speedAvg, float maxSpeed){

    int speed = (int)speedFloat;
    if(speed < 0 || speed > 300){
      speed = 0;
    }
    if(speedAvg < 0 || speedAvg > 300){
      speedAvg = 0;
    }
    if(maxSpeed < 0 || maxSpeed > 300){
      maxSpeed = 0;
    }

    if(typeScr != GPS_SPEEDOMETER){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("SPEED kmh");
      
      
      typeScr = GPS_SPEEDOMETER;
    }               
      lcd.setCursor(12,0);
      if(m_newData){
        bringFormat(speed); 
      } else {
        lcd.print("---"); 
      } 
      lcd.setCursor(3,1);
      bringFormat(speedAvg);       
      lcd.setCursor(12,1);
      bringFormat(maxSpeed);        
     
  }

  void printDateTime(int yearN, byte month, byte dayN, byte hour, byte minute){
    
    if(yearN < 2020 || yearN > 2100){
      yearN = 2021;
    }
    if(month < 1 || month > 12){
      month = 1;
    }
    if(dayN < 1 || dayN > 31){
      dayN = 1;
    }
    if(hour < 0 || hour > 59){
      hour = 0;
    }
    if(minute < 0 || minute > 59){
      minute = 0;
    }

     if(typeScr != DATE_TIME_ALL){
      printDateTimePrivate(yearN, month, dayN, hour, minute);
      typeScr = DATE_TIME_ALL;
     } else {
        printDateTimePrivate(yearN, month, dayN, hour, minute);
     }
  }

  void printTotalTrack(float track, float moving){

    if(track < 0 || track > 5000000){
      track = 0;
    }
    if(moving < 0 || moving > 5000000){
      moving = 0;
    }

    if(typeScr != TOTAL_TRACK){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("TRACK km");
      lcd.setCursor(0,1);
      lcd.print("MOVING");         
      
      typeScr = TOTAL_TRACK;
    }      
    lcd.setCursor(8,0);
    lcd.print(meterKilometer(track)); 
    lcd.setCursor(8,1);
    lcd.print(meterKilometer(moving));      
        
  }

  void printCourse(std::string strCourse, int intCourse, int totalCourse){

    if(intCourse < 0 || intCourse > 360){
      intCourse = 0;
    }
    if(totalCourse < 0 || totalCourse > 360){
      totalCourse = 0;
    }

    if(typeScr != COURSE){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("COURSE");
      lcd.setCursor(0,1);
      lcd.print("TOTAL");
      
      typeScr = COURSE;
    }       
      lcd.setCursor(8,0);
      for (size_t i = 0; i < 3 - strCourse.size(); i++)
      {
        lcd.print(" ");
      }
      lcd.print(strCourse.c_str());
      lcd.setCursor(13,0);
      if(m_newData){
        bringFormat(intCourse); 
      } else {
        lcd.print("---"); 
      }         
      lcd.setCursor(13,1);
      bringFormat(totalCourse);       
       
  }

  void printAltitude(float altitude, float totalAltitude){

    if(altitude < 0 || altitude > 5000){
      altitude = 0;
    }
    if(totalAltitude < -3000 || totalAltitude > 5000){
      totalAltitude = 0;
    }

    if(typeScr != ALTITUDE){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ALTITUDE");
      lcd.setCursor(0,1);
      lcd.print("INCREASE");
           
      typeScr = ALTITUDE;
    }       
    lcd.setCursor(11,0);
    if(m_newData){
      bringFormat1000(altitude);
    } else {
      lcd.print("----");
    }           
    lcd.setCursor(10,1);
    bringFormatMinus(totalAltitude);    
    bringFormat1000(totalAltitude);
      
  }

  void printVoltage(float voltageVal, float maxVoltage, float minVoltage){

    if(typeScr != VOLTAGE){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("VOLTAGE"); 

      typeScr = VOLTAGE;
    } 
    lcd.setCursor(11,0);
    lcd.print(voltageVal);
    lcd.setCursor(0,1);
    lcd.print(maxVoltage);
    lcd.setCursor(11,1);
    lcd.print(minVoltage);    
  }

  void printTemperature(int inTemp, int outTemp){

    if(inTemp < -55 || inTemp > 100){
      inTemp = -40;
    }
    if(outTemp < -55 || outTemp > 100){
      outTemp = -40;
    }

    if(typeScr != TEMPERATURE){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("TEMPERATURE   *C");
      lcd.setCursor(0,1);
      lcd.print("IN      OUT");

      typeScr = TEMPERATURE;
    }       
    lcd.setCursor(3,1);
    if(inTemp > -40){
      bringFormatMinus(inTemp);    
      bringFormat(inTemp);
    } else {
      lcd.print("  ---");
    }  
    lcd.setCursor(12,1);
    if(inTemp > -40){
      bringFormatMinus(outTemp);    
      bringFormat(outTemp);
    } else {
      lcd.print("  ---");
    } 

  }

  void printFrecuency(int frecuencyNow, int frecuencyMin, int frecuencyMax){    

    if(typeScr != FREQUENCY){
      lcd.clear();
      lcd.setCursor(10,0);
      lcd.print("min-1");      

      typeScr = FREQUENCY;
    } 
    lcd.setCursor(4,0);    
    bringFormat1000(frecuencyNow);        
    lcd.setCursor(0,1);
    if(frecuencyMin == 9000){
      bringFormat1000(0);
    } else {
      bringFormat1000(frecuencyMin);
    }    
    lcd.setCursor(11,1);
    bringFormat1000(frecuencyMax);    
  }

  void printPosition(long lat, long lon){
    if(typeScr != POSITION_GPS){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LAT");      
      lcd.setCursor(0,1);
      lcd.print("LON"); 
      typeScr = POSITION_GPS;
    } 
    lcd.setCursor(6,0);
    lcd.print(addPoint(lat));
    lcd.setCursor(6,1);
    lcd.print(addPoint(lon));       
  }

  void printSatellite(int satellite, int satelliteMax){

    if(satellite > 200){
      satellite = 0;
    }
    
    if(typeScr != SATELLITES){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("SATELLITES");      
      lcd.setCursor(0,1);
      lcd.print("MAX"); 
      
      typeScr = SATELLITES;
    }        
      lcd.setCursor(12,0);
      if(m_newData){
        bringFormat(satellite); 
      } else {
        lcd.print("---"); 
      }              
      lcd.setCursor(4,1);
      bringFormat(satelliteMax);
       
  }  

  void printGreeting(){
    lcd.print("  tavria start  ");
    delay(2000); 
    lcd.clear();   
  }

  void setNewDataFalse(){
    m_newData = false;
  }

  void setNewDataTrue(){
    m_newData = true;
  } 
 
};    
#endif