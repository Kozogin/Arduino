#include <SimpleTimer.h>
#include <EEPROM.h>
#include "Lighter.h"

const char* ssid = "lamp";
const char* password = "12345678";

IPAddress local_ip (192,168,1,1);
IPAddress gateway (192,168,1,1);
IPAddress subnet (255,255,255,0);
ESP8266WebServer server (80);

Lighter lighter = Lighter();
SimpleTimer timer2;

void handleRoot()
{  
  char arrDate[8]; 
	server.send ( 200, "text/html", lighter.getPage());

//----------------------------------------------------------
  if ( server.hasArg("range_brightness")){
    strcpy(arrDate,server.arg("range_brightness").c_str());        
    lighter.setRangeBrightness(arrDate);
  }
  if ( server.hasArg("range_sector")){
    strcpy(arrDate,server.arg("range_sector").c_str());        
    lighter.setRangeSector(arrDate);
  }
  if ( server.hasArg("range_position")){
    strcpy(arrDate,server.arg("range_position").c_str());        
    lighter.setRangePosition(arrDate);
  }
  
  //--------------------------------------------------------
  if ( server.hasArg("range_red")){
    strcpy(arrDate,server.arg("range_red").c_str());        
    lighter.setRangeRed(arrDate);
  }
  if ( server.hasArg("range_green")){
    strcpy(arrDate,server.arg("range_green").c_str());        
    lighter.setRangeGreen(arrDate);
  }
  if ( server.hasArg("range_blue")){
    strcpy(arrDate,server.arg("range_blue").c_str());        
    lighter.setRangeBlue(arrDate);
  }
  if ( server.hasArg("range_speed")){
    strcpy(arrDate,server.arg("range_speed").c_str());        
    lighter.setRangeSpeed(arrDate);
  }
  //--------------------------------------------------------
  if ( server.hasArg("lampp")){
    strcpy(arrDate,server.arg("lampp").c_str());
    lighter.setLampState(arrDate);
  }

  if ( server.hasArg("rdo_color")){
    strcpy(arrDate,server.arg("rdo_color").c_str());
    lighter.setColor(arrDate);
  }

  if ( server.hasArg("rdo_program")){
    strcpy(arrDate,server.arg("rdo_program").c_str());
    lighter.setProgram(arrDate);
  }  
  //-------------------------------------------------------- 

  lighter.resetSaveCheck();
  if ( server.hasArg("saves")){
    strcpy(arrDate,server.arg("saves").c_str());
    lighter.setSaveCheck();      
  }

  lighter.perfomer();

}
  

void setup()
{ 
  Serial.begin(9600);
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
  EEPROM.begin(32);      
    delay(500); 
	
  WiFi.mode(WIFI_AP_STA);

	WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);     
	
  server.on ( "/", handleRoot );
  server.begin();  

  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels = new Adafruit_NeoPixel(NUM_PIXEL, PIN, colorOrder);
  pixels->begin();
  lighter.readFromEeprom();
  
  lighter.perfomer();
  timer2.setInterval(10L, timerLight); 
}

void timerLight(){
  lighter.timeDraining();
}

void loop()
{
  timer2.run();
	server.handleClient();   
  delay(2);
}
