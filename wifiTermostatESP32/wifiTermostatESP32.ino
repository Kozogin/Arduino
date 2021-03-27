#define BLYNK_PRINT Serial
#include "esp_system.h" //dwt
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>

#define touthtime 50
#define threshold 25

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(25);
DallasTemperature ds(&oneWire);
RTC_DS3231 rtc;
SimpleTimer timer2;

const int loopTimeCtl = 0;
hw_timer_t *timer = NULL;

int counttouch, pin, numset = 1, intwoodstart = 0;
int wifi_restart;
float tstreet, troomin, t;
int tboilroom, twood1, twood2, tgazboil;
float floatTwood1, floatTwood2;
volatile long lastTurn = 0, stawdt, staeeprom, sta = 0, sta1, sta2, sta3;

bool permissionGazBlynk = true;

bool touchMdetected = false;
bool touchEdetected = false;
bool touchMidetected = false;
bool touchPldetected = false;
bool touchEntdetected = false;

bool menuactualy = false, menuactualy0 = false, menuactualy1 = false, menuactualy2 = false;
bool enteractualy = false;
bool releasebotton = false;
bool espactualy = false, espactualy1 = false;
bool setactualyTH = false;
bool changactualyTH = false, changactualyTH1 = false;
bool saveyes = false;

bool boreletermostar = false, borelepump1 = false, borelepump2 = false;
bool bowoodstart = false, postBowoodstart = false;
char letter;
byte sensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int onlyone = 0, cukl = 0;
int signpm, signbotton;
byte sensornumber = 0;
bool inici = false;
int numwifi, rotateword;
int postTimePump1, postBowoodstartFinish;

int g_hour = 0, g_minut = 20;
int g_date = 22, g_mou = 9;
int g_yea = 2019;
byte dayofweek = 3;
int tempsettingjob, tempsettingweek, corecttemp, tempsetting; //sweeth reles
int whichsettingjob, whichsettingweek, whichjob = 0, whichweek = 0;
int kodascii = 48, k, kmemory, touthauto;
String Stringauth, Stringssid, Stringpass;

float twood2one, twood2two, twood2three;
int attempt1, attempt12, attempt2;

const char *auth = "";
const char *ssid = "";
const char *pass = "";
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Numers //батьківський
{
protected:
  int m_nn;
  String m_label;

public:
  Numers(int nn, String label)
      : m_nn(nn), m_label(label)
  {
  }

  void setnumber(int nn)
  {
    m_nn = nn;
  }

  void lcdMenu()
  {
    lcd.setCursor(0, 0);
    lcd.print("  Menu  ");
    lcd.setCursor(7, 0);
    lcd.print(m_nn);
    lcd.print("/");
    lcd.print("36     ");
    if (enteractualy)
    {
      lcd.setCursor(13, 0);
      lcd.print("<>");
    }
  }

  int getnn() const { return m_nn; }
  String getlabel() const { return m_label; }
};

class TempSensors : public Numers
{
private:
  byte m_sensor[8];
  int m_actual;
  int m_corectsens;

public:
  TempSensors(int nn, String label)
      : Numers(nn, label)
  {
    for (int i = 0; i < 8; ++i)
    {
      m_sensor[i] = 0;
      m_actual = 0;
      m_corectsens = 0;
    }
  }

  void eepromwriteSens()
  {
    byte corect;
    EEPROM.write(193 - 22 + m_nn, m_actual);
    corect = m_corectsens + 100;
    EEPROM.write(201 - 22 + m_nn, corect);
  }

  void eepromreadSens()
  {
    byte corect;
    m_actual = EEPROM.read(193 - 22 + m_nn);
    corect = EEPROM.read(201 - 22 + m_nn);
    m_corectsens = corect - 100;
  }

  void changeTools(int sign)
  {
    m_actual += sign;
    if (m_actual > 7)
    {
      m_actual = 7;
    }
    if (m_actual < 0)
    {
      m_actual = 0;
    }
    signpm = 0;
  }

  void changeCorect(int sign)
  {
    m_corectsens += sign;
    if (m_corectsens > 100)
    {
      m_corectsens = 100;
    }
    if (m_corectsens < -100)
    {
      m_corectsens = -100;
    }
    signpm = 0;
  }

  void lcdMenu()
  {
    Numers::lcdMenu();

    lcd.setCursor(0, 1);
    lcd.print(m_label);
    lcd.setCursor(7, 1);
    lcd.print(m_actual);
    lcd.setCursor(10, 1);
    lcd.print(t + (float)m_corectsens / 10);
    lcd.print(" ");
  }

  void lcdMenuCorect()
  {
    lcd.setCursor(0, 0);
    lcd.print("  Menu  ");
    lcd.setCursor(7, 0);
    lcd.print(m_nn - 11);
    lcd.print("/");
    lcd.print("36     ");
    if (enteractualy)
    {
      lcd.setCursor(13, 0);
      lcd.print("<>");
    }

    lcd.setCursor(0, 1);
    lcd.print(m_label);
    lcd.setCursor(7, 1);
    lcd.print((float)m_corectsens / 10);
    lcd.print("  ");
  }

  void getaddres()
  {
    for (byte bi = 0; bi < 8; ++bi)
    {
      m_sensor[bi] = EEPROM.read(8 * m_actual + bi);
    }
  }

  void printA()
  {
    Serial.println();
    Serial.print(m_nn);
    Serial.print("  ");
    Serial.print(m_label);
    Serial.print("  ");
    for (byte bi = 0; bi < 8; ++bi)
    {
      Serial.print(" ");
      Serial.print(m_sensor[bi], HEX);
    }
    Serial.println();
  }

  int getcorectsens() const { return m_corectsens; }
  void getsensor() const
  {
    for (int i = 0; i < 8; ++i)
    {
      sensor[i] = m_sensor[i];
    }
  }
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TempHour : public Numers
{
private:
  int m_hour;
  int m_minut;
  int m_temp;

public:
  TempHour(int nn, int hourA, int minut, int temp, String label)
      : Numers(nn, label),
        m_hour(hourA), m_minut(minut), m_temp(temp)
  {
  }

  void resetA()
  {
    m_hour = 0;
    m_minut = 0;
    m_temp = 0;
  }

  void eepromwriteTempHour()
  {
    EEPROM.write(210 - 1 + 0 + 3 * m_nn, m_hour);
    EEPROM.write(210 - 1 + 1 + 3 * m_nn, m_minut);
    EEPROM.write(210 - 1 + 2 + 3 * m_nn, m_temp - 45);
  }

  void eepromreadTempHour()
  {
    m_hour = EEPROM.read(210 - 1 + 0 + 3 * m_nn);
    m_minut = EEPROM.read(210 - 1 + 1 + 3 * m_nn);
    m_temp = EEPROM.read(210 - 1 + 2 + 3 * m_nn) + 45;
  }

  bool notequallyTempHour()
  {
    if (m_hour != EEPROM.read(210 - 1 + 0 + 3 * m_nn))
    {
      return true;
    }
    if (m_minut != EEPROM.read(210 - 1 + 1 + 3 * m_nn))
    {
      return true;
    }
    if (m_temp != EEPROM.read(210 - 1 + 2 + 3 * m_nn) + 45)
    {
      return true;
    }
    return false;
  }

  void printA() const
  {
    Serial.print(m_nn);
    Serial.print("  ");
    Serial.print(m_hour);
    Serial.print(":");
    Serial.print(m_minut);
    Serial.print(" - ");
    Serial.print(((float)(m_temp)) / 10);
    Serial.println();
  }

  void settimeA(int hourA, int minut)
  {
    m_hour = hourA;
    m_minut = minut;
  }

  void changeTH()
  {
    if (changactualyTH && !changactualyTH1)
    {
      m_minut += 10 * signbotton;
      if (m_minut > 59)
      {
        m_minut = 0;
        ++m_hour;
        if (m_hour > 23)
        {
          m_hour = 0;
        }
      }
      if (m_minut < 0)
      {
        m_minut = 50;
        --m_hour;
        if (m_hour < 0)
        {
          m_hour = 23;
        }
      }
    }
    if (changactualyTH && changactualyTH1)
    {
      m_temp += signbotton;
      if (m_temp > 300)
      {
        m_temp = 300;
      }
      if (m_temp < 50)
      {
        m_temp = 50;
      }
    }
  }

  void lcdTempHour()
  {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    lcd.setCursor(0, 0);
    lcd.print(m_nn);
    lcd.setCursor(3, 0);
    lcd.print(m_label);
    if (m_nn != 15)
    {
      if (changactualyTH)
      {
        lcd.print("  <>");
      }

      if (m_hour > 9)
      {
        lcd.setCursor(0, 1);
      }
      else
      {
        lcd.setCursor(1, 1);
      }
      lcd.print(m_hour);
      lcd.print(":");
      if (m_minut > 9)
      {
        lcd.print(m_minut);
      }
      else
      {
        lcd.print("0");
        lcd.print(m_minut);
      }
      if (changactualyTH && !changactualyTH1)
      {
        lcd.print(" <>");
      }

      lcd.setCursor(9, 1);
      lcd.print((float)m_temp / 10, 1);
      if (changactualyTH && changactualyTH1)
      {
        lcd.print(" <>");
      }
    }
    if (m_nn == 15 && changactualyTH)
    {

      setactualyTH = false;
      changactualyTH = false;
      changactualyTH1 = true;
      enteractualy = false;
      espactualy = false;
      menuactualy = false;
      numset = 1;
    }
  }

  int gethour() const { return m_hour; }
  int getminut() const { return m_minut; }
  int gettemp() const { return m_temp; }

  friend bool operator>(const TempHour &d1, const TempHour &d2);
  friend bool operator<=(const TempHour &d1, const TempHour &d2);

  friend bool operator<(const TempHour &d1, const TempHour &d2);
  friend bool operator>=(const TempHour &d1, const TempHour &d2); //--------
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class WifiToken : public Numers
{
private:
  String m_actualname;

public:
  WifiToken(int nn, String actualname, String label)
      : Numers(nn, label),
        m_actualname(actualname)
  {
  }

  void setnamelogin(String actualname)
  {
    m_actualname = actualname;
  }

  void typingnamelogin()
  {
    letter = ' ';
    switch (signbotton)
    {
    case 1:
      ++kodascii;
      if (kodascii > 126)
      {
        kodascii = 32;
      }
      break;
    case -1:
      --kodascii;
      if (kodascii < 32)
      {
        kodascii = 126;
      }
      break;
    }
    letter = (char)(kodascii);
    Serial.print(letter);

    if (enteractualy)
    {
      m_actualname += letter;
      enteractualy = false;
    }
    if (espactualy)
    {
      m_actualname = m_actualname.substring(0, m_actualname.length() - 1);
      espactualy = false;
    }
  }

  void writewifi()
  {
    byte simbolwifi;
    for (byte numsimbol = 0; numsimbol < m_actualname.length(); ++numsimbol)
    {
      simbolwifi = m_actualname[numsimbol];
      EEPROM.write(66 + 40 * (m_nn - 18) + numsimbol, simbolwifi);
    }
    EEPROM.write(105 + 40 * (m_nn - 18), m_actualname.length());
    //EEPROM.commit();
  }

  void readwifi()
  {
    int lengthWiFi = EEPROM.read(105 + 40 * (m_nn - 18));
    if (lengthWiFi > 39)
    {
      lengthWiFi = 39;
    }
    Serial.println("eeprom");
    Serial.println(EEPROM.read(105 + 40 * (m_nn - 18)));

    m_actualname = "";

    for (int i = 0; i < lengthWiFi; ++i)
    {
      if (EEPROM.read(66 + 40 * (m_nn - 18) + i) < 32 || //32-126
          EEPROM.read(66 + 40 * (m_nn - 18) + i) > 126)
      {
        m_actualname = m_actualname + char(48);
      }
      else
      {
        m_actualname = m_actualname + char(EEPROM.read(66 + 40 * (m_nn - 18) + i));
      }
    }
    Serial.print(m_actualname);
    Serial.println();
    //ssid=m_namelogin.c_str(); //string to const char*
  }

  void scanwifi()
  {
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
      Serial.println("no networks found");
      m_actualname = "no found";
    }
    else
    {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i)
      {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        delay(10);
      }
    }
    Serial.println("");
    numwifi += signbotton;
    if (numwifi > n - 1)
    {
      numwifi = 0;
    }
    m_actualname = WiFi.SSID(numwifi);
  }

  void lcdMenu()
  {
    Numers::lcdMenu();

    String rotatename;
    lcd.setCursor(0, 1);
    lcd.print(m_label);
    lcd.setCursor(5, 1);

    if (m_actualname.length() > 11)
    {
      rotatename = m_actualname.substring(rotateword);
      lcd.print(rotatename);
      ++rotateword;
      if (rotateword > m_actualname.length() - 11)
      {
        rotateword = 0;
      }
    }
    else
    {
      lcd.print(m_actualname);
    }
    sta3 = millis();
  }

  void lcdcreate()
  {
    lcd.setCursor(0, 0);
    lcd.print("                ");

    lcd.setCursor(0, 1);
    if (m_actualname.length() < 14)
    {
      lcd.print(m_actualname);
      lcd.print(" ");
    }
    else
    {
      lcd.print(m_actualname.substring(m_actualname.length() - 14));
      lcd.print(" ");
    }
    lcd.print(letter);
    lcd.print(" ");
  }

  void printA() const
  {
    Serial.println();
    Serial.print(m_actualname);
    Serial.print("  ");
    Serial.println();
  }

  String getactualname() const { return m_actualname; }
};

class MenuTools : public Numers
{
private:
  int m_actual;
  int m_min;
  int m_max;

public:
  MenuTools(int nn, int actual, int minA, int maxA, String label)
      : Numers(nn, label),
        m_actual(actual), m_min(minA), m_max(maxA)
  {
  }

  void setsetting(int actual)
  {
    m_actual = actual;
  }

  void changeTools(int sign)
  {
    m_actual += sign;
    if (m_actual > m_max)
    {
      m_actual = m_max;
    }
    if (m_actual < m_min)
    {
      m_actual = m_min;
    }
    signpm = 0;
  }

  void eepromMenuTools()
  {
    EEPROM.write(256 + m_nn, m_actual); //185
  }

  void eepromreadMenuTools()
  {
    m_actual = EEPROM.read(256 + m_nn); //185
  }

  void printA() const
  {
    Serial.print(m_actual);
    Serial.print("  ");
    Serial.print(m_min);
    Serial.print(":");
    Serial.print(m_max);
    Serial.print("  ");
    Serial.print(m_label);
    Serial.println();
  }

  void lcdMenu()
  {
    Numers::lcdMenu();

    lcd.setCursor(0, 1);
    lcd.print(m_label);
    lcd.setCursor(12, 1);
    if (m_nn > 6 && m_nn < 10)
    {
      lcd.print((float)(m_actual) / 10);
    }
    else
    {
      lcd.print(m_actual);
    }
  }

  int getactual() const { return m_actual; }
  int getmin() const { return m_min; }
  int getmax() const { return m_max; }

  friend bool operator+(const MenuTools &d1, const MenuTools &d2);
  friend bool operator-(const MenuTools &d1, const MenuTools &d2);
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                        "1234567890123456"
//                        "Stree    1 -15.6"
TempSensors street(22, "Street          ");
TempSensors boilroom(23, "Boroom          ");
TempSensors roomin(24, "Room            ");
TempSensors wood1(25, "Wood1           ");
TempSensors wood2(26, "Wood2           ");
TempSensors gazboil(27, "GazBoi          ");
TempSensors floor2(28, "floor2          ");

TempHour setting1(1, 6, 0, 205, "job1  ");
TempHour setting2(2, 7, 0, 160, "job2  ");
TempHour setting3(3, 14, 0, 195, "job3  ");
TempHour setting4(4, 16, 30, 200, "job4  ");
TempHour setting5(5, 21, 0, 190, "job5  ");
TempHour setting6(6, 23, 0, 185, "job6  ");
TempHour setting11(7, 6, 0, 200, "week1 ");
TempHour setting12(8, 7, 30, 165, "week2 ");
TempHour setting13(9, 12, 0, 190, "week3 ");
TempHour setting14(10, 17, 0, 205, "week4 ");
TempHour setting15(11, 21, 0, 200, "week5 ");
TempHour setting16(12, 23, 0, 187, "week6 ");
TempHour savesetting(15, 0, 0, 0, "Save Y/N ? ");

MenuTools Tonpump1(1, 40, 20, 70, "pumpON1  *C     ");
MenuTools Tonpump2(2, 40, 20, 70, "pumpON2  *C     ");
MenuTools Toffpump1(3, 25, 10, 50, "pumpOFF1 *C     ");
MenuTools Toffpump2(4, 25, 10, 50, "pumpOFF2 *C     ");
MenuTools Tmin(5, 7, 1, 20, "Tmin     *C     ");
MenuTools Tmax(6, 75, 40, 95, "Tmax     *C     ");
MenuTools Tgistereza(7, 5, 1, 50, "GisterGZ *C     ");
MenuTools TgisterezaP1(8, 5, 1, 100, "GisterP1 *C     ");
MenuTools TgisterezaP2(9, 5, 1, 100, "GisterP2 *C     ");
MenuTools TimeWood2(10, 5, 1, 30, "TimeP2  Min     ");

MenuTools Thour(30, 0, 0, 23, "Oclock Hour     ");
MenuTools Tminut(31, 0, 0, 59, "Oclock Min      ");
MenuTools Tdate(32, 1, 1, 31, "Oclock Date     ");
MenuTools Tmount(33, 1, 1, 12, "Oclock Moun     ");
MenuTools Tyear(34, 2019, 2019, 2099, "Oclock Year     ");
MenuTools Testablish(35, 0, 0, 1, "installtime     ");

WifiToken wifissid(18, "none", "ssid            ");
WifiToken password(19, "none", "pass            ");
WifiToken tokenblynk(20, "none", "tokn            ");
WifiToken pinsetting(21, "****", "pin             ");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void lcd0()
{
  lcd.setCursor(0, 0);
  lcd.print("K=     ");
  lcd.setCursor(2, 0);
  lcd.print(troomin, 1);
  lcd.print(" ");

  lcd.setCursor(7, 0);
  lcd.print("B=   ");
  lcd.setCursor(9, 0);
  lcd.print(tboilroom);

  lcd.setCursor(12, 0);
  lcd.print("O=   ");
  lcd.setCursor(14, 0);
  lcd.print(tgazboil);

  lcd.setCursor(0, 1);
  lcd.print("V=   ");
  lcd.setCursor(2, 1);
  lcd.print(tstreet, 1);
  lcd.print("  ");

  lcd.setCursor(8, 1);
  lcd.print("N= ");
  lcd.setCursor(11, 1);
  lcd.print(twood1);
  lcd.setCursor(13, 1);
  lcd.print(",");
  lcd.setCursor(14, 1);
  lcd.print(twood2);
}

void lcdtermostat()
{
  lcd.setCursor(0, 0);
  lcd.print(troomin, 1);
  lcd.print("*C      ");

  lcd.setCursor(11, 0);
  lcd.print(g_hour);
  lcd.print(":");

  if (g_minut < 10)
  {
    lcd.print("0");
  }
  lcd.print(g_minut);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print(((float)tempsetting) / 10, 1);
  lcd.print("   ");

  lcd.setCursor(5, 1);

  if (boreletermostar)
  {
    lcd.print(" OnGaz     ");
  }
  if (!boreletermostar && !bowoodstart)
  {
    lcd.print(" OffGaz    ");
  }
  if (!boreletermostar && bowoodstart && !(borelepump1 || borelepump2))
  {
    lcd.print(" OnWood    ");
  }
  if (!boreletermostar && bowoodstart && (borelepump1 || borelepump2))
  {
    lcd.print(" OnPump ");
    if (borelepump1)
    {
      lcd.print("1 ");
    }
    if (borelepump2)
    {
      lcd.print("2 ");
    }
    lcd.print("     ");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    lcd.setCursor(7, 0);
    lcd.print("@");
  }
  lcd.setCursor(8, 0);
  if (!permissionGazBlynk)
  {
    lcd.print("-");
  }
  else
  {
    lcd.print(" ");
  }
}
//--------------------------------------------------------------------------------------------------------
void bottonscan()
{

  digitalWrite(16, LOW); //on lcd backlite

  if (menuactualy)
  {
    if (menuactualy2)
    {
      menuactualy2 = false;
    }
    else
    {
      if (menuactualy1)
      {
        menuactualy1 = false;
      }
      else
      {
        if (menuactualy0)
        {
          menuactualy0 = false;
          pinsetting.setnamelogin("****");
          readmenuset();
        }
        else
        {
          if (!setactualyTH)
          {
            menuactualy0 = true;
          }
        }
      }
    }
    menuactualy = false;
  }

  //date displey
  if (espactualy && !espactualy1 && !menuactualy0 && !setactualyTH)
  {
    permissionGazBlynk = true;
    Blynk.virtualWrite(V3, 1);
    espactualy1 = true;
    espactualy = false;
    sta3 = millis();
    lcddate();
    bool saveLCDreset = false;
    if (EEPROM.read(279) == 0)
    {
      saveLCDreset = true;
    }
    EEPROM.write(279, 1);
    if (millis() - staeeprom > 3000)
    { //затримка для запису eeprom
      int delay_eeprom;
      delay_eeprom = 6000 - (millis() - staeeprom);
      delay(delay_eeprom);
    }
    if (saveLCDreset)
    {
      EEPROM.commit();
    }

    intwoodstart = 0;
    bowoodstart = false;
  }
  //change temperature actual
  if (!menuactualy0 && !menuactualy1 && !menuactualy2 && !enteractualy && !setactualyTH)
  {
    corecttemp += 5 * signbotton;
    sta = millis();
    sta1 = millis() - 3400;
    signpm = 0;
  }
  //setting change 6+6
  if (!menuactualy0 && !menuactualy1 && !menuactualy2 && enteractualy && !setactualyTH)
  {
    setactualyTH = true;
    enteractualy = false;
  }

  if (setactualyTH)
  {
    setSettingTH();
  }

  //entered menus
  if (menuactualy0 && !menuactualy1 && !menuactualy2 && !enteractualy)
  {
    enteractualy = false;
    sta1 = millis() - 3500;
    //menuset(Tonpump1, Tonpump2, Toffpump1, Toffpump2, Tmin, Tmax, Tgistereza);
  }
  //change setting menus
  if (menuactualy0 && !menuactualy1 && !menuactualy2 && enteractualy)
  {
    signpm = signbotton;
    if (k > 17 && k < 22)
    {
      kmemory = k;
      menuactualy1 = true;
      enteractualy = false;
    }
    sta1 = millis() - 3500;
    menuset(Tonpump1, Tonpump2, Toffpump1, Toffpump2, Tmin, Tmax, Tgistereza);
    signpm = 0;
    signbotton = 0;
  }
  //wifi entered password
  if (menuactualy0 && menuactualy1 && !menuactualy2)
  {
    pinchange();
  }
  if (menuactualy0 && menuactualy1 && menuactualy2)
  {

    if (kmemory == 18)
    {
      wifissid.scanwifi();
      sta1 = millis() - 3500;
      k = 18;
    }
    if (kmemory == 19)
    {
      password.typingnamelogin();
      password.lcdcreate();

      sta1 = millis() - 3000;
      k = 19;
    }
    if (kmemory == 20)
    {
      tokenblynk.typingnamelogin();
      tokenblynk.lcdcreate();

      sta1 = millis() - 3000;
      k = 20;
    }
    if (kmemory == 21)
    {
      pinsetting.typingnamelogin();
      pinsetting.lcdcreate();

      sta1 = millis() - 3000;
      k = 21;
    }
  }
  signpm = 0;
  signbotton = 0;
}
//********************************************************************************************************
void setSettingTH()
{
  if (changactualyTH || changactualyTH1)
  {
  }
  else
  {
    numset += signbotton;
    signbotton = 0;
  }
  if (numset > 13)
  {
    numset = 1;
  }
  if (numset < 1)
  {
    numset = 13;
  }

  if (espactualy)
  {
    setactualyTH = false;
    changactualyTH = false;
    changactualyTH1 = false;
    enteractualy = false;
    espactualy = false;
    menuactualy = false;
    numset = 1;
    readTempHour();
  }

  if (enteractualy && !changactualyTH)
  {
    changactualyTH = true;
    enteractualy = false;
  }

  if (enteractualy && !changactualyTH1)
  {
    changactualyTH1 = true;
    enteractualy = false;
  }

  if (enteractualy && changactualyTH1)
  {
    changactualyTH = false;
    changactualyTH1 = false;
    enteractualy = false;
  }
  bool boolsavesetting = false;
  switch (numset)
  {
  case 1:
    setting1.changeTH();
    setting1.lcdTempHour();
    if (100 * setting1.gethour() + setting1.getminut() > 100 * setting2.gethour() + setting2.getminut())
    {
      setting2.settimeA(setting1.gethour(), setting1.getminut());
    }
    break;
  case 2:
    setting2.changeTH();
    setting2.lcdTempHour();
    if (100 * setting2.gethour() + setting2.getminut() > 100 * setting3.gethour() + setting3.getminut())
    {
      setting3.settimeA(setting2.gethour(), setting2.getminut());
    }
    break;
  case 3:
    setting3.changeTH();
    setting3.lcdTempHour();
    if (100 * setting3.gethour() + setting3.getminut() > 100 * setting4.gethour() + setting4.getminut())
    {
      setting4.settimeA(setting3.gethour(), setting3.getminut());
    }
    break;
  case 4:
    setting4.changeTH();
    setting4.lcdTempHour();
    if (100 * setting4.gethour() + setting4.getminut() > 100 * setting5.gethour() + setting5.getminut())
    {
      setting5.settimeA(setting4.gethour(), setting4.getminut());
    }
    break;
  case 5:
    setting5.changeTH();
    setting5.lcdTempHour();
    if (100 * setting5.gethour() + setting5.getminut() > 100 * setting6.gethour() + setting6.getminut())
    {
      setting6.settimeA(setting5.gethour(), setting5.getminut());
    }
    break;
  case 6:
    setting6.changeTH();
    setting6.lcdTempHour();
    break;
  case 7:
    setting11.changeTH();
    setting11.lcdTempHour();
    if (100 * setting11.gethour() + setting11.getminut() > 100 * setting12.gethour() + setting12.getminut())
    {
      setting12.settimeA(setting11.gethour(), setting11.getminut());
    }
    break;
  case 8:
    setting12.changeTH();
    setting12.lcdTempHour();
    if (100 * setting12.gethour() + setting12.getminut() > 100 * setting13.gethour() + setting13.getminut())
    {
      setting13.settimeA(setting12.gethour(), setting12.getminut());
    }
    break;
  case 9:
    setting13.changeTH();
    setting13.lcdTempHour();
    if (100 * setting13.gethour() + setting13.getminut() > 100 * setting14.gethour() + setting14.getminut())
    {
      setting14.settimeA(setting13.gethour(), setting13.getminut());
    }
    break;
  case 10:
    setting14.changeTH();
    setting14.lcdTempHour();
    if (100 * setting14.gethour() + setting14.getminut() > 100 * setting15.gethour() + setting15.getminut())
    {
      setting15.settimeA(setting14.gethour(), setting14.getminut());
    }
    break;
  case 11:
    setting15.changeTH();
    setting15.lcdTempHour();
    if (100 * setting15.gethour() + setting15.getminut() > 100 * setting16.gethour() + setting16.getminut())
    {
      setting16.settimeA(setting15.gethour(), setting15.getminut());
    }
    break;
  case 12:
    setting16.changeTH();
    setting16.lcdTempHour();
    break;
  case 13:
    savesetting.lcdTempHour();
    if (changactualyTH1)
    {
      if (setting1.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting2.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting3.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting4.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting5.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting6.notequallyTempHour())
      {
        boolsavesetting = true;
      }

      if (setting11.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting12.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting13.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting14.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting15.notequallyTempHour())
      {
        boolsavesetting = true;
      }
      if (setting16.notequallyTempHour())
      {
        boolsavesetting = true;
      }

      if (boolsavesetting)
      {
        if (millis() - staeeprom > 3000)
        { //затримка для запису eeprom
          int delay_eeprom;
          delay_eeprom = 6000 - (millis() - staeeprom);
          delay(delay_eeprom);
        }
        saveyes = true;
        setting1.eepromwriteTempHour();
        setting2.eepromwriteTempHour();
        setting3.eepromwriteTempHour();
        setting4.eepromwriteTempHour();
        setting5.eepromwriteTempHour();
        setting6.eepromwriteTempHour();

        setting11.eepromwriteTempHour();
        setting12.eepromwriteTempHour();
        setting13.eepromwriteTempHour();
        setting14.eepromwriteTempHour();
        setting15.eepromwriteTempHour();
        setting16.eepromwriteTempHour();
        lcd.setCursor(0, 1);
        lcd.print("Begin save      ");
        delay(2000);
        saveyes = true;
        EEPROM.commit();
        //EEPROM.end();
        saveyes = false;
        lcd.setCursor(0, 1);
        lcd.print("Save OK         ");
        delay(2000);
      }

      //lcd.setCursor(0,1);
      //lcd.print("Save OK         ");
      delay(2000);
      boolsavesetting = false;
    }
    changactualyTH1 = false;

    break;
  }
}

void readTempHour()
{
  setting1.eepromreadTempHour();
  setting2.eepromreadTempHour();
  setting3.eepromreadTempHour();
  setting4.eepromreadTempHour();
  setting5.eepromreadTempHour();
  setting6.eepromreadTempHour();

  setting11.eepromreadTempHour();
  setting12.eepromreadTempHour();
  setting13.eepromreadTempHour();
  setting14.eepromreadTempHour();
  setting15.eepromreadTempHour();
  setting16.eepromreadTempHour();
}

void pinchange()
{
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN change");
  pin += signbotton;
  if (enteractualy)
  {
    if (pin == 18)
    {
      menuactualy2 = true;
      pin = 0;
      DateTime now = rtc.now();
      Thour.setsetting(now.hour());
      Tminut.setsetting(now.minute());
      Tdate.setsetting(now.day());
      Tmount.setsetting(now.month());
      Tyear.setsetting(now.year());

      //if(kmemory==9){password.setnamelogin("");}
      //if(kmemory==10){tokenblynk.setnamelogin("");}
      if (kmemory == 21)
      {
        pinsetting.setnamelogin("");
      }
    }
    else
    {
      menuactualy1 = false;
      k = kmemory;
      pin = 0;
    }
    sta1 = millis() - 3500;
  }
  enteractualy = false;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(pin);
}

void buttonsens()
{

  if (touchRead(T0) > threshold && touchRead(T3) > threshold && touchRead(T4) > threshold && touchRead(T8) > threshold && touchRead(T9) > threshold)
  {
    releasebotton = true;
    counttouch = 0;
  }

  if (touchMdetected)
  {
    menuactualy = true;
    enteractualy = false;
    k = 1;
    bottonscan();
    touchMdetected = false;
  }

  if (touchEdetected)
  {
    espactualy = true;
    bottonscan();
    touchEdetected = false;
  }

  if (touchMidetected)
  {
    if (!enteractualy)
    {
      k--;
      rotateword = -1;
    }
    signbotton = -1;
    bottonscan();
    touchMidetected = false;
  }

  if (touchPldetected)
  {
    if (!enteractualy)
    {
      k++;
      rotateword = -1;
    }
    signbotton = 1;
    bottonscan();
    touchPldetected = false;
  }

  if (touchEntdetected)
  {
    enteractualy = !enteractualy;
    signpm = 0;
    signbotton = 0;
    bottonscan();
    touchEntdetected = false;
  }
}
//********************************************************************************************************

void gotTouchM()
{
  if (counttouch > 4 && releasebotton)
  {
    touchMdetected = true;
    releasebotton = false;
  }
  lastTurn = millis();
  ++counttouch;
}

void gotTouchE()
{
  if (counttouch > 4 && releasebotton)
  {
    touchEdetected = true;
    releasebotton = false;
  }
  lastTurn = millis();
  ++counttouch;
}

void gotTouchMi()
{
  if (counttouch > 4 && releasebotton)
  {
    touchMidetected = true;
    releasebotton = false;
    lastTurn = millis();
  }
  touthauto++;
  if (counttouch > 5)
  {
    if (millis() - lastTurn > 200)
    {
      touchMidetected = true;
      counttouch = 0;
      lastTurn = millis();
    }
  }
  ++counttouch;
}

void gotTouchPl()
{
  if (counttouch > 4 && releasebotton)
  {
    touchPldetected = true;
    releasebotton = false;
    lastTurn = millis();
  }
  touthauto++;
  if (counttouch > 5)
  {
    if (millis() - lastTurn > 200)
    {
      touchPldetected = true;
      counttouch = 0;
      lastTurn = millis();
    }
  }
  ++counttouch;
}

void gotTouchEnt()
{
  if (counttouch > 4 && releasebotton)
  {
    touchEntdetected = true;
    releasebotton = false;
  }
  lastTurn = millis();
  ++counttouch;
}
//*********************************************************************************************************
void searh18b20()
{ //no metod (function)
  digitalWrite(14, HIGH);
  digitalWrite(26, HIGH);
  delay(25);
  byte addr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  Serial.print(" onlyone ");
  Serial.print(onlyone);
  Serial.print(" sensor ");
  Serial.print(sensornumber);
  Serial.println();
  if (!oneWire.search(addr))
  {
    if (onlyone == 2)
    {
      saveyes = true;
      if (millis() - staeeprom > 3000)
      { //затримка для запису eeprom
        int delay_eeprom;
        delay_eeprom = 6000 - (millis() - staeeprom);
        delay(delay_eeprom);
      }
      EEPROM.write(65, sensornumber);
      EEPROM.commit();
      saveyes = false;
    }
    sensornumber = 0;
    ++onlyone;
    oneWire.reset_search();
    delay(1000);
    return;
  }
  if (onlyone == 2)
  {
    for (byte bi = 0; bi < 8; ++bi)
    {
      EEPROM.write(8 * sensornumber + bi, addr[bi]);
    }
  }
  ++sensornumber;
  delay(25);
  digitalWrite(26, LOW);
  digitalWrite(14, LOW);
}
void readsens()
{
  digitalWrite(14, HIGH);
  digitalWrite(26, HIGH);
  delay(20);
  ds.requestTemperatures(); // считываем температуру с датчиков
  Serial.print("Sensor 1: ");
  street.getsensor();
  tstreet = printTemperature(sensor) + (float)(street.getcorectsens()) / 10;
  Serial.print("Sensor 2: ");
  boilroom.getsensor();
  tboilroom = printTemperature(sensor) + (float)(boilroom.getcorectsens()) / 10;
  Serial.print("Sensor 3: ");
  roomin.getsensor();
  troomin = printTemperature(sensor) + (float)(roomin.getcorectsens()) / 10;
  Serial.print("Sensor 4: ");
  wood1.getsensor();
  floatTwood1 = printTemperature(sensor) + (float)(wood1.getcorectsens()) / 10;
  twood1 = (int)floatTwood1;
  Serial.print("Sensor 5: ");
  wood2.getsensor();
  floatTwood2 = printTemperature(sensor) + (float)(wood2.getcorectsens()) / 10;
  twood2 = (int)floatTwood2;
  Serial.print("Sensor 6: ");
  gazboil.getsensor();
  tgazboil = printTemperature(sensor) + (float)(gazboil.getcorectsens()) / 10;
  Serial.println();
  delay(15);
  digitalWrite(14, LOW);
  digitalWrite(26, LOW);
}

float printTemperature(DeviceAddress deviceAddress)
{
  digitalWrite(14, HIGH);
  digitalWrite(26, HIGH);
  delay(20);
  float tempC = ds.getTempC(deviceAddress);
  //delay(15);
  Serial.print(tempC);
  Serial.print("*");
  Serial.println("C ");
  return tempC;
}

//*********************************************************************************************************

/*int operator+ (const MenuTools& mt1, const MenuTools& mt2){
  return (mt1.m_actual+mt2.m_actual);
}
int operator- (const MenuTools& mt1, const MenuTools& mt3){
  return (mt1.m_actual-mt3.m_actual);
}
*/
bool operator>(const TempHour &d1, const TempHour &d2) //+++++++++
{
  return (2400 > 100 * g_hour + g_minut && 100 * g_hour + g_minut > 100 * d2.m_hour + d2.m_minut);
}

bool operator>=(const TempHour &d1, const TempHour &d2) //-------
{
  return (100 * d1.m_hour + d1.m_minut >= 100 * g_hour + g_minut && 100 * g_hour + g_minut >= 100 * d2.m_hour + d2.m_minut);
}

bool operator<(const TempHour &d1, const TempHour &d2) //+++++
{
  return (0 < 100 * g_hour + g_minut && 100 * g_hour + g_minut < 100 * d2.m_hour + d2.m_minut);
}

bool operator<=(const TempHour &d1, const TempHour &d2) //++++
{
  return (100 * d1.m_hour + d1.m_minut <= 100 * g_hour + g_minut && 100 * g_hour + g_minut <= 100 * d2.m_hour + d2.m_minut);
}

void apartsetting(const TempHour &d1, const TempHour &d2, const TempHour &d3, const TempHour &d4, const TempHour &d5, const TempHour &d6, const TempHour &d11, const TempHour &d12, const TempHour &d13, const TempHour &d14, const TempHour &d15, const TempHour &d16)
{

  if (dayofweek > 0 && dayofweek < 6)
  {
    if (d1 < d1)
    {
      tempsettingjob = d6.gettemp() + corecttemp;
      whichsettingjob = 6;
    }

    if (d1 <= d2)
    {
      tempsettingjob = d1.gettemp() + corecttemp;
      whichsettingjob = 1;
    }
    if (d2 <= d3)
    {
      tempsettingjob = d2.gettemp() + corecttemp;
      whichsettingjob = 2;
    }
    if (d3 <= d4)
    {
      tempsettingjob = d3.gettemp() + corecttemp;
      whichsettingjob = 3;
    }
    if (d4 <= d5)
    {
      tempsettingjob = d4.gettemp() + corecttemp;
      whichsettingjob = 4;
    }
    if (d5 <= d6)
    {
      tempsettingjob = d5.gettemp() + corecttemp;
      whichsettingjob = 5;
    }
    if (d6 > d6)
    {
      tempsettingjob = d6.gettemp() + corecttemp;
      whichsettingjob = 6;
    }
    if (whichjob != whichsettingjob)
    {
      corecttemp = 0;
      whichjob = whichsettingjob;
    }
    if (tempsettingjob > 300)
    {
      corecttemp -= 5;
      tempsettingjob = 300;
    }
    if (tempsettingjob < 30)
    {
      corecttemp += 5;
      tempsettingjob = 30;
    }
    tempsetting = tempsettingjob;
  }
  if (dayofweek == 0 || dayofweek == 6)
  {
    if (d11 < d11)
    {
      tempsettingweek = d16.gettemp() + corecttemp;
      whichsettingweek = 16;
    }
    if (d11 <= d12)
    {
      tempsettingweek = d11.gettemp() + corecttemp;
      whichsettingweek = 11;
    }
    if (d12 <= d13)
    {
      tempsettingweek = d12.gettemp() + corecttemp;
      whichsettingweek = 12;
    }
    if (d13 <= d14)
    {
      tempsettingweek = d13.gettemp() + corecttemp;
      whichsettingweek = 13;
    }
    if (d14 <= d15)
    {
      tempsettingweek = d14.gettemp() + corecttemp;
      whichsettingweek = 14;
    }
    if (d15 <= d16)
    {
      tempsettingweek = d15.gettemp() + corecttemp;
      whichsettingweek = 15;
    }
    if (d16 > d16)
    {
      tempsettingweek = d16.gettemp() + corecttemp;
      whichsettingweek = 16;
    }
    if (whichweek != whichsettingweek)
    {
      corecttemp = 0;
      whichweek = whichsettingweek;
    }
    if (tempsettingweek > 300)
    {
      corecttemp -= 5;
      tempsettingweek = 300;
    }
    if (tempsettingweek < 30)
    {
      corecttemp += 5;
      tempsettingweek = 30;
    }
    tempsetting = tempsettingweek;
  }
  Serial.print(tempsettingjob);
  Serial.print("  ");
  Serial.print(corecttemp);
  //Serial.print(tempsettingweek);
  Serial.println();
}

void printTempHour(const TempHour &d1, const TempHour &d2, const TempHour &d3, const TempHour &d4, const TempHour &d5, const TempHour &d6, const TempHour &d11, const TempHour &d12, const TempHour &d13, const TempHour &d14, const TempHour &d15, const TempHour &d16)
{
  d1.printA();
  d2.printA();
  d3.printA();
  d4.printA();
  d5.printA();
  d6.printA();
  d11.printA();
  d12.printA();
  d13.printA();
  d14.printA();
  d15.printA();
  d16.printA();
}
void resetTempHour(TempHour &d1, TempHour &d2, TempHour &d3, TempHour &d4, TempHour &d5, TempHour &d6, TempHour &d11, TempHour &d12, TempHour &d13, TempHour &d14, TempHour &d15, TempHour &d16)
{
  d1.resetA();
  d2.resetA();
  d3.resetA();
  d4.resetA();
  d5.resetA();
  d6.resetA();
  d11.resetA();
  d12.resetA();
  d13.resetA();
  d14.resetA();
  d15.resetA();
  d16.resetA();
}
//*************************************************************************************************
void printMenuTools(const MenuTools &onpump1, const MenuTools &onpump2, const MenuTools &offpump1, const MenuTools &offpump2, const MenuTools &minA, const MenuTools &maxA, const MenuTools &gistereza)
{
  onpump1.printA();
  onpump2.printA();
  offpump1.printA();
  offpump2.printA();
  minA.printA();
  maxA.printA();
  gistereza.printA();
}

void menuset(MenuTools &onpump1, MenuTools &onpump2, MenuTools &offpump1, MenuTools &offpump2, MenuTools &minA, MenuTools &maxA, MenuTools &gistereza)
{
  if (k > 36)
  {
    k = 1;
  }
  if (k < 1)
  {
    k = 36;
  }
  switch (k)
  {
  case 1:
    onpump1.changeTools(signpm);
    onpump1.lcdMenu();
    timeNow();
    {
      DateTime now = rtc.now();
      Thour.setsetting(now.hour());
      Tminut.setsetting(now.minute());
      Tdate.setsetting(now.day());
      Tmount.setsetting(now.month());
      Tyear.setsetting(now.year());
    }

    break;
  case 2:
    onpump2.changeTools(signpm);
    onpump2.lcdMenu();
    break;
  case 3:
    offpump1.changeTools(signpm);
    offpump1.lcdMenu();
    break;
  case 4:
    offpump2.lcdMenu();
    offpump2.changeTools(signpm);
    break;
  case 5:
    minA.changeTools(signpm);
    minA.lcdMenu();
    break;
  case 6:
    maxA.changeTools(signpm);
    maxA.lcdMenu();
    break;
  case 7:
    gistereza.changeTools(signpm);
    gistereza.lcdMenu();
    break;
  case 8:
    TgisterezaP1.changeTools(signpm);
    TgisterezaP1.lcdMenu();
    break;
  case 9:
    TgisterezaP2.changeTools(signpm);
    TgisterezaP2.lcdMenu();
    break;
  case 10:
    TimeWood2.changeTools(signpm);
    TimeWood2.lcdMenu();
    break;
  case 11:
    street.changeCorect(signpm);
    street.lcdMenuCorect();
    break;
  case 12:
    boilroom.changeCorect(signpm);
    boilroom.lcdMenuCorect();
    break;
  case 13:
    roomin.changeCorect(signpm);
    roomin.lcdMenuCorect();
    break;
  case 14:
    wood1.changeCorect(signpm);
    wood1.lcdMenuCorect();
    break;
  case 15:
    wood2.changeCorect(signpm);
    wood2.lcdMenuCorect();
    break;
  case 16:
    gazboil.changeCorect(signpm);
    gazboil.lcdMenuCorect();
    break;
  case 17:
    floor2.changeCorect(signpm);
    floor2.lcdMenuCorect();
    numwifi = 0;
    break;
  case 18:
    wifissid.lcdMenu();
    break;
  case 19:
    if (!menuactualy2)
    {
      password.lcdMenu();
    }
    numwifi = 0;
    break;
  case 20:
    if (!menuactualy2)
    {
      tokenblynk.lcdMenu();
    }
    if (pinsetting.getactualname() == "0000")
    {
      ESP.restart();
    }
    if (pinsetting.getactualname() == "2222")
    {
      tokenblynk.setnamelogin("vIMbChC4VCULuMHnt1z57NU6pQsR2M4i");
    }
    if (pinsetting.getactualname() == "9999")
    {
      tokenblynk.setnamelogin("4429c3dc3ae0445ba3b6023897401155");
    }
    break;
  case 21:
    if (!menuactualy2)
    {
      pinsetting.lcdMenu();
    }
    break;
  //-----------------------------------------
  case 22:
    if (pinsetting.getactualname() == "1111")
    {
      street.changeTools(signpm);
      street.getaddres();
      ds.requestTemperatures();
      street.getsensor();
      t = printTemperature(sensor);
      street.lcdMenu();
      sta3 = millis() - 500;
    }
    else
    {
      k = 30;
    }
    break;
  case 23:
    boilroom.changeTools(signpm);
    boilroom.getaddres();
    ds.requestTemperatures();
    boilroom.getsensor();
    t = printTemperature(sensor);
    boilroom.lcdMenu();
    sta3 = millis() - 500;
    break;
  case 24:
    roomin.changeTools(signpm);
    roomin.getaddres();
    ds.requestTemperatures();
    roomin.getsensor();
    t = printTemperature(sensor);
    roomin.lcdMenu();
    sta3 = millis() - 500;
    break;
  case 25:
    wood1.changeTools(signpm);
    wood1.getaddres();
    ds.requestTemperatures();
    wood1.getsensor();
    t = printTemperature(sensor);
    wood1.lcdMenu();
    sta3 = millis() - 500;
    break;
  case 26:
    wood2.changeTools(signpm);
    wood2.getaddres();
    ds.requestTemperatures();
    wood2.getsensor();
    t = printTemperature(sensor);
    wood2.lcdMenu();
    sta3 = millis() - 500;
    break;
  case 27:
    gazboil.changeTools(signpm);
    gazboil.getaddres();
    ds.requestTemperatures();
    gazboil.getsensor();
    t = printTemperature(sensor);
    gazboil.lcdMenu();
    sta3 = millis() - 500;
    break;
  case 28:
    floor2.changeTools(signpm);
    floor2.getaddres();
    ds.requestTemperatures();
    floor2.getsensor();
    t = printTemperature(sensor);
    floor2.lcdMenu();
    sta3 = millis() - 500;
    break;
  case 29:
    if (pinsetting.getactualname() == "1111")
    {
      lcd.setCursor(0, 0);
      lcd.print("  Menu  ");
      lcd.setCursor(7, 0);
      lcd.print("29");
      lcd.print("/");
      lcd.print("35     ");
      lcd.setCursor(0, 1);
      lcd.print("Seach sens      ");
    }
    else
    {
      k = 21;
    }
    if (enteractualy)
    {
      lcd.setCursor(13, 0);
      lcd.print("<>");
      delay(500);
      searh18b20();
      lcd.setCursor(13, 0);
      lcd.print("  ");

      lcd.setCursor(0, 1);
      lcd.print("Seach sens ");
      lcd.print(onlyone);
      lcd.print(sensornumber);

      lcd.print("     ");
      delay(1000);
      enteractualy = false;
    }
    break;
  case 30:
    Thour.changeTools(signpm);
    Thour.lcdMenu();
    break;
  case 31:
    Tminut.changeTools(signpm);
    Tminut.lcdMenu();
    break;
  case 32:
    Tdate.changeTools(signpm);
    Tdate.lcdMenu();
    break;
  case 33:
    Tmount.changeTools(signpm);
    Tmount.lcdMenu();
    break;
  case 34:
    Tyear.changeTools(signpm);
    Tyear.lcdMenu();
    break;
  case 35:
    Testablish.changeTools(signpm);
    Testablish.lcdMenu();
    if (Testablish.getactual() == 1)
    {
      rtc.adjust(DateTime(Tyear.getactual(), Tmount.getactual(), Tdate.getactual(), Thour.getactual(), Tminut.getactual(), 0));

      Testablish.setsetting(0);
    }
    break;
  case 36:
    lcd.setCursor(0, 0);
    lcd.print("  Menu  ");
    lcd.setCursor(7, 0);
    lcd.print("36");
    lcd.print("/");
    lcd.print("36     ");
    lcd.setCursor(0, 1);
    lcd.print("Save            ");
    if (enteractualy)
    {
      lcd.setCursor(13, 0);
      lcd.print("<>");
      delay(500);
      //searh18b20();
      lcd.setCursor(13, 0);
      lcd.print("  ");

      lcd.setCursor(0, 1);
      lcd.print("Save");
      if (millis() - staeeprom > 3000)
      { //затримка для запису eeprom
        int delay_eeprom;
        delay_eeprom = 6000 - (millis() - staeeprom);
        delay(delay_eeprom);
      }
      saveyes = true;
      wifissid.writewifi();
      password.writewifi();
      tokenblynk.writewifi();

      Tonpump1.eepromMenuTools();
      Tonpump2.eepromMenuTools();
      Toffpump1.eepromMenuTools();
      Toffpump2.eepromMenuTools();
      Tmin.eepromMenuTools();
      Tmax.eepromMenuTools();
      Tgistereza.eepromMenuTools();
      TgisterezaP1.eepromMenuTools();
      TgisterezaP2.eepromMenuTools();
      TimeWood2.eepromMenuTools();

      street.eepromwriteSens();
      boilroom.eepromwriteSens();
      roomin.eepromwriteSens();
      wood1.eepromwriteSens();
      wood2.eepromwriteSens();
      gazboil.eepromwriteSens();
      floor2.eepromwriteSens();

      saveyes = true;
      EEPROM.commit();
      saveyes = false;

      pinsetting.setnamelogin("****");
      lcd.print(" OK     ");
      enteractualy = false;
      menuactualy0 = false;
      delay(2000);
    }
    break;
  }
}

void readmenuset()
{
  wifissid.readwifi();
  password.readwifi();
  tokenblynk.readwifi();

  Tonpump1.eepromreadMenuTools();
  Tonpump2.eepromreadMenuTools();
  Toffpump1.eepromreadMenuTools();
  Toffpump2.eepromreadMenuTools();
  Tmin.eepromreadMenuTools();
  Tmax.eepromreadMenuTools();
  Tgistereza.eepromreadMenuTools();
  TgisterezaP1.eepromreadMenuTools();
  TgisterezaP2.eepromreadMenuTools();
  TimeWood2.eepromreadMenuTools();

  street.eepromreadSens();
  boilroom.eepromreadSens();
  roomin.eepromreadSens();
  wood1.eepromreadSens();
  wood2.eepromreadSens();
  gazboil.eepromreadSens();
  floor2.eepromreadSens();
}

void timeNow()
{
  DateTime now = rtc.now();
  g_hour = now.hour();
  g_minut = now.minute();
  g_date = now.day();
  g_mou = now.month();
  g_yea = now.year();
  dayofweek = now.dayOfTheWeek();
}

void logicSensRele()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_restart++;
    if (wifi_restart > 720)
    {
      ESP.restart();
    }
  }

  if (!saveyes)
  { //не можна преривати EEPROM.commit()
    timeNow();

    if (g_hour == 5 && g_minut == 0 && millis() > 600000)
    {
      ESP.restart();
    }

    digitalWrite(26, HIGH);
    digitalWrite(14, HIGH);
    delay(20);
    readsens();
    delay(10);
    digitalWrite(14, LOW);
    digitalWrite(26, LOW);

    if (tboilroom < -116)
    {
      tboilroom = 6;
      Blynk.virtualWrite(V15, 255);
    }
    else
    {
      Blynk.virtualWrite(V15, 0);
    }
    if (bowoodstart && !postBowoodstart)
    {
      ++intwoodstart;
      if (intwoodstart > 120)
      {
        intwoodstart = 0;

        if (Toffpump1.getactual() > floatTwood1 && Toffpump2.getactual() > floatTwood2)
        {
          bowoodstart = false;
          postBowoodstart = true;
          postTimePump1 = 48;
          postBowoodstartFinish = 720;
        }
      }
    }

    if (inici)
    {
      if (!postBowoodstart)
      {
        if (Toffpump1.getactual() + 6 < floatTwood1 || Toffpump2.getactual() + 6 < floatTwood2)
        {
          bowoodstart = true;
        }
      }
    }

    inici = true;

    if (postBowoodstart)
    {
      if (Toffpump2.getactual() + 6 < floatTwood2)
      {
        postBowoodstart = false;
      }
      if (Toffpump1.getactual() + 6 < floatTwood1)
      {
        borelepump1 = true;
      }

      if (!borelepump1)
      {
        postTimePump1 = 48;
      }

      if (borelepump1)
      {
        postBowoodstartFinish = 720;
        if (postTimePump1-- < 0)
        {
          borelepump1 = false;
        }
      }

      if (!borelepump1)
      {
        if (postBowoodstartFinish-- < 0)
        {
          postBowoodstart = false;
        }
      }
    }

    if ((!boreletermostar && troomin < (float)(tempsetting) / 10) || tboilroom < Tmin.getactual())
    { //on
      if (!bowoodstart && permissionGazBlynk)
      {
        boreletermostar = true;
        Serial.println("termo true");
      }
    }

    if ((boreletermostar && troomin > (float)(tempsetting + Tgistereza.getactual()) / 10) || bowoodstart || !permissionGazBlynk)
    { //off
      if (tboilroom > Tmin.getactual())
      {
        boreletermostar = false;
        Serial.println("termo false");
      }
    }

    ///***/////*****////*****////*******/////************//////////////**************///////////////*****************///////////////////*****************/////////
    if (bowoodstart)
    {
      //ON OFF pump1. refresh t -> TimeWood2 for pump2

      if (floatTwood1 > Tonpump1.getactual())
      {
        borelepump1 = true;
        attempt1 = 0;
        attempt12++;
        if (attempt12 / 3 > TimeWood2.getactual())
        {
          attempt12 = 0;
          twood2one = twood2two;
          twood2two = twood2three;
          twood2three = floatTwood1;
        }
      }
      else if (floatTwood1 < Toffpump1.getactual())
      {
        borelepump1 = false;
        attempt1 = 0;
        attempt12 = 0;
      }
      else
      {
        attempt12 = 0;
        if (!borelepump1)
        {
          attempt1++;
          if (attempt1 / 3 > TimeWood2.getactual())
          {
            attempt1 = 0;
            borelepump1 = true;
          }
        }
        else
        {
          attempt1++;
          if (attempt1 / 3 > TimeWood2.getactual())
          {
            attempt1 = 0;
            borelepump1 = false;
            twood2one = twood2two;
            twood2two = twood2three;
            twood2three = floatTwood1;
          }
        }
      }

      //-------------------------------------------------------------------------------------------------------------------

      //ON OFF pump2.

      if (floatTwood2 > Tonpump2.getactual())
      {
        borelepump2 = true;
        attempt2 = 6;
      }
      else if (floatTwood2 < Toffpump2.getactual())
      {
        borelepump2 = false;
        attempt2 = 0;
      }
      else
      {

        if (twood2three - twood2one > 0 && twood2three - twood2one <= (float)TgisterezaP1.getactual() / 10 && twood2one > 10 && twood2three - floatTwood2 >= 0 && twood2three - floatTwood2 < (float)TgisterezaP2.getactual() / 10)
        {

          attempt2 += 1;
        }
        else if (twood2three - twood2one <= 0 && twood2one > 10 && twood2three - floatTwood2 >= 0 && twood2three - floatTwood2 < (float)TgisterezaP2.getactual() / 10)
        {

          attempt2 += 1;
        }
        else if (twood2three < floatTwood2 && twood2three - floatTwood2 < (float)TgisterezaP2.getactual() / 10 && twood2one > 10)
        {

          attempt2 += 1;
        }
        else
        {

          attempt2 -= 1;
        }
      }

      //----------------------------------------------------------------------------------------------

      if (attempt2 < 0)
      {
        attempt2 = 0;
        borelepump2 = false;
      }
      if (attempt2 > 6)
      {
        attempt2 = 6;
        borelepump2 = true;
      }

    } //bowooodstart
    /////////////////*******************//////////////////////////***********************************//////////////////////////*************************************

    if (bowoodstart)
    {
      Blynk.virtualWrite(V9, 255);
    }
    else
    {
      Blynk.virtualWrite(V9, 0);
    }
    if (borelepump1)
    {
      Blynk.virtualWrite(V1, 255);
      digitalWrite(5, HIGH);
    }
    else
    {
      if (twood1 > -116)
      {
        Blynk.virtualWrite(V1, 0);
        digitalWrite(5, LOW);
      }
    }
    if (borelepump2)
    {
      Blynk.virtualWrite(V2, 255);
      digitalWrite(17, HIGH);
    }
    else
    {
      if (twood2 > -116)
      {
        Blynk.virtualWrite(V2, 0);
        digitalWrite(17, LOW);
      }
    }
    Blynk.virtualWrite(V4, (float)(tempsetting) / 10);
    Blynk.virtualWrite(V5, troomin);
    Blynk.virtualWrite(V6, tstreet);

    Blynk.virtualWrite(V7, StringTempLcd(tboilroom, tgazboil, 1));
    Blynk.virtualWrite(V8, StringTempLcd(twood1, twood2, 2));

    //----захист системи від відриву сенсора--------------
    if (twood1 < -116)
    {
      Blynk.virtualWrite(V1, 255);
      digitalWrite(5, HIGH);
      Blynk.virtualWrite(V15, 255);
    }
    if (twood2 < -116)
    {
      Blynk.virtualWrite(V2, 255);
      digitalWrite(17, HIGH);
      Blynk.virtualWrite(V15, 255);
    }

    if (troomin < -116)
    {
      boreletermostar = false;
      Blynk.virtualWrite(V15, 255);
    }

    if (boreletermostar)
    {
      Blynk.virtualWrite(V0, 255);
      digitalWrite(18, LOW);
    }
    else
    {
      Blynk.virtualWrite(V0, 0);
      digitalWrite(18, HIGH);
    }
  }
  staeeprom = millis();
}

String StringTempLcd(float t1, float t2, int lcd)
{
  String boilString = "Бойл Опал=", tgazString = ", ", wood1String = "Насос1,2= ", wood2String = ", ";
  if (lcd == 1)
  {
    return boilString + String(t1, 0) + tgazString + String(t2, 0);
  }
  return wood1String + String(t1, 0) + wood2String + String(t2, 0);
}

BLYNK_WRITE(V3)
{
  if (param.asInt())
  {
    permissionGazBlynk = true;
    EEPROM.write(279, 1);
    Blynk.virtualWrite(V0, 1);
    digitalWrite(18, LOW);
  }
  else
  {
    permissionGazBlynk = false;
    EEPROM.write(279, 0);
    Blynk.virtualWrite(V0, 0);
    digitalWrite(18, HIGH);
  }
  if (millis() - staeeprom > 3000)
  { //затримка для запису eeprom
    int delay_eeprom;
    delay_eeprom = 6000 - (millis() - staeeprom);
    delay(delay_eeprom);
  }
  EEPROM.commit();
}

void lcddate()
{
  lcd.setCursor(0, 0);
  lcd.print(g_date);
  lcd.print("/");
  lcd.print(g_mou);
  lcd.print("/");
  lcd.print(g_yea);
  lcd.print("   ");

  lcd.setCursor(11, 0);
  lcd.print(g_hour);
  lcd.print(":");

  if (g_minut < 10)
  {
    lcd.print("0");
  }
  lcd.print(g_minut);
  lcd.print("  ");
}

void IRAM_ATTR resetModule()
{
  ets_printf("reboot\n");
  esp_restart();
}

//************************************************************************************************************
void setup()
{
  //Serial.begin(9600);
  ds.begin();

  EEPROM.begin(280);
  delay(500);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  pinMode(18, OUTPUT); //reles termostat
  pinMode(5, OUTPUT);  //reles nasos tverdogo kotla1
  pinMode(17, OUTPUT); //reles nasos tverdogo kotla2
  pinMode(16, OUTPUT); //reles лсд підвітка
  pinMode(14, OUTPUT); //захист лінії ds18b20
  pinMode(26, OUTPUT); //захист лінії ds18b20 open drain
  REG_WRITE(GPIO_PIN26_REG, GPIO_PIN26_PAD_DRIVER);
  digitalWrite(26, LOW);
  digitalWrite(14, LOW);
  touchAttachInterrupt(T9, gotTouchM, threshold);
  touchAttachInterrupt(T8, gotTouchE, threshold);
  touchAttachInterrupt(T0, gotTouchMi, threshold);
  touchAttachInterrupt(T4, gotTouchPl, threshold);
  touchAttachInterrupt(T3, gotTouchEnt, threshold);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("wi-fi avtomatic ");
  lcd.setCursor(0, 1);
  lcd.print(" Vovchinec Inc  ");
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  //Stringauth, Stringssid, Stringpass;
  readmenuset();

  Stringauth = tokenblynk.getactualname();
  auth = Stringauth.c_str();
  Stringssid = wifissid.getactualname();
  ssid = Stringssid.c_str();
  Stringpass = password.getactualname();
  pass = Stringpass.c_str();

  delay(500);
  Serial.print(auth);
  Serial.print("   ");
  Serial.print(ssid);
  Serial.print("   ");
  Serial.print(pass);
  //auth = "4429c3dc3ae0445ba3b6023897401155";
  //ssid = "TP-LINK_7ACD84";
  //pass = "310719822804";
  Blynk.begin(auth, ssid, pass);

  street.getaddres();
  boilroom.getaddres();
  roomin.getaddres();
  wood1.getaddres();
  wood2.getaddres();
  gazboil.getaddres();

  readTempHour();
  if (EEPROM.read(279) == 1)
  {
    Blynk.virtualWrite(V3, 1);
    permissionGazBlynk = true;
  }
  else
  {
    Blynk.virtualWrite(V3, 0);
    permissionGazBlynk = false;
  }
  timer2.setInterval(5000L, logicSensRele);
  // dwt ---------- dwt--------------------dwt
  timer = timerBegin(0, 80, true); //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);
  timerAlarmWrite(timer, 14000000, false); //set time in us
  timerAlarmEnable(timer);                 //enable interrupt
}

void loop()
{
  if (millis() - stawdt > 2000)
  {
    timerWrite(timer, 0);
    stawdt = millis();
  }
  timer2.run();
  Blynk.run();

  if (sta2 > millis())
  {
    stawdt = millis();
    staeeprom = millis();
    sta = millis();
    sta1 = millis();
    sta2 = millis();
    sta3 = millis();
    lastTurn = millis();
  }

  if (millis() - lastTurn > 20000)
  {
    digitalWrite(16, HIGH);
  }

  if (millis() - sta2 > 50)
  {
    buttonsens();
    sta2 = millis();
  }
  //-------------
  if (millis() - lastTurn > 60000)
  {
    if (menuactualy0 || setactualyTH)
    {
      setactualyTH = false;
      changactualyTH = false;
      changactualyTH1 = false;
      enteractualy = false;
      espactualy = false;
      menuactualy = false;
      numset = 1;
      readTempHour();

      menuactualy0 = false;
      menuactualy1 = false;
      menuactualy2 = false;
      pinsetting.setnamelogin("****");
      readmenuset();
    }
  }

  if (millis() - sta3 > 4000 && espactualy1)
  {
    espactualy1 = false;
  }

  if (menuactualy0 && !setactualyTH && !espactualy1)
  {
    if ((millis() - sta1 > 3500 || millis() - sta3 > 1000) && !(menuactualy1 && !menuactualy2))
    {
      menuset(Tonpump1, Tonpump2, Toffpump1, Toffpump2, Tmin, Tmax, Tgistereza);
      sta1 = millis();
      sta3 = millis();
    }
  }

  //------------
  if (!menuactualy0 && !setactualyTH && !espactualy1)
  {
    if (millis() - sta > 7000)
    {
      Blynk.virtualWrite(V10, 0);
      lcd0();
      sta = millis();
      sta1 = millis();
    }
    if (millis() - sta1 > 3500 && !espactualy1)
    {
      Blynk.virtualWrite(V10, 255);
      apartsetting(setting1, setting2, setting3, setting4, setting5, setting6,
                   setting11, setting12, setting13, setting14, setting15, setting16);
      lcdtermostat();
      sta1 = millis();
    }
  }
  //-----------
}
