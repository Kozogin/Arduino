#include <SimpleTimer.h>
#include <EEPROM.h>
#include "Lighter.h"

const char* ssid = "Sun_link";
const char* password = "*****************";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);

Lighter lighter = Lighter();
SimpleTimer timer2;
bool m_startCheck = false;
bool m_timeLighterTake;

void handleTimer(){
  char arrDate[8];  
  server.send ( 200, "text/html", lighter.getPageTimer());

  if (m_timeLighterTake && server.hasArg("timer_light")){
    strcpy(arrDate,server.arg("timer_light").c_str());        
    lighter.setTimerLight(arrDate);
  }

  if ( server.hasArg("range_Light")){
    strcpy(arrDate,server.arg("range_Light").c_str());        
    lighter.setRangeLight(arrDate);
  }  
  
  if(m_startCheck){
    lighter.resetStrobCheck();    
  }
  m_startCheck = true;
  if ( server.hasArg("strob_ckd")){
    strcpy(arrDate,server.arg("strob_ckd").c_str());
    lighter.setStrobCheck();      
  }
  if ( server.hasArg("save_ckd")){
    strcpy(arrDate,server.arg("save_ckd").c_str());
    lighter.setSaveCheck();      
  }  
  
  if ( server.hasArg("rdo_event")){
    strcpy(arrDate,server.arg("rdo_event").c_str());    
    lighter.setEvent(arrDate);  
  } 
  
  if ( server.hasArg("rdo_color")){
    strcpy(arrDate,server.arg("rdo_color").c_str());
    lighter.setColor(arrDate);
  }
  m_timeLighterTake = false;
  digitalWrite(0, LOW); 
  digitalWrite(2, HIGH); 
}

void handleRoot()
{  
	server.send ( 200, "text/html", lighter.getPage());  
  m_timeLighterTake = true;
  digitalWrite(0, LOW); 
  digitalWrite(2, HIGH);
}
  

void setup()
{ 
  EEPROM.begin(512);      
    delay(500); 
	pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, LOW); 
  digitalWrite(2, HIGH); 
  WiFi.mode(WIFI_AP_STA);

	WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);     
	
  server.on ( "/", handleRoot );
  server.on ( "/timer", handleTimer);
  server.begin();  

  lighter.setRangeLightEeprom((int)EEPROM.read(0));
  if(EEPROM.read(1)){
    lighter.setStrobCheck();
  }
  lighter.setColorEeprom((int)EEPROM.read(2));

  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels = new Adafruit_NeoPixel(numPixel, PIN, colorOrder);
  pixels->begin();

  lighter.setEventTimerStart();
  timer2.setInterval(500L, timerLight); 
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
