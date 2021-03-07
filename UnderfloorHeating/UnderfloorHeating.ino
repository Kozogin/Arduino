/* 
 * Underfloor heating controller
 * @Author Vasil Kozogin
 *
 * for controller ESP8266
 * display SSD1306 oled
 * rotation - choice time 
 * rotation + click - choice temperature
 * click short - wake up, review selected temperature
 * click middle - on/off heating mode
 * click long (4sec) - write to flash selected time and temperature  
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
//#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/*
 * pin 12, 13, 14 encoder
 * pin 2 reley
 * pin D8 one wire ds18b20
 *  
 */

#define  SENSOR D8

const int ENCODER_BUTTON = 14;
const int ENCODER_ROTATE_AVERS = 12;
const int ENCODER_ROTATE_REVERS = 13;
const int RELE = 2;
const int OLED_RESET = 16;

const int GISTEREZA = 5;// 0.5*C

Adafruit_SSD1306 display(OLED_RESET);

OneWire oneWire(SENSOR);
DallasTemperature sensor(&oneWire);

/* -----class encoder handler----- */
class RotateClickEncoder
{
  private:
    int m_pause;
    long m_lastTurn;
    long m_lastTurnMillis;

    int m_actualcount;
    int m_state;
    int m_pinAValue;
    int m_pinBValue;
    int m_sign;

    int m_durationCounter;
    int m_offButtonCounter;
    bool m_lockingInterrapt;

  public:
    RotateClickEncoder(int pause)
      : m_pause(pause)
    {
    }

    int duration() {
      if (m_lockingInterrapt) {
        int statePin = digitalRead(ENCODER_BUTTON);
        if (millis() - m_lastTurnMillis < 4000) {

          if (statePin == 0)
          {
            m_durationCounter++;
            m_offButtonCounter = 0;
          }
          else {
            m_offButtonCounter++;
            m_durationCounter--;
          }

          if (m_offButtonCounter > 4) {
            m_lockingInterrapt = false;
            m_offButtonCounter = 0;

            if (m_durationCounter > 120)
            {
              m_durationCounter = 0;
              return 2;
            }
            if (m_durationCounter > 5)
            {
              m_durationCounter = 0;
              return 1;
            }
          }
        } else {
          if (statePin == 1) {
            m_offButtonCounter++;
          }

          if (m_offButtonCounter > 10) {
            m_lockingInterrapt = false;
            m_durationCounter = 0;
            m_offButtonCounter = 0;
          }

          if (m_durationCounter > 20) {
            m_durationCounter = 0;
            m_offButtonCounter = 0;
            return 3;
          }
          else {
            m_durationCounter = 0;
            return -10;
          }
        }
        return -1;
      }
      return 0;
    }

    void clickButton() {

      if (!m_lockingInterrapt) {
        m_lastTurnMillis = millis();
        m_lockingInterrapt = true;
        m_durationCounter = 0;
        m_offButtonCounter = 0;
      }
    }

    void rotateAversEncoder() {
      if (micros() - m_lastTurn < m_pause) return;
      m_pinAValue = digitalRead(ENCODER_ROTATE_AVERS);
      m_pinBValue = digitalRead(ENCODER_ROTATE_REVERS);

      cli();
      if (m_state == 0  && !m_pinAValue &&  m_pinBValue || m_state == 2  && m_pinAValue && !m_pinBValue) {
        m_state += 1;
        m_lastTurn = micros();
      }
      if (m_state == -1 && !m_pinAValue && !m_pinBValue || m_state == -3 && m_pinAValue &&  m_pinBValue) {
        m_state -= 1;
        m_lastTurn = micros();
      }
      setCount(m_state);
      sei();

      if (m_pinAValue && m_pinBValue && m_state != 0) m_state = 0;
    }

    void rotateReversEncoder() {
      if (micros() - m_lastTurn < m_pause) return;
      m_pinAValue = digitalRead(ENCODER_ROTATE_AVERS);
      m_pinBValue = digitalRead(ENCODER_ROTATE_REVERS);

      cli();
      if (m_state == 1 && !m_pinAValue && !m_pinBValue || m_state == 3 && m_pinAValue && m_pinBValue) {
        m_state += 1;
        m_lastTurn = micros();
      }
      if (m_state == 0 && m_pinAValue && !m_pinBValue || m_state == -2 && !m_pinAValue && m_pinBValue) {
        m_state -= 1;
        m_lastTurn = micros();
      }
      setCount(m_state);
      sei();

      if (m_pinAValue && m_pinBValue && m_state != 0) m_state = 0;
    }

    void setCount(int m_state) {
      if (m_state == 4 || m_state == -4) {
        m_lastTurn = micros();
        m_sign = (int)(m_state / 4);

        if (!digitalRead(ENCODER_BUTTON)) {
          m_lockingInterrapt = false;
          m_durationCounter = 0;
          m_offButtonCounter = 0;
          m_sign *= 2;
        }
      }
    }

    void resetSign() {
      m_sign = 0;
    }

    int getSign() const {
      return m_sign;
    }
    int getmLastTurnMillis() const {
      return m_lastTurnMillis;
    }

};

/* -----class ExposeTimeAndTemterature ----- */
class ExposeTimeAndTemterature {

  private:
    int m_tempFloorExpose;
    float m_tempFloorSensor;
    int m_timeExpose;
    int m_timeRemains;
    bool m_permission;
    bool m_rele;

  public:

    ExposeTimeAndTemterature()
    {
    }

    void addTemp() {
      m_tempFloorExpose += 5;
      m_tempFloorExpose = m_tempFloorExpose > 350 ? 350 : m_tempFloorExpose;
    }

    void subtractTemp() {
      m_tempFloorExpose -= 5;
      m_tempFloorExpose = m_tempFloorExpose < 50 ? 50 : m_tempFloorExpose;
    }

    void addTime() {
      if (m_timeExpose > 55) {
        m_timeExpose += 10;
      } else {
        m_timeExpose += 5;
      }
      m_timeExpose = m_timeExpose > 250 ? 250 : m_timeExpose;
    }

    void subtractTime() {
      if (m_timeExpose > 65) {
        m_timeExpose -= 10;
      } else {
        m_timeExpose -= 5;
      }
      m_timeExpose = m_timeExpose < 5 ? 5 : m_timeExpose;
    }

    void refresh() {
      if (!m_permission) {
        m_rele = false;
        m_timeRemains = 36 * m_timeExpose;
      }
    }

    void startReadout() {
      m_permission = !m_permission;
      if (m_permission) {
        m_rele = true;
      } else {
        m_rele = false;
      }
    }

    void drainingTime() {
      readSensor();

      if (m_permission) {

        if (m_rele && m_tempFloorExpose + GISTEREZA < 10 * m_tempFloorSensor) {
          m_rele = false;
        }

        if (!m_rele && m_tempFloorExpose > 10 * m_tempFloorSensor) {
          m_rele = true;
        }

        if (m_timeExpose != 250) {
          m_timeRemains--;
        }

        if (m_timeRemains < 1) {
          m_permission = false;
          m_rele = false;
        }
      }
    }

    void readSensor() {

      sensor.requestTemperatures();
      m_tempFloorSensor = sensor.getTempCByIndex(0);
    }

    void setTempFloorExpose(byte temp) {
      m_tempFloorExpose = temp;
    }
    void setTimeExpose(byte timeE) {
      m_timeExpose = timeE;
    }
    float getTempFloorSensor() const {
      return m_tempFloorSensor;
    }
    int getTempFloorExpose() const {
      return m_tempFloorExpose;
    }

    int getTimeExpose() const {
      return m_timeExpose;
    }
    int getTimeRemains() const {
      return m_timeRemains;
    }
    bool isPermission() const {
      return m_permission;
    }
    bool isReleOn() const {
      return m_rele;
    }

};

/* -----class ShowDisplay ----- */
class ShowDisplay {
  private:
    bool m_visible;

  public:
    ShowDisplay(bool visible)
      : m_visible(visible)
    {
    }

    void displayPrint(String message, int sizeText, int x, int y, bool clearB) {
      if (m_visible) {
        if (clearB) {
          display.clearDisplay();
        }
        display.setTextSize(sizeText);
        display.setTextColor(WHITE);
        display.setCursor(x, y);
        display.print (message);
        display.display();
      }
    }


    void printTempFloorExpose(ExposeTimeAndTemterature *m_timerTemp) {

      String mes;
      if (m_timerTemp -> getTempFloorExpose() % 10 == 0) {
        mes = "<>" + (String)(m_timerTemp -> getTempFloorExpose() / 10) + ".0C";
      } else {
        mes = "<>" + (String)(m_timerTemp -> getTempFloorExpose() / 10) + ".5C";
      }

      displayPrint(mes, 3, 0, 32, true);
      displayPrint("Temp Floor", 2, 0, 0, false);
    }


    void printTimeExpose(ExposeTimeAndTemterature *m_timerTemp) {

      String mes;
      if (m_timerTemp -> getTimeExpose() % 10 == 0) {
        mes = "<>" + (String)(m_timerTemp -> getTimeExpose() / 10) + ".0h";
      } else {
        mes = "<>" + (String)(m_timerTemp -> getTimeExpose() / 10) + ".5h";
      }

      displayPrint(mes, 3, 0, 32, true);
      displayPrint("Heat Time", 2, 0, 0, false);
    }

    void printTimeRemain(ExposeTimeAndTemterature *m_timerTemp) {

      String mes;
      int hourRemain = m_timerTemp -> getTimeRemains() / 360;
      int minutRemain = (m_timerTemp -> getTimeRemains() - 360 * hourRemain) / 6;

      if (minutRemain == 0) {
        mes = (String)hourRemain + ":" + (String)minutRemain + "0";
      } else if (minutRemain < 10) {
        mes = (String)hourRemain + ":0" + (String)minutRemain;
      } else {
        mes = (String)hourRemain + ":" + (String)minutRemain ;
      }

      float tFloor = m_timerTemp -> getTempFloorSensor();

      displayPrint(mes, 3, 25, 32, true);

      if (m_timerTemp -> isPermission()) {
        displayPrint("on", 2, 0, 40, false);
      }

      String floorCorect;

      if (tFloor > -40 && tFloor < 60) {
        String strTFloor = String( (float)((int)(10 * tFloor)) / 10);
        floorCorect = strTFloor.substring(0, strTFloor.length() - 1) + " C";
      } else if (tFloor >= 60) {
        String strTFloor = String( (float)((int)(10 * tFloor)) / 10);
        tFloor = 20;
        floorCorect = "sensor crash";
      } else {
        floorCorect = "no sensor";
      }

      displayPrint(floorCorect, 2, 0, 0, false);
    }

    void setVisible(bool visible) {
      m_visible = visible;
    }

    void changeVisible() {
      m_visible = !m_visible;
    }

    bool getVisible() {
      return m_visible;
    }

};

/* -----class HandlerService ----- */
class HandlerService {

  private:
    RotateClickEncoder *m_encoder;
    ExposeTimeAndTemterature *m_timerTemp;
    ShowDisplay *m_showDisplay;

    long m_lastTurnMillis;
    long m_timer;
    long m_timeSwitchOffDisplay;

  public:
    HandlerService(RotateClickEncoder *encoder = nullptr, ExposeTimeAndTemterature *timerTemp = nullptr, ShowDisplay *showDisplay = nullptr)
      : m_encoder(encoder), m_timerTemp(timerTemp), m_showDisplay(showDisplay)
    {
    }

    void printTempFloorExpose() {
      m_showDisplay -> printTempFloorExpose(m_timerTemp);
    }

    void printTimeExpose() {
      m_showDisplay -> printTimeExpose(m_timerTemp);
    }

    void printTimeRemain() {
      m_showDisplay -> printTimeRemain(m_timerTemp);
    }

    void drainingTime() {
      m_timerTemp -> drainingTime();
    }

    void refresh() {
      m_timerTemp -> refresh();
    }

    void startReadout() {
      m_timerTemp -> startReadout();
    }

    int getDuration() {
      if (millis() - m_lastTurnMillis > 5) {
        m_lastTurnMillis = millis();
        return m_encoder -> duration();
      }
      return -1;
    }

    void setTempFloorExpose(byte temp) {
      m_timerTemp -> setTempFloorExpose(temp);
    }
    void setTimeExpose(byte timeE) {
      m_timerTemp -> setTimeExpose(timeE);
    }

    void interraptButton() {
      m_encoder -> clickButton();
    }
    void interraptrotateAvers() {
      m_encoder -> rotateAversEncoder();
    }
    void interraptrotateRevers() {
      m_encoder -> rotateReversEncoder();
    }
    void resetSign() {
      m_encoder -> resetSign();
    }

    void displayPrint(String message, int sizeText, int x, int y, bool clearB) {
      m_showDisplay -> displayPrint(message, sizeText, x, y, clearB);
    }
    void setVisible(bool visible) {
      m_showDisplay -> setVisible(visible);
    }
    void changeVisible() {
      m_showDisplay -> changeVisible();
    }

    void subtractTemp() {
      m_timerTemp -> subtractTemp();
    }
    void subtractTime() {
      m_timerTemp -> subtractTime();
    }
    void addTime() {
      m_timerTemp -> addTime();
    }
    void addTemp() {
      m_timerTemp -> addTemp();
    }

    int getSign() const {
      return m_encoder -> getSign();
    }
    int getmLastTurnMillis() const {
      return m_encoder -> getmLastTurnMillis();
    }

    int getTempFloorExpose() const {
      return m_timerTemp -> getTempFloorExpose();
    }
    float getTempFloorSensor() const {
      return m_timerTemp -> getTempFloorSensor();
    }
    int getTimeExpose() const {
      return m_timerTemp -> getTimeExpose();
    }
    int getTimeRemains() const {
      return m_timerTemp -> getTimeRemains();
    }
    bool isPermission() const {
      return m_timerTemp -> isPermission();
    }
    bool isReleOn() const {
      return m_timerTemp -> isReleOn();
    }

    bool getVisible()const {
      return m_showDisplay -> getVisible();
    }

    void setTimerE(long timer) {
      m_timer = timer;
    }
    void setTimeSwitchOffDisplay(long timer) {
      m_timeSwitchOffDisplay = timer;
    }

    long getTimerE()const {
      return m_timer;
    }
    long getTimeSwitchOffDisplay()const {
      return m_timeSwitchOffDisplay;
    }
};

/* global object */
HandlerService handlerService(new RotateClickEncoder(50), new ExposeTimeAndTemterature(), new ShowDisplay(true));

/* -----functions for interrupt handling  ----- */

ICACHE_RAM_ATTR void clickButton() {
  handlerService.interraptButton();
  handlerService.setTimeSwitchOffDisplay(millis());
}

ICACHE_RAM_ATTR void rotateAvers() {
  handlerService.interraptrotateAvers();
  handlerService.setTimerE(millis() - 8000);
  handlerService.setTimeSwitchOffDisplay(millis());
}

ICACHE_RAM_ATTR void rotateRevers() {
  handlerService.interraptrotateRevers();
  handlerService.setTimerE(millis() - 8000);
  handlerService.setTimeSwitchOffDisplay(millis());
}

/* ----- setup  ----- */
void setup() {
  //Serial.begin(115200);
  EEPROM.begin(12);

  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_ROTATE_AVERS, INPUT_PULLUP);
  pinMode(ENCODER_ROTATE_REVERS, INPUT_PULLUP);
  pinMode(RELE, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), clickButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_ROTATE_AVERS), rotateAvers, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_ROTATE_REVERS), rotateRevers, CHANGE);

  digitalWrite(RELE, HIGH);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  sensor.begin();

  handlerService.displayPrint("start", 4, 0, 0, true);
  delay(1000);
  handlerService.setTempFloorExpose(EEPROM.read(0) * 5);
  handlerService.setTimeExpose(EEPROM.read(1));
  handlerService.refresh();
}

void loop() {

// countdown timer 
  if (millis() - handlerService.getTimerE() > 10000) {
    handlerService.drainingTime();
    handlerService.printTimeRemain();
    handlerService.refresh();

    if (handlerService.isReleOn()) {
      digitalWrite(RELE, LOW);
    } else {
      digitalWrite(RELE, HIGH);
    }

    if (millis() - handlerService.getTimeSwitchOffDisplay() > 90000) {
      handlerService.displayPrint("", 3, 0, 0, true);
      handlerService.setVisible(false);
    }

    handlerService.setTimerE(millis());
  }

// rotation encoder selection
  switch (handlerService.getSign()) {
    case -2:
      if (handlerService.getVisible()) {
        handlerService.subtractTemp();
        handlerService.printTempFloorExpose();
      }
      break;
    case -1:
      if (handlerService.getVisible()) {
        handlerService.subtractTime();
        handlerService.refresh();
        handlerService.printTimeExpose();
      }
      break;
    case 1:
      if (handlerService.getVisible()) {
        handlerService.addTime();
        handlerService.refresh();
        handlerService.printTimeExpose();
      }
      break;
    case 2:
      if (handlerService.getVisible()) {
        handlerService.addTemp();
        handlerService.printTempFloorExpose();
      }
      break;
    default:
      break;
  }

// click encoder selection
  switch (handlerService.getDuration()) {
    case 1:
      if (handlerService.getVisible()) {
        handlerService.printTempFloorExpose();
        handlerService.setTimerE(millis() - 8000);
      } else {
        handlerService.setVisible(true);
        handlerService.setTimerE(millis() - 10000);
      }

      break;
    case 2:
      if (handlerService.getVisible()) {
        handlerService.startReadout();
        handlerService.setTimerE(millis() - 10000);
      }
      break;
    case 3:
      if (handlerService.getVisible()) {
        EEPROM.write(0, handlerService.getTempFloorExpose() / 5);
        EEPROM.write(1, handlerService.getTimeExpose());
        EEPROM.commit();
        handlerService.displayPrint("Save", 4, 0, 0, true);
        handlerService.setTimerE(millis() - 8000);
      }
      break;
    default:
      break;
  }

  handlerService.resetSign();
  delay(2);

}
