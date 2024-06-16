#ifndef LOGIC_RELAY_TELEGRAM_H
#define LOGIC_RELAY_TELEGRAM_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_SENSOR_CONTOUR_2   32
#define TEMP_SENSOR_CONTOUR_3   33
#define TEMP_SENSOR_CONTOUR_4   25
#define TEMP_SENSOR_CONTOUR_5   26
#define TEMP_SENSOR_CONTOUR_6   27
#define TEMP_SENSOR_OUT         14

#define RELAY_PWM_2       19
#define RELAY_PWM_3       18
#define RELAY_PWM_4       5
#define RELAY_PWM_5       17
#define RELAY_PWM_6       16
#define RELAY_PUMP_4      4

#define LED_POOL_RADIATOR_ON 2
#define threshold 30  //62  

#define FIRST_PWM   140
#define DELTA       6
#define RESTART_ALARM_COUNT 10

OneWire oneWireContuor_2(TEMP_SENSOR_CONTOUR_2);
OneWire oneWireContuor_3(TEMP_SENSOR_CONTOUR_3);
OneWire oneWireContuor_4(TEMP_SENSOR_CONTOUR_4);
OneWire oneWireContuor_5(TEMP_SENSOR_CONTOUR_5);
OneWire oneWireContuor_6(TEMP_SENSOR_CONTOUR_6);
OneWire oneWireOut(TEMP_SENSOR_OUT);

DallasTemperature sensorContuor_2(&oneWireContuor_2);
DallasTemperature sensorContuor_3(&oneWireContuor_3);
DallasTemperature sensorContuor_4(&oneWireContuor_4);
DallasTemperature sensorContuor_5(&oneWireContuor_5);
DallasTemperature sensorContuor_6(&oneWireContuor_6);
DallasTemperature sensorOut(&oneWireOut);

class LogicRelayTelegram{

private:

  DataPropertiesEeprom* ptrDataProperties;
  TelegramHandler* ptrTelegramHandler;

  unsigned long timer10;
  unsigned long timer01;

  float m_tempContuora[5] = {22.2, 33.3, 44.44, 55.5, 66.6};

  bool m_isPumpOn;
  bool m_onSendMessage;  

  float m_tempOut = -12.3;  
  
  int m_tStabila[5];  

  int m_pwmServoa[5] = {255, 255, 255, 255, 255};
  int m_pwmParta[5] = {0, 0, 0, 0, 0};
  int m_timeCorrectiona[5] = {0, 0, 0, 0, 0};
  int m_valueCorrectiona[5] = {0, 0, 0, 0, 0};
  float m_tAbsMina[5] = {50, 50, 50, 50, 50};
  float m_tAbsMaxa[5] = {0, 0, 0, 0, 0};  

  bool m_relayPWM[5];

  int countTimer = 0;
  int m_countAlarmMessage;
  int m_restartAlarm;

public:

void setDataPropertiesEeprom(DataPropertiesEeprom *dataPro){
  ptrDataProperties = dataPro;
}

void setTelegramHandler(TelegramHandler *telegr){
  ptrTelegramHandler = telegr;
}

void ticking(){

for(int i = 0; i < 5; i++){

  m_relayPWM[i] = false;
  if(!countTimer){
    if(m_pwmServoa[i]){
      m_relayPWM[i] = false;
      //digitalWrite(RELAY, HIGH);
    } else {
      m_relayPWM[i] = true;
      //digitalWrite(RELAY, LOW);
    }
  }

    if (countTimer >= m_pwmServoa[i] + 1) {
      m_relayPWM[i] = true;
      //digitalWrite(RELAY, LOW);
    }
    
  }

  countTimer++;
  if(countTimer > 255){
    countTimer = 0;    
  }

  moveRelay();  
}

void onOffPoolRadiator(){
  //Serial.println("LogicRelay onOffPoolRadiator()");
  ptrDataProperties->changeOnRadiatorPool();
}

void moveRelay(){

  if(!ptrDataProperties->getTypeServoNO_2()){
    m_relayPWM[2-2] = !m_relayPWM[2-2];
  }
  if(m_relayPWM[2-2]){
    digitalWrite(RELAY_PWM_2, LOW);
  } else {
    digitalWrite(RELAY_PWM_2, HIGH);
  }

  if(!ptrDataProperties->getTypeServoNO_3()){
    m_relayPWM[3-2] = !m_relayPWM[3-2];
  }
  if(m_relayPWM[3-2]){
    digitalWrite(RELAY_PWM_3, LOW);
  } else {
    digitalWrite(RELAY_PWM_3, HIGH);
  }

  if(!ptrDataProperties->getTypeServoNO_4()){
    m_relayPWM[4-2] = !m_relayPWM[4-2];
  }
  if(m_relayPWM[4-2]){
    digitalWrite(RELAY_PWM_4, LOW);
  } else {
    digitalWrite(RELAY_PWM_4, HIGH);
  }

  if(!ptrDataProperties->getTypeServoNO_5()){
    m_relayPWM[5-2] = !m_relayPWM[5-2];
  }
  if(m_relayPWM[5-2]){
    digitalWrite(RELAY_PWM_5, LOW);
  } else {
    digitalWrite(RELAY_PWM_5, HIGH);
  }

  if(!ptrDataProperties->getTypeServoNO_6()){
    m_relayPWM[6-2] = !m_relayPWM[6-2];
  }
  if(m_relayPWM[6-2]){
    digitalWrite(RELAY_PWM_6, LOW);
  } else {
    digitalWrite(RELAY_PWM_6, HIGH);
  }

}

void alarmCheck(){

  bool isSendMessage = false;
  if(m_onSendMessage){
    if(m_tempOut < ptrDataProperties->getOutLower()){
      isSendMessage = true;
    }

    if(m_tempContuora[ptrDataProperties->getNumberContuor() - 2] < ptrDataProperties->getContuorLower()){
      isSendMessage = true;
    }

    if(isSendMessage){
      
      if(m_countAlarmMessage < ptrDataProperties->getCountMessage()){
        ptrTelegramHandler->alarmSend();
      }
      m_countAlarmMessage++;
    }

    if(m_countAlarmMessage){
      m_restartAlarm++;
    }
    if(m_restartAlarm > RESTART_ALARM_COUNT){
      m_countAlarmMessage = 0;
      m_restartAlarm = 0;
    }

  }
}

void onPump() {

  alarmCheck();
  ptrTelegramHandler->restartESP10_00();

  FB_Time t = bot.getTime(ptrDataProperties->getCorrectX());

  int corEndTime = 0;
	bool afterNight = true;	

	if (!ptrDataProperties->getStartTime() && !ptrDataProperties->getEndTime()) {
		afterNight = false;
		m_isPumpOn = false;
	}

	if (afterNight) {
		if (ptrDataProperties->getStartTime() == ptrDataProperties->getEndTime()) {
			afterNight = false;
			m_isPumpOn = true;
		}
	}

	if (afterNight) {
		if (ptrDataProperties->getEndTime() < ptrDataProperties->getStartTime()) {
			corEndTime = ptrDataProperties->getEndTime() + 24;
			if (t.hour < ptrDataProperties->getEndTime()) {
				afterNight = false;
				m_isPumpOn = true;
			}
		}
		else {
			corEndTime = ptrDataProperties->getEndTime();
		}
	}
	
	if (afterNight) {
		if (t.hour >= ptrDataProperties->getStartTime() && t.hour < corEndTime) {
			m_isPumpOn = true;
		}
		else {
			m_isPumpOn = false;
		}
	}

  if(m_isPumpOn){
    digitalWrite(RELAY_PUMP_4, LOW);
  } else {
    digitalWrite(RELAY_PUMP_4, HIGH);
  }
	
}

float getTemperatureSensor(DallasTemperature &sensor){

  float temp;
  sensor.requestTemperatures();
  temp = sensor.getTempCByIndex(0);
  int iter = 0;
  while(temp < -100 && iter < 3){
    sensor.requestTemperatures();
    temp = sensor.getTempCByIndex(0);    
    iter++;
    delay(10);
  } 

  return temp;
}

void readSensor() {

  m_tempContuora[2-2] = getTemperatureSensor(sensorContuor_2);
  m_tempContuora[3-2] = getTemperatureSensor(sensorContuor_3);
  m_tempContuora[4-2] = getTemperatureSensor(sensorContuor_4);
  m_tempContuora[5-2] = getTemperatureSensor(sensorContuor_5);
  m_tempContuora[6-2] = getTemperatureSensor(sensorContuor_6);

  m_tempOut = getTemperatureSensor(sensorOut);  
}

void correctionRestart(int index) {
	m_timeCorrectiona[index] = 0;
	m_tAbsMina[index] = 50;
	m_tAbsMaxa[index] = 0;
}

void getStabilFromEeprom(){

if(ptrDataProperties->getOnRadiatorPool()){
  if(ptrDataProperties->getStatus_2()){
    m_tStabila[2 - 2] = ptrDataProperties->getTempStabil_2();
  } else {
    m_tStabila[2 - 2] = calculateTemteratureCoolant(m_tempOut, ptrDataProperties->getCurve_2());
  }
} else {
  m_tStabila[2 - 2] = 5;
}

  if(ptrDataProperties->getStatus_3()){
    m_tStabila[3 - 2] = ptrDataProperties->getTempStabil_3();
  } else {
    m_tStabila[3 - 2] = calculateTemteratureCoolant(m_tempOut, ptrDataProperties->getCurve_3());
  }

  if(ptrDataProperties->getStatus_4()){
    m_tStabila[4 - 2] = ptrDataProperties->getTempStabil_4();
  } else {
    m_tStabila[4 - 2] = calculateTemteratureCoolant(m_tempOut, ptrDataProperties->getCurve_4());
  }

  if(ptrDataProperties->getStatus_5()){
    m_tStabila[5 - 2] = ptrDataProperties->getTempStabil_5();
  } else {
    m_tStabila[5 - 2] = calculateTemteratureCoolant(m_tempOut, ptrDataProperties->getCurve_5());
  }

  if(ptrDataProperties->getStatus_6()){
    m_tStabila[6 - 2] = ptrDataProperties->getTempStabil_6();
  } else {
    m_tStabila[6 - 2] = calculateTemteratureCoolant(m_tempOut, ptrDataProperties->getCurve_6());
  }
  
}

float calculateTemteratureCoolant(float outTemp, int numberCurve) {
	 
	float result;	
	float k = 1 + (numberCurve - 10.0) * 0.05;
	result = k * (-0.667 * outTemp + 12.5) + 20 + ptrDataProperties->getCorrectA();	

	return result;
}


void correctionPwm() { 

for(int i = 0; i < 5; i++){
	/*min delta fixed min difference temp stabil & sensor
	min value launching max fixed difference & restarting timer
	Its point of transition or approach to tStabil*/
	if (abs(m_tStabila[i] - m_tempContuora[i]) < abs(m_tAbsMina[i])) {
		m_tAbsMina[i] = m_tStabila[i] - m_tempContuora[i];
		m_tAbsMaxa[i] = 0;
		m_timeCorrectiona[i] = 0;
	}

	/*max value delta restarting timer 
	Its point of inconsistency between temp stabil & sensor*/
	if (abs(m_tStabila[i] - m_tempContuora[i]) > abs(m_tAbsMaxa[i])) {
		m_tAbsMaxa[i] = m_tStabila[i] - m_tempContuora[i];
		m_timeCorrectiona[i] = 0;
	}

	/*the timer will allow the temperature to stabilize */
	if (m_timeCorrectiona[i] > 3){
		m_valueCorrectiona[i] += 10 * (m_tStabila[i] - m_tempContuora[i]);

    if (m_pwmParta[i] + m_valueCorrectiona[i] < 0) {
			m_valueCorrectiona[i] = -1 * m_pwmParta[i];
		}
		if (m_pwmParta[i] + m_valueCorrectiona[i] > 255) {
			m_valueCorrectiona[i] = 255 - m_pwmParta[i];
		}        

		correctionRestart(i);
	}

	m_timeCorrectiona[i]++;
  } // for
}


void calculatePwm() {

  getStabilFromEeprom();

  for(int i = 0; i < 5; i++){
    if(abs(m_tStabila[i] - m_tempContuora[i]) > DELTA){
      if(m_tStabila[i] > m_tempContuora[i]){
        m_pwmServoa[i] = 255;
      } else {
        m_pwmServoa[i] = 0;
      }
      m_valueCorrectiona[i] = 0;
      correctionRestart(i);
    }

    if(abs(m_tStabila[i] - m_tempContuora[i]) <= DELTA){
      m_pwmParta[i] = FIRST_PWM + 30 * (m_tStabila[i] - m_tempContuora[i]);  
      
      m_pwmServoa[i] = m_pwmParta[i] + m_valueCorrectiona[i];  

      if (m_pwmParta[i] > 255) {
        m_pwmParta[i] = 255;
      }
      if (m_pwmParta[i] < 0) {
        m_pwmParta[i] = 0;
      }

      correctionPwm();
    }
  }	
}

   
void logicRelay(){

  if(millis() - timer01 > 35){
    ticking();
    timer01 = millis();
  } 

  if(millis() - timer10 > 10000){
    readSensor();
    calculatePwm();  
    timer10 = millis(); 
  }  
}


bool *getSendMessage(){
  return &m_onSendMessage;
}

bool *getIsPumpOn(){
  return &m_isPumpOn;
}

float *getPtrTempContuor_2(){
  return &m_tempContuora[2-2];
}
float *getPtrTempContuor_3(){
  return &m_tempContuora[3-2];
}
float *getPtrTempContuor_4(){
  return &m_tempContuora[4-2];
}
float *getPtrTempContuor_5(){
  return &m_tempContuora[5-2];
}
float *getPtrTempContuor_6(){
  return &m_tempContuora[6-2];
}
float *getPtrTempOut(){
  return &m_tempOut;
}

int *getPtrStabilChange(){
  return m_tStabila;
}
  

};


#endif