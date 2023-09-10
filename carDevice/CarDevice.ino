#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SimpleTimer.h>

#include "GpsNeo6mVK.h"
#include "led1602CarDevice.h"
#include "DateTime.h"
#include "HandlerDataGPS.h"
#include "WebControl.h"

#define gpsSerial Serial2

#define RXD2 16
#define TXD2 17

#define touthtime 500
#define threshold 50 //62  // 40
#define TACHOMETER 25

char* ssid = "Tav_Spi";
char* password = "12345679";

WebServer server(80);

GpsNeo6mVK gps;
TinyGPS gpsTiny;
SimpleTimer timer5;
led1602CarDevice lcdDev = led1602CarDevice();
DateTime dateTime = DateTime();
HandlerDataGPS handlerGPS = HandlerDataGPS();
WebContlol webContlol = WebContlol(&handlerGPS);

float voltage;
float flat, flon;
unsigned long iiage;
int iyear;
byte bmonth, bday, bhour, bminute, bsecond;
unsigned long touchTimeSwitch;
unsigned long taxometerReset;
//String data;
int viewResetAllDisplay;
bool bViewClear;
int bProductMountDayCount;

TypeScreen typeScreenDisplay;
//----------------------------------------------------------------------------------------------------------------

void handleRoot() {
  char arrDate[8];

  try {
    server.send(200, "text/html", webContlol.getPage());

    webContlol.resetCheck();

    if (server.hasArg("time_zone")) {
      strcpy(arrDate, server.arg("time_zone").c_str());
      webContlol.setTimeZone(arrDate);
    }

    if (server.hasArg("time_zone_check")) {
      webContlol.saveTimeZone();
    }

    if (server.hasArg("mileage")) {
      strcpy(arrDate, server.arg("mileage").c_str());
      webContlol.setMileage(arrDate);
    }

    if (server.hasArg("mileage_check")) {
      webContlol.saveMileage();
    }

    if (server.hasArg("gaz_check")) {
      webContlol.resetTrackGaz();
    }

    if (server.hasArg("oil_check")) {
      webContlol.resetTrackOil();
    }

    webContlol.saveAll();
    handlerGPS.saveCheckWeb();

  } catch (...) {}
}

void refreshLcd() {

  Serial.println(millis() / 1000);

  handlerGPS.checkFrequency();  

  if(bViewClear && typeScreenDisplay == ALL_DISPLAY){
    lcdDev.clearLcdDisplay();
    bViewClear = false;
  }

  viewResetAllDisplay++;

  try {    

    if (millis() - taxometerReset > 900) {
      handlerGPS.setFrequencyZero();
    }    

    if (gps.getNewTime()) {
      dateTime.setDateTime(gps.getYear(), gps.getMonth(), gps.getDay(), gps.getHour(), gps.getMinute());
      dateTime.plusHour(handlerGPS.getTimeZone());      
    }

    if (gps.getNewData()) {
      lcdDev.setNewDataTrue();
      bProductMountDayCount++;

      if(bProductMountDayCount > 4){
        handlerGPS.setProductMountDay(100 * dateTime.getMonth() + dateTime.getDay(), gps.getYear());      
      }

      handlerGPS.calcMaxSpeed(gps.f_speed_kmph());
      handlerGPS.calcAltitude(gps.f_altitude());
      handlerGPS.calcSatellites(gps.satellites());
      handlerGPS.calcPosition(gps.getLat(), gps.getLon());

    } else {
      lcdDev.setNewDataFalse();
      bProductMountDayCount = 0;
    }

    handlerGPS.readSensor();
    handlerGPS.calcVoltage();
    dateTime.runningClock();

    switch (typeScreenDisplay) {
      case ALL_DISPLAY:
        lcdDev.printAllDisplay(gps.f_speed_kmph(), handlerGPS.getFrequency(), dateTime.getHour(), dateTime.getMinute(), 
          handlerGPS.getVoltagee(), handlerGPS.getTrack(), handlerGPS.getTempOut(), gpsTiny.cardinal(handlerGPS.f_course()));       
        break;
      case MILEAGE:
        lcdDev.printMileage(handlerGPS.getTrackDay(), handlerGPS.getTrackAll(), handlerGPS.getTrackGaz(), handlerGPS.getTrackOil());
        break;
      case ENGINE_HOURS:
        lcdDev.printEngineHours(handlerGPS.getEngineTimeDay(), handlerGPS.getEngineTimeAll(), 
          handlerGPS.getEngineTimeGaz(), handlerGPS.getEngineTimeOil());
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
        lcdDev.printCourse(gpsTiny.cardinal(handlerGPS.f_course()), handlerGPS.f_course(), handlerGPS.getTotalCourse());
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
          lcdDev.printFrecuency(handlerGPS.getFrequency(), handlerGPS.getMinFrequency(), handlerGPS.getMaxFrequency());
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
  } catch (...) {
    Serial.println("main refreshLcd()");
  }
}

void switchDisplay() {

  bViewClear = true;

  Serial.print("viewResetAllDisplay  ");
  Serial.println(viewResetAllDisplay);
  
  if(viewResetAllDisplay > 3 && typeScreenDisplay != ALL_DISPLAY){
    typeScreenDisplay = SATELLITES;
  }
  viewResetAllDisplay = 0;

  try {
    if (millis() - touchTimeSwitch > touthtime) {
      switch (typeScreenDisplay) {
        case ALL_DISPLAY:
          typeScreenDisplay = MILEAGE;
          break;
        case MILEAGE:
          typeScreenDisplay = ENGINE_HOURS;
          break;
        case ENGINE_HOURS:
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
  } catch (...) {}
}

ICACHE_RAM_ATTR void tachoCalc() {
  handlerGPS.tachoCalcClass();
  taxometerReset = millis();
}

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(TACHOMETER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TACHOMETER), tachoCalc, FALLING);
  touchAttachInterrupt(T0, switchDisplay, threshold);

  EEPROM.begin(200);
  delay(500);

  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.begin();

  sensorIn.begin();
  sensorOut.begin();
  lcd.begin();
  lcd.backlight();  

  lcdDev.printGreeting();
  typeScreenDisplay = ALL_DISPLAY;
  handlerGPS.readTrack(); 
  timer5.setInterval(1000L, refreshLcd);
}

void loop() {
  timer5.run();
  server.handleClient();

  char inChar;
  String data = "";  

  if(gpsSerial.available() > 0){

    while (gpsSerial.available() > 0) {
      inChar = gpsSerial.read();
      data += inChar;
    }
    gps.dataProcessing(data);
  }  
  //Serial.println(data);    

  delay(2);
}