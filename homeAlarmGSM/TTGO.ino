#include <Arduino.h>
#include "utilities.h"
#include "GsmService.h"

#include <WiFi.h>
#include <WebServer.h>

#include <EEPROM.h>
#include "WebControl.h"
#include "state.h"

#define  RELE_HIGH_ON 14
#define  SETUP_ON_WEBSERVER 32 //D2 //4

char* ssid = "";
char* password = "";

WebServer server(80);

long lastTurn;

WebContlol webContlol = WebContlol();
GsmService gsmService = GsmService();

void handleRoot()
	{     	
    char arrDate[14];

  //if(digitalRead(SETUP_ON_WEBSERVER)){ 
	  server.send ( 200, "text/html", webContlol.getPage());
  //}

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

void setupModem()
{
#ifdef MODEM_RST
    // Keep reset high
    pinMode(MODEM_RST, OUTPUT);
    digitalWrite(MODEM_RST, HIGH);
#endif

    pinMode(MODEM_PWRKEY, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);

    // Turn on the Modem power first
    digitalWrite(MODEM_POWER_ON, HIGH);

    // Pull down PWRKEY for more than 1 second according to manual requirements
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);

    // Initialize the indicator as an output
    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LED_ON);
}

void setup()
{    
    SerialMon.begin(9600);
    SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
    setupModem();
    //---------------------------------------------------------------
      
    
    // SerialAT.println("AT+CMGF=1");    
    // SerialAT.println("AT+CSCS=\"GSM\"");   
    
    delay(5000);
    SerialMon.println("AT+CMGDA=\"DEL READ\"");
    delay(5000);
    SerialMon.println("AT+CMGDA=\"DEL SENT\"");
    delay(5000); 

    
    EEPROM.begin(512); 
    pinMode(SETUP_ON_WEBSERVER, INPUT_PULLUP); 
    delay(500); 
  //if(digitalRead(SETUP_ON_WEBSERVER)){
    ssid = "Al_Gs";
    password = "12345678";
     
     WiFi.softAP(ssid, password);

  //}

  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH); 
  pinMode(RELE_HIGH_ON, OUTPUT);
  digitalWrite(RELE_HIGH_ON,LOW);   
  

    //if(digitalRead(SETUP_ON_WEBSERVER)){ 
    // Start web server
      server.on ( "/", handleRoot );
      server.begin();  
    //}

  delay(1000);
  webContlol.refreshData();
    
}

void loop()
{
    gsmService.runService();
    server.handleClient();

  if(millis() - lastTurn > 5000){
      //  Serial.println("digitalRead(SETUP_ON_WEBSERVER)");
      //  Serial.println(digitalRead(SETUP_ON_WEBSERVER));    

    lastTurn = millis();
  }  
  delay(2);
}
