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
		try{
			for (int i = 0; i < FRAGMENT_NUMBER; i++)
			{
				m_fragment[i] = "";
			}
		}
		catch (...)
		{
			std::cout << "HandlerMessage::resetArr()    --------***********------------***********     " << "\n";
		}	
	}

	bool HandlerMessage::pass(std::string part) {

		bool exist = false;
		std::string frag = "";
		try{
			for (int i = 0; i < FRAGMENT_NUMBER; i++)
			{
				frag = m_fragment[i];
				if (findSub(frag, part)) {
					exist = true;
					break;
				}
			}
		}
		catch (...)
		{
			std::cout << "HandlerMessage::pass()    --------***********------------***********     " << "\n";
		}
		return exist;
	}

	void HandlerMessage::parseKod() {

		m_kod = "";

		try{
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
				m_indexUnsolicitedMessage = 127;
			}

			if (pass("CCLK")) {
				m_kod = "+CCLK";
			}

			if (pass("NO ANSWER")) {
				m_kod = "NO ANSWER";
				m_indexUnsolicitedMessage = 127;
			}

			if (pass("BUSY")) {
				m_kod = "BUSY";
				m_indexUnsolicitedMessage = 127;
			}

			if (pass("NO DIALTONE")) {
				m_kod = "NO DIALTONE";
			}
			
			if (pass("COLP")) {
				m_kod = "+COLP";
				m_indexUnsolicitedMessage = 191;
			}

			if (pass("CMGS")) {
				m_kod = "+CMGS";
				m_indexUnsolicitedMessage -= 50;
			}
			
			if (pass("CUSD")) {
				m_kod = "+CUSD";
			}

			// if (pass("OK")) {
			// 	m_kod = "OK";
			// }
		}	
		catch (...)
		{
			std::cout << "HandlerMessage::parseKod()    --------***********------------***********     " << "\n";
		}	

	}

	bool HandlerMessage::checkTelephone(std::string telephone){

		bool digitOnly = false;
		try{
			digitOnly = true;
			if (telephone.length() > 11) {
				if (telephone.substr(0, 1) == "+") {
					for (int letter = 1; letter < telephone.length(); letter++)
					{
						int asciiLetter = static_cast<int>(telephone[letter]);
						if (asciiLetter < 48 || asciiLetter > 57) {
							digitOnly = false;
						}
					}						
				}
			} else{
				digitOnly = false;
			}
		}
		catch (...)
		{
			std::cout << "HandlerMessage::checkTelephone()    --------***********------------***********     " << "\n";
		}
		return digitOnly;
	}

	void HandlerMessage::parseTelephone() {

		m_telephone = "";
		std::string frag = "";
		bool digitOnly = true;

		try{
			for (int i = 1; i < FRAGMENT_NUMBER; i++)
			{
				if (i != m_indexMessage) {
					frag = m_fragment[i];
				}
				else {
					frag = "";
				}

				if (frag.length() > 11) {
					if (frag.substr(0, 1) == "+") {
						for (int letter = 1; letter < frag.length(); letter++)
						{
							int asciiLetter = static_cast<int>(frag[letter]);
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
			if(m_telephone != ""){
				m_telephoneSave = m_telephone;
			}
		}
		catch (...)
		{
			std::cout << "HandlerMessage::parseTelephone()    --------***********------------***********     " << "\n";
		}

	}

	void HandlerMessage::determineTypeMessage() {
		m_typeMessage = INFO;

		try{

			if(m_kod == "RING" || m_kod == "+CLIP" || m_kod == "NO CARRIER"
					|| m_kod == "NO ANSWER" || m_kod == "BUSY" || m_kod == "+COLP" || m_kod == "NO DIALTONE") {
				m_typeMessage = CALL;
				smsEnd();
			}
			else if (m_kod == "+CMGL" || m_kod == "+CMGS") {
				m_typeMessage = SMS;
				callEnd();
			}
			else if (m_kod == "OK") {
				m_typeMessage = OK_TYPE;
			}
			else {
				m_typeMessage = INFO;
				callEnd();
				smsEnd();
			}
		}
		catch (...)
		{
			std::cout << "HandlerMessage::determineTypeMessage()    --------***********------------***********     " << "\n";
		}
	}

	void HandlerMessage::parseSmsMessage() {
		
		try
		{	
			if(m_indexEnter + 1 < FRAGMENT_NUMBER){
				m_message = m_fragment[m_indexEnter + 1];
			}

			if(m_indexEnter + 4 < FRAGMENT_NUMBER){
				
				if ((m_fragment[m_indexEnter + 1] != "" 
					&& (int)m_fragment[m_indexEnter + 2].at(0) > 31 
					&& m_fragment[m_indexEnter + 2].length() == 1) 
					|| m_fragment[m_indexEnter + 2].length() > 1) {
					
					m_message = m_message.append(",");
					m_message = m_message.append(m_fragment[m_indexEnter + 2]);
					m_message = m_message.append(":");
					m_message = m_message.append(m_fragment[m_indexEnter + 3]);
					m_message = m_message.append(":");
					m_message = m_message.append(m_fragment[m_indexEnter + 4]);	
				}
			}
			
			if(m_kod == "+CUSD"){
				for (int i = 0; i < FRAGMENT_NUMBER; i++){
					if(m_fragment[i].find(std::string("Na Vashomu")) != -1){
						m_message = m_fragment[i];
						m_messageSave = m_message;
					}
				}
			}
			
		}
		catch (...)
		{
			std::cout << "HandlerMessage::parseSmsMessage()    --------***********------------***********     " << "\n";
		}		
	}

	bool HandlerMessage::findSub(std::string word, std::string part) {
		bool exist = false;
		try{
			for (int i = 0; i < word.length(); i++)
			{
				if (word.substr(i, part.length()) == part) {
					exist = true;
				}
			}
		}
		catch (...)
		{
			std::cout << "HandlerMessage::findSub()    --------***********------------***********     " << "\n";
		}
		return exist;
	}

	void HandlerMessage::split(std::string message) {

		m_indexEnter = 0;
		int index = -1;
		m_sizeMessage = 0;
		std::string fragment = "";
		
		try{	

			resetArr();

			while (message.length() >= ++index)
			{
				if (message[index] == ENTER && m_indexEnter == 0) {
					m_indexEnter = m_sizeMessage;				
				}
				
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
		catch (...)
		{
			std::cout << "HandlerMessage::split()    --------***********------------***********     " << "\n";
		}
	}

	void HandlerMessage::setHandlerMessage() {
		
		try{
			parseKod();
			determineTypeMessage();
			parseSmsMessage();
			parseTelephone();
			
			//printObjMessage();
		}
		catch (...)
		{
			std::cout << "HandlerMessage::setHandlerMessage()    --------***********------------***********     " << "\n";
		}
	}

	void HandlerMessage::printObjMessage() {
		try{
			std::cout << "obj kod          " << m_kod << "\n";
			std::cout << "obj telephone    " << m_telephone << "\n";
			std::cout << "obj message      " << m_message << "\n";
			std::cout << "obj indexMessage " << m_indexMessage << "\n";

			std::cout << "obj typeMessage  " << m_typeMessage << "\n";
			std::cout << "obj noCarrier    " << m_noCarrier << "\n";
			std::cout << "obj ring         " << m_ring << "\n";
			std::cout << "obj sizeMessage  " << m_sizeMessage << "\n";

			std::cout << "obj array  " << m_sizeMessage << "\n";
			for (int i = 0; i < FRAGMENT_NUMBER; i++)
			{
				std::cout << i << "- " << m_fragment[i] << ", ";
			}
			std::cout << "\n";
		}
		catch (...)
		{
			std::cout << "HandlerMessage::printObjMessage()    --------***********------------***********     " << "\n";
		}
	}

	void HandlerMessage::getFragment(std::string *arr) {
		try
		{
			for (int i = 0; i < 5; i++)
			{
				arr[i] = m_fragment[i];
			}
		}
		catch (...) {
			std::cout << "HandlerMessage::getFragment(std::string *arr)    --------***********------------***********     " << "\n";
		}
	} 