#ifndef HANDLER_MESSAGE_H
#define HANDLER_MESSAGE_H

#include <iostream>

const int FRAGMENT_NUMBER = 20;

enum TypeMessage {
	CALL, SMS, INFO, OK_TYPE
};

class HandlerMessage {
private:
	const int COMMA = 0x2C;
	const int ENTER = 0x0A;
	const int SPACE = 0x20;
	const int DOUBLE_POINT = 0x3A;

	std::string m_kod;
	std::string m_telephone;
	std::string m_telephoneSave;
	std::string m_message;
	std::string m_messageSave;
	int m_indexMessage;
	TypeMessage m_typeMessage;
	std::string m_fragment[FRAGMENT_NUMBER];

	bool m_noCarrier;
	int m_ring;
	int m_sizeMessage;
	int m_indexEnter;
	int m_indexUnsolicitedMessage;

public:
	HandlerMessage();
	void resetAll();

private:

	void smsEnd();

	void callEnd();	

	void resetArr();

	bool pass(std::string part);

	void parseKod();

	void parseTelephone();

	void determineTypeMessage();

	void parseSmsMessage();

	bool findSub(std::string word, std::string part);

public:

	void split(std::string message);

	void setHandlerMessage();

	void printObjMessage();

	bool checkTelephone(std::string telephone);

	void setIndexUnsolicitedMessage(int indexUnsolicitedMessage){
		m_indexUnsolicitedMessage = indexUnsolicitedMessage;
	}	

	std::string getTelephone() { return m_telephoneSave; }
	std::string getMessage() { return m_message; }
	std::string getMessageSave() { return m_messageSave; }
	TypeMessage getTypeMessage() { return m_typeMessage; }
	bool getNoCarrier() { return m_noCarrier; }
	int getRing() { return (int)m_ring; }
	int getIndexUnsolicitedMessage() { return (int)m_indexUnsolicitedMessage; }
	int getSizeMessage() { return (int)m_sizeMessage; }	
	void getFragment(std::string *arr);

};

#endif