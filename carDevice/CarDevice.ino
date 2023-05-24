#include "GpsNeo6mVK.h"
#include <EEPROM.h>
#include <SimpleTimer.h>

#include "led1602CarDevice.h"
#include "DateTime.h"
#include "HandlerDataGPS.h"

#define gpsSerial Serial2

#define RXD2 16
#define TXD2 17

#define touthtime 500
#define threshold 62 // 40
#define TACHOMETER 25 

GpsNeo6mVK gps;
TinyGPS gpsTiny;
SimpleTimer timer2;
led1602CarDevice lcdDev = led1602CarDevice();
DateTime dateTime = DateTime();
HandlerDataGPS handlerGPS = HandlerDataGPS();

float voltage;
float flat, flon;
unsigned long iiage;
int iyear;
byte bmonth, bday, bhour, bminute, bsecond;
unsigned long touchTimeSwitch;
unsigned long taxometerReset;

TypeScreen typeScreenDisplay;
//----------------------------------------------------------------------------------------------------------------
void refreshLcd(){

  if (gps.getNewTime()){
    dateTime.setDateTime(gps.getYear(), gps.getMonth(), gps.getDay(), gps.getHour(), gps.getMinute());
    dateTime.plusHour(3);
  }     
    
  if (gps.getNewData())
  {
    lcdDev.setNewDataTrue(); 

    handlerGPS.calcMaxSpeed(gps.f_speed_kmph());
    handlerGPS.calcAltitude(gps.f_altitude());
    handlerGPS.calcSatellites(gps.satellites());
    handlerGPS.calcPosition(gps.getLat(), gps.getLon());

  } else {
    lcdDev.setNewDataFalse();
  }
  
  handlerGPS.readSensor();
  handlerGPS.calcVoltage();
  dateTime.runningClock(); 

  switch (typeScreenDisplay)
  {    
    case ALL_DISPLAY:
    if(millis() - taxometerReset < 1300){
        lcdDev.printAllDisplay(gps.f_speed_kmph(), handlerGPS.getFrequency(), dateTime.getHour(), dateTime.getMinute(), handlerGPS.getVoltagee(), handlerGPS.getTrack(), handlerGPS.getTempIn());
    } else {
        lcdDev.printAllDisplay(gps.f_speed_kmph(), 0, dateTime.getHour(), dateTime.getMinute(), handlerGPS.getVoltagee(), handlerGPS.getTrack(), handlerGPS.getTempIn());
    }      
          
    break;
    case GPS_SPEEDOMETER:
      lcdDev.printSpeed(gps.f_speed_kmph(), handlerGPS.getAvgSpeed(), handlerGPS.getMaxSpeed());
    break;
    case DATE_TIME_ALL:
      lcdDev.printDateTime(dateTime.getYear(), dateTime.getMonth(), dateTime.getDay(), dateTime.getHour(), dateTime.getMinute());
      
    break;
    case TOTAL_TRACK:
      lcdDev.printTotalTrack(handlerGPS.getTrack(), handlerGPS.getTotalTrack());
    break;
    case COURSE:
      lcdDev.printCourse(gpsTiny.cardinal (handlerGPS.f_course()), handlerGPS.f_course(), handlerGPS.getTotalCourse());
    break;
    case ALTITUDE:
      lcdDev.printAltitude(gps.f_altitude(), handlerGPS.getTotalAltitude());
    break;
    case VOLTAGE:
      lcdDev.printVoltage(handlerGPS.getVoltagee(), handlerGPS.getMinVoltagee(), handlerGPS.getMaxVoltage());
    break;
    case TEMPERATURE:
      lcdDev.printTemperature(handlerGPS.getTempIn(), handlerGPS.getTempOut());
    break;
    case FREQUENCY:
      if(millis() - taxometerReset < 1300){
        lcdDev.printFrecuency(handlerGPS.getFrequency(), handlerGPS.getMinFrequency(), handlerGPS.getMaxFrequency());
      } else {
        lcdDev.printFrecuency(0, handlerGPS.getMinFrequency(), handlerGPS.getMaxFrequency());
      }
    break;
    case POSITION_GPS:
      lcdDev.printPosition(1000000 * gps.getLat(), 1000000 * gps.getLon());
    break;
    case SATELLITES:
      lcdDev.printSatellite(gps.satellites(), handlerGPS.getMaxSatellites());
    break;    
    
  default:
    break;
  }

}

void switchDisplay(){

  if(millis() - touchTimeSwitch > touthtime){
    switch (typeScreenDisplay)
    {   
      case ALL_DISPLAY:
        typeScreenDisplay = GPS_SPEEDOMETER;
      break;
      case GPS_SPEEDOMETER:
        typeScreenDisplay = DATE_TIME_ALL;
      break;
      case DATE_TIME_ALL:
        typeScreenDisplay = TOTAL_TRACK;
      break;
      case TOTAL_TRACK:
        typeScreenDisplay = COURSE;
      break;
      case COURSE:
        typeScreenDisplay = ALTITUDE;
      break;
      case ALTITUDE:
        typeScreenDisplay = VOLTAGE;
      break;
      case VOLTAGE:
        typeScreenDisplay = TEMPERATURE;
      break;
      case TEMPERATURE:
        typeScreenDisplay = FREQUENCY;
      break;
      case FREQUENCY:
        typeScreenDisplay = POSITION_GPS;
      break;
      case POSITION_GPS:
        typeScreenDisplay = SATELLITES;
      break;
      case SATELLITES:
        typeScreenDisplay = ALL_DISPLAY;
      break;    
      
    default:
      break;
    }
    touchTimeSwitch = millis();
  }

}

ICACHE_RAM_ATTR void tachoCalc() {
  handlerGPS.tachoCalcClass();
  taxometerReset = millis();
}

void setup()
{	 		
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    

    pinMode(TACHOMETER, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TACHOMETER), tachoCalc, FALLING);

    touchAttachInterrupt(T0, switchDisplay, threshold);

    EEPROM.begin(12);
    sensorIn.begin();
    sensorOut.begin();
    lcd.begin();
    lcd.backlight();
    
    timer2.setInterval(500L, refreshLcd);

    lcdDev.printGreeting();
    typeScreenDisplay = ALL_DISPLAY;
        
}

void loop()
{  
  timer2.run();

  char inChar;
  String data;

   while (gpsSerial.available() > 0){
     inChar = gpsSerial.read();
     data += inChar;
   }  
   //Serial.println(data);

  gps.dataProcessing(data);

  delay(2);
}    