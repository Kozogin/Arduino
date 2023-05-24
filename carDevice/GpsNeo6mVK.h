#ifndef GPS_NEO6M_VK_H
#define GPS_NEO6M_VK_H

#include <string>
#include <vector>  
#include <sstream>  


class GpsNeo6mVK{
private:
	std::vector <std::string> vect_rowGN___;
	std::vector <std::string> vect_data;
	
	int m_year;
	int m_mounth;
	int m_day;

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
	
	void dataProcessing(String const& nmeaString)
	{
    std::string nmea = nmeaString.c_str();

		vect_rowGN___.clear();
		splitNmea(nmea, '$', vect_rowGN___);

		for (const auto& row : vect_rowGN___)
		{
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
	}



	void parseGNRMC() {

		std::string m_dateGps;

		if (vect_data.size() > 9) {
			m_dateGps = vect_data[9];

			if (m_dateGps.size() > 5) {
				m_year = 2000 + (int)atoi(m_dateGps.substr(4, 2).c_str());
				m_mounth = (int)atoi(m_dateGps.substr(2, 2).c_str());
				m_day = (int)atoi(m_dateGps.substr(0, 2).c_str());
			}

			if (vect_data[7].size()) {
				m_speedGpsI = 1.852f * (float)atof(vect_data[7].c_str());
			}			
		}	
		
	}

	void parseGNGGA() {

		std::string m_timeGps;

		if (vect_data.size() > 10) {

			m_timeGps = vect_data[1];

			if (m_timeGps.size() > 5) {
				m_sec = (int)atoi(m_timeGps.substr(4, 2).c_str());
				m_minute = (int)atoi(m_timeGps.substr(2, 2).c_str());
				m_hour = (int)atoi(m_timeGps.substr(0, 2).c_str());

        m_boolTime = true;
			} else {
        m_boolTime = false;
      }

			if (vect_data[2].size()) {
				m_latGpsD = 0.01 * (float)atof(vect_data[2].c_str());
				m_boolNewData = true;				
			}
			else {
				m_boolNewData = false;
			}
			if (vect_data[4].size()) {
				m_lonGpsD = 0.01 * (float)atof(vect_data[4].c_str());
			}
			if (vect_data[9].size()) {
				m_altitudeGpsF = (float)atof(vect_data[9].c_str());
			}
			if (vect_data[8].size()) {
				m_directionGpsI = (int)atoi(vect_data[8].c_str());
			}
			if (vect_data[7].size()) {
				m_numSatelitesGpsI = (int)atoi(vect_data[7].c_str());
			}
			
		}
		
	}

	void splitNmea(std::string const& str, const char delim, std::vector <std::string>& out)
	{
		std::stringstream s(str);

		std::string s2;
		while (std::getline(s, s2, delim))
		{
			out.push_back(s2);
		}
	}


	int f_speed_kmph() { return m_speedGpsI; }
	int f_course() { return m_directionGpsI; }
	float f_altitude() { return m_altitudeGpsF; }
	int satellites() { return m_numSatelitesGpsI; }

	double getLon() { return m_lonGpsD; }
	double getLat() { return m_latGpsD; }
	
	int getYear() { return m_year; }
	int getMonth() { return m_mounth; }
	int getDay() { return m_day; }
	int getHour() { return m_hour; }
	int getMinute() { return m_minute; }
	int getSec() { return m_sec; }

	bool getNewData() { return m_boolNewData;}
  bool getNewTime() { return m_boolTime;}
  
};

#endif