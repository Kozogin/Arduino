#ifndef LIGHTER_H
#define LIGHTER_H

#include "WebControl.h"
#include "ServiceColor24.h"

ServiceColor24 serviceColor24 = ServiceColor24();

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 5
//int numPixel = 6;
uint8_t colorOrder = 0x52;
Adafruit_NeoPixel *pixels;
int delayval = 5; 

enum ProgramFirst{
  STATIC_FIRST, RAIN_FIRST, RAINBOW_FIRST, ON_OFF_FIRST, ON_OFF_COLOR_FIRST, RUN_STRING_FIRST, TRANSFUSION_FIRST, MULTY_FIRST, CHANGE_FIRST
};

class Lighter : public WebControl{

private:    
    int pos;
    int divider;
    ProgramFirst programFirst;
    int onOff;
	bool onOffDirect; 
    
public:
    Lighter(){}

    void timeDraining(){
        if(divider > m_rangeSpeed){
            chooseProgram();
            lightString();
            divider = 0;
        }
        divider++;
    }

    void perfomer(){        
        chooseColor();
        chooseProgram();
        lightString();      
    }

    void chooseColor(){       
        
        serviceColor24.chooseColor(m_colorLight, m_rangeRed, m_rangeGreen, m_rangeBlue);
        
        if(m_colorLight != UKRAINEC){
            serviceColor24.fillString(serviceColor24.getColor24());
            serviceColor24.cutSector(m_rangeSector);            
        } else {
            serviceColor24.fillStringUkraine(m_rangeSector);           
        }
        serviceColor24.cutPosition(m_rangePosition, false);
        serviceColor24.applyBrightness(m_rangeBrightness, false);
    }

    void chooseProgram(){
        switch (m_programPerfomer)
        {
            case STATICP:              
                programFirst = STATIC_FIRST;
            break;
            case RAIN:
                if(programFirst == RAIN_FIRST){
                    serviceColor24.rain(Color24(serviceColor24.getColor24()));
                }
                serviceColor24.cutPosition(pos++, false);
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                pos %= 360;
                programFirst = RAIN_FIRST;

            break;
            case RAINBOW:
                serviceColor24.changeColorRainbow();
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                programFirst = RAINBOW_FIRST;
            break;
            case ON_OFF:
                serviceColor24.applyBrightness(attenuationOnOff(), false);
                programFirst = ON_OFF_FIRST;
            break;
            case ON_OFF_COLOR:
                serviceColor24.changeColorRainbow();
                serviceColor24.applyBrightness(attenuationOnOff(), false);
                programFirst = ON_OFF_COLOR_FIRST;        
            break;
            case RUN_STRING:
                if(programFirst == RUN_STRING_FIRST){
                    serviceColor24.runString();
                }
                serviceColor24.cutPosition(pos++, false);
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                pos %= 360;
                programFirst = RUN_STRING_FIRST;            
            break;
            case TRANSFUSION:
                serviceColor24.transfusionProgram();
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                programFirst = TRANSFUSION_FIRST;          
            break;
            case MULTY:
                serviceColor24.multy();
                serviceColor24.cutPosition(pos++, false);
                serviceColor24.applyBrightness(m_rangeBrightness, false); 
                programFirst = MULTY_FIRST;         
            break;
            case CHANGEP:
                serviceColor24.cutPosition(pos++, false);
                programFirst = CHANGE_FIRST;          
            break;                       
            default:
            break;            
        }
     }

   
	int attenuationOnOff() {

		if (onOffDirect) {
			onOff++;
		}
		else {
			onOff--;
		}		
		
		if (onOff < 1) {
			onOffDirect = true;
			onOff = 0;
		}
		if (onOff > 99) {
			onOffDirect = false;
			onOff = 100;
		}
		return onOff * m_rangeBrightness / 100;
	}
    

    void lightString(){

        int i;
        for(Color24 pix : serviceColor24.getListBrightness())
        {
            pixels -> setPixelColor(i++, pixels -> Color(pix.getR(), pix.getG(), pix.getB()));
        }
        pixels -> show();
        delay(delayval);
    }
 

};

#endif