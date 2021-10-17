#include <Arduino.h>
#include "utilities.h"
#include "GsmService.h"

GsmService gsmService = GsmService();

//HandlerMessage mes = HandlerMessage();

void setupModem()
{
#ifdef MODEM_RST
    // Keep reset high
    pinMode(MODEM_RST, OUTPUT);
    digitalWrite(MODEM_RST, HIGH);
#endif

    pinMode(MODEM_PWRKEY, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);

    // Turn on the Modem power first
    digitalWrite(MODEM_POWER_ON, HIGH);

    // Pull down PWRKEY for more than 1 second according to manual requirements
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);

    // Initialize the indicator as an output
    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LED_ON);
}

void setup()
{    
    SerialMon.begin(115200);
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    setupModem();
    //---------------------------------------------------------------

    SerialMon.println("AT");
    delay(500);
    SerialMon.println("AT+CMGF=1");
    delay(500);
    SerialMon.println("AT+CMGS=\"+380676759301\"\r");
    delay(500);
    SerialMon.print("Hello World from Dev Craze!001");
    delay(500);
    SerialMon.write(26);

    SerialAT.println("AT");
    delay(500);
    SerialAT.println("AT+CMGF=1");
    delay(500);
    SerialAT.write(26);
    delay(500);

    

    //----------------------------------------------------------------
}

void loop()
{
    gsmService.runService();

}