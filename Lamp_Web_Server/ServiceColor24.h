#ifndef SERVICE_COLOR_24_H
#define SERVICE_COLOR_24_H

#include <list>
#include "Color24.h"

const int NUM_PIXEL = 40;

class ServiceColor24{
   private:
	std::list<Color24> m_colorString;
	std::list<Color24> m_colorStringBrightness;
	Color24 color24S = Color24();
	Color24 red;
	Color24 green;
	Color24 blue;
	Color24 yellow;

public:
	ServiceColor24() {
		m_colorString.resize(NUM_PIXEL, Color24(0, 0, 0));		
	}	

	void moveRight(Color24 color) {
		m_colorString.push_front(color);
		m_colorString.pop_back();
	}

	void moveLeft(Color24 color) {
		m_colorString.push_back(color);
		m_colorString.pop_front();
	}

	void fillString(Color24 color) {
		m_colorString.assign(NUM_PIXEL, color);		
	}

    void fillStringUkraine(int sector) {
		m_colorString.clear();
		m_colorString.resize(NUM_PIXEL * sector / 720, Color24(0, 0, 255));
		m_colorString.resize(NUM_PIXEL * sector / 360, Color24(255, 255, 0));
		m_colorString.resize(NUM_PIXEL, Color24(0, 0, 0));
	}

	void cutSector(int sector) {
		m_colorString.resize(NUM_PIXEL * sector / 360, Color24(0, 0, 0));
		m_colorString.resize(NUM_PIXEL, Color24(0, 0, 0));
	}

	void cutPosition(int position, bool reverse) {
		int step = NUM_PIXEL * position / 360;
		for (int i = 0; i < step; i++)
		{
			
			if (reverse) {
				Color24 color = m_colorString.front();
				moveLeft(color);
			}
			else {
				Color24 color = m_colorString.back();
				moveRight(color);
			}
		}
	}

	void applyBrightness(int brightness, bool reverse) {
		m_colorStringBrightness.clear();
		for (Color24 color : m_colorString)
		{
			Color24 colorBrightness(0, 0, 0);
			if (reverse) {
				m_colorStringBrightness.push_front(colorBrightness.setBrightnessC(color, brightness));
			}
			else {
				m_colorStringBrightness.push_back(colorBrightness.setBrightnessC(color, brightness));
			}
		}
	}

	void rain(Color24 color) {
		getNewColor24(color, 100);
		getNewColor24(color, 80);
		getNewColor24(color, 60);
		getNewColor24(color, 45);
		getNewColor24(color, 30);
		getNewColor24(color, 10);
	}

	void runString(){
		m_colorString.clear();
		for (size_t i = 0; i < 8; i++)
		{
			getNewColor24(Color24(255, 0, 0), 100);
			getNewColor24(Color24(0, 255, 0), 100);
			getNewColor24(Color24(0, 0, 255), 100);
			getNewColor24(Color24(255, 255, 0), 100);
			getNewColor24(Color24(0, 0, 0), 100);
		}		
	}

	void getNewColor24(Color24 color, int attenuation) {
		Color24 colorNew(0, 0, 0);		
		colorNew = colorNew.setBrightnessC(color, attenuation);
		moveRight(colorNew);
	}	

	void transfusionProgram() {
		
		red.transfusionProgram(0);
		green.transfusionProgram(1);
		blue.transfusionProgram(2);
		yellow.transfusionProgram(3);

		m_colorString.clear();

		for (size_t i = 0; i < NUM_PIXEL / 4 ; i++)
		{
			m_colorString.push_front(red);
			m_colorString.push_front(green);
			m_colorString.push_front(blue);
			m_colorString.push_front(yellow);
		}
	}

	void multy() {

		m_colorString.clear();
		for (size_t k = 0; k < NUM_PIXEL; k++)
		{
			for (size_t i = 0; i < 755 / NUM_PIXEL; i++)
			{
				changeColorRainbow();
			}
			m_colorString.push_front(color24S);
		}		
	}

	void chooseColor(ColorLight color, int r, int g, int b){
		color24S.chooseColor(color, r, g, b);
	}

	void changeColorRainbow(){
		color24S.changeColorRainbow();
	}	

	Color24 getColor24(){ return color24S;}
	std::list<Color24> getList() { return m_colorString;}
	std::list<Color24> getListBrightness() { return m_colorStringBrightness;}
};

#endif