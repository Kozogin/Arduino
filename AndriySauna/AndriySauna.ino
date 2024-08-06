// где получить токен и ID https://kit.alexgyver.ru/tutorials/telegram-basic/

//https://github.com/GyverLibs/FastBot/blob/main/examples/menu/menu.ino
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <SimpleTimer.h>
#include "ServiceTelegram.h"

#define PIN_TELEGRAM_OR_WEB 23

ServiceTelegram serviceTelegram;
SimpleTimer timer;

const char* ssid = "Kolector";
const char* password = "source221313";

unsigned long wifiRestart;
unsigned long realTouchtime;
unsigned long timeButtonRadiatorPool;

int countStatePoolRadiator;

bool telegram0Web1 = false;
bool statePinPoolRadiator;
bool previousStatePinPoolRadiator;
bool nowStatePinPoolRadiator;

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", serviceTelegram.getPage());
}

void handleRootC2() {

  //Serial.begin(115200);
  //Serial.println("handleRootC2(){");
  server.send(200, "text/html", serviceTelegram.getPageC2());
  char arrDate[3];

  bool onRadiator = false;
  bool onManual = false;
  bool boolSave = false;

  if (server.hasArg("save_ckd")) {
    boolSave = true;
  }

  if (boolSave) {
    if (server.hasArg("on_radiator_ckd")) {
      onRadiator = true;
    }
    if (server.hasArg("temp_stabil")) {
      strcpy(arrDate, server.arg("temp_stabil").c_str());
      serviceTelegram.setTempStabil_2(arrDate);
    }
    if (server.hasArg("on_manual_ckd")) {
      onManual = true;
    }
    if (server.hasArg("curve")) {
      strcpy(arrDate, server.arg("curve").c_str());
      serviceTelegram.setCurve_2(arrDate);
    }

    serviceTelegram.changeOnRadiatorPool(onRadiator);
    serviceTelegram.setStatus_2(onManual);

    serviceTelegram.saveToEeprom();
  }
}

void handleRootC3() {
  server.send(200, "text/html", serviceTelegram.getPageC3());
  char arrDate[3];
  bool onManual = false;
  bool boolSave = false;

  if (server.hasArg("save_ckd")) {
    boolSave = true;
  }

  if (boolSave) {

    if (server.hasArg("temp_stabil")) {
      strcpy(arrDate, server.arg("temp_stabil").c_str());
      serviceTelegram.setTempStabil_3(arrDate);
    }
    if (server.hasArg("on_manual_ckd")) {
      onManual = true;
    }
    if (server.hasArg("curve")) {
      strcpy(arrDate, server.arg("curve").c_str());
      serviceTelegram.setCurve_3(arrDate);
    }

    serviceTelegram.setStatus_3(onManual);

    serviceTelegram.saveToEeprom();
  }
}

void handleRootC4() {
  server.send(200, "text/html", serviceTelegram.getPageC4());
  char arrDate[3];
  bool onManual = false;
  bool boolSave = false;

  if (server.hasArg("save_ckd")) {
    boolSave = true;
  }

  if (boolSave) {

    if (server.hasArg("temp_stabil")) {
      strcpy(arrDate, server.arg("temp_stabil").c_str());
      serviceTelegram.setTempStabil_4(arrDate);
    }
    if (server.hasArg("on_manual_ckd")) {
      onManual = true;
    }
    if (server.hasArg("curve")) {
      strcpy(arrDate, server.arg("curve").c_str());
      serviceTelegram.setCurve_4(arrDate);
    }

    serviceTelegram.setStatus_4(onManual);

    if (server.hasArg("time_start")) {
      strcpy(arrDate, server.arg("time_start").c_str());
      serviceTelegram.setStartTime(arrDate);
    }
    if (server.hasArg("time_stop")) {
      strcpy(arrDate, server.arg("time_stop").c_str());
      serviceTelegram.setEndTime(arrDate);
    }
    serviceTelegram.saveToEeprom();
  }
}

void handleRootC5() {
  server.send(200, "text/html", serviceTelegram.getPageC5());
  char arrDate[3];
  bool onManual = false;

  bool boolSave = false;

  if (server.hasArg("save_ckd")) {
    boolSave = true;
  }

  if (boolSave) {

    if (server.hasArg("temp_stabil")) {
      strcpy(arrDate, server.arg("temp_stabil").c_str());
      serviceTelegram.setTempStabil_5(arrDate);
    }
    if (server.hasArg("on_manual_ckd")) {
      onManual = true;
    }
    if (server.hasArg("curve")) {
      strcpy(arrDate, server.arg("curve").c_str());
      serviceTelegram.setCurve_5(arrDate);
    }

    serviceTelegram.setStatus_5(onManual);

    serviceTelegram.saveToEeprom();
  }
}

void handleRootC6() {
  server.send(200, "text/html", serviceTelegram.getPageC6());
  char arrDate[3];
  bool onManual = false;

  bool boolSave = false;

  if (server.hasArg("save_ckd")) {
    boolSave = true;
  }

  if (boolSave) {

    if (server.hasArg("temp_stabil")) {
      strcpy(arrDate, server.arg("temp_stabil").c_str());
      serviceTelegram.setTempStabil_6(arrDate);
    }
    if (server.hasArg("on_manual_ckd")) {
      onManual = true;
    }
    if (server.hasArg("curve")) {
      strcpy(arrDate, server.arg("curve").c_str());
      serviceTelegram.setCurve_6(arrDate);
    }

    serviceTelegram.setStatus_6(onManual);

    serviceTelegram.saveToEeprom();
  }
}

void handleRootOption() {
  server.send(200, "text/html", serviceTelegram.getPageOption());

  char arrDate[3];
  bool typeNO_2 = false;
  bool typeNO_3 = false;
  bool typeNO_4 = false;
  bool typeNO_5 = false;
  bool typeNO_6 = false;

  bool boolSave = false;

  if (server.hasArg("save_ckd")) {
    boolSave = true;
  }

  if (boolSave) {

    if (server.hasArg("correct_x")) {
      strcpy(arrDate, server.arg("correct_x").c_str());
      serviceTelegram.setCorrectX(arrDate);
    }
    if (server.hasArg("correct_a")) {
      strcpy(arrDate, server.arg("correct_a").c_str());
      serviceTelegram.setCorrectA(arrDate);
    }

    if (server.hasArg("out_lower")) {
      strcpy(arrDate, server.arg("out_lower").c_str());
      serviceTelegram.setOutLower(arrDate);
    }
    if (server.hasArg("contuor_lower")) {
      strcpy(arrDate, server.arg("contuor_lower").c_str());
      serviceTelegram.setContuorLower(arrDate);
    }
    if (server.hasArg("contour_number")) {
      strcpy(arrDate, server.arg("contour_number").c_str());
      serviceTelegram.setNumberContuor(arrDate);
    }
    if (server.hasArg("count_message")) {
      strcpy(arrDate, server.arg("count_message").c_str());
      serviceTelegram.setCountMessage(arrDate);
    }

    if (server.hasArg("type_servo_2_ckd")) {
      typeNO_2 = true;
    }
    if (server.hasArg("type_servo_3_ckd")) {
      typeNO_3 = true;
    }
    if (server.hasArg("type_servo_4_ckd")) {
      typeNO_4 = true;
    }
    if (server.hasArg("type_servo_5_ckd")) {
      typeNO_5 = true;
    }
    if (server.hasArg("type_servo_6_ckd")) {
      typeNO_6 = true;
    }

    serviceTelegram.setTypeServoNO_2(typeNO_2);
    serviceTelegram.setTypeServoNO_3(typeNO_3);
    serviceTelegram.setTypeServoNO_4(typeNO_4);
    serviceTelegram.setTypeServoNO_5(typeNO_5);
    serviceTelegram.setTypeServoNO_6(typeNO_6);

    String pass;
    if (server.hasArg("pass_txt")) {
      strcpy(arrDate, server.arg("pass_txt").c_str());
      pass = (String)arrDate;
    }
    if (pass == "123") {
      
      char arrDateWF[60];
      if (server.hasArg("ssid_txt")) {
        strcpy(arrDateWF, server.arg("ssid_txt").c_str());
        serviceTelegram.setSsidT(arrDateWF);
      }
      if (server.hasArg("pass_ssid_txt")) {
        strcpy(arrDateWF, server.arg("pass_ssid_txt").c_str());
        serviceTelegram.setPasswordT(arrDateWF);
      }
      if (server.hasArg("bot_token_txt")) {
        strcpy(arrDateWF, server.arg("bot_token_txt").c_str());
        serviceTelegram.setTokenT(arrDateWF);
      }
      if (server.hasArg("chat_id_txt")) {
        strcpy(arrDateWF, server.arg("chat_id_txt").c_str());
        serviceTelegram.setChatIdT(arrDateWF);
      }
    }

    serviceTelegram.saveToEeprom();
  }
}


void setup() {
  Serial.begin(9600);
  EEPROM.begin(255);

  pinMode(RELAY_PWM_2, OUTPUT);
  pinMode(RELAY_PWM_3, OUTPUT);
  pinMode(RELAY_PWM_4, OUTPUT);
  pinMode(RELAY_PWM_5, OUTPUT);
  pinMode(RELAY_PWM_6, OUTPUT);
  pinMode(RELAY_PUMP_4, OUTPUT);
  pinMode(LED_POOL_RADIATOR_ON, OUTPUT);
  pinMode(PIN_TELEGRAM_OR_WEB, INPUT_PULLUP);
  pinMode(PIN_POOL_RADIATOR_ON, INPUT_PULLUP);

  digitalWrite(RELAY_PUMP_4, HIGH);
  
  if (digitalRead(PIN_TELEGRAM_OR_WEB)) {
    telegram0Web1 = false;
  } else {
    telegram0Web1 = true;
  }

  delay(500);
  previousStatePinPoolRadiator = digitalRead(PIN_POOL_RADIATOR_ON);
  nowStatePinPoolRadiator = previousStatePinPoolRadiator;

  sensorContuor_2.begin();
  sensorContuor_3.begin();
  sensorContuor_4.begin();
  sensorContuor_5.begin();
  sensorContuor_6.begin();
  sensorOut.begin();

  
  wifiRestart = 0;
  serviceTelegram.readFromEeprom();
  delay(1000);

  if (telegram0Web1) {
    WiFi.softAP(ssid, password);
    server.on("/", handleRoot);
    server.on("/c2", handleRootC2);
    server.on("/c3", handleRootC3);
    server.on("/c4", handleRootC4);
    server.on("/c5", handleRootC5);
    server.on("/c6", handleRootC6);
    server.on("/option", handleRootOption);

    server.begin();
  } else {
    connectWiFi();
    bot.setToken(BOT_TOKEN);
    bot.setChatID(CHAT_ID);
    bot.attach(newMsg);
    serviceTelegram.startTelegram();
  }

  
  timer.setInterval(180000L, getTimeTelegram);
}

void getTimeTelegram() {
  if (!telegram0Web1) {
    serviceTelegram.getTimeTelegram();
  } else {
    digitalWrite(RELAY_PUMP_4, LOW);
  }
}

void newMsg(FB_msg& msg) {
  if (!telegram0Web1) {
    serviceTelegram.newMsg(msg);
  }
}

void loop() {

  if (!telegram0Web1) {
    bot.tick();
    if (WiFi.status() != WL_CONNECTED) {
      if (millis() - wifiRestart > 600000) {
        wifiRestart = millis();
        connectWiFi();
      }
    }
  }

  if(millis() - timeButtonRadiatorPool > 50){
    statePinPoolRadiator = digitalRead(PIN_POOL_RADIATOR_ON);

    if(statePinPoolRadiator){
      countStatePoolRadiator++;
    } else {
      countStatePoolRadiator--;
    }

    if(countStatePoolRadiator > 3){
      countStatePoolRadiator = 3;
      nowStatePinPoolRadiator = true;
    }
    if(countStatePoolRadiator < -3){
      countStatePoolRadiator = -3;
      nowStatePinPoolRadiator = false;
    }

    if(previousStatePinPoolRadiator == !nowStatePinPoolRadiator){
      serviceTelegram.onOffPoolRadiator();
      serviceTelegram.saveToEeprom();
      previousStatePinPoolRadiator = nowStatePinPoolRadiator;
    }

    timeButtonRadiatorPool = millis();
  }


  server.handleClient();
  timer.run();
  serviceTelegram.logicRelay();

  if (wifiRestart > millis()) {
    wifiRestart = millis();
  }
}

void connectWiFi() {
  //delay(2000);
    //Serial.println("WIFI_SSID connectWiFi");
    //Serial.println(WIFI_SSID);
    //Serial.println("WIFI_PASS connectWiFi");
    //Serial.println(WIFI_PASS);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - wifiRestart > 20000) {
      delay(1000);
      return;
    }
  }
  Serial.println("Connected");
}
