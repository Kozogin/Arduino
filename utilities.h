#include "DateTime.h"

#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26


#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

#define  RELE_SIREN 0
#define  RELE_USER 14
#define  ALARM_PIN 15
#define  SENSOR 13

#define  ZONA_1 35
#define  ZONA_2 32
#define  ZONA_3 2
#define  ZONA_4 33
#define  ZONA_5 34

DateTime dateTime = DateTime();

#define  EEPROM_ON_ALARM 225
#define  EEPROM_ON_RNDRELE 226