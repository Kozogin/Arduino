#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <string>

enum dayOfWeekEn {
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  SUNDAY,
  OVER
};

const int TICKING_SEC = 1;

class DateTime {

private:
  int m_second;
  int m_minute;
  int m_hour;
  int m_day = 1;
  int m_month = 1;
  int m_year = 2021;
  int m_dayOfWeek;

public:

  void runningClock() {
    m_second += TICKING_SEC;
    calculateClock();
  }

  void calculateClock() {

    try {
      if (m_second > 59) {
        m_second %= 60;
        m_minute += 1;
      }
      if (m_minute > 59) {
        m_minute %= 60;
        m_hour += 1;
      }
      if (m_hour > 23) {
        m_hour %= 24;
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
    } catch (...) {}
  }

  void calculateDayOfWeek() {

    try {
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
    } catch (...) {}
  }

  int numberDaysMonth(int year, int month) {

    try {
      switch (month) {
        case 2:
          if (year % 4 == 0) {
            return 29;
          } else {
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
    } catch (...) {}
    return 31;
  }

  String getStringDayOfWeek() {

    try {
      switch (m_dayOfWeek) {
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
    } catch (...) {}
    return "error";
  }

  void printDateTime() {
    //std::cout << getStringDateTime() << "\n";
  }

  String getStringDateTime() {

    return static_cast< String >(m_day) + "/" + static_cast< String >(m_month) + "/" + static_cast< String >(m_year) + " , "
           + static_cast< String >(m_hour) + ":" + static_cast< String >(m_minute) + ":" + static_cast< String >(m_second)
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

  void plusHour(int plusHour) {
    m_hour += plusHour;
    calculateClock();
  }

  int getSecond() {
    return m_second;
  }
  int getMinute() {
    return m_minute;
  }
  int getHour() {
    return m_hour;
  }
  int getDay() {
    return m_day;
  }
  int getMonth() {
    return m_month;
  }
  int getYear() {
    return m_year;
  }
  int getDayOfWeek() {
    return m_dayOfWeek;
  }
};

#endif