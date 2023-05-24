#ifndef HANDLER_DATA_GPS_H
#define HANDLER_DATA_GPS_H

#include <TinyGPS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

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
const int TIME_INTERRUPT_AGAIN = 120000 / NUMBER_CYLINDERS;
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

  int m_maxSattelites;
  bool m_startGPS;
  unsigned long tachoCalcTimePrevious;
  unsigned long tachoLastTurnTime;
  bool m_stopInterrupt;
  int m_tempIn;
  int m_tempOut;

public:

  HandlerDataGPS() {
    m_minFrequency = 9000;
    m_minCandidateFrequency = 9000;
  }

  void readSensor() {

    sensorIn.requestTemperatures();
    sensorOut.requestTemperatures();
    m_tempIn = sensorIn.getTempCByIndex(0);
    m_tempOut = sensorOut.getTempCByIndex(0);
  }

  void tachoCalcClass() {

    if (micros() - tachoLastTurnTime > 2 * TIME_INTERRUPT_AGAIN) {
      m_stopInterrupt = false;
      m_frequency = 0;
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
  }

  int getAvgSpeed() {
    return 3600 * m_track / millis();
  }

  void calcMaxSpeed(int speed) {
    if (m_maxSpeed == 0) {
      m_maxSpeed = speed;
    }
    if (m_maxSpeed < speed) {
      m_maxSpeed = speed;
    }
  }

  void calcAltitude(int altitude) {
    if (!m_startGPS) {
      m_startAltitude = altitude;
    }
    m_totalAltitude = altitude - m_startAltitude;
  }

  void calcVoltage() {

    int analog = analogRead(VOLTAGE_PIN);
    m_voltage = (float)(1.5674 * 3.3 * analog / 1024);

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
  }

  void calcSatellites(int satellites) {
    if (satellites > 50) {
      satellites = 0;
    }
    if (m_maxSattelites < satellites) {
      m_maxSattelites = satellites;
    }
  }

  void calcPosition(float lat, float lon) {
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
      if(distance < m_previousDistance + 100){
        m_track += distance;
        m_previousDistance = distance;
      } else {
        m_track += m_previousDistance;
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
  }

  int f_course(){
    return m_course;
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
};

#endif