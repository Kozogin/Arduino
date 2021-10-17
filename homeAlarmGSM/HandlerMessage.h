#ifndef HANDLER_MESSAGE_H
#define HANDLER_MESSAGE_H

#include <iostream>

const int FRAGMENT_NUMBER = 20;

enum TypeMessage {
	CALL, SMS, INFO
};

class HandlerMessage {
private:
	const uint8_t COMMA = 0x2C;
	const uint8_t ENTER = 0x0A;
	const uint8_t SPACE = 0x20;
	const uint8_t DOUBLE_POINT = 0x3A;

	std::string m_kod;
	std::string m_telephone;
	std::string m_message;
	uint8_t m_indexMessage;
	TypeMessage m_typeMessage;
	std::string m_fragment[FRAGMENT_NUMBER];

	bool m_noCarrier;
	uint8_t m_ring;
	uint8_t m_sizeMessage;

public:
	HandlerMessage();

private:

	void smsEnd();

	void callEnd();

	void resetAll();

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

	std::string getTelephone() { return m_telephone; }
	std::string getMessage() { return m_message; }
	TypeMessage getTypeMessage() { return m_typeMessage; }
	bool getNoCarrier() { return m_noCarrier; }
	int getRing() { return (int)m_ring; }
	int getSizeMessage() { return (int)m_sizeMessage; }
	std::string* getFragment() { return m_fragment; }

};

#endif