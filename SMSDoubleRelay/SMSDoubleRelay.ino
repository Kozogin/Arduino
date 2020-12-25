#include <DS3231.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
DS3231 clock;
RTCDateTime dt;
SoftwareSerial mySerial(2, 3);

unsigned long sta;
char inChar;   // переменная для хранения полученного байта
String telefon="", tempstr="";
byte sendtype;

byte g_hou = 18, g_min = 24;
byte g_date = 22, g_mou = 9; 
int g_yea = 2019;
byte dayofweek = 3;
byte m = 0;
byte houon, houof, minon, minof, minon1, minof1;
bool boolrndon, boolcallback, boolreset, booltimedate;
bool g_spracmem1 = false, g_spracmem2 = false;    //для перевірки 2 каналів з класу
String g_sms = "";
byte lineadres=0;
//String g_sms = " reset rn don 4,0:17,8:59, 5,16:45,23:15, 14,7:30,7:50,15,6:7,19:19 back "; //смс з режимом роботи
// String g_sms = " time  20:35, 21- 5- 2019 back "; //смс з режимом роботи
//String g_sms = " rn don ba ck";

/*
+CMT: "+38067675####","","19/09/30,19:44:25+12"
reset rnd on 1,0:17,8:59, 2,16:45,23:45, 3,7:30,7:50,4,6:7,23:19, 5,0:17,8:59, 6,16:45,23:45, 11,7:30,7:50,12,6:7,23:19, 13,0:17,8:59, 14,16:45,23:45, 15,7:30,7:50,16,6:7,23:19 back   
+CMT: "+38067675####","","19/09/30,19:44:25+12"
1,0:17,8:59, back 
*/

class Timenf          //клас 
{
private:
  byte m_nn;         //номер
  byte m_onh;        //включення година
  byte m_onm;        //включення хвилина
  byte m_ofh;        //викл година
  byte m_ofm;        //викл хвилина

public:
  Timenf() // конструктор по умолчанию 
  {
    m_nn = 0;
    m_onh = 7;
    m_onm = 50;
    m_ofh = 8;
    m_ofm = 0;
  }

  Timenf(byte nn, byte onh, byte onm, byte ofh, byte ofm) //конструктор класу
  {
    m_nn = nn;
    m_onh = onh;
    m_onm = onm;
    m_ofh = ofh;
    m_ofm = ofm;
  }

  void resetA() {
    m_onh = 0;
    m_onm = 0;
    m_ofh = 0;
    m_ofm = 0;
  }

  bool porivnati() const {  //порівняння часу для обох каналів

    if ((100 * m_onh + m_onm) == (100 * m_ofh + m_ofm)) { return false; }  //якщо час вкл і викл співпадають

    if ((100 * m_onh + m_onm) <= (100 * g_hou + g_min) && (100 * m_ofh + m_ofm) >= (100 * g_hou + g_min)) { //звичайне
      return true;
    }
    if ((100 * m_onh + m_onm) > (100 * m_ofh + m_ofm)) {                                          //перехід через 24
      if ((100 * m_onh + m_onm) <= (100 * g_hou + g_min) && 2401 >= (100 * g_hou + g_min)
        || (100 * m_ofh + m_ofm) >= (100 * g_hou + g_min)) {
        return true;
      }
    }
    return false;
  }

  bool neodnake() const {
    if ((100 * m_onh + m_onm) == (100 * m_ofh + m_ofm)) { return false; }
    return true;
  }

  void printA() const {
    Serial.print(m_nn);
    Serial.print(" ");
    Serial.print(m_onh);
    Serial.print(":");
    Serial.print(m_onm);
    Serial.print("-");
    Serial.print(m_ofh);
    Serial.print(":");
    Serial.print(m_ofm);
    Serial.println(" ");
  }

  void settimeA(byte nn, byte onh, byte onm, byte ofh, byte ofm) {
    m_nn = nn;
    m_onh = onh;
    m_onm = onm;
    m_ofh = ofh;
    m_ofm = ofm;
  }

  void takeeeprom(byte adres) const {
     EEPROM.update(adres, m_nn);
     EEPROM.update(adres+1, m_onh);
     EEPROM.update(adres+2, m_onm);
     EEPROM.update(adres+3, m_ofh);
     EEPROM.update(adres+4, m_ofm);
  }
  void geteeprom(byte adres){
   m_nn=EEPROM.read(adres);
   m_onh=EEPROM.read(adres+1);
   m_onm=EEPROM.read(adres+2);
   m_ofh=EEPROM.read(adres+3);
   m_ofm=EEPROM.read(adres+4);
  }
  void geteepromprint(byte adres) const {
   Serial.println(EEPROM.read(adres));
   Serial.println(EEPROM.read(adres+1));
   Serial.println(EEPROM.read(adres+2));
   Serial.println(EEPROM.read(adres+3));
   Serial.println(EEPROM.read(adres+4));
   Serial.println("---------------------------------");
  }

  byte getnn()  const { return m_nn; }   // гетер
  byte getonh() const { return m_onh; }
  byte getonm() const { return m_onm; }
  byte getofh() const { return m_ofh; }
  byte getofm() const { return m_ofm; }
};//--------------------------------------------------------

  Timenf ustan1(1, 7, 19, 8, 19); 
  Timenf ustan2(2, 16, 30, 18, 0);
  Timenf ustan3(3, 1, 30, 3, 0);
  Timenf ustan4(4, 23, 30, 1, 45);
  Timenf ustan5(5, 4, 30, 4, 30);
  Timenf ustan6(6, 5, 30, 23, 0);

  Timenf ustan11(11, 6, 30, 7, 50);
  Timenf ustan12(12, 16, 45, 17, 20);
  Timenf ustan13(13, 20, 30, 21, 0);
  Timenf ustan14(14, 22, 30, 23, 30);
  Timenf ustan15(15, 0, 0, 0, 0);
  Timenf ustan16(16, 15, 0, 23, 45);


void onofrnd() {
  
  switch (g_mou) {
  case 1:
  case 2:
  case 12:
    houon = 18;
    break;
  case 3:
  case 10:
  case 11:
    houon = 19;
    break;
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  
    houon = 20;
    break;
  }
  houof = 23;
  minon = (134 * g_date) % 59;
  minof = (258 * g_date) % 59;

  minon1 = (184 * g_date) % 59;
  minof1 = (218 * g_date) % 59;

  if (100 * houon + minon < 100 * g_hou + g_min && 100 * houof + minof > 100 * g_hou + g_min) {
    g_spracmem1 = true;
    //lamp1 on
  }
  else {
    g_spracmem1 = false;
    //lamp1 off
  }
  if (100 * houon + minon1 < 100 * g_hou + g_min && 100 * houof + minof1 > 100 * g_hou + g_min) {
    g_spracmem2 = true;
    //lamp2 on
  }
  else {
    g_spracmem2 = false;
    //lamp2 off
  }
}

void smsfinddigit(int *smsarray) {
  
  int i = 62;
    m= 255;
  int dig = 0;
  byte readeprom;
  while (i < 62+1+(EEPROM.read(61)) ) {
    readeprom=(EEPROM.read(i));
    if ( (readeprom>47 && readeprom<58)||
      (char)readeprom == '-' || 
      (char)readeprom == ' ' ||
      (char)readeprom == ':' ||  
      (char)readeprom == ',') {                 //пропускає цифри і символи

      if (readeprom>47 && readeprom<58) {  //якщо цифра формується число
        dig = 10 * dig + (static_cast<int>(readeprom - 48));
      }

      if (i!=62) {          //якщо і!=0
        
        if ((!(readeprom>47 && readeprom<58) && 
        (((EEPROM.read(i-1))>47 && (EEPROM.read(i-1))<58)))  ||             //якщо не цифра завершення формування
          i == 62+(EEPROM.read(61)) + 1) {
          ++m;
          smsarray[m] = dig;
          Serial.print(" d=");
          Serial.print(dig);
          dig = 0;
        }
      }
    }
    ++i;
  }
}

bool smsverifityarrayustan(int *smsarray) {
  Serial.print(" m=");
          Serial.println(m);        
  bool smsprom = false, smspro=false;
  if ((m+1)% 5 == 0 && m < 63) {//ok
    smsprom = true;
  }
  for (int i = 0; i < m-1 ; ++i) {
    smspro = false;
    //----------------------------------------------------
    switch ((i + 1) % 5)
    {
    case 1:
      if ((smsarray[i] > 0 && smsarray[i] < 7) || (smsarray[i] > 10 && smsarray[i] < 17)) {
        smspro = true;//}
        //Serial.print("  m1 d =");
        //Serial.print(smspro);
        }
      break;
    case 2:
      if (smsarray[i] >= 0 && smsarray[i] < 24) {
        smspro = true;
        }
      break;
    case 3:
      if (smsarray[i] >= 0 && smsarray[i] < 60) {
        smspro = true;
        }
      break;
    case 4:
      if (smsarray[i] >= 0 && smsarray[i] < 24) {
        smspro = true;
        }
      break;
    case 0:
      if (smsarray[i] >= 0 && smsarray[i] < 60) {
        smspro = true;
        }
      break;
    }   
    if (smspro && smsprom) { smsprom = true; }
    else { smsprom = false; }
    
  }
  return smsprom;
}

bool smsverifityarraytimedate(int *smsarray) {
  Serial.print(" m=");
          Serial.println(m);
  bool smsprom = false, smspro;
  if (m+1 == 5) {//ok
    smsprom = true;
  }
  for (int i = 0; i < 5; ++i) {
    smspro = false;
    //----------------------------------------------------

    switch (i)
    {
    case 0:
      if (smsarray[i] >= 0 && smsarray[i] < 23) { smspro = true;}
      break;
    case 1:
      if (smsarray[i] >= 0 && smsarray[i] < 60) { smspro = true;}
      break;
    case 2:
      if (smsarray[i] > 0 && smsarray[i] < 32) {  smspro = true;}
      break;
    case 3:
      if (smsarray[i] > 0 && smsarray[i] < 13) { smspro = true;}
      break;
    case 4:
      if (smsarray[i] > 1975 && smsarray[i] < 2100) { smspro = true;}
      break;
    }

    if (smspro && smsprom) {
      smsprom = true;
        }
    else { smsprom = false; }
  }
    int god;
    int minA;
    int dat;
    int mis;
    int rik; 
  if (smsprom) {
     god = smsarray[0];//локальні
     minA = smsarray[1];
     dat = smsarray[2];
     mis = smsarray[3];
     rik = smsarray[4];
     
          Serial.print("  ");
          Serial.print(god);
          Serial.print(" : ");
          Serial.print(minA);
          Serial.print("   ");
          Serial.print(dat);
          Serial.print(" / ");
          Serial.print(mis);
          Serial.print(" / ");
          Serial.print(rik);
          Serial.print("   ");         
  }
  return smsprom;
}


void resetustan() {//------
    ustan1.resetA();
    ustan2.resetA();
    ustan3.resetA();
    ustan4.resetA();
    ustan5.resetA();
    ustan6.resetA();
    ustan11.resetA();
    ustan12.resetA();
    ustan13.resetA();
    ustan14.resetA();
    ustan15.resetA();
    ustan16.resetA();
}

void changeustan(int *smsarray) {//------
  byte s0, s1, s2, s3, s4;

  byte i = 0;
  while (i < m + 1) {
    s0 = smsarray[i];
    s1 = smsarray[i + 1];
    s2 = smsarray[i + 2];
    s3 = smsarray[i + 3];
    s4 = smsarray[i + 4];
    if ((100 * s1 + s2) != (100 * s3 + s4)) {
      

      switch (s0) {
      case 1:
        ustan1.settimeA(s0, s1, s2, s3, s4);
        break;
      case 2:
        ustan2.settimeA(s0, s1, s2, s3, s4);
        break;
      case 3:
        ustan3.settimeA(s0, s1, s2, s3, s4);
        break;
      case 4:
        ustan4.settimeA(s0, s1, s2, s3, s4);
        break;
      case 5:
        ustan5.settimeA(s0, s1, s2, s3, s4);
        break;
      case 6:
        ustan6.settimeA(s0, s1, s2, s3, s4);
        break;
      case 11:
        ustan11.settimeA(s0, s1, s2, s3, s4);
        break;
      case 12:
        ustan12.settimeA(s0, s1, s2, s3, s4);
        break;
      case 13:
        ustan13.settimeA(s0, s1, s2, s3, s4);
        break;
      case 14:
        ustan14.settimeA(s0, s1, s2, s3, s4);
        break;
      case 15:
        ustan15.settimeA(s0, s1, s2, s3, s4);
        break;
      case 16:
        ustan16.settimeA(s0, s1, s2, s3, s4);
        break;
      }
    }
    i += 5;
  }
}

void sendeeprom() {
      ustan1.takeeeprom(1); 
      ustan2.takeeeprom(6); 
      ustan3.takeeeprom(11); 
      ustan4.takeeeprom(16); 
      ustan5.takeeeprom(21); 
      ustan6.takeeeprom(26); 
      ustan11.takeeeprom(31); 
      ustan12.takeeeprom(36); 
      ustan13.takeeeprom(41); 
      ustan14.takeeeprom(46); 
      ustan15.takeeeprom(51); 
      ustan16.takeeeprom(56); 
       
}
void bandeeprom() {
      ustan1.geteepromprint(1); 
      ustan2.geteepromprint(6); 
      ustan3.geteepromprint(11); 
      ustan4.geteepromprint(16); 
      ustan5.geteepromprint(21); 
      ustan6.geteepromprint(26); 
      ustan11.geteepromprint(31); 
      ustan12.geteepromprint(36); 
      ustan13.geteepromprint(41); 
      ustan14.geteepromprint(46); 
      ustan15.geteepromprint(51); 
      ustan16.geteepromprint(56); 
       
}
 
void smsrndsend(){
   //smsout = "";
   String shouon = (String)(houon);
   String sminon = (String)(minon);
   String shouof = (String)(houof);
   String sminof = (String)(minof);
   String sminon1 = (String)(minon1);
   String sminof1= (String)(minof1);
   
//   String rnd = "rnd  1 ", p2 = ":", flash = " /2 ", minus = "-";;

//smsout = rnd+shouon+p2+sminon+minus+shouof+p2+sminof+flash+ shouon + p2 + sminon1 + minus + shouof + p2 + sminof1;
}

void smsrndon(){
 for (int i=62;i<62+(EEPROM.read(61))-5;++i){
      if ((char)(EEPROM.read(i))  =='r' &&
          (char)(EEPROM.read(i+1))=='n' &&
          (char)(EEPROM.read(i+2))=='d' &&
          (char)(EEPROM.read(i+3))=='o' &&
          (char)(EEPROM.read(i+4))=='n') {
             boolrndon=true;
            break; 
          }
          else {
             boolrndon=false;
          }
  }  
}

void smsrndof(){
 for (int i=62;i<62+(EEPROM.read(61))-5;++i){
      if ((char)(EEPROM.read(i))  =='r' &&
          (char)(EEPROM.read(i+1))=='n' &&
          (char)(EEPROM.read(i+2))=='d' &&
          (char)(EEPROM.read(i+3))=='o' &&
          (char)(EEPROM.read(i+4))=='f') {
             boolrndon=false;
            break; 
          }
          else {
             boolrndon=true;
          }
  }  
}

void smsreset(){
 for (int i=62;i<62+(EEPROM.read(61))-5;++i){
      if ((char)(EEPROM.read(i))  =='r' &&
          (char)(EEPROM.read(i+1))=='e' &&
          (char)(EEPROM.read(i+2))=='s' &&
          (char)(EEPROM.read(i+3))=='e' &&
          (char)(EEPROM.read(i+4))=='t') {
             boolreset=true;
           break;  
          }
          else {
             boolreset=false;
          }
  }  
}

void smsrtimedate(){
  for (int i=62;i<62+(EEPROM.read(61))-4;++i){
      if ((char)(EEPROM.read(i))  =='t' &&
          (char)(EEPROM.read(i+1))=='i' &&
          (char)(EEPROM.read(i+2))=='m' &&
          (char)(EEPROM.read(i+3))=='e')  {
             booltimedate=true;
            break; 
          }
          else {
             booltimedate=false;
          }
  }  
}

void smsback(){
  for (int i=62;i<62+(EEPROM.read(61))-4;++i){
      if ((char)(EEPROM.read(i))  =='b' &&
          (char)(EEPROM.read(i+1))=='a' &&
          (char)(EEPROM.read(i+2))=='c' &&
          (char)(EEPROM.read(i+3))=='k')  {
             boolcallback=true;
            break; 
          }
          else {
             boolcallback=false;
          }
  }  
}


void setup() {
  Serial.begin(9600);
mySerial.begin(9600);
Serial.println("Initializing..."); 
  delay(1000);
  
  //Serial.print(":1");
     pinMode(A2, OUTPUT);
     pinMode(A3, OUTPUT);
       pinMode(5, OUTPUT);
       pinMode(9, OUTPUT);
       pinMode(10, OUTPUT);
       
      digitalWrite(10,HIGH);//restart
      digitalWrite(A2, LOW);//живлення rtc
      digitalWrite(A3, HIGH);//живлення rtc
        digitalWrite(5, LOW);
        digitalWrite(9, LOW);
   clock.begin();  
   
    //clock.setDateTime(__DATE__, __TIME__);
    //clock.setDateTime(2019, 9, 29, 12, 18, 0); 
    ///////EEPROM.update(1, 0);//adres value litofalse
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();  
}

void loop() {
 updateSerial(); 
   
if (millis()>8000 && millis()<16000){  
  telefon="";  
}

delay(400);
//---------------------------------------------------------------------------------------------
// якщо прийшло смс
if(telefon=="+38067675####"){          // якща прийло смс   
    {              // блок для обмеження життя масиву
      bool boolverifitytimedate, boolverifityustan;
      int smsarray[60]; 
           smsrndon();     
      if (boolrndon){
           smsrndof();
      }                      
     smsrtimedate();  // перевірка установки часу і дати
     smsreset();// перевірка на ресет
     smsback(); // перевірка кал бак
     /*
Serial.print("  booltimedate ");
Serial.println(booltimedate);
Serial.print("  boolreset ");
Serial.println(boolreset);
Serial.print("  boolcallback ");
Serial.println(boolcallback);

Serial.print("  boolrndon ");
Serial.println(boolrndon);
     */

      if (booltimedate && !boolrndon){
          smsfinddigit(smsarray);             // знаходимо цифри в смс і в масив
          boolverifitytimedate = smsverifityarraytimedate(smsarray);  //перевірка величини масиву і коректності цифр
             if (boolverifitytimedate){
                // set time & date
             }
      }

      if (boolreset && !boolrndon && !booltimedate){
          resetustan();
          
      }

      if (!boolrndon && !booltimedate){        // приймання нових установок часів
          smsfinddigit(smsarray);
          boolverifityustan = smsverifityarrayustan(smsarray);
             if (boolverifityustan){
                 changeustan(smsarray);
             }
      }
      
//----------------------------------------------------------------------------
  
    }             //знищуємо масив, обробка смс закінчена
             //кал бек
 
 
      if (boolrndon) {              //якщо рнд провірка калбек
          if (boolcallback) {
            onofrnd();
            smsrndsend();
          }
      }

      if (boolcallback && !boolrndon) {
//          sendsms();
      }  
telefon="";
}
//----------------------------------------------------------------------------

  if(millis()-sta>3000){
dt = clock.getDateTime();
g_hou=dt.hour;
g_min=dt.minute ;
g_date=dt.day;
g_mou=dt.month;
g_yea=dt.year;
dayofweek=dt.dayOfWeek;

Serial.println(telefon);

  for (int i=62;i<62+EEPROM.read(61);++i){
    Serial.print((char)(EEPROM.read(i)));
 }
  Serial.println("");
       sendtype=0;
       tempstr="";       
    sta=millis();    
//якщо прийло смс 
 //---------------------------------------------------------------------
 //вмикання і вимикання реле згідно установок або рнд    
      if (boolrndon) {            //вмикання rnd
        onofrnd();
      }

        if (!boolrndon) {
          g_spracmem1 = false; //провірка 2 каналів на вмикання
          g_spracmem2 = false;

          if (dayofweek > 0 && dayofweek < 6) {  // понеділок-пятниця
            if (ustan1.porivnati()) { g_spracmem1 = true; }
            if (ustan2.porivnati()) { g_spracmem1 = true; }
            if (ustan3.porivnati()) { g_spracmem1 = true; }
            if (ustan11.porivnati()) { g_spracmem2 = true; }
            if (ustan12.porivnati()) { g_spracmem2 = true; }
            if (ustan13.porivnati()) { g_spracmem2 = true; }
          }
          if (dayofweek == 7) {                   //неділя
            if (ustan6.porivnati()) { g_spracmem1 = true; }
            if (ustan16.porivnati()) { g_spracmem2 = true; }
          }
          if (dayofweek == 6) {                 //субота

            if (ustan4.porivnati()) { g_spracmem1 = true; }
            if (ustan5.porivnati()) { g_spracmem1 = true; }
            if (ustan14.porivnati()) { g_spracmem2 = true; }
            if (ustan15.porivnati()) { g_spracmem2 = true; }
          }
        }

if (g_spracmem1){
  digitalWrite(5, HIGH);
}
else {
  digitalWrite(5, LOW);
}
if (g_spracmem2){
  digitalWrite(9, HIGH);
}
else {
  digitalWrite(9, LOW);
}        
    ustan1.printA();
    ustan2.printA();
    ustan3.printA();
    ustan4.printA();
    ustan5.printA();
    ustan6.printA();

    ustan11.printA();
    ustan12.printA();
    ustan13.printA();
    ustan14.printA();
    ustan15.printA();
    ustan16.printA();
  
  }
}

void deshufrator(){
  if (inChar!=(char)34){
             tempstr+=inChar;
             }
  else{
             ++sendtype;
             tempstr="";
  }
    switch (sendtype)
    {
      break;  
    case 1:
       telefon= tempstr;
      break;
    case 6:
    EEPROM.update(62+lineadres, inChar);
    ++lineadres;
      break;
    } 
}

void updateSerial()
{
  
    while (Serial.available()) 
  {
    inChar = Serial.read();
      mySerial.write(inChar);
      deshufrator();
      sta=millis();
    
   }
   while(mySerial.available()) 
  {
    inChar = mySerial.read();
      Serial.write(inChar);
      deshufrator();
       sta=millis();
    
   }
   if(lineadres){
   EEPROM.update(61, lineadres);
   lineadres=0;
   }
}
