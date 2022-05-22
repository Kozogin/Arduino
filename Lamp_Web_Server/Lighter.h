#ifndef LIGHTER_H
#define LIGHTER_H

#include "WebControl.h"
#include "ServiceColor24.h"

ServiceColor24 serviceColor24 = ServiceColor24();

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 4
uint8_t colorOrder = 0x52;
Adafruit_NeoPixel *pixels;
int delayval = 2;

enum ProgramFirst{
  STATIC_FIRST, RAIN_FIRST, RAINBOW_FIRST, ON_OFF_FIRST, ON_OFF_COLOR_FIRST, RUN_STRING_FIRST, TRANSFUSION_FIRST, MULTY_FIRST, CHANGE_FIRST
};

class Lighter : public WebControl{

private:    
    int pos;
    int divider;
    int onOff;
	bool onOffDirect;
    ProgramFirst programFirst;
    
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
        //m_chkLamp
        if(m_chkLamp){
            digitalWrite(D4, HIGH);
        } else {
            digitalWrite(D4, LOW);
        }
        programFirst = STATIC_FIRST;     
    }

    void chooseColor(){
        if(m_programPerfomer != RAINBOW || m_programPerfomer != ON_OFF_COLOR || m_programPerfomer != TRANSFUSION 
        || m_programPerfomer != RUN_STRING || m_programPerfomer != MULTY){
            serviceColor24.chooseColor(m_colorLight, m_rangeRed, m_rangeGreen, m_rangeBlue); 
        }
    }

    void fill(){
        if(m_colorLight != UKRAINEC){
            serviceColor24.fillString(serviceColor24.getColor24choose().getR(), serviceColor24.getColor24choose().getG(), serviceColor24.getColor24choose().getB());    
            serviceColor24.cutSector(m_rangeSector);            
        } else {
            serviceColor24.fillStringUkraine(m_rangeSector);           
        }
        serviceColor24.cutPosition(m_rangePosition, false);
    }    

    void chooseProgram(){
        switch (m_programPerfomer)
        {
            case STATICP:
                fill();
                serviceColor24.applyBrightness(m_rangeBrightness, false);            
                programFirst = STATIC_FIRST;                
            break;
            case RAIN:
                if(programFirst != RAIN_FIRST){
                    serviceColor24.rain(serviceColor24.getColor24choose().getR(), serviceColor24.getColor24choose().getG(), serviceColor24.getColor24choose().getB());    
                }
                serviceColor24.shiftLeft();
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                programFirst = RAIN_FIRST;
            break;
            case RAINBOW:
                serviceColor24.changeColorRainbow();
                serviceColor24.cutSector(m_rangeSector);
                serviceColor24.cutPosition(m_rangePosition, false);
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                programFirst = RAINBOW_FIRST; 
            break;
            case ON_OFF:
                serviceColor24.applyBrightness(attenuationOnOff(), false);
                programFirst = ON_OFF_FIRST;
            break;
            case ON_OFF_COLOR:
                serviceColor24.changeColorRainbow();
                serviceColor24.cutSector(m_rangeSector);
                serviceColor24.cutPosition(m_rangePosition, false);
                serviceColor24.applyBrightness(attenuationOnOff(), false);
                programFirst = ON_OFF_COLOR_FIRST;
            break;
            case RUN_STRING:
                if(programFirst != RUN_STRING_FIRST){
                    serviceColor24.runString();
                    serviceColor24.cutSector(m_rangeSector);
                }
                serviceColor24.shiftLeft();
                serviceColor24.applyBrightness(m_rangeBrightness, false);                
                programFirst = RUN_STRING_FIRST;                        
            break;
            case TRANSFUSION:
                serviceColor24.transfusionProgram();                
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                programFirst = TRANSFUSION_FIRST;
            break;
            case MULTY:
                if(programFirst != MULTY_FIRST){
                    serviceColor24.multy();
                    serviceColor24.cutSector(m_rangeSector); 
                }
                serviceColor24.shiftLeft();
                serviceColor24.applyBrightness(m_rangeBrightness, false);
                programFirst = MULTY_FIRST;
            break;
            case CHANGEP:
                if(programFirst != CHANGE_FIRST){
                    chooseColor();
                    fill();                     
                }
                    serviceColor24.shiftLeft();
                    serviceColor24.applyBrightness(m_rangeBrightness, false);
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

        if(serviceColor24.getListBrightness().size() > 0){
            int i = 0;            
            for(Color24 pix : serviceColor24.getListBrightness())
            { 
                pixels -> setPixelColor(i++, pixels -> Color(pix.getR(), pix.getG(), pix.getB()));
                //pixels -> setPixelColor(i++, pixels -> Color(255, 127, 0));
            }
        }
        pixels -> show();
        delay(delayval);

    }
 

};

#endif