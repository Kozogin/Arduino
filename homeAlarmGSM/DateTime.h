#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <iostream>
#include <string>

enum dayOfWeekEn{
	MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY, OVER
};

const int TICKING_SEC = 30;

class DateTime {

private:
	int m_second;
	int m_minute;
	int m_hour;
	int m_day = 1;
	int m_month = 1;
	int m_year = 2021;	
	int m_dayOfWeek;

	int m_hour_morning_on;
	int m_minute_morning_on;
	int m_hour_morning_off;
	int m_minute_morning_off;
	
	int m_hour_evening_on;
	int m_minute_evening_on;
	int m_hour_evening_off;
	int m_minute_evening_off;

	bool m_releOn;

public:

	void rndLight() {
		try{
			if (m_year > 2021 && m_year < 2090) {
				choiceHour();
				calculateMinute();

				if ((100 * m_hour + m_minute > 100 * m_hour_morning_on + m_minute_morning_on
					&& 100 * m_hour + m_minute < 100 * m_hour_morning_off + m_minute_morning_off)
					|| (100 * m_hour + m_minute > 100 * m_hour_evening_on + m_minute_evening_on
						&& 100 * m_hour + m_minute < 100 * m_hour_evening_off + m_minute_evening_off)
					) {
					m_releOn = true;
				}
				else {
					m_releOn = false;
				}		
			}
		}catch (...){}
	}

	void choiceHour() {
		try{
			switch (m_month)
			{
			case 1:
			case 12:
				m_hour_morning_on = 6;
				m_minute_morning_on = 30;
				m_hour_evening_on = 16;
				m_minute_evening_on = 30;
				m_hour_evening_off = 22;
				break;
			case 2:
			case 11:
				m_hour_morning_on = 6;
				m_minute_morning_on = 30;
				m_hour_evening_on = 17;
				m_minute_evening_on = 30;
				m_hour_evening_off = 23;			
				break;
			case 3:
			case 10:
				m_hour_morning_on = 0;
				m_minute_morning_on = 0;
				m_hour_evening_on = 18;
				m_minute_evening_on = 0;
				m_hour_evening_off = 23;			
				break;
			case 4:
			case 9:
				m_hour_morning_on = 0;
				m_minute_morning_on = 0;
				m_hour_evening_on = 19;
				m_minute_evening_on = 0;
				m_hour_evening_off = 23;
				break;
			case 5:
			case 8:
				m_hour_morning_on = 0;
				m_minute_morning_on = 0;
				m_hour_evening_on = 20;
				m_minute_evening_on = 0;
				m_hour_evening_off = 23;
				break;
			case 6:
			case 7:
				m_hour_morning_on = 0;
				m_minute_morning_on = 0;
				m_hour_evening_on = 20;
				m_minute_evening_on = 30;
				m_hour_evening_off = 23;
				break;
			default:
				break;
			}
			m_minute_evening_off = 0;

		}catch (...){}
	}

	void calculateMinute() {

		try{
			int minuteOnMorning = (134 * m_day) % 19;
			int durationMorning = 20 + (258 * m_day) % 19;

			int minuteOnEvening = (184 * m_day) % 59;
			int minuteOffEvening = (218 * m_day) % 59;
			
			if (m_hour_morning_on != 0) {
				calculateTimeOnOff(m_hour_morning_on, m_minute_morning_on, minuteOnMorning);
				m_hour_morning_off = m_hour_morning_on;
				m_minute_morning_off = m_minute_morning_on;
				calculateTimeOnOff(m_hour_morning_off, m_minute_morning_off, durationMorning);
			}		

			calculateTimeOnOff(m_hour_evening_on, m_minute_evening_on, minuteOnEvening);
			calculateTimeOnOff(m_hour_evening_off, m_minute_evening_off, minuteOffEvening);	

		}catch (...){}
	}

	void calculateTimeOnOff(int &hour, int &minute, int rndMinute) {

		try{
			minute += rndMinute;
			if (minute > 59) {
				minute -= 60;
				hour += 1;
			}
		}catch (...){}
	}	

	void runningClock() {

		try{
			m_second += TICKING_SEC;
			if (m_second > 59) {
				m_second %= 60;
				m_minute += 1;
			}
			if (m_minute > 59) {
				m_minute = 0;
				m_hour += 1;
			}
			if (m_hour > 23) {
				m_hour = 0;
				m_day += 1;
				m_dayOfWeek += 1;
			}
			if (m_dayOfWeek > 6) {
				m_dayOfWeek = 0;
			}
			if (m_day > numberDaysMonth(m_year, m_month)) {
				m_day = 1;
				m_month += 1;			
			}
			if (m_month > 12) {
				m_month = 1;
				m_year += 1;
			}
			rndLight();
		}catch (...){}
	}

	void calculateDayOfWeek() {

		try{
			int dayOfWeek_c = 5;
			int year_c = 2022;
			int month_c = 1;
			int day_c = 1;
			
			if (m_year > 2045) {
				year_c = 2046;
				dayOfWeek_c = 0;
			}			

			while (true) {  
				
				day_c += 1;
				dayOfWeek_c += 1;


				if (dayOfWeek_c > 6) {
					dayOfWeek_c = 0;
				}
				if (day_c > numberDaysMonth(year_c, month_c)) {
					day_c = 1;
					month_c += 1;
				}
				if (month_c > 12) {
					month_c = 1;
					year_c += 1;
				}

				if (year_c == m_year && month_c == m_month && day_c == m_day) {
					m_dayOfWeek = dayOfWeek_c;
					break;
				}

				if (year_c < 2022 || year_c > 2070) {
					break;
				}			
			}
		}catch (...){}
	}

	int numberDaysMonth(int year, int month) {

		try{
			switch (month)
			{
			case 2:
				if (year % 4 == 0) {
					return 29;
				}
				else {
					return 28;
				}
			case 4:
			case 6:
			case 9:
			case 11:
				return 30;			
			default:
				return 31;
			}
		}catch (...){}
		return 31;
	}

	String getStringDayOfWeek() {

		try{
			switch (m_dayOfWeek)
			{
			case 0:
				return "Monday";
			case 1:
				return "Tuesday";
			case 2:
				return "Wednesday";
			case 3:
				return "Thursday";
			case 4:
				return "Friday";
			case 5:
				return "Saturday";
			case 6:
				return "Sunday";
			default:
				return "error";
			}
		}catch (...){}
		return "error";
	}
	
	void printDateTime() {
		//std::cout << getStringDateTime() << "\n";
	}

	String getStringDateTime() {
        
		return  static_cast < String >(m_day) + "/" + static_cast < String >(m_month) + "/" + static_cast < String >(m_year) + " , "
		 	+ static_cast < String >(m_hour) + ":" + static_cast < String >(m_minute) + ":" + static_cast < String >(m_second)
		 	+ "  " + getStringDayOfWeek();
	}

	void setDateTime(int year, int month, int day, int hour, int minute) {

		m_second = 0;
		m_minute = minute;
		m_hour = hour;
		m_day = day;
		m_month = month;
		m_year = year;
		calculateDayOfWeek();
	}

	void plusHour() {
		m_hour += 1;
	}

	void minusHour() {
		m_hour -= 1;
	}

	int getSecond() { return m_second; }
	int getMinute() { return m_minute; }
	int getHour() { return m_hour; }
	int getDay() { return m_day; }
	int getMonth() { return m_month; }
	int getYear() { return m_year; }
	int getDayOfWeek() { return m_dayOfWeek; }
	bool getReleOn() { return m_releOn; }

	int getHourMorningOn() { return m_hour_morning_on; }
	int getMinuteMorningOn() { return m_minute_morning_on; }
	int getHourMorningOff() { return m_hour_morning_off; }
	int getMinuteMorningOff() { return m_minute_morning_off; }

	int getHourEveningOn() { return m_hour_evening_on; }
	int getMinuteEveningOn() { return m_minute_evening_on; }
	int getHourEveningOff() { return m_hour_evening_off; }
	int getMinuteEveningOff() { return m_minute_evening_off; }

};

#endif