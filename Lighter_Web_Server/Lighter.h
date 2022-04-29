#ifndef LIGHTER_H
#define LIGHTER_H

#include "WebControl.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 5
int numPixel = 6;
uint8_t colorOrder = 0x52;
Adafruit_NeoPixel *pixels;
int delayval = 5; 

class Lighter : public WebControl{

private:
    bool c_strob;
    bool c_rightLeftAllow;
    bool c_waveBool;
    int c_waveInt;
    
public:
    Lighter(){}

    void timeDraining(){
        if(m_eventPerfomer == TIMER_START){
            m_timerLight -= 5;
            if(m_timerLight <= 0){
                m_timerLight = 0;
                if(m_colorLight == RIGHTC){
                    if (!c_rightLeftAllow)
                    {
                        lightRight();
                    }                    
                    
                } else if (m_colorLight == LEFTC){
                    if (!c_rightLeftAllow)
                    {
                        lightLeft();
                    }
                                       
                } else {
                    lightStatic();
                }
            }
        }

        if(m_timerLight > 0 || m_eventPerfomer != TIMER_START){
            lightOff();
        }    
            
    }

    void lightStatic(){
        if(m_chkStrob){
            c_strob = !c_strob;
        } else {
            c_strob = true;            
        }
            
        if(c_strob){
            lightOn();
        } else {
            lightOff();
        }
        
    }

    void lightOn(){

        int r = 0;
        int g = 0;
        int b = 0;
        switch (m_colorLight)
        {
            case WHITEC:
                r = 255;
                g = 255;
                b = 255;
                lightMono(r, g, b);
            break;
            case UKRAINEC:
                lightUkraine();
            break;
            case REDC:
                r = 255;
                lightMono(r, g, b);
            break;
            case GREENC:
                g = 255;
                lightMono(r, g, b);
            break;

            case BLUEC:
                b = 255;
                lightMono(r, g, b);
            break;
            case PURPLEC:
                r = 139;                
                b = 255;
                lightMono(r, g, b);
            break;
            case SALADC:
                g = 255;
                b = 255;
                lightMono(r, g, b);
                digitalWrite(0, HIGH); 
                digitalWrite(2, LOW);
            break;
            case YELLOWC:
                r = 255;
                g = 255;
                lightMono(r, g, b);
            break;
            case OFFC:                
                lightOff();
            break;
        default:
            break;            
        }        
    }

    void lightMono(int r, int g, int b){
        for(int i = 0; i < numPixel; i++){
            pixels -> setPixelColor(i, pixels -> Color(r * m_rangeLight / 100, g * m_rangeLight / 100, b * m_rangeLight / 100));
        }
        pixels -> show();
        delay(delayval);
    }

    void lightUkraine(){
        c_waveInt++;
        if(m_chkStrob){
            c_waveBool = !c_waveBool; 
        } else {
            if(c_waveInt % 4 == 0){
                c_waveBool = !c_waveBool;
            }
        }     
              
        if(c_waveBool){
            for(int i = 0; i < numPixel / 2; i++){
                pixels -> setPixelColor(i, pixels -> Color(0, 0, 255 * m_rangeLight / 100));            
            }
            for(int i = numPixel / 2; i < numPixel; i++){
                pixels -> setPixelColor(i, pixels -> Color(255 * m_rangeLight / 100, 255 * m_rangeLight / 100, 0));
            }
        } else {
            for(int i = 0; i < numPixel / 2 - 1; i++){
                pixels -> setPixelColor(i, pixels -> Color(0, 0, 255 * m_rangeLight / 100));            
            }
            for(int i = numPixel / 2 - 1; i < numPixel; i++){
                pixels -> setPixelColor(i, pixels -> Color(255 * m_rangeLight / 100, 255 * m_rangeLight / 100, 0));
            }
        }
        pixels -> show();
        delay(delayval);
    }

    void lightRight(){
        c_rightLeftAllow = true;
        for (int j = 1; j < numPixel + 1; j++) {
		    for (int i = 0; i < j; i++) {
                pixels -> setPixelColor(i, pixels -> Color(255 * m_rangeLight / 100, 100 * m_rangeLight / 100, 0));
            }
            pixels -> show();
            delay(20 * delayval);
        }
        lightOff();
        delay(500);
        c_rightLeftAllow = false;
    }

    void lightLeft(){
        c_rightLeftAllow = true;
        for (int j = numPixel; j > 0; j--) {
            bool lightBool[8] = {};

            for (int i = numPixel - 1; i > j - 2; i--) {                
                lightBool[i] = true;			
            }
            
            for (int i = 0; i < numPixel; i++)
            {
                if (lightBool[i])
                {
                    pixels -> setPixelColor(i, pixels -> Color(255 * m_rangeLight / 100, 100 * m_rangeLight / 100, 0));
                } else {
                    pixels -> setPixelColor(i, pixels -> Color(0, 0, 0));
                }
            }
            pixels -> show();
            delay(20 * delayval);
        }
        lightOff();
        delay(500);                    
        c_rightLeftAllow = false;
    }

    void lightOff(){
        for(int i = 0; i < numPixel; i++){
            pixels -> setPixelColor(i, pixels -> Color(0, 0, 0));
        }
        pixels -> show();
        delay(delayval);        
    }

};

#endif