#ifndef WEB_CONTROL_H
#define WEB_CONTROL_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

enum ColorLight{
  WHITEC, BLACKC, UKRAINEC, REDC, GREENC, BLUEC, PURPLEC, SALADC, YELLOWC, CHOOSEC, OFFC
};

enum ProgramPerfomer{
  STATICP, RAIN, RAINBOW, ON_OFF, ON_OFF_COLOR, RUN_STRING, TRANSFUSION, MULTY, CHANGEP
};

class WebControl{

protected:    
  int m_rangeBrightness;
  int m_rangeSector;
  int m_rangePosition;
  bool m_chkLamp;
  bool m_chkSave;
  int m_rangeRed;
  int m_rangeGreen;
  int m_rangeBlue;
  int m_rangeSpeed;  
    
  ColorLight m_colorLight;
  ProgramPerfomer m_programPerfomer; 
  int m_intRefhesh; 

public:

WebControl(){
  m_colorLight = WHITEC;
  m_programPerfomer = STATICP;
  m_chkSave = false;
  int m_intRefhesh = 0;
}

String getPage(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<html lang=\"en\">\n";
  ptr +="<head>\n";
    ptr +="<meta charset=\"UTF-8\">\n";
    ptr +="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
    ptr +="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
   
    ptr +="<title>Web lamp</title>\n";
    ptr +="<style>\n";
        ptr +="body{margin-top: 20px;background-color: #333;}\n";
        ptr +="table {color: #888;margin: 10px auto 30px; text-align: center;}\n";
        ptr +="td {width: 80px; text-align:  center;}\n";
        ptr +="h1 {color: #888; margin: 0 50px;}\n";
        ptr +=".post {display: block;width: 90px;background-color: #992121;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
       
    ptr +="</style>\n";
ptr +="</head>\n";
ptr +="<body>\n";
    ptr +="<div style='margin: auto;'>\n";
        ptr +="<form action='/' id='colorForm' method='POST'>\n";
        ptr +="<table>\n";
            ptr +="<tr>\n";   
                ptr +="<th><h1>Lamp web control</h1></th>\n";                 
            ptr +="</tr>\n";

            ptr +="<tr><td><div>\n";
                    ptr +="<input type='radio' id='lampoff' name='lampp' value='lampoff' " + lampStateOffChecked() + ">\n";
                    ptr +="<label for='lampp'>LAMP OFF</label>\n";
                  ptr +="</div></td></tr>\n";
            ptr +="<tr><td><div>\n";
                    ptr +="<input type='radio' id='lamponn' name='lampp' value='lampon' " + lampStateOnChecked() + ">\n";
                    ptr +="<label for='lampp'>LAMP ON</label>\n";
                  ptr +="</div></td></tr> \n";

            ptr +="<tr>\n";
                ptr +="<td><h3>brightness</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><input type='range' id='range_brightness' name='range_brightness' min='0' max='100' step='1' value='" + (String)m_rangeBrightness + "'></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>sector</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><input type='range' id='range_sector' name='range_sector' min='0' max='360' step='1' value='" + (String)m_rangeSector + "'></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>position</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><input type='range' id='range_position' name='range_position' min='0' max='360' step='1' value='" + (String)m_rangePosition + "'></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";          
                ptr +="<td><INPUT TYPE='submit' class='post' VALUE='OK'></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";   
                ptr +="<th><h2>choose color</h2></th>\n";                 
            ptr +="</tr>\n";
        ptr +="</table>\n";

        ptr +="<table>\n";           
            ptr +="<tr>\n";   
                ptr +="<td><h3 style='color: #fff;'>white</h3></td>\n";
                ptr +="<td><h3 style='color: #000;'>black</h3></td>\n"; 
                ptr +="<td><h3 style='color: #00f;'>Ukr<span style='color: #ff0;'>aine</span></h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td><input type='radio' id='radio_white' name='rdo_color' value='radio_white' " + colorWhiteChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_black' name='rdo_color' value='radio_black' " + colorBlackChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_ukraine' name='rdo_color' value='radio_ukraine' " + colorUkraineChecked() + "></td>\n";
            ptr +="</tr>\n"; 
            ptr +="<tr><td></td></tr>\n";
            ptr +="<tr><td></td></tr>\n";
            ptr +="<tr><td></td></tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td><h3 style='color: #f00;'>red</h3></td>\n";
                ptr +="<td><h3 style='color: #0f0;'>green</h3></td>\n"; 
                ptr +="<td><h3 style='color: #00f;'>blue</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td><input type='radio' id='radio_red' name='rdo_color' value='radio_red' " + colorRedChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_green' name='rdo_color' value='radio_green' " + colorGreenChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_blue' name='rdo_color' value='radio_blue' " + colorBlueChecked() + "></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr><td></td></tr>\n";
            ptr +="<tr><td></td></tr>\n";
            ptr +="<tr><td></td></tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td><h3 style='color: #90f;'>purple</h3></td>\n";
                ptr +="<td><h3 style='color: #0ff;'>salad</h3></td>\n"; 
                ptr +="<td><h3 style='color: #ff0;'>yellow</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td><input type='radio' id='radio_purple' name='rdo_color' value='radio_purple' " + colorPurpleChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_salad' name='rdo_color' value='radio_salad' " + colorSaladChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_yellow' name='rdo_color' value='radio_yellow' " + colorYellowChecked() + "></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td></td>\n";
                ptr +="<td><h3>choose</h3></td>\n"; 
                ptr +="<td></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";   
                ptr +="<td></td>\n";
                ptr +="<td><input type='radio' id='radio_choose' name='rdo_color' value='radio_choose' " + colorChooseChecked() + "></td>\n";
                ptr +="<td></td>\n";
            ptr +="</tr>\n";
        ptr +="</table>\n";

        ptr +="<table>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>red channel</h3></td>\n";
                ptr +="<td><div style='background-color: #f00;'><input type='range' id='range_red' name='range_red' min='0' max='255' step='1' value='" + (String)m_rangeRed + "'></div></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>green channel</h3></td>\n";
                ptr +="<td><div style='background-color: #0f0;'><input type='range' id='range_green' name='range_green' min='0' max='255' step='1' value='" + (String)m_rangeGreen + "'></div></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>blue channel</h3></td>\n";
                ptr +="<td><div style='background-color: #00f;'><input type='range' id='range_blue' name='range_blue' min='0' max='255' step='1' value='" + (String)m_rangeBlue + "'></div></td>\n";
            ptr +="</tr> \n";           
        ptr +="</table>\n";

        ptr +="<table>\n";
            ptr +="<tr><td><h2>choose program</h2></td></tr>\n";
            ptr +="<tr><td><h3>speed</h3></td></tr>\n";
            ptr +="<tr><td><input type='range' id='range_speed' name='range_speed' min='1' max='20' step='1' value='" + (String)m_rangeSpeed + "'></td></tr>\n";
            ptr +="<tr> <td><INPUT TYPE='submit' class='post' VALUE='OK'></td></tr>\n";

            ptr +="<tr><td><div><input type='checkbox' id='saves' name='saves'>\n";
              ptr +="<label for='saves'>Save</label></div></td></tr>\n";             
        ptr +="</table>\n";

        ptr +="<table>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>static</h3></td>\n";
                ptr +="<td><h3>rain</h3></td>\n";
                ptr +="<td><h3>rainbow</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><input type='radio' id='radio_static' name='rdo_program' value='radio_static' " + programStaticChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_rain' name='rdo_program' value='radio_rain' " + programRainChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_rainbow' name='rdo_program' value='radio_rainbow' " + programRainbowChecked() + "></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>on_off</h3></td>\n";
                ptr +="<td><h3>on_off_color</h3></td>\n";
                ptr +="<td><h3>run string</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><input type='radio' id='radio_on_off' name='rdo_program' value='radio_on_off' " + programOnOffChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_on_off_color' name='rdo_program' value='radio_on_off_color' " + programOnOffColorChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_run_string' name='rdo_program' value='radio_run_string' " + programRunStringChecked() + "></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><h3>transfusion</h3></td>\n";
                ptr +="<td><h3>multy</h3></td>\n";
                ptr +="<td><h3>change</h3></td>\n";
            ptr +="</tr>\n";
            ptr +="<tr>\n";
                ptr +="<td><input type='radio' id='radio_transfusion' name='rdo_program' value='radio_transfusion' " + programTransfusionChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_multy_run' name='rdo_program' value='radio_multy_run' " + programMultyChecked() + "></td>\n";
                ptr +="<td><input type='radio' id='radio_change_color' name='rdo_program' value='radio_change_color' " + programChangeChecked() + "></td>\n";
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
//-----------------------------------------------------
String colorWhiteChecked(){
  return m_colorLight == WHITEC ? "checked" : "";
}

String colorBlackChecked(){
  return m_colorLight == BLACKC ? "checked" : "";
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

String colorChooseChecked(){
  return m_colorLight == CHOOSEC ? "checked" : "";
}
//-----------------------------------------------------
String lampStateOffChecked(){
  return m_chkLamp ? "" : "checked";
}

String lampStateOnChecked(){
  return m_chkLamp ? "checked" : "";
}
//-----------------------------------------------------
String programStaticChecked(){
  return m_programPerfomer == STATICP ? "checked" : "";
}
String programRainChecked(){
  return m_programPerfomer == RAIN ? "checked" : "";
}
String programRainbowChecked(){
  return m_programPerfomer == RAINBOW ? "checked" : "";
}

String programOnOffChecked(){
  return m_programPerfomer == ON_OFF ? "checked" : "";
}
String programOnOffColorChecked(){
  return m_programPerfomer == ON_OFF_COLOR ? "checked" : "";
}
String programRunStringChecked(){
  return m_programPerfomer == RUN_STRING ? "checked" : "";
}

String programTransfusionChecked(){
  return m_programPerfomer == TRANSFUSION ? "checked" : "";
}
String programMultyChecked(){
  return m_programPerfomer == MULTY ? "checked" : "";
}
String programChangeChecked(){
  return m_programPerfomer == CHANGEP ? "checked" : "";
}
//-----------------------------------------------------

void setRangeBrightness(char *array){ 
  m_rangeBrightness = ((String)array).toInt(); 
  Serial.println(m_rangeBrightness);
}

void setRangeSector(char *array){ 
  m_rangeSector = ((String)array).toInt(); 
  Serial.println(m_rangeSector);
}

void setRangePosition(char *array){ 
  m_rangePosition = ((String)array).toInt(); 
  Serial.println(m_rangePosition);
}
//----------------------------------------------------

void setLampState(char *state){

  if((String)state == "lampoff"){
    m_chkLamp = false; 
  }
  if((String)state == "lampon"){
    m_chkLamp = true; 
  }

  Serial.println("setLampRadio");
  Serial.println(m_chkLamp);
}

void resetSaveCheck(){
  m_chkSave = false;
  Serial.println("void resetSaveCheck(){");
}

void setSaveCheck(){
  m_chkSave = true;
  saveToEeprom();
  Serial.println("void setSaveCheck() commit(){");
}
//----------------------------------------------------
void setColor(char *color){

  if((String)color == "radio_white"){
    m_colorLight = WHITEC;
    Serial.println("radio_white"); 
  }
  if((String)color == "radio_black"){
    m_colorLight = BLACKC; 
    Serial.println("radio_black"); 
  }
  if((String)color == "radio_ukraine"){
    m_colorLight = UKRAINEC;
    Serial.println("radio_ukraine"); 
  }

  if((String)color == "radio_red"){
    m_colorLight = REDC;
    Serial.println("radio_red");   
  }
  if((String)color == "radio_green"){
    m_colorLight = GREENC;
    Serial.println("radio_green"); 
  }
  if((String)color == "radio_blue"){
    m_colorLight = BLUEC;
    Serial.println("radio_blue");   
  }

  if((String)color == "radio_purple"){
    m_colorLight = PURPLEC;
    Serial.println("radio_purple"); 
  }
  if((String)color == "radio_salad"){
    m_colorLight = SALADC;
    Serial.println("radio_salad"); 
  }
  if((String)color == "radio_yellow"){
    m_colorLight = YELLOWC;
    Serial.println("radio_yellow"); 
  } 

  if((String)color == "radio_choose"){
    m_colorLight = CHOOSEC;
    Serial.println("radio_choose"); 
  } 
    
}

//----------------------------------------------------
void setRangeRed(char *array){ 
  m_rangeRed = ((String)array).toInt(); 
  Serial.println(m_rangeRed);
}

void setRangeGreen(char *array){ 
  m_rangeGreen = ((String)array).toInt(); 
  Serial.println(m_rangeGreen);
}

void setRangeBlue(char *array){ 
  m_rangeBlue = ((String)array).toInt(); 
  Serial.println(m_rangeBlue);
}

void setRangeSpeed(char *array){ 
  m_rangeSpeed = ((String)array).toInt(); 
  Serial.println(m_rangeSpeed);
}

void setProgram(char *program){

  if((String)program == "radio_static"){
    m_programPerfomer = STATICP;
    Serial.println((String)program); 
  }  
  if((String)program == "radio_rain"){
    m_programPerfomer = RAIN;
    Serial.println((String)program); 
  }  
  if((String)program == "radio_rainbow"){
    m_programPerfomer = RAINBOW;
    Serial.println((String)program); 
  }

  if((String)program == "radio_on_off"){
    m_programPerfomer = ON_OFF;
    Serial.println((String)program); 
  }  
  if((String)program == "radio_on_off_color"){
    m_programPerfomer = ON_OFF_COLOR;
    Serial.println((String)program); 
  }  
  if((String)program == "radio_run_string"){
    m_programPerfomer = RUN_STRING;
    Serial.println((String)program); 
  }

  if((String)program == "radio_transfusion"){
    m_programPerfomer = TRANSFUSION;
    Serial.println((String)program); 
  }  
  if((String)program == "radio_multy_run"){
    m_programPerfomer = MULTY;
    Serial.println((String)program); 
  }  
  if((String)program == "radio_change_color"){
    m_programPerfomer = CHANGEP;
    Serial.println((String)program); 
  }   
    
}

void saveToEeprom(){
  // Serial.println("saveToEeprom()"); 
  // Serial.print("m_rangeBrightness  ");
  // Serial.println(m_rangeBrightness);
  // Serial.print("m_rangeSector  ");
  // Serial.println(m_rangeSector);
  // Serial.print("m_rangeSector  ");
  // Serial.println(m_rangePosition);
  // Serial.print("m_rangeRed  ");
  // Serial.println(m_rangeRed);
  // Serial.print("m_rangeGreen  ");
  // Serial.println(m_rangeGreen);
  // Serial.print("m_rangeBlue  ");
  // Serial.println(m_rangeBlue);
  // Serial.print("m_rangeSpeed  ");
  // Serial.println(m_rangeSpeed);

  Serial.print("m_chkLamp  ");
  Serial.println(m_chkLamp);
  Serial.print("m_colorLight  ");
  Serial.println(m_colorLight);
  Serial.print("m_programPerfomer  ");
  Serial.println(m_programPerfomer); 

  EEPROM.write(1, m_rangeBrightness);
  EEPROM.write(12, m_rangeSector);
  EEPROM.write(2, m_rangePosition);
  EEPROM.write(3, m_rangeRed);
  EEPROM.write(4, m_rangeGreen);
  EEPROM.write(5, m_rangeBlue);
  EEPROM.write(6, m_rangeSpeed);

  if(m_chkLamp){
    EEPROM.write(7, 255);
  } else {
    EEPROM.write(7, 0);
  }

  EEPROM.write(8, (int)m_colorLight);
  EEPROM.write(9, (int)m_programPerfomer);

  EEPROM.commit();
} 

void readFromEeprom(){

  m_rangeBrightness = (int)EEPROM.read(1);
  m_rangeSector = (int)EEPROM.read(12);
  m_rangePosition = (int)EEPROM.read(2);
  m_rangeRed = (int)EEPROM.read(3);
  m_rangeGreen = (int)EEPROM.read(4);
  m_rangeBlue = (int)EEPROM.read(5);
  m_rangeSpeed = (int)EEPROM.read(6);
 
  if((int)EEPROM.read(7) > 31){
    m_chkLamp = true;
  } else {
    m_chkLamp = false;
  }

  m_colorLight = (ColorLight)EEPROM.read(8);
  m_programPerfomer = (ProgramPerfomer)EEPROM.read(9);

  // Serial.println("readToEeprom()"); 
  // Serial.print("m_rangeBrightness  ");
  // Serial.println(m_rangeBrightness);
  // Serial.print("m_rangeSector  ");
  // Serial.println(m_rangeSector);
  // Serial.print("m_rangePosition  ");
  // Serial.println(m_rangePosition);
  // Serial.print("m_rangeRed  ");
  // Serial.println(m_rangeRed);
  // Serial.print("m_rangeGreen  ");
  // Serial.println(m_rangeGreen);
  // Serial.print("m_rangeBlue  ");
  // Serial.println(m_rangeBlue);
  // Serial.print("m_rangeSpeed  ");
  // Serial.println(m_rangeSpeed);

  // Serial.print("m_chkLamp  ");
  // Serial.println(m_chkLamp);
  // Serial.print("m_colorLight  ");
  // Serial.println(m_colorLight);
  // Serial.print("m_programPerfomer  ");
  // Serial.println(m_programPerfomer);  

}

int getRangeBrightness(){return m_rangeBrightness;}
int getRangeSector(){return m_rangeSector;}
int getRangePosition(){return m_rangePosition;}
//bool getChkLamp(){{return m_chkLamp;}
//bool getChkSave(){{return m_chkSave;}
int getRangeRed(){return m_rangeRed;}
int getRangeGreen(){return m_rangeGreen;}
int getRangeBlue(){return m_rangeBlue;}
int getRangeSpeed(){return m_rangeSpeed;}

ColorLight getColorR(){return m_colorLight;}
ProgramPerfomer getProgram(){return m_programPerfomer;}

};

#endif