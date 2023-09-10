#ifndef GPS_NEO6M_VK_H
#define GPS_NEO6M_VK_H

#include <string>
#include <vector>
#include <sstream>

class GpsNeo6mVK {
private:
  std::vector<std::string> vect_rowGN___;
  std::vector<std::string> vect_data;

  int m_year;
  int m_mounth;
  int m_day;
  int m_mounthCandidate;
  int m_dayCandidate;
  int m_countRelevantDay;
  int m_countRelevantMounth;

  int m_hour;
  int m_minute;
  int m_sec;

  float m_lonGpsD;
  float m_latGpsD;

  float m_altitudeGpsF;
  int m_speedGpsI;
  int m_directionGpsI;
  int m_numSatelitesGpsI;

  bool m_boolNewData;
  bool m_boolTime;

public:

  void prevails(int data, int &relevant, int &candidate, int &countRelevant) {
    if (relevant == 0) {
      relevant = data;
    }
    if (relevant == data) {
      countRelevant++;
    }
    else {
      countRelevant--;
      candidate = data;
    }

    if(countRelevant > 20){
      countRelevant = 20;
    }

    if (countRelevant < 0) {
      int changeRelevant;
      changeRelevant = relevant;
      relevant = candidate;
      candidate = changeRelevant;
      countRelevant = 1;
    }

  }

  void dataProcessing(String const& nmeaString) {
    std::string nmea = nmeaString.c_str();

    try {

      vect_rowGN___.clear();
      splitNmea(nmea, '$', vect_rowGN___);

      for (const auto& row : vect_rowGN___) {
        vect_data.clear();
        splitNmea(row, ',', vect_data);

        if (vect_data.size() > 0) {
          if (vect_data[0] == "GNRMC") {
            parseGNRMC();
          }
          if (vect_data[0] == "GNGGA") {
            parseGNGGA();
          }
        }
      }
    } catch (...) {
      Serial.println("GpsNeo6mVK dataprocessing");
    }
  }

  void parseGNRMC() {

    std::string m_dateGps;
    int dayData;
    int mounthData;

    try {
      if (vect_data.size() > 9) {
        m_dateGps = vect_data[9];

        if (m_dateGps.size() > 5) {

          if (checkIsDigit(m_dateGps.substr(4, 2))) {
            m_year = 2000 + (int)atoi(m_dateGps.substr(4, 2).c_str());
          }

          if (checkIsDigit(m_dateGps.substr(2, 2))) {
            mounthData = (int)atoi(m_dateGps.substr(2, 2).c_str());
          }

          if (checkIsDigit(m_dateGps.substr(0, 2))) {
            dayData = (int)atoi(m_dateGps.substr(0, 2).c_str());
          }
        }

        if(dayData < 0 && dayData > 31){
          m_year = 2020;
        }

        if(mounthData < 0 && mounthData > 12){
          m_year = 2020;
        }

        prevails(dayData, m_day, m_dayCandidate, m_countRelevantDay);
        prevails(mounthData, m_mounth, m_mounthCandidate, m_countRelevantMounth);
        
        if (vect_data[7].size()) {
          if (checkIsDigit(vect_data[7])) {
            m_speedGpsI = 1.852f * (float)atof(vect_data[7].c_str());
          }
        }
      }
    } catch (...) {
      Serial.println("GpsNeo6mVK parseGNRMC");
    }
  }

  void parseGNGGA() {

    std::string m_timeGps;

    try {
      if (vect_data.size() > 10) {

        m_timeGps = vect_data[1];

        if (m_timeGps.size() > 5) {

          if(checkIsDigit(m_timeGps.substr(4, 2))){
            m_sec = (int)atoi(m_timeGps.substr(4, 2).c_str());
          }

          if(checkIsDigit(m_timeGps.substr(2, 2))){
            m_minute = (int)atoi(m_timeGps.substr(2, 2).c_str());
          }

          if(checkIsDigit(m_timeGps.substr(0, 2))){
            m_hour = (int)atoi(m_timeGps.substr(0, 2).c_str());
          }    

          m_boolTime = true;
        } else {
          m_boolTime = false;
        }

        if (vect_data[2].size()) {

          if(checkIsDigit(vect_data[2])){
            m_latGpsD = 0.01 * (float)atof(vect_data[2].c_str());
          }          
          m_boolNewData = true;
        } else {
          m_boolNewData = false;
        }
        if (vect_data[4].size() && checkIsDigit(vect_data[4])) {
          m_lonGpsD = 0.01 * (float)atof(vect_data[4].c_str());
        }
        if (vect_data[9].size() && checkIsDigit(vect_data[9])) {
          m_altitudeGpsF = (float)atof(vect_data[9].c_str());
        }
        if (vect_data[8].size() && checkIsDigit(vect_data[8])) {
          m_directionGpsI = (int)atoi(vect_data[8].c_str());
        }
        if (vect_data[7].size() && checkIsDigit(vect_data[7])) {
          m_numSatelitesGpsI = (int)atoi(vect_data[7].c_str());
        }
      }
    } catch (...) {
      Serial.println("GpsNeo6mVK parseGNGGA");
    }
  }

  void splitNmea(std::string const& str, const char delim, std::vector<std::string>& out) {
    std::stringstream s(str);
    std::string s2;

    try {
      while (std::getline(s, s2, delim)) {
        out.push_back(s2);
      }
    } catch (...) {
      Serial.println("GpsNeo6mVK splitNmea");
    }
  }

  bool checkIsDigit(std::string candidate) {

    for (int i = 0; i < candidate.length(); i++) {
      if ((int)(candidate[i]) == 32 || (int)(candidate[i]) == 46 || (int)(candidate[i]) >= 48 && (int)(candidate[i]) <= 57) {
		} else {
        return false;
      }
    }

    return true;
  }


  int f_speed_kmph() {
    return m_speedGpsI;
  }
  int f_course() {
    return m_directionGpsI;
  }
  float f_altitude() {
    return m_altitudeGpsF;
  }
  int satellites() {
    return m_numSatelitesGpsI;
  }

  double getLon() {
    return m_lonGpsD;
  }
  double getLat() {
    return m_latGpsD;
  }

  int getYear() {
    return m_year;
  }
  int getMonth() {
    return m_mounth;
  }
  int getDay() {
    return m_day;
  }
  int getHour() {
    return m_hour;
  }
  int getMinute() {
    return m_minute;
  }
  int getSec() {
    return m_sec;
  }

  bool getNewData() {
    return m_boolNewData;
  }
  bool getNewTime() {
    return m_boolTime;
  }
};

#endif