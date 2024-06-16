#ifndef WEB_CONTROL_H
#define WEB_CONTROL_H

class WebContlol {

private:
  DataPropertiesEeprom *ptrDataProperties;

  float *m_ptrTempContuor_2;
  float *m_ptrTempContuor_3;
  float *m_ptrTempContuor_4;
  float *m_ptrTempContuor_5;
  float *m_ptrTempContuor_6;

  int *m_ptrStabilArr;
  float *m_ptrTempOut;

public:
  WebContlol() {}

  void setDataPropertiesEeprom(DataPropertiesEeprom *dataPro) {
    ptrDataProperties = dataPro;
  }


  String getPage() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 17px;}\n";
    ptr += ".picture {width: 600px; height: 600px;}\n";

    ptr += "</style>\n";
    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/' method='POST'>\n";
    ptr += "<h3>головна</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td class = 'actuale'><a href='/'>головна</a></td>\n";
    ptr += "<td><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";

    ptr += "<div class='picture'><br>\n";

    ptr += "<div style=' position: relative; margin: auto; width: 0px ;height: 00px ; border-left: 250px solid transparent; border-right: 250px solid transparent; border-bottom: 150px solid #440;text-align: center;'>\n";
    ptr += "<div style='position: absolute; top: 20px; left: -250px; width: 90px; height: 120px;'>\n";
    ptr += "<h2>" + (String)(*m_ptrTempOut) + "ºC</h2></div>\n";
    ptr += "</div>\n";

    ptr += "<div style='position: relative; margin: auto; width: 400px;height: 500px; background: #440;'>\n";
    ptr += "<h2>контур 2 радіатор басейну " + (String)(*m_ptrTempContuor_2) + "ºC (" + (String)(*m_ptrStabilArr) + "ºC)</h2>\n";
    ptr += "<h2>контур 3 другий поверх " + (String)(*m_ptrTempContuor_3) + "ºC (" + (String)(*(m_ptrStabilArr + 1)) + "ºC)</h2>\n";
    ptr += "<h2>контур 4 приточна вентиляція " + (String)(*m_ptrTempContuor_4) + "ºC (" + (String)(*(m_ptrStabilArr + 2)) + "ºC)</h2>\n";
    ptr += "<h2>контур 5 перший поверх захід " + (String)(*m_ptrTempContuor_5) + "ºC (" + (String)(*(m_ptrStabilArr + 3)) + "ºC)</h2>\n";
    ptr += "<h2>контур 6 другий третій поверхи захід " + (String)(*m_ptrTempContuor_6) + "ºC (" + (String)(*(m_ptrStabilArr + 4)) + "ºC)</h2>\n";

    ptr += "</div>\n";
    ptr += "</div>\n";
    ptr += "</form>\n";
    ptr += "</div>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }

  String getPageC2() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 47px; background: black; color: white;}\n";
    ptr += ".butt{background: red; width: 100px; height: 50px; color: white}\n";
    ptr += "</style>\n";
    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/c2' method='POST'>\n";
    ptr += "<h3>2 контур радіатор в басейні</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/'>головна</a></td>\n";
    ptr += "<td class = 'actuale'><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td>радіатор в басейні гріє</td>\n";
    ptr += "<td> <input type='checkbox' name='on_radiator_ckd' " + boolToChecked(ptrDataProperties->getOnRadiatorPool()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>радіатор, ручний режим</td>\n";
    ptr += "<td> <input type='number' min='-135' max='90' value='" + (String)(ptrDataProperties->getSTempStabil_2()) + "' name='temp_stabil'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>ручний режим ввімкнути</td>\n";
    ptr += "<td> <input type='checkbox' name='on_manual_ckd' " + boolToChecked(ptrDataProperties->getStatus_2()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>температурна крива</td>\n";
    ptr += "<td> <input type='number' min='0' max='50' value='" + (String)(ptrDataProperties->getSCurve_2()) + "' name='curve'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>записати</td>\n";
    ptr += "<td> <input type='checkbox' name='save_ckd'></td>\n";
    ptr += "</tr>\n";
    ptr += "</table>\n";

    ptr += "<br><br><br>\n";
    ptr += "<br><INPUT TYPE='submit' id='post' VALUE='Save' class='butt'><br><br>\n"; 
    ptr += "<br><br><br><br><br><br>\n";
    ptr += "</form>\n";
    ptr += "</div>\n";

    ptr += "<br><br><br><br><br><br>\n";


    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }

  String getPageC3() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 47px; background: black; color: white;}\n";
    ptr += ".butt{background: red; width: 100px; height: 50px; color: white}\n";
    ptr += "</style>\n";

    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/c3' method='POST'>\n";
    ptr += "<h3>3 контур 2-ий поверх</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/'>головна</a></td>\n";
    ptr += "<td><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td class = 'actuale'><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td>радіатори, ручний режим</td>\n";
    ptr += "<td> <input type='number' min='-135' max='90' value='" + (String)(ptrDataProperties->getSTempStabil_3()) + "' name='temp_stabil'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>ручний режим ввімкнути</td>\n";
    ptr += "<td> <input type='checkbox' name='on_manual_ckd' " + boolToChecked(ptrDataProperties->getStatus_3()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>температурна крива</td>\n";
    ptr += "<td> <input type='number' min='0' max='50' value='" + (String)(ptrDataProperties->getSCurve_3()) + "' name='curve'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>записати</td>\n";
    ptr += "<td> <input type='checkbox' name='save_ckd'></td>\n";
    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<br><br><br>\n";
    ptr += "<br><INPUT TYPE='submit' id='post' VALUE='Save' class='butt'><br><br>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</form>\n";
    ptr += "</div>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }

  String getPageC4() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 47px; background: black; color: white;}\n";
    ptr += ".butt{background: red; width: 100px; height: 50px; color: white}\n";
    ptr += "</style>\n";

    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/c4' method='POST'>\n";
    ptr += "<h3>4 контур вентиляція</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/'>головна</a></td>\n";
    ptr += "<td><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td class = 'actuale'><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<table>\n";

    ptr += "<tr>\n";
    ptr += "<td>вентиляція, ручний режим</td>\n";
    ptr += "<td> <input type='number' min='-135' max='90' value='" + (String)(ptrDataProperties->getSTempStabil_4()) + "' name='temp_stabil'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>ручний режим ввімкнути</td>\n";
    ptr += "<td> <input type='checkbox' name='on_manual_ckd' " + boolToChecked(ptrDataProperties->getStatus_4()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>температурна крива</td>\n";
    ptr += "<td> <input type='number' min='0' max='50' value='" + (String)(ptrDataProperties->getSCurve_4()) + "' name='curve'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>запуск насосу вентиляції</td>\n";
    ptr += "<td> <input type='number' min='0' max='24' value='" + (String)(ptrDataProperties->getSStartTime()) + "' name='time_start'>год</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>стоп насосу вентиляції</td>\n";
    ptr += "<td> <input type='number' min='0' max='24' value='" + (String)(ptrDataProperties->getSEndTime()) + "' name='time_stop'>год</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>записати</td>\n";
    ptr += "<td> <input type='checkbox' name='save_ckd'></td>\n";
    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<br><br><br>\n";
    ptr += "<br><INPUT TYPE='submit' id='post' VALUE='Save' class='butt'><br><br>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</form>\n";
    ptr += "</div>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }

  String getPageC5() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 47px; background: black; color: white;}\n";
    ptr += ".butt{background: red; width: 100px; height: 50px; color: white}\n";
    ptr += "</style>\n";

    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/c5' method='POST'>\n";

    ptr += "<h3>5 контур 1-ий поверх захід</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/'>головна</a></td>\n";
    ptr += "<td><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td class = 'actuale'><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td>радіатори, ручний режим</td>\n";
    ptr += "<td> <input type='number' min='-135' max='90' value='" + (String)(ptrDataProperties->getSTempStabil_5()) + "' name='temp_stabil'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>ручний режим ввімкнути</td>\n";
    ptr += "<td> <input type='checkbox' name='on_manual_ckd' " + boolToChecked(ptrDataProperties->getStatus_5()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>температурна крива</td>\n";
    ptr += "<td> <input type='number' min='0' max='50' value='" + (String)(ptrDataProperties->getSCurve_5()) + "' name='curve'></td>\n";
    ptr += "</tr> \n";
    ptr += "<tr>\n";
    ptr += "<td>записати</td>\n";
    ptr += "<td> <input type='checkbox' name='save_ckd'></td>\n";
    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<br><br><br>\n";
    ptr += "<br><INPUT TYPE='submit' id='post' VALUE='Save' class='butt'><br><br>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</form>\n";
    ptr += "</div>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }

  String getPageC6() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 47px; background: black; color: white;}\n";
    ptr += ".butt{background: red; width: 100px; height: 50px; color: white}\n";
    ptr += "</style>\n";

    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/c6' method='POST'>\n";
    ptr += "<h3>6 контур 2,3-ій поверх захід</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/'>головна</a></td>\n";
    ptr += "<td><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td class = 'actuale'><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td>радіатори, ручний режим</td>\n";
    ptr += "<td> <input type='number' min='-135' max='90' value='" + (String)(ptrDataProperties->getSTempStabil_6()) + "' name='temp_stabil'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>ручний режим ввімкнути</td>\n";
    ptr += "<td> <input type='checkbox' name='on_manual_ckd' " + boolToChecked(ptrDataProperties->getStatus_6()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>температурна крива</td>\n";
    ptr += "<td> <input type='number' min='0' max='50' value='" + (String)(ptrDataProperties->getSCurve_6()) + "' name='curve'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>записати</td>\n";
    ptr += "<td> <input type='checkbox' name='save_ckd'></td>\n";
    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<br><br><br>\n";
    ptr += "<br><INPUT TYPE='submit' id='post' VALUE='Save' class='butt'><br><br>\n";  ////////////
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</form>\n";
    ptr += "</div>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }

  String getPageOption() {

    String ptr = "<!DOCTYPE html>\n";

    ptr += "<html lang=\"en\">\n";
    ptr += "<head>\n";
    ptr += "<meta charset=\"UTF-8\">\n";

    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

    ptr += "<style>\n";
    ptr += "body {background: black; color:white}\n";
    ptr += ".center {margin: auto; width: 600px;}\n";
    ptr += "h1, h3 {text-align: center;}\n";
    ptr += "table {margin: auto; text-align: center;}\n";
    ptr += "tr, td {border: 1px solid white; width: 200px; height: 50px;}\n";
    ptr += "a {text-decoration: none; color: white}\n";
    ptr += ".actuale {background: red;}\n";
    ptr += "input {width: 47px; background: black; color: white;}\n";
    ptr += ".butt{background: red; width: 100px; height: 50px; color: white}\n";
    ptr += ".ssid_pass {width: 150px; background: black; color: white;}\n";
    ptr += "</style>\n";

    ptr += "<title>Керуванням колектором</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div class='center'>\n";
    ptr += "<h1>Керування колектором</h1>\n";
    ptr += "<form action='/option' method='POST'>\n";
    ptr += "<h3>опції</h3>\n";

    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/'>головна</a></td>\n";
    ptr += "<td><a href='/c2'>2 контур радіатор в басейні</a></td>\n";
    ptr += "<td><a href='/c3'>3 контур 2-ий поверх</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td><a href='/c4'>4 контур вентиляція</a></td>\n";
    ptr += "<td><a href='/c5'>5 контур 1-ий поверх захід</a></td>\n";
    ptr += "<td><a href='/c6'>6 контур 2,3-ій поверх захід</a></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td class = 'actuale'><a href='/option'>опції</a></td>\n";

    ptr += "</tr>\n";
    ptr += "</table>\n";
    ptr += "<table>\n";
    ptr += "<tr>\n";
    ptr += "<td>часовий пояс</td>\n";
    ptr += "<td> <input type='number' min='0' max='24' value='" + (String)(ptrDataProperties->getSCorrectX()) + "' name='correct_x'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>параметр А (температурна крива)</td>\n";
    ptr += "<td> <input type='number' min='-50' max='50' value='" + (String)(ptrDataProperties->getSCorrectA()) + "' name='correct_a'>ºC</td>\n";
    ptr += "</tr>\n";

    ptr += "<tr>\n";
    ptr += "<td>Повідомлення</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>якщо температура ззовні нижче</td>\n";
    ptr += "<td> <input type='number' min='-135' max='50' value='" + (String)(ptrDataProperties->getSOutLower()) + "' name='out_lower'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>якщо температура контура нижче</td>\n";
    ptr += "<td> <input type='number' min='-135' max='80' value='" + (String)(ptrDataProperties->getSContuorLower()) + "' name='contuor_lower'>ºC</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>номер контрольованого контура</td>\n";
    ptr += "<td> <input type='number' min='0' max='6' value='" + (String)(ptrDataProperties->getSNumberContuor()) + "' name='contour_number'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>кількість повідомлень через 3 хв</td>\n";
    ptr += "<td> <input type='number' min='0' max='80' value='" + (String)(ptrDataProperties->getSCountMessage()) + "' name='count_message'></td>\n";
    ptr += "</tr>\n";

    ptr += "<tr>\n";
    ptr += "<td>Тип сервоприводу</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>2 контур радіатор басейну NO</td>\n";
    ptr += "<td> <input type='checkbox' name='type_servo_2_ckd' " + boolToChecked(ptrDataProperties->getTypeServoNO_2()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>3 контур 2-ий поверх NO</td>\n";
    ptr += "<td> <input type='checkbox' name='type_servo_3_ckd' " + boolToChecked(ptrDataProperties->getTypeServoNO_3()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>4 контур вентиляція NO</td>\n";
    ptr += "<td> <input type='checkbox' name='type_servo_4_ckd' " + boolToChecked(ptrDataProperties->getTypeServoNO_4()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>5 контур 1-ий поверх захід NO</td>\n";
    ptr += "<td> <input type='checkbox' name='type_servo_5_ckd' " + boolToChecked(ptrDataProperties->getTypeServoNO_5()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>6 контур 2,3-ій поверх захід NO</td>\n";
    ptr += "<td> <input type='checkbox' name='type_servo_6_ckd' " + boolToChecked(ptrDataProperties->getTypeServoNO_6()) + "></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>записати</td>\n";
    ptr += "<td> <input type='checkbox' name='save_ckd'></td>\n";
    ptr += "</tr>\n";

    ptr += "<tr>\n";
    ptr += "<td>критичні налаштування</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>пароль для зміни 123</td>\n";
    ptr += "<td> <input type='text' name='pass_txt'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>SSID</td>\n";
    ptr += "<td> <input type='text' class ='ssid_pass' name='ssid_txt' value ='" + ptrDataProperties->getSSsidT() + "'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>password</td>\n";
    ptr += "<td> <input type='text' class ='ssid_pass' name='pass_ssid_txt' value ='" + ptrDataProperties->getSPasswordT() + "'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>bot token</td>\n";
    ptr += "<td> <input type='text' class ='ssid_pass' name='bot_token_txt' value ='" + ptrDataProperties->getSTokenT() + "'></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td>chat id</td>\n";
    ptr += "<td> <input type='text' class ='ssid_pass' name='chat_id_txt' value ='" + ptrDataProperties->getSChatIdT() + "'></td>\n";
    ptr += "</tr>\n";

    ptr += "</table>\n";
    ptr += "<br><br><br>\n";
    ptr += "<br><INPUT TYPE='submit' id='post' VALUE='Save' class='butt'><br><br>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</form>\n";
    ptr += "</div>\n";
    ptr += "<br><br><br><br><br><br>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
  }


  String boolToChecked(bool state) {
    if (state) {
      return "checked";
    } else {
      return "";
    }
  }

  void setPtrTempContuor_2(float *ptr) {
    m_ptrTempContuor_2 = ptr;
  }
  void setPtrTempContuor_3(float *ptr) {
    m_ptrTempContuor_3 = ptr;
  }
  void setPtrTempContuor_4(float *ptr) {
    m_ptrTempContuor_4 = ptr;
  }
  void setPtrTempContuor_5(float *ptr) {
    m_ptrTempContuor_5 = ptr;
  }
  void setPtrTempContuor_6(float *ptr) {
    m_ptrTempContuor_6 = ptr;
  }

  void setPtrTempOut(float *ptr) {
    m_ptrTempOut = ptr;
  }

  void setPtrStabilArr(int *ptr) {
    m_ptrStabilArr = ptr;
  }
  // void setPtrStabil_3(int *ptr){
  //   m_ptrStabil_3 = ptr;
  // }
  // void setPtrStabil_4(int *ptr){
  //   m_ptrStabil_4 = ptr;
  // }
  // void setPtrStabil_5(int *ptr){
  //   m_ptrStabil_5 = ptr;
  // }
  // void setPtrStabil_6(int *ptr){
  //   m_ptrStabil_6 = ptr;
  // }
};


#endif