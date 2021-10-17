#include <iostream>
#include "HandlerMessage.h"

	HandlerMessage::HandlerMessage() {
		resetAll();
	}

	void HandlerMessage::smsEnd() {
		m_message = "";
		m_indexMessage = 0;
	}

	void HandlerMessage::callEnd() {
		m_noCarrier = false;
		m_ring = 0;
	}

	void HandlerMessage::resetAll() {
		m_kod = "";
		m_telephone = "";
		m_message = "";
		m_indexMessage = 0;
		m_typeMessage = INFO;
		resetArr();

		m_noCarrier = false;
		m_ring = 0;
		m_sizeMessage = 0;
	}

	void HandlerMessage::resetArr() {
		for (uint8_t i = 0; i < FRAGMENT_NUMBER; i++)
		{
			m_fragment[i] = "";
		}
	}

	bool HandlerMessage::pass(std::string part) {

		bool exist = false;
		std::string frag;
		for (uint8_t i = 0; i < FRAGMENT_NUMBER; i++)
		{
			frag = m_fragment[i];
			if (findSub(frag, part)) {
				exist = true;
				break;
			}
		}
		return exist;
	}

	void HandlerMessage::parseKod() {

		m_kod = "";

		if (pass("RING")) {
			m_kod = "RING";
			m_ring++;
		}		

		if (pass("CIEV")) {
			m_kod = "+CIEV";
		}

		if (pass("CLIP")) {
			m_kod = "+CLIP";
		}

		if (pass("CMGL")) {
			m_kod = "+CMGL";
		}

		if (pass("NO CARRIER")) {
			m_kod = "NO CARRIER";
			m_noCarrier = true;
		}

	}

	void HandlerMessage::parseTelephone() {

		m_telephone = "";
		std::string frag;
		bool digitOnly = true;
		for (uint8_t i = 1; i < FRAGMENT_NUMBER; i++)
		{
			if (i != m_indexMessage) {
				frag = m_fragment[i];
			}
			else {
				frag = "";
			}

			if (frag.length() > 11) {
				if (frag.substr(0, 1) == "+") {
					for (uint8_t letter = 1; letter < frag.length(); letter++)
					{
						int asciiLetter = static_cast<std::int8_t>(frag[letter]);
						if (asciiLetter < 48 || asciiLetter > 57) {
							digitOnly = false;
						}
					}
					if (digitOnly) {
						m_telephone = frag;
					}
				}
			}
		}
	}

	void HandlerMessage::determineTypeMessage() {
		m_typeMessage = INFO;

		if (m_kod == "RING" || m_kod == "+CLIP" || m_kod == "NO CARRIER") {
			m_typeMessage = CALL;
			smsEnd();
		}
		else if (m_kod == "+CMGL") {
			m_typeMessage = SMS;
			callEnd();
		}
		else {
			m_typeMessage = INFO;
			callEnd();
			smsEnd();
		}
	}

	void HandlerMessage::parseSmsMessage() {

		std::string frag;
		std::uint8_t index = 0;
		if (m_typeMessage == SMS) {
			for (uint8_t i = 0; i < FRAGMENT_NUMBER; i++)
			{
				frag = m_fragment[i];
				if (findSub(frag, "/")) {
					index = i;
				}
			}
			//if (findSub(m_fragment[index + 5], "OK")) {
				m_indexMessage = index + 4;
				m_message = m_fragment[m_indexMessage];
			//}
		}
	}

	bool HandlerMessage::findSub(std::string word, std::string part) {
		bool exist = false;
		for (uint8_t i = 0; i < word.length(); i++)
		{
			if (word.substr(i, part.length()) == part) {
				exist = true;
			}
		}
		return exist;
	}

	void HandlerMessage::split(std::string message) {

		int8_t index = -1;
		m_sizeMessage = 0;
		std::string fragment;

		resetArr();

		while (message.length() > ++index)
		{
			if (message[index] == COMMA || message[index] == ENTER || message[index] == DOUBLE_POINT)
			{
				m_fragment[m_sizeMessage] = fragment;
				if (fragment == "" || fragment == " " || fragment == "\n") {
				}
				else {
					m_sizeMessage++;
				}
				fragment = "";
			}
			else if (message[index] == '"') {
			}
			else
			{
				fragment += message[index];
				if (fragment == " ") {
					fragment = "";
				}
			}

			if (message.length() - 1 == index)
			{
				m_fragment[m_sizeMessage] = fragment;
				m_sizeMessage++;
				fragment = "";
			}
		}
	}

	void HandlerMessage::setHandlerMessage() {
		
		parseKod();
		determineTypeMessage();
		parseSmsMessage();
		parseTelephone();
		
		printObjMessage();
	}

	void HandlerMessage::printObjMessage() {
		std::cout << "obj kod          " << m_kod << "\n";
		std::cout << "obj telephone    " << m_telephone << "\n";
		std::cout << "obj message      " << m_message << "\n";
		std::cout << "obj indexMessage " << (int)m_indexMessage << "\n";

		std::cout << "obj typeMessage  " << m_typeMessage << "\n";
		std::cout << "obj noCarrier    " << m_noCarrier << "\n";
		std::cout << "obj ring         " << (int)m_ring << "\n";
		std::cout << "obj sizeMessage  " << (int)m_sizeMessage << "\n";

		std::cout << "obj array  " << m_sizeMessage << "\n";
		for (uint8_t i = 0; i < FRAGMENT_NUMBER; i++)
		{
			std::cout << i << "- " << m_fragment[i] << ", ";
		}
		std::cout << "\n";
	}