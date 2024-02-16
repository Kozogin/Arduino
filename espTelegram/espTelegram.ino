//https://github.com/GyverLibs/FastBot/blob/main/examples/menu/menu.ino 


#include "ServiceTelegram.h"

ServiceTelegram serviceTelegram;

unsigned long wifiRestart;

void setup() {
  EEPROM.begin(63);

  lcd.begin();
  lcd.backlight();

  pinMode(PUMP_WOOD, OUTPUT);
  pinMode(PUMP_CAPACITY, OUTPUT);
  pinMode(ALARM_PIN, OUTPUT);
  
  sensorWood.begin();
  sensorCapacity.begin();
   
    delay(500); 
    lcd.print("wood automatic");
    
  connectWiFi();
  wifiRestart = 0;

  bot.setChatID(CHAT_ID);
  bot.attach(newMsg);
  serviceTelegram.readFromEeprom();
  String menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t Off");
  bot.inlineMenu("Boiler cottage1", menu1);
}

void newMsg(FB_msg& msg) {
  serviceTelegram.newMsg(msg);
}

void loop() {
  bot.tick();
  serviceTelegram.logicRelay();
  if(wifiRestart > millis()){
    wifiRestart = millis();
    Serial.println("wifiRestart = millis();");
  }
  if(WiFi.status() != WL_CONNECTED){
    if(millis() - wifiRestart > 600000){
      wifiRestart = millis();
      Serial.println("if(millis() - wifiRestart > 600000){");
      connectWiFi();      
    }
  }
}

void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - wifiRestart > 10000) return;
    //if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}
