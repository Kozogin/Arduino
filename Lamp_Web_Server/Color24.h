#ifndef COLOR_24_H
#define COLOR_24_H

class Color24 {
private:
	int m_r;
	int m_g;
	int m_b;
	int m_rotate;
	int m_stepBrighness;

public:
	Color24() {
		m_r = 0;
		m_g = 0;
		m_b = 0;
		m_rotate = 0;
		m_stepBrighness = 0;
	}
	
	Color24(int r, int g, int b) {
		m_r = r;
		m_g = g;
		m_b = b;
		m_rotate = 0;
		m_stepBrighness = 0;
	}	

	void chooseColor(ColorLight color, int r, int g, int b){

        m_r = 0;
        m_g = 0;
        m_b = 0;
        switch (color)
        {
            case WHITEC:
                m_r = 255;
                m_g = 255;
                m_b = 255;                
            break;
            case BLACKC:                
            break;
            case UKRAINEC:                
            break;
            case REDC:
                m_r = 255;                
            break;
            case GREENC:
                m_g = 255;                
            break;
            case BLUEC:
                m_b = 255;                
            break;
            case PURPLEC:
                m_r = 139;                
                m_b = 255;                
            break;
            case SALADC:
                m_g = 255;
                m_b = 255;                
            break;
            case YELLOWC:
                m_r = 255;
                m_g = 255;                
            break;
            case CHOOSEC:
                m_r = r;
                m_g = g;
                m_b = b;                
            break;            
            default:
            break;     
        }        
    }

	void changeColorRainbow() {
		if (m_r == 0 || m_g == 0 || m_b == 0) {
		}
		else {
			m_b = 0;
		}
		if (m_r == 0 && m_g == 0 && m_b == 0) {
			m_r = 255;
		}

		if (m_b == 0) {
			m_r -= 1;
			if (m_r < 0) {
				m_r = 0;
			}
			m_g += 1;
			if (m_g > 255) {
				m_g = 255;
			}
		}

		if (m_r == 0) {
			m_g -= 1;
			if (m_g < 0) {
				m_g = 0;
			}
			m_b += 1;
			if (m_b > 255) {
				m_b = 255;
			}
		}

		if (m_g == 0) {
			m_b -= 1;
			if (m_b < 0) {
				m_b = 0;
			}
			m_r += 1;
			if (m_r > 255) {
				m_r = 255;
			}
		}

		if (m_r + m_g + m_b > 300) {
			m_g = 0;
		}
	}

	void transfusionProgram(int numLed) {
		m_r = 0;
		m_g = 0;
		m_b = 0;

		if (m_rotate == 0) {
			if (numLed == 0) {
				m_r = 255 - m_stepBrighness;
			}
			if (numLed == 1) {
				m_g = m_stepBrighness;
			}
		}

		if (m_rotate == 1) {
			if (numLed == 1) {
				m_g = 255 - m_stepBrighness;
			}
			if (numLed == 2) {
				m_b = m_stepBrighness;
			}
		}

		if (m_rotate == 2) {
			if (numLed == 2) {
				m_b = 255 - m_stepBrighness;
			}
			if (numLed == 3) {
				m_r = m_stepBrighness;
				m_g = m_stepBrighness;
			}
		}

		if (m_rotate == 3) {
			if (numLed == 3) {
				m_r = 255 - m_stepBrighness;
				m_g = 255 - m_stepBrighness;
			}
			if (numLed == 0) {
				m_r = m_stepBrighness;
			}
		}

		m_stepBrighness++;

		if (m_stepBrighness > 255) {
			m_stepBrighness = 0;
			m_rotate++;
			m_rotate %= 4;
		}

	}		

	void setColorC(int r, int g, int b) {
		m_r = r;
		m_g = g;
		m_b = b;
	}

	Color24 setBrightnessC(Color24 color, int brightness) {
		m_r = brightness * color.m_r / 100;
		m_g = brightness * color.m_g / 100;
		m_b = brightness * color.m_b / 100;
		return Color24(m_r, m_g, m_b);
	}

	//Color24  getColor24(){ return ;}
	int getR() { return m_r; }
	int getG() { return m_g; }
	int getB() { return m_b; }
	
};

#endif