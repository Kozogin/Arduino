#include <Arduino.h>

#include "utilities.h"
#include "GsmService.h"

#include <WiFi.h>
#include <WebServer.h>

#include <EEPROM.h>
#include "WebControl.h"
#include "state.h"
#include "InterruptHandled.h"
#include <SimpleTimer.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define TIMEOUT_SMS 25000
#define TIMEOUT_CALL 90000

OneWire oneWire(SENSOR);
DallasTemperature sensor(&oneWire);

// ////////////////////////////////////////
#include "esp_system.h"               //
#define WATCHDOG_TIMEOUT_S 90         //
hw_timer_t * watchDogTimer = NULL;    //
// ////////////////////////////////////////
char* ssid = "Al_Gs";
char* password = "12345678";

WebServer server(80);

unsigned long lastTurn;
unsigned long timeZona1;
unsigned long timeForSmsCall;
bool wifiEnabled;
bool smsStart;
int ownerNumber;
int ussdWait;
std::string ownerTelephones[6];

WebContlol webContlol = WebContlol();
GsmService gsmService = GsmService();
InterruptHandler interruptHandler = InterruptHandler();
SimpleTimer timer2;

void handleRoot()
	{  
    char arrDate[14]; 

try{
	  server.send ( 200, "text/html", webContlol.getPage()); 

    if ( server.hasArg("owner1")){
        strcpy(arrDate,server.arg("owner1").c_str());        
        webContlol.setOwner1(arrDate);
    }

    if ( server.hasArg("owner2")){
        strcpy(arrDate,server.arg("owner2").c_str());
        webContlol.setOwner2(arrDate); 
    }

     if ( server.hasArg("owner3")){
        strcpy(arrDate,server.arg("owner3").c_str());
        webContlol.setOwner3(arrDate);
    }

    if ( server.hasArg("owner4")){
        strcpy(arrDate,server.arg("owner4").c_str());
        webContlol.setOwner4(arrDate);
    }

     if ( server.hasArg("owner5")){
        strcpy(arrDate,server.arg("owner5").c_str());
        webContlol.setOwner5(arrDate);
    }

     if ( server.hasArg("owner6")){      
        strcpy(arrDate,server.arg("owner6").c_str()); 
        webContlol.setOwner6(arrDate);
    }
//-----------------------------------------------------------------------------------------

    webContlol.setOwnerCheck(OFF, OFF, OFF, OFF, OFF, OFF);

    if ( server.hasArg("owner1_ckd")){
      strcpy(arrDate,server.arg("owner1_ckd").c_str());
      webContlol.setOwnerCheck(ON, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED);      
      }

     if ( server.hasArg("owner2_ckd")){
      strcpy(arrDate,server.arg("owner2_ckd").c_str());
      webContlol.setOwnerCheck(UNSPECIFIED, ON, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED);
       }

     if ( server.hasArg("owner3_ckd")){
      strcpy(arrDate,server.arg("owner3_ckd").c_str());
      webContlol.setOwnerCheck(UNSPECIFIED, UNSPECIFIED, ON, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED);
     }

    if ( server.hasArg("owner4_ckd")){
      strcpy(arrDate,server.arg("owner4_ckd").c_str());
      webContlol.setOwnerCheck(UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, ON, UNSPECIFIED, UNSPECIFIED);
     }

    if ( server.hasArg("owner5_ckd")){
      strcpy(arrDate,server.arg("owner5_ckd").c_str());
      webContlol.setOwnerCheck(UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, ON, UNSPECIFIED); 
     }
     
    if ( server.hasArg("owner6_ckd")){
      strcpy(arrDate,server.arg("owner6_ckd").c_str());
      webContlol.setOwnerCheck(UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, UNSPECIFIED, ON); 
     } 
//-------------------------------------------------------------------------------------------------

    if ( server.hasArg("uss")){
      strcpy(arrDate,server.arg("uss").c_str());	
      webContlol.setUss(arrDate);       
    }

    if ( server.hasArg("uss_sum")){
      strcpy(arrDate,server.arg("uss_sum").c_str());	
      webContlol.setUssSum(arrDate);       
    }
//------------------------------------------------------------------------------

    if ( server.hasArg("zona1")){
      strcpy(arrDate,server.arg("zona1").c_str());	
      webContlol.setZona1(arrDate);       
    }
    
    if ( server.hasArg("zona2")){
      strcpy(arrDate,server.arg("zona2").c_str());	
      webContlol.setZona2(arrDate);       
    }

    if ( server.hasArg("zona3")){
      strcpy(arrDate,server.arg("zona3").c_str());	
      webContlol.setZona3(arrDate);       
    }

    if ( server.hasArg("zona4")){
      strcpy(arrDate,server.arg("zona4").c_str());	
      webContlol.setZona4(arrDate);       
    }

    if ( server.hasArg("zona5")){
      strcpy(arrDate,server.arg("zona5").c_str());	
      webContlol.setZona5(arrDate);       
    }   

    if ( server.hasArg("password")){
      strcpy(arrDate,server.arg("password").c_str());	
      webContlol.setPassword(arrDate);       
    }  
//-----------------------------------------------------------------------------------    
    
    if ( server.hasArg("really_ckd")){
      {
        char arrDate[14];
        strcpy(arrDate,server.arg("really_ckd").c_str());
      }
         webContlol.saveOwnerTelephone();
         webContlol.saveAllCheckbox();
         webContlol.saveUssZona();

        Serial.println("EEPROM.commit();");
        EEPROM.commit(); 
     }     

    webContlol.refreshData(); 
    }
		catch (...)
		{
		}   
    
}

void setupModem()
{
  pinMode(MODEM_POWER_ON, OUTPUT);    
  digitalWrite(MODEM_POWER_ON, HIGH);    
}

ICACHE_RAM_ATTR void operationZona1() {
  interruptHandler.runTest();
}
ICACHE_RAM_ATTR void operationZona2() {
  interruptHandler.runTest();
}
ICACHE_RAM_ATTR void operationZona3() {
  interruptHandler.runTest();
}

void IRAM_ATTR watchDogInterrupt() {  
  esp_restart();
  ESP.restart();
}

void watchDogRefresh()
{
  timerWrite(watchDogTimer, 0);                    //reset timer (feed watchdog)
}

void setup()
{  
  ///////////////////////////////////////////////////////////////////////    
  watchDogTimer = timerBegin(2, 80, true);
  timerAttachInterrupt(watchDogTimer, &watchDogInterrupt, true);
  timerAlarmWrite(watchDogTimer, WATCHDOG_TIMEOUT_S * 1000000, false);
  timerAlarmEnable(watchDogTimer);
  ///////////////////////////////////////////////////////////////////////
  
    SerialMon.begin(9600);
    SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);        

    delay(100);
    hallRead();
    delay(1000);

    if(hallRead() < 0 || hallRead() > 18){
      wifiEnabled = true;
    } else{
      wifiEnabled = false;
    }

    pinMode(ZONA_1, INPUT_PULLUP);
    pinMode(ZONA_2, INPUT_PULLUP);
    pinMode(ZONA_3, INPUT_PULLUP);
    pinMode(ALARM_PIN, INPUT_PULLUP);    

    pinMode(RELE_SIREN, OUTPUT);
    pinMode(RELE_USER, OUTPUT);
    digitalWrite(RELE_SIREN,HIGH);
    digitalWrite(RELE_USER,HIGH);

    attachInterrupt(digitalPinToInterrupt(ZONA_1), operationZona1, CHANGE); //0->1
    attachInterrupt(digitalPinToInterrupt(ZONA_2), operationZona2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ZONA_3), operationZona3, CHANGE);
   
      setupModem();    
    //--------------------------------------------------------------- 
    delay(1000); 
    
    EEPROM.begin(512);      
    delay(500);      
   
   if(wifiEnabled){ 
     WiFi.softAP(ssid, password); 
      server.on ( "/", handleRoot );
      server.begin();
   }

  delay(1000);
  webContlol.refreshData();
  gsmService.setWebcontrol(webContlol);
  gsmService.refreshUssd();
    
    webContlol.getOwnerTelephones(ownerTelephones);    

  if((int)EEPROM.read(EEPROM_ON_ALARM) < 64){
    gsmService.setOnAlarm(false);
  }else{
    gsmService.setOnAlarm(true);
  }
  if((int)EEPROM.read(EEPROM_ON_RNDRELE) < 64){
    gsmService.setRelayUser(false);
  }else{
    gsmService.setRelayUser(true);
  } 

    timer2.setInterval(30000L, timeSystemTtgo); 
}

void timeSystemTtgo(){
  dateTime.runningClock();
}

void noticeOnlyCallCheck(){
  try{
    bool next = true;
    if(gsmService.getIndexUnsolicitedMessage() == 0){
      while (next)
      {        
        if(gsmService.checkTelephone(ownerTelephones[ownerNumber]) && webContlol.getCheck(ownerNumber)){
          next = false;          
        } else{
          ownerNumber++;
          if(ownerNumber > 5){
            ownerNumber = 0;
            gsmService.setIndexUnsolicitedMessage(8);            
            gsmService.setCallBack(false);
            next = false;
            SerialAT.println("AT+CMGL=\"REC UNREAD\"");
            delay(500);
          }
        } 
      }
    }    

    if(gsmService.getIndexUnsolicitedMessage() == 0){
      gsmService.setIndexUnsolicitedMessage(283);
      timeForSmsCall = millis();
    }
    if((gsmService.getIndexUnsolicitedMessage() == 283 || (millis() - timeForSmsCall > TIMEOUT_SMS 
        && millis() - timeForSmsCall < 2 * TIMEOUT_SMS)) && gsmService.getIndexUnsolicitedMessage() != 8){
          delay(500);
      callToOwner(ownerTelephones[ownerNumber].c_str());      
      timeForSmsCall = millis() - 2 * TIMEOUT_SMS;
      gsmService.setIndexUnsolicitedMessage(383); 
    }
    if((gsmService.getIndexUnsolicitedMessage() == 127 || (millis() - timeForSmsCall > 2 * TIMEOUT_SMS + TIMEOUT_CALL 
        && millis() - timeForSmsCall < 2 * (TIMEOUT_SMS + TIMEOUT_CALL))) && gsmService.getIndexUnsolicitedMessage() != 8){ 
          delay(500);  
      gsmService.setIndexUnsolicitedMessage(0);
      ownerNumber++;       
    } 
    if(gsmService.getIndexUnsolicitedMessage() == 8){ 
      gsmService.setIndexUnsolicitedMessage(0);
      interruptHandler.reset();
    }    
}
		catch (...)
		{
		} 

}

void noticeAll(){

 try{
    bool next = true;
    if(gsmService.getIndexUnsolicitedMessage() == 0){
      while (next)
      {        
        if(gsmService.checkTelephone(ownerTelephones[ownerNumber])){
          next = false;          
        } else{
          ownerNumber++;
          if(ownerNumber > 5){
            ownerNumber = 0;
            gsmService.setIndexUnsolicitedMessage(8);
            interruptHandler.resetZona1Delay();
            next = false;
            SerialAT.println("AT+CMGL=\"REC UNREAD\"");
            delay(700);
          }
        } 
      }
    }    

    if(gsmService.getIndexUnsolicitedMessage() == 0){ 
      digitalWrite(RELE_SIREN,LOW);   
      sendSmsToOwner(ownerTelephones[ownerNumber].c_str());
      gsmService.setIndexUnsolicitedMessage(383);    
      SerialMon.println(textSms());
      timeForSmsCall = millis();
    }
    if((gsmService.getIndexUnsolicitedMessage() == 283 || (millis() - timeForSmsCall > TIMEOUT_SMS 
        && millis() - timeForSmsCall < 2 * TIMEOUT_SMS)) && gsmService.getIndexUnsolicitedMessage() != 8){
          delay(1500);
      callToOwner(ownerTelephones[ownerNumber].c_str());       
      timeForSmsCall = millis() - 2 * TIMEOUT_SMS;
      gsmService.setIndexUnsolicitedMessage(383); 
    }
    if((gsmService.getIndexUnsolicitedMessage() == 127 || (millis() - timeForSmsCall > 2 * TIMEOUT_SMS + TIMEOUT_CALL 
        && millis() - timeForSmsCall < 2 * (TIMEOUT_SMS + TIMEOUT_CALL))) && gsmService.getIndexUnsolicitedMessage() != 8){ 
          delay(1500); 
      gsmService.setIndexUnsolicitedMessage(0);
      ownerNumber++;       
    } 
    if(gsmService.getIndexUnsolicitedMessage() == 8){
      gsmService.setIndexUnsolicitedMessage(0); 
      interruptHandler.reset();
      digitalWrite(RELE_SIREN,HIGH);      
    }    
}
		catch (...)
		{
		} 

}

int readSensor() {
  sensor.requestTemperatures();
  return sensor.getTempCByIndex(0);
}

String textSmsStatusBack(){
  String messageSms = "";
  String zonas [5];

  try{
    webContlol.getZonas(zonas);

    messageSms += "Alarm ";
    if(gsmService.getOnAlarm()){
      messageSms += "ON \n";
    } else {
      messageSms += "OFF \n";
    }
    
    messageSms += "ReleUser ";
    if(gsmService.getRelayUser()){
      messageSms += "ON \n";
    } else {
      messageSms += "OFF \n";
    }

    messageSms += "ReleTimeRnd ";
    if(dateTime.getReleOn()){
      messageSms += "ON \n";
    } else {
      messageSms += "OFF \n";
    }

    messageSms = messageSms + "T: " + readSensor() + "C\n";
    
    messageSms += zonas[0] + " ";
    if(!digitalRead(ZONA_1)){
      messageSms += "OK \n";
    } else {
      messageSms += "no \n";
    }
    messageSms += zonas[1] + " ";
    if(!digitalRead(ZONA_2)){
      messageSms += "OK \n";
    } else {
      messageSms += "no \n";
    }
    messageSms += zonas[2] + " ";
    if(!digitalRead(ZONA_2)){
      messageSms += "OK \n";
    } else {
      messageSms += "no \n";
    }         
     
    messageSms += dateTime.getStringDateTime();
    messageSms += "\n RND time \n morning ";
    messageSms = messageSms + dateTime.getHourMorningOn() + ":" + dateTime.getMinuteMorningOn() + " -> "
      + dateTime.getHourMorningOff() + ":" + dateTime.getMinuteMorningOff() + "\n evening ";

    messageSms = messageSms + dateTime.getHourEveningOn() + ":" + dateTime.getMinuteEveningOn() + " -> "
      + dateTime.getHourEveningOff() + ":" + dateTime.getMinuteEveningOff() + "\n";  
     
    }
		catch (...)
		{
		}       
      
   return messageSms;
}

String textSms(){

  String messageSms = "< ";  
  bool arr[5];
  String zonas [5];

  try{
      interruptHandler.getBoolZones(arr);
      webContlol.getZonas(zonas);
      if(arr[0]){
        messageSms += zonas[0] + ", ";
      }
      if(arr[1]){
        messageSms += zonas[1] + ", ";
      }
      if(arr[2]){
        messageSms += zonas[2] + ", ";
      }
      messageSms += dateTime.getStringDateTime();
      messageSms = messageSms +"\n" + "T: " + readSensor() + "C";
     
    }
		catch (...)
		{
		}       
      
   return messageSms;
}

void callToOwner(String telephone){
  try{
  String callKomand = "ATD" + telephone +";";
  SerialAT.println(callKomand);
  }
  catch (...)
  {
  } 
}

void sendSmsToOwner(String telephone){
  try{
  String callKomand = "AT+CMGS=\"" + telephone + "\"";
    SerialAT.println(callKomand);
    delay(500);

  SerialAT.println(textSms() + "\r\n" + (String)((char)26));
  }
  catch (...)
  {
  } 
}

void sendSmsStatusBack(String telephone){
  try{
    String callKomand = "AT+CMGS=\"" + telephone + "\"";
    SerialAT.println(callKomand);
    delay(500);

    SerialAT.println(textSmsStatusBack() + "\r\n" + (String)((char)26));
    }
		catch (...)
		{
		}
}

void sendSmsStatusUssd(String telephone){
  try{
    String callKomand = "AT+CMGS=\"" + telephone + "\"";
    SerialAT.println(callKomand);
    delay(500);

    SerialAT.println((String)(gsmService.getMessageSave().c_str()) + "\r\n" + (String)((char)26));
    }
		catch (...)
		{
		}
}

void sendSmsStart(){  

  try{
    String messageSms = "Alarm ";
    if(gsmService.getOnAlarm()){
      messageSms += "ON \n";
    } else {
      messageSms += "OFF \n";
    }

    if(gsmService.checkTelephone(ownerTelephones[0])){
      String callKomand = (String)"AT+CMGS=\"" + ownerTelephones[0].c_str() + "\"";
      SerialAT.println(callKomand);
      delay(500);

      SerialAT.println((String)"Start device\n" + (String)"T:" + readSensor() + "C\n" 
      + messageSms
      + "\r\n" + (String)((char)26));
    }
  }
  catch (...)
  {
  }
}

void loop()
{
  timer2.run();
  try{
    gsmService.runService(); 

    if(wifiEnabled){ 
      server.handleClient();
    } 

    if(!smsStart && millis() > 20000){
      sendSmsStart();
      delay(500);
      smsStart = true;
    }
 
 if(!wifiEnabled){ 
    if(gsmService.getOnAlarm()){
      if(interruptHandler.getFirstTime()){
        interruptHandler.readPins();
      }
    
      if(interruptHandler.getNeedSend() && millis() > 15000){         
          if(interruptHandler.isSendSms()){ 
            if(interruptHandler.getZona1Delay()){
              noticeAll(); 
            }
          } 
      }     
          
      if(millis() > 15000 && millis() < 25000){
        bool arr[5]; 
        interruptHandler.getBoolZones(arr);
      }
    }   
 }

  if(gsmService.getCallBack()){    
    noticeOnlyCallCheck();
  }

//-----------------------------------------------------------------
  interruptHandler.alarmPinCheck();
  if(interruptHandler.getAlarmPinState()){
    if(!interruptHandler.getAlarmPinChange()){
      ownerNumber = 0;
      gsmService.setIndexUnsolicitedMessage(0);
      interruptHandler.reset();
      interruptHandler.resetZona1Delay();
      digitalWrite(RELE_SIREN,HIGH);
      EEPROM.write(EEPROM_ON_ALARM, 255);
      interruptHandler.runTest();
    } else {
      EEPROM.write(EEPROM_ON_ALARM, 0);
    }    
    EEPROM.commit();
    gsmService.setOnAlarm(!interruptHandler.getAlarmPinChange());
    if(gsmService.getOnAlarm()){      
      delay(30000);
      interruptHandler.reset();
    }
    gsmService.setCallBack(true);
    interruptHandler.setAlarmPinStateReset();
  }
//----------------------------------------------------------------

  if(millis() - lastTurn > 5000){

    readSensor();

    if(lastTurn > millis()){
      lastTurn = millis();
      timeZona1 = millis();
      timeForSmsCall = millis();
    }

    if(gsmService.getIndexUnsolicitedMessage() < 0){
      gsmService.setIndexUnsolicitedMessage(0);
    }

    // std::cout << " gsmService.getIndexUnsolicitedMessage() " << gsmService.getIndexUnsolicitedMessage() << "\n";
    // std::cout << " getZona0() -->" << interruptHandler.getZona0() 
    //           << " ,millis() - timeZona1 -->" << millis() - interruptHandler.gettimeZona1()
    //           << " ,millis() -->" << millis() 
    //           << " ,getFirstTime() -->" << interruptHandler.getFirstTime() << "\n";

    if(gsmService.getSmsBack()){
      sendSmsStatusBack(gsmService.getTelephone().c_str());
      gsmService.setSmsBack(false);
    }

    if(gsmService.getSmsUssd()){

      ussdWait++;
      
      if(ussdWait > 4){
        sendSmsStatusUssd(gsmService.getTelephone().c_str());
        gsmService.setSmsUssd(false);
        ussdWait = 0;
      }
    }

    if(gsmService.getOnAlarm()){
      if(gsmService.getRelayUser()){
        if(dateTime.getReleOn()){
          digitalWrite(RELE_USER,LOW);          
        } else {
          digitalWrite(RELE_USER,HIGH);
        }
      } else {
        digitalWrite(RELE_USER,HIGH);
      } 
    } else {
      digitalWrite(RELE_USER,HIGH);
    }        

    //SerialMon.println(gsmService.getOnAlarm());
    //SerialMon.println(dateTime.getStringDateTime());
    //SerialMon.println(readSensor());
    delay(25);
    watchDogRefresh();
    lastTurn = millis();
  } 
  }
		catch (...)
		{
		}  
  delay(25);
}

//reset all fields copy millis()