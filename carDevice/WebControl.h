#ifndef WEB_CONTROL_H
#define WEB_CONTROL_H

class WebContlol {

private:
  HandlerDataGPS *m_handlerGPS;

  int m_timeZone;
  float m_mileage;
  bool m_chkTimeZone;
  bool m_chkMileage;
  bool m_chkGaz;
  bool m_chkOil;  

public:
  WebContlol() {}

  WebContlol(HandlerDataGPS *handlerGPS) {
    m_handlerGPS = handlerGPS;
  }

  String getPage() {

    String ptr = "<!DOCTYPE html>\n";

    //try {

      ptr += "<html lang=\"en\">\n";
      ptr += "<head>\n";
      ptr += "<meta charset=\"UTF-8\">\n";

      ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
      ptr += "<title>Alarm</title>\n";
      ptr += "<style>\n";
      ptr += "body{margin-top: 20px;background-color: #222;}\n";
      ptr += "div, table {color: #ccc; margin: 10px auto 30px; text-align: center; font-size: 30px;}\n";
      ptr += "td {width: 80px; text-align:  center; padding: 15px 20px;}\n";
      ptr += "h1 {color: #ccc; margin: 0 40px;}\n";
      ptr += "#time_zone, #mileage {background: black; color:aliceblue; font-size: 20px; width: 120px;}\n";
      ptr += ".post {display: block;width: 90px;background-color: #992121;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";

      ptr += "</style>\n";
      ptr += "</head>\n";

      ptr += "<body>\n";

      ptr += "<div>\n";
      ptr += "<h1>speedometer control</h1>\n";
      ptr += "<br><br>\n";
      ptr += "<form action='/' id='mileageForm' method='POST'>\n";
      ptr += "<table>\n";

      ptr += "<tr>\n";
      ptr += "<td>time zone</td>\n";
      ptr += "<td><input type='number' id='time_zone' name='time_zone'></td>\n";
      ptr += "<td><input type='checkbox' id='time_zone_check' name='time_zone_check'></td>\n";
      ptr += "</tr>\n";

      ptr += "<tr>\n";
      ptr += "<td>mileage</td>\n";
      ptr += "<td><input type='number' id='mileage' name='mileage'></td>\n";
      ptr += "<td><input type='checkbox' id='mileage_check' name='mileage_check'></td>\n";
      ptr += "</tr>\n";

      ptr += "<tr>\n";
      ptr += "<td>gaz</td>\n";
      ptr += "<td>erase</td>\n";
      ptr += "<td><input type='checkbox' id='gaz_check' name='gaz_check'></td>\n";
      ptr += "</tr>\n";

      ptr += "<tr>\n";
      ptr += "<td>oil</td>\n";
      ptr += "<td>erase</td>\n";
      ptr += "<td><input type='checkbox' id='oil_check' name='oil_check'></td>\n";
      ptr += "</tr>\n";

      ptr += "</table>\n";
      ptr += "<INPUT TYPE='submit' class='post' VALUE='OK'>\n";
      ptr += "</form>\n";
      ptr += "</div>\n";

      ptr += "<br> <br> <br> <br> <br> \n";

      ptr += "</body>\n";
      ptr += "</html>\n";

    // } catch (...) {
    // }
    return ptr;
  }

  void resetCheck() {
    m_chkTimeZone = false;
    m_chkMileage = false;
    m_chkGaz = false;
    m_chkOil = false;
  }

  void setTimeZone(char *array) {
    try {
      m_timeZone = ((String)array).toInt();
    } catch (...) {}
  }

  void saveTimeZone() {
    m_chkTimeZone = true;
  }

  void setMileage(char *array) {
    try {
      m_mileage = ((String)array).toFloat();
    } catch (...) {}
  }

  void saveMileage() {
    m_chkMileage = true;
  }

  void resetTrackGaz() {
    try{
    m_handlerGPS->resetTrackGaz();
    m_chkGaz = true;
    } catch (...) {}
  }

  void resetTrackOil() {
    try{
    m_handlerGPS->resetTrackOil();
    m_chkOil = true;
    } catch (...) {}
  }

  void saveAll() {

    try{
    if (m_chkTimeZone) {
      m_handlerGPS->setTimeZone(m_timeZone);
    }
    if (m_chkMileage) {
      m_handlerGPS->setTrackAll(m_mileage);
    }
    if(m_chkTimeZone || m_chkMileage || m_chkGaz || m_chkOil){
      m_handlerGPS->allowWebControl(true);
    } else {
      m_handlerGPS->allowWebControl(false);
    }
    } catch (...) {}
  }
};

#endif