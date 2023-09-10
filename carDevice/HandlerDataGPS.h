#ifndef HANDLER_DATA_GPS_H
#define HANDLER_DATA_GPS_H

#include <TinyGPS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

#include <string>

#define TEMP_SENSOR_IN 18
#define TEMP_SENSOR_OUT 19
#define VOLTAGE_PIN 34

#define MAX_FREQUENCY_ALLOW 2000
#define MIN_FREQUENCY_ALLOW_UP 1500
#define MIN_FREQUENCY_ALLOW_DOWN 400

OneWire oneWireIn(TEMP_SENSOR_IN);
OneWire oneWireOut(TEMP_SENSOR_OUT);
DallasTemperature sensorIn(&oneWireIn);
DallasTemperature sensorOut(&oneWireOut);

const int DISTANCE_CALC = 40;
const int DISTANCE_COURSE = 15;

const int NUMBER_CYLINDERS = 4;
const int TIME_INTERRUPT_AGAIN = 480000 / NUMBER_CYLINDERS;
const int TIME_INTERRUPT_NEXT = 9000 / NUMBER_CYLINDERS;

class HandlerDataGPS {
private:
  int m_maxSpeed;
  float m_previousDistance;
  float m_track;
  int m_course;
  int m_totalTrack;
  int m_totalCourse;
  int m_startAltitude;
  int m_totalAltitude;
  float m_voltage;
  float m_minVoltage;
  float m_maxVoltage;
  int m_frequency;
  int m_maxFrequency;
  int m_minFrequency;
  int m_minCandidateFrequency;
  bool m_allowMinNumber;

  float m_startLat;
  float m_startLon;
  float m_previousLat;
  float m_previousLon;
  float m_previousLatCourse;
  float m_previousLonCourse;

  float m_trackDay;
  float m_trackAll;
  float m_trackGaz;
  float m_trackOil;

  int m_maxSattelites;
  bool m_startGPS;
  unsigned long tachoCalcTimePrevious;
  unsigned long tachoLastTurnTime;
  bool m_stopInterrupt;
  int m_tempIn;
  int m_tempOut;

  bool saveMileageBool;
  unsigned long saveMileageTime;
  int saveTimesFrequency0;
  int m_timeZone;

  int m_productMountDay;
  bool m_allowSaveWebControl;

  unsigned long m_engineTimeDay;
  unsigned long m_engineTimeAll;
  unsigned long m_engineTimeGaz;
  unsigned long m_engineTimeOil;

public:

  HandlerDataGPS() {
    m_minFrequency = 9000;
    m_minCandidateFrequency = 9000;
    saveMileageBool = false;
    saveMileageTime = 0;
    saveTimesFrequency0 = 0;
  }

  void checkFrequency() {

    float trackAll = 0;

    try {
      
      if (m_frequency) {
        m_engineTimeAll++;
        if (saveMileageTime > 60) {
          saveMileageBool = true;
        }
        saveMileageTime++;
        saveTimesFrequency0 = 0;
      } else {
        saveMileageTime = 0;
        if (saveMileageBool) {
          if (saveTimesFrequency0 > 1) {
            EEPROM.get(12, trackAll);
            if (m_trackAll - trackAll > 0.3) {
              saveTrack();
            }
            saveMileageBool = false;
          }
          saveTimesFrequency0++;
        }
      }
    } catch (...) {
      Serial.println("HandlerDataGPS checkFrequency");
    }
  }

  void readSensor() {
    try {
      sensorIn.requestTemperatures();
      sensorOut.requestTemperatures();
      m_tempIn = sensorIn.getTempCByIndex(0);
      m_tempOut = sensorOut.getTempCByIndex(0);
    } catch (...) {
      Serial.println("HandlerDataGPS readSensor");
    }
  }

  void tachoCalcClass() {
    try {
      if (micros() - tachoLastTurnTime > 2 * TIME_INTERRUPT_AGAIN) {
        m_stopInterrupt = false;
        m_frequency = 10;
      }

      if (m_stopInterrupt && micros() - tachoLastTurnTime > TIME_INTERRUPT_NEXT) {

        unsigned long tachoNow = micros();
        m_frequency = 2 * (60000000 / NUMBER_CYLINDERS) / (tachoNow - tachoCalcTimePrevious);
        if (m_frequency > m_maxFrequency) {
          if (m_frequency < MAX_FREQUENCY_ALLOW) {
            m_maxFrequency = m_frequency;
          } else {
            if (m_frequency < 1.1 * m_maxFrequency) {
              m_maxFrequency = m_frequency;
            }
          }
        }

        if (m_frequency > MIN_FREQUENCY_ALLOW_UP) {
          if (m_frequency < m_minFrequency) {
            m_minFrequency = m_frequency;
          }
          m_allowMinNumber = true;
        } else {
          if (m_frequency <= MIN_FREQUENCY_ALLOW_DOWN) {
            m_allowMinNumber = false;
          }

          if (m_frequency > MIN_FREQUENCY_ALLOW_DOWN && m_frequency > m_minCandidateFrequency && m_allowMinNumber) {
            if (m_frequency < m_minFrequency) {
              m_minFrequency = m_frequency;
            }
          }
          m_minCandidateFrequency = m_frequency;

          if (m_minFrequency < MIN_FREQUENCY_ALLOW_DOWN) {
            m_minFrequency = 1000;
          }
        }

        m_stopInterrupt = false;
        tachoLastTurnTime = micros();
      }

      if (micros() - tachoLastTurnTime > TIME_INTERRUPT_AGAIN) {
        tachoCalcTimePrevious = micros();
        m_stopInterrupt = true;
        tachoLastTurnTime = micros();
      }
    } catch (...) {
      Serial.println("HandlerDataGPS tachoCalcClass");
    }
  }

  int getAvgSpeed() {
    try {
      return 3600 * m_track / millis();
    } catch (...) {
      Serial.println("HandlerDataGPS getAvgSpeed");
    }
  }

  void calcMaxSpeed(int speed) {
    try {
      if (m_maxSpeed == 0) {
        m_maxSpeed = speed;
      }
      if (m_maxSpeed < speed) {
        m_maxSpeed = speed;
      }
    } catch (...) {
      Serial.println("HandlerDataGPS calcMaxSpeed");
    }
  }

  void calcAltitude(int altitude) {

    try {
      if (!m_startGPS) {
        m_startAltitude = altitude;
      }
      m_totalAltitude = altitude - m_startAltitude;
    } catch (...) {
      Serial.println("HandlerDataGPS calcAltitude");
    }
  }

  void calcVoltage() {

    try {
      int analog = analogRead(VOLTAGE_PIN);
      m_voltage = (float)(1.5287 * 3.3 * analog / 1024);

      if (m_minVoltage == 0 || m_maxVoltage == 0) {
        m_minVoltage = m_voltage;
        m_maxVoltage = m_voltage;
      }
      if (m_minVoltage > m_voltage) {
        m_minVoltage = m_voltage;
      }
      if (m_maxVoltage < m_voltage) {
        m_maxVoltage = m_voltage;
      }
    } catch (...) {
      Serial.println("HandlerDataGPS calcVoltage");
    }
  }

  void calcSatellites(int satellites) {

    try {
      if (satellites > 50) {
        satellites = 0;
      }
      if (m_maxSattelites < satellites) {
        m_maxSattelites = satellites;
      }
    } catch (...) {
      Serial.println("HandlerDataGPS calcSatellites");
    }
  }

  void calcPosition(float lat, float lon) {

    try {
      if (!m_startGPS) {
        m_startLat = lat;
        m_startLon = lon;
        m_previousLat = lat;
        m_previousLon = lon;
        m_previousLatCourse = lat;
        m_previousLonCourse = lon;
        m_startGPS = true;
      }

      TinyGPS gpsLocal;

      m_totalCourse = gpsLocal.course_to(m_startLat, m_startLon, lat, lon);
      m_totalTrack = 1.852 * gpsLocal.distance_between(m_startLat, m_startLon, lat, lon);

      float distance = 1.852 * gpsLocal.distance_between(m_previousLat, m_previousLon, lat, lon);
      if (distance > DISTANCE_CALC) {
        if (distance < 1000) {
          m_track += distance;
          m_trackAll += distance / 1000;
          m_previousDistance = distance;
        } else {
          m_track += m_previousDistance;
          m_trackAll += m_previousDistance / 1000;
        }

        m_previousLat = lat;
        m_previousLon = lon;
      }

      distance = gpsLocal.distance_between(m_previousLatCourse, m_previousLonCourse, lat, lon);
      if (distance > DISTANCE_COURSE) {
        m_course = gpsLocal.course_to(m_previousLatCourse, m_previousLonCourse, lat, lon);
        m_previousLatCourse = lat;
        m_previousLonCourse = lon;
      }      

    } catch (...) {
      Serial.print("HandlerDataGPS calcPosition  ");      
    }
  }  

  void saveCheckWeb(){
    if(m_allowSaveWebControl){
      saveTrack();
    }
  }

  void saveTrack() {

    try {
      EEPROM.put(0, m_trackDay);
      EEPROM.put(4, m_trackGaz);
      EEPROM.put(8, m_trackOil);
      EEPROM.put(12, m_trackAll);
      EEPROM.put(16, m_timeZone);
      EEPROM.put(20, m_productMountDay);
      EEPROM.put(24, m_engineTimeDay);
      EEPROM.put(28, m_engineTimeAll);
      EEPROM.put(32, m_engineTimeGaz);
      EEPROM.put(36, m_engineTimeOil);
      EEPROM.commit();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Save successfull");
      delay(2000);
      lcd.clear();

      m_allowSaveWebControl = false;

    } catch (...) {
      Serial.println("HandlerDataGPS saveTrack");
    }
  } 

  void readTrack() {

    try {
      EEPROM.get(0, m_trackDay);
      EEPROM.get(4, m_trackGaz);
      EEPROM.get(8, m_trackOil);
      EEPROM.get(12, m_trackAll);
      EEPROM.get(16, m_timeZone);
      EEPROM.get(20, m_productMountDay);

      EEPROM.get(24, m_engineTimeDay);
      EEPROM.get(28, m_engineTimeAll);
      EEPROM.get(32, m_engineTimeGaz);
      EEPROM.get(36, m_engineTimeOil);

    } catch (...) {
      Serial.println("HandlerDataGPS readTrack");
    }
  }  

  void setFrequencyZero(){
    m_frequency = 0;
  }

  void resetTrackDay() {
    m_trackDay = m_trackAll;
    m_engineTimeDay = m_engineTimeAll;
  }

  void resetTrackGaz() {
    m_trackGaz = m_trackAll;
    m_engineTimeGaz = m_engineTimeAll;
  }

  void resetTrackOil() {
    m_trackOil = m_trackAll;
    m_engineTimeOil = m_engineTimeAll;
  }

  void allowWebControl(bool state){
    m_allowSaveWebControl = state;
  }

  void setProductMountDay(int productMountDay, int year) {

    try {
      if (m_productMountDay != productMountDay && year > 2022) {
        resetTrackDay();
      }
      m_productMountDay = productMountDay;
    } catch (...) {
      Serial.println("HandlerDataGPS setProductMountDay");
    }
  }

  void setTimeZone(int timeZone) {
    m_timeZone = timeZone;
  }

  void setTrackAll(float track) {
    m_trackAll = track;
  }

  int f_course() {
    return m_course;
  }

  float getTrackDay() {
    return m_trackAll - m_trackDay;
  }

  float getTrackAll() {
    return m_trackAll;
  }

  float getTrackGaz() {
    return m_trackAll - m_trackGaz;
  }

  float getTrackOil() {
    return m_trackAll - m_trackOil;
  }

  int getMaxSpeed() {
    return m_maxSpeed;
  }
  int getTotalAltitude() {
    return m_totalAltitude;
  }
  float getVoltagee() {
    return m_voltage;
  }
  float getMinVoltagee() {
    return m_minVoltage;
  }
  float getMaxVoltage() {
    return m_maxVoltage;
  }
  int getMaxSatellites() {
    return m_maxSattelites;
  }
  int getTotalCourse() {
    return m_totalCourse;
  }
  int getTotalTrack() {
    return m_totalTrack;
  }
  int getTrack() {
    return m_track;
  }

  int getFrequency() {
    return m_frequency;
  }
  int getMaxFrequency() {
    return m_maxFrequency;
  }
  int getMinFrequency() {
    return m_minFrequency;
  }
  int getTempIn() {
    return m_tempIn;
  }
  int getTempOut() {
    return m_tempOut;
  }
  int getTimeZone() {
    return m_timeZone;
  }

  unsigned long getEngineTimeDay(){
    return m_engineTimeAll - m_engineTimeDay;
  }

  unsigned long getEngineTimeAll(){
    return m_engineTimeAll;
  }

  unsigned long getEngineTimeGaz(){
    return m_engineTimeAll - m_engineTimeGaz;
  }

  unsigned long getEngineTimeOil(){
    return m_engineTimeAll - m_engineTimeOil;
  }

};

#endif