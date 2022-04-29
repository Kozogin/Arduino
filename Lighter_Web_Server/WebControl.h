#ifndef WEB_CONTROL_H
#define WEB_CONTROL_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

enum EventPerfomer{
  NOTHING, TIMER_START
};

enum ColorLight{
  TRANSPARENT, WHITEC, UKRAINEC, REDC, GREENC, BLUEC, PURPLEC, SALADC, YELLOWC, RIGHTC, LEFTC, OFFC
};

class WebControl{

protected:    
  int m_timerLight;
  int m_rangeLight;
  bool m_chkStrob;
  EventPerfomer m_eventPerfomer;
  ColorLight m_colorLight;
  String divLight1;
  String divLight2;
  int m_intRefhesh; 

public:

WebControl(){
  m_timerLight = 0;
  m_rangeLight = 100;
  int m_intRefhesh = 0;
}

String getPageTimer(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<html lang=\"en\">\n";
  ptr +="<head>\n";
    ptr +="<meta charset=\"UTF-8\">\n";
    ptr +="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
    ptr +="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ptr +="<title>Timer</title>\n";

    ptr +="<style>\n";
        ptr +="body{margin-top: 20px;background-color: #333;}\n";
        ptr +="table {color: #888;margin: 10px auto 30px;} \n";
        ptr +="h1, h2 {color: #888;margin: 10px auto 30px;} \n";    
        ptr +="h3 {color: #888;margin-bottom: 30px; padding-bottom: 0px; padding-right: 30px ;}\n";
        ptr +=".owner_ckd{padding: 0px 0px; margin: 0px auto 35px; background-color: #248c9c; }\n";
        ptr +=".owner {display: inline;width: 90px;background-color: #945c0c;border: none;color: white;padding: 10px 30px;text-decoration: none;font-size: 20px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;} \n";   
        ptr +="#post, #post1 {display: block;width: 90px;background-color: #992121;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
        ptr +="#sub_ok {display: block;width: 130px;background-color: #992121;border: none;color: white;padding: 13px 0px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
        ptr +=".arefr{color:lavenderblush; background-color: rgb(231, 55, 55); padding: 10px; font-size: 20px; text-decoration: none;}\n";
        ptr +="#divLightOn1{display: block; background-color: #" + divLight1 + "; width: 120px; height: 120px;}\n";
        ptr +="#divLightOn2{display: block; background-color: #" + divLight2 + "; width: 120px; height: 120px;}\n";        
        
        if(m_timerLight < 1){
          ptr +="body{background-color: #363;}\n";
        }
    ptr +="</style>\n";
ptr +="</head>\n";
ptr +="<body>\n";
    ptr +="<form id='form_timer' method='POST' action='/timer'>\n";
    ptr +="<table>\n";
        ptr +="<caption><h1>Ліхтар</h1></caption>\n";

        ptr +="<tr>\n";   
          ptr +="<th><div id='divLightOn1'></div></th>\n"; 
          ptr +="<th><div id='divLightOn2'></div></th>\n"; 
        ptr +="</tr>\n";        
        ptr +="<tr>\n";
            ptr +="<td></td>\n";
             ptr +="<td><INPUT TYPE='button' id='sub_ok' VALUE='обновити'></td>\n";            
        ptr +="</tr>\n";
        ptr +="<tr>\n";
            ptr +="<td><h3></h3></td>\n";
            ptr +="<td></td>\n";
        ptr +="</tr>\n";
        ptr +="<tr>\n";
          ptr +="<td><h3>Таймер, сек</h3></td>\n";
          ptr +="<td><h2>" + (String)(m_timerLight / 10) + "</h2></td>\n";
        ptr +="</tr>\n";
        ptr +="<tr>\n";
        ptr +="<td><h3>Стробоскоп</h3></td>\n";            
              ptr +="<td><input type='checkbox' id='strob_ckd' class='owner_ckd' name='strob_ckd' " + chekedT() + "></td> \n";                    
        ptr +="</tr>\n"; 

         ptr +="<tr>\n";
             ptr +="<td><h3>Яскравість</h3></td>\n";
             ptr +="<td><div><input type='range' id='range_br' name='range_Light' min='0' max='100' step='1' value='" + (String)m_rangeLight + "'></div></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #fff;'>Білий</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_white' class='owner' name='rdo_color' value='radio_white' " + colorWhiteChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #00f;'>Укр <span style='color: #ff0;'>аїна</span></h3></td>\n";
             ptr +="<td><input type='radio' id='radio_ukraine' class='owner' name='rdo_color' value='radio_ukraine' " + colorUkraineChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #f00;'>Червоний</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_red' class='owner' name='rdo_color' value='radio_red' " + colorRedChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #0f0;'>Зелений</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_green' class='owner' name='rdo_color' value='radio_green' " + colorGreenChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #00f;'>Синій</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_blue' class='owner' name='rdo_color' value='radio_blue' " + colorBlueChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #f0f;'>Фіолет</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_purple' class='owner' name='rdo_color' value='radio_purple' " + colorPurpleChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #0ff;'>Салат</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_salad' class='owner' name='rdo_color' value='radio_salad' " + colorSaladChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #ff0;'>Жовтий</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_yellow' class='owner' name='rdo_color' value='radio_yellow' " + colorYellowChecked() + "></td>\n";
         ptr +="</tr>\n";
        //------------- turn left-right
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #f50;'>Поворот направо</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_right' class='owner' name='rdo_color' value='radio_right' " + colorRightChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #f50;'>Поворот наліво</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_left' class='owner' name='rdo_color' value='radio_left' " + colorLeftChecked() + "></td>\n";
         ptr +="</tr>\n";
         ptr +="<tr>\n";
             ptr +="<td><h3 style='color: #fff;'>Вимкнено</h3></td>\n";
             ptr +="<td><input type='radio' id='radio_color_off' class='owner' name='rdo_color' value='radio_color_off' " + colorOffChecked() + "></td>\n";
         ptr +="</tr>\n";

         ptr +="<tr>\n";
             ptr +="<td></td>\n";          
             ptr +="<td><INPUT TYPE='submit' id='post' VALUE='OK'></td>\n";
         ptr +="</tr>\n";

         ptr +="<tr>\n";
             ptr +="<td></td>\n";          
             ptr +="<td><a href='/' id='post1'>назад</a></td>\n";
         ptr +="</tr>\n";
         ptr +="<td><h3>записати</h3></td>\n";            
             ptr +="<td><input type='checkbox' id='save_ckd' class='owner_ckd' name='save_ckd'></td> \n";                    
         ptr +="</tr>\n";
          ptr +="</table>\n";
         ptr +="</form>\n";

         ptr +="<script>\n"; 
          if(m_intRefhesh != 1){
            ptr +="function codeAddress(){ window.location.reload();}\n";
            ptr +="window.onload = codeAddress; \n";
          } 
          ptr +="sub_ok.onclick = function() { window.location.reload();}\n";
        ptr +="</script> \n";
        
 ptr +="</body>\n";
 ptr +="</html>\n";

  m_intRefhesh++;
  if(m_intRefhesh == 2){
    m_intRefhesh = 0;
  }
  
  return ptr;
}

String getPage(){

String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<html lang=\"en\">\n";
  ptr +="<head>\n";
    ptr +="<meta charset=\"UTF-8\">\n";
    ptr +="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
    ptr +="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ptr +="<title>Candle</title>\n";

    ptr +="<style>\n";
        ptr +="body{margin-top: 20px;background-color: #333;}\n";
        ptr +="table {color: #888;margin: 10px auto 30px;} \n";
        ptr +="h1 {color: #888;margin: 10px auto 30px;} \n";    
        ptr +="h3 {color: #888;margin-bottom: 30px; padding-bottom: 0px; padding-right: 30px ;}\n";
        ptr +=".owner_ckd{padding: 0px 0px; margin: 0px auto 35px; background-color: #248c9c; }\n";
        ptr +=".owner {display: inline;width: 90px;background-color: #945c0c;border: none;color: white;padding: 10px 30px;text-decoration: none;font-size: 20px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;} \n";   
        ptr +="#post {display: block;width: 90px;background-color: #992121;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
        ptr +=".arefr{color:lavenderblush; background-color: rgb(231, 55, 55); padding: 10px; font-size: 20px; text-decoration: none;}\n";
      ptr +="</style>\n";
ptr +="</head>\n";
ptr +="<body>\n";
    ptr +="<form id='form_post' method='POST' action='/timer'>\n";
    ptr +="<table>\n";
        ptr +="<caption><h1>Ліхтар</h1></caption>\n";
        ptr +="<tr> \n";  
          ptr +="<th></th> \n";
          ptr +="<th></th> \n";
        ptr +="</tr> \n";        
        ptr +="<tr>\n";
            ptr +="<td><h3></h3></td>\n";
            ptr +="<td></td>\n";
        ptr +="</tr>\n";
        ptr +="<tr>\n";
          ptr +="<td><h3>Таймер, сек</h3></td>\n";
          ptr +="<td><input type='number' id='timer_light' class='owner' name='timer_light' value='" + (String)(m_timerLight / 10) + "' ></td>\n";
        ptr +="</tr>\n";       
         ptr +="<tr>\n";
             ptr +="<td><h3>Запуск таймера</h3></td>\n";          
             ptr +="<td><input type='radio' id='radio_timer' class='owner' name='rdo_event' value='radio_timer' checked></td>\n";
         ptr +="</tr>\n"; 
         ptr +="<tr>\n";
             ptr +="<td></td>\n";          
             ptr +="<td><INPUT TYPE='submit' id='post' VALUE='OK'></td>\n";
         ptr +="</tr>\n";
        
        ptr +="</table>\n";
     ptr +="</form>\n";

     ptr +="<script>\n";
            ptr +="var MAX = 36000; var MIN = 1;\n";   
            ptr +="timer_light.oninput = function() {timer_light.value -= 0; if(timer_light.value < 0 || timer_light.value > MAX) { timer_light.value = '';}}\n";
            ptr +="timer_light.onblur = function() {timer_light.value -= 0; if(timer_light.value < MIN ) {timer_light.value = MIN;}} \n";
                        
        ptr +="</script> \n";    
 ptr +="</body>\n";
 ptr +="</html>\n";
  
  return ptr;
}


void setTimerLight(char *array){ 
    m_timerLight = ((String)array).toInt();
    m_timerLight *= 10;
    Serial.println(m_timerLight);    
}

void setRangeLightEeprom(int rangeE){ 
    m_rangeLight = rangeE;
}

void setRangeLight(char *array){ 
    m_rangeLight = ((String)array).toInt(); 
    Serial.println(m_rangeLight);
}

void resetStrobCheck(){
  m_chkStrob = false;
  Serial.println((String)m_chkStrob);
}

void setSaveCheck(){
  EEPROM.write(0, m_rangeLight);
  EEPROM.write(1, (int)m_chkStrob);
  EEPROM.write(2, (int)m_colorLight);
  
  EEPROM.commit();
}
//----------------------------------------------------------
String chekedT(){
  return m_chkStrob ? "checked" : "";
}

String colorWhiteChecked(){
  return m_colorLight == WHITEC ? "checked" : "";
}

String colorUkraineChecked(){
  return m_colorLight == UKRAINEC ? "checked" : "";
}

String colorRedChecked(){
  return m_colorLight == REDC ? "checked" : "";
}

String colorGreenChecked(){
  return m_colorLight == GREENC ? "checked" : "";
}

String colorBlueChecked(){
  return m_colorLight == BLUEC ? "checked" : "";
}

String colorPurpleChecked(){
  return m_colorLight == PURPLEC ? "checked" : "";
}

String colorSaladChecked(){
  return m_colorLight == SALADC ? "checked" : "";
}

String colorYellowChecked(){
  return m_colorLight == YELLOWC ? "checked" : "";
}

String colorRightChecked(){
  return m_colorLight == RIGHTC ? "checked" : "";
}

String colorLeftChecked(){
  return m_colorLight == LEFTC ? "checked" : "";
}

String colorOffChecked(){
  return m_colorLight == OFFC ? "checked" : "";
}

//----------------------------------------------------------
void setStrobCheck(){
  m_chkStrob = true;
  Serial.println((String)m_chkStrob);  
}

void resetRadioEvent(){
  m_eventPerfomer = NOTHING;
  Serial.println((String)m_eventPerfomer);  
}

void setEventTimerStart(){
  m_eventPerfomer = TIMER_START;
}

void setEvent(char *event){
  if((String)event == "radio_timer"){
    m_eventPerfomer = TIMER_START;
  }     
}

void resetRadioColor(){
  m_colorLight = WHITEC;  
}

void setColorEeprom(int color){
  m_colorLight = (ColorLight)color;
  setColorShow();
}

void setColorShow(){
  switch (m_colorLight)
  {
    case WHITEC:
      divLight1 = "fff";
      divLight2 = "fff";
    break;
    case UKRAINEC:
      divLight1 = "00f";
      divLight2 = "ff0";
    break;
    case REDC:
      divLight1 = "f00";
      divLight2 = "f00";
    break;
    case GREENC:
      divLight1 = "0f0";
      divLight2 = "0f0";
    break;
    case BLUEC:
      divLight1 = "00f";
      divLight2 = "00f";
    break;
    case PURPLEC:
      divLight1 = "f0f";
      divLight2 = "f0f"; 
    break;
    case SALADC:
      divLight1 = "0ff";
      divLight2 = "0ff";
    break;
    case YELLOWC:
      divLight1 = "ff0";
      divLight2 = "ff0";    
    break;
    case RIGHTC:
      divLight1 = "820";
      divLight2 = "fa0";
    break;
    case LEFTC:
      divLight1 = "fa0";
      divLight2 = "820";
    break;
    case OFFC:
      divLight1 = "000";
      divLight2 = "000";
    break;
  default:
    break;
  }
}

void setColor(char *color){

  if((String)color == "radio_white"){
    m_colorLight = WHITEC; 
  }
  if((String)color == "radio_ukraine"){
    m_colorLight = UKRAINEC;
  }
  if((String)color == "radio_red"){
    m_colorLight = REDC;  
  }
  if((String)color == "radio_green"){
    m_colorLight = GREENC;
  }

  if((String)color == "radio_blue"){
    m_colorLight = BLUEC;  
  }
  if((String)color == "radio_purple"){
    m_colorLight = PURPLEC;
  }
  if((String)color == "radio_salad"){
    m_colorLight = SALADC; 
  }
  if((String)color == "radio_yellow"){
    m_colorLight = YELLOWC;
  } 

  if((String)color == "radio_right"){
    m_colorLight = RIGHTC;
  } 
  if((String)color == "radio_left"){
    m_colorLight = LEFTC;
  } 
  if((String)color == "radio_color_off"){
    m_colorLight = OFFC;
  } 
  setColorShow(); 
    
}

int getTimerLight(){return m_timerLight;}
int getRangeLight(){return m_rangeLight;}
bool getChkStrob(){return m_chkStrob;}
EventPerfomer getEventPerfomer(){return m_eventPerfomer;}
ColorLight getColorLight(){return m_colorLight;}

};

#endif