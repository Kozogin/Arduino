#ifndef SERVICE_COLOR_24_H
#define SERVICE_COLOR_24_H

#include <list>
#include "Color24.h"


const int NUM_PIXEL = 40;

class ServiceColor24 {
private:
	std::list<Color24> m_colorString;
	std::list<Color24> m_colorStringBrightness;
	//--------------------------------------------
	Color24 color24s;
	Color24 color24choose;
	Color24 red;
	Color24 green;
	Color24 blue;
	Color24 yellow;

public:
	ServiceColor24() {
		color24s = Color24();
		color24choose = Color24();
		red = Color24(255, 0, 0);
		green = Color24(0, 255, 0);
		blue = Color24(0, 0, 255);
		yellow = Color24(255, 255, 0);			
	}	

	void fillString(int r, int g, int b) {
		
		m_colorString.clear();
		for (size_t i = 0; i < NUM_PIXEL; i++)
		{
			color24s.setColorC(r, g, b);
			m_colorString.push_front(color24s);
		}		
	}

	void fillStringUkraine(int sector) {
		m_colorString.clear();
		for (size_t i = 0; i < NUM_PIXEL; i++)
		{
			if (i < NUM_PIXEL * sector / 720) {
				color24s.setColorC(0, 0, 255);
			}
			if (i >= NUM_PIXEL * sector / 720 && i < NUM_PIXEL * sector / 360) {
				color24s.setColorC(255, 255, 0);
			}
			if (i >= NUM_PIXEL * sector / 360) {
				color24s.setColorC(0, 0, 0);
			}
			m_colorString.push_back(color24s);
		}		
	}

	void rain(int r, int g, int b) {
		
		m_colorString.clear();
		color24s.setColorC(r, g, b);
		m_colorString.push_back(color24s);
		color24s.setColorC(0.6 * r, 0.6 * g, 0.6 * b);
		m_colorString.push_back(color24s);
		color24s.setColorC(0.4 * r, 0.4 * g, 0.4 * b);
		m_colorString.push_back(color24s);
		color24s.setColorC(0.3 * r, 0.3 * g, 0.3 * b);
		m_colorString.push_back(color24s);
		color24s.setColorC(0.2 * r, 0.2 * g, 0.2 * b);
		m_colorString.push_back(color24s);
		color24s.setColorC(0, 0, 0);
		m_colorString.resize(NUM_PIXEL, color24s);		
	}

	void runString() {
		m_colorString.clear();		
		
		for (size_t i = 0; i < NUM_PIXEL / 6; i++)
		{
			color24s.setColorC(255, 0, 0);
			m_colorString.push_back(color24s);
			color24s.setColorC(0, 255, 0);
			m_colorString.push_back(color24s);
			color24s.setColorC(0, 0, 255);
			m_colorString.push_back(color24s);
			color24s.setColorC(255, 255, 0);
			m_colorString.push_back(color24s);
			color24s.setColorC(0, 0, 0);
			m_colorString.push_back(color24s);
			color24s.setColorC(0, 0, 0);
			m_colorString.push_back(color24s);
		}
		color24s.setColorC(0, 0, 0);
		m_colorString.resize(NUM_PIXEL, color24s);
	}

	void multy() {

		m_colorString.clear();
		for (size_t k = 0; k < NUM_PIXEL; k++)
		{
			for (size_t i = 0; i < 755 / NUM_PIXEL; i++)
			{
				color24choose.changeColorRainbow();
			}
			color24choose.copyColor(color24choose);
			m_colorString.push_front(color24choose);
		}
	}

	void transfusionProgram() {
		
		red.transfusionProgram(0);
		green.transfusionProgram(1);
		blue.transfusionProgram(2);
		yellow.transfusionProgram(3);

		m_colorString.clear();

		for (size_t i = 0; i < NUM_PIXEL / 4 + 1; i++)
		{
			m_colorString.push_front(red);
			m_colorString.push_front(green);
			m_colorString.push_front(blue);
			m_colorString.push_front(yellow);
		}
		color24s.setColorC(0, 0, 0);
		m_colorString.resize(NUM_PIXEL);
	}	
	
	void cutSector(int sector) {
		m_colorString.resize(NUM_PIXEL * sector / 360);
		color24s.setColorC(0, 0, 0);
		m_colorString.resize(NUM_PIXEL, color24s);	
	}

	void cutPosition(int position, bool reverse) {
		int step = NUM_PIXEL * position / 360;
		for (int i = 0; i < step; i++)
		{
			
			if (reverse) {
				color24s.copyColor(m_colorString.front());
				m_colorString.push_back(color24s);
				m_colorString.pop_front();
			}
			else {
				color24s.copyColor(m_colorString.back());
				m_colorString.push_front(color24s);
				m_colorString.pop_back();
			}
		}
	}

	void shiftRight(){
		color24s.copyColor(m_colorString.back());
		m_colorString.push_front(color24s);
		m_colorString.pop_back();

	}

	void shiftLeft(){
		color24s.copyColor(m_colorString.front());
		m_colorString.push_back(color24s);
		m_colorString.pop_front();
	}

	void applyBrightness(int brightness, bool reverse) {
		
		m_colorStringBrightness.clear();
		int i = 0;
		for (Color24 color : m_colorString)
		{
			color24s.setBrightnessC(color, brightness);
			m_colorStringBrightness.push_front(color24s);			
			i++;
		}		
	}

	void chooseColor(ColorLight color, int r, int g, int b){
		color24choose.chooseColor(color, r, g, b);
	}

	void changeColorRainbow(){
		color24choose.changeColorRainbow();
		fillString(color24choose.getR(), color24choose.getG(), color24choose.getB()); 
	}	
	
	Color24 getColor24(){ return color24s;}
	Color24 getColor24choose(){ return color24choose;}
	std::list<Color24> getList() { return m_colorString;}
	std::list<Color24> getListBrightness() { return m_colorStringBrightness; }
};


#endif