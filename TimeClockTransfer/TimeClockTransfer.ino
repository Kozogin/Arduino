#include <Arduino.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>
#include <EEPROM.h>
int pivsec, tkimn, yas=255, inter=1, dark=0, perevid=0;
DS3231 clock;
RTCDateTime dt;
unsigned long sta;
bool krapka=false ;
byte lito;
String g_chas;
int hourg,minutex,dayd, mou, week, yea;

#define  kimnata 2     //2 нога дс18б20
OneWire oneWire_kimnata(kimnata);
DallasTemperature sensor_kimnata(&oneWire_kimnata);


void setup() {
  //Serial.begin(9600);
     pinMode(3, OUTPUT); //живлення 18б20
     pinMode(4, OUTPUT);
     pinMode(5, OUTPUT);
     pinMode(7, OUTPUT);
     pinMode(8, OUTPUT);
     pinMode(12, OUTPUT);
     pinMode(13, OUTPUT);
     pinMode(A0, OUTPUT);
     pinMode(A1, OUTPUT);// крапка
     pinMode(A6, INPUT);
     
     pinMode(6, OUTPUT); //шим строби
     pinMode(9, OUTPUT);
     pinMode(10, OUTPUT);
     pinMode(11, OUTPUT);
     
      digitalWrite(3, HIGH); 
      digitalWrite(4, HIGH); 
      digitalWrite(5, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);

     analogWrite(6, yas);
     analogWrite(9, yas);
     analogWrite(10, yas);
     analogWrite(11, yas);

   clock.begin();  
    //clock.setDateTime(__DATE__, __TIME__);
    //clock.setDateTime(2019, 6, 30, 11, 52, 0); 
    ///////EEPROM.update(1, 0);//adres value litofalse
temter();
lito = EEPROM.read(1);               //lito = 0 ce zuma
g_chas=clockread(true);              //зчитуємо дату
zimalito();                          //перевіряємо чи потрібно перевід часу
}
//--------------------------------------------------------------------
void zimalito(){// lito = 0 ce zuma
  perevid=0;
    switch (mou){
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:  
                    case 9:
                           if (lito==0){
                              perevid=1;
                           }             //літні місяці значить лшеш=1
              break;      
                    case 1:
                    case 2:
                    case 11:
                    case 12: 
                           if (lito==1){
                              perevid=-1;
                           }            //зимові літо=0
              break;                           
                    case 3:
                            if (dayd-week<25){   //перевіряємо до 25 березня, 
                             if (lito==1){       //якщо час літній вернути зимовий
                              perevid=-1;
                             }
                            }
                           if (dayd-week>24){   //перевіряємо чи в березні була остання неділя
                               if (lito==0){    //якщо була а індекс літо=0 значить переводимо
                                  perevid=1;
                               }
                           }
                           if (dayd-week>=18 && dayd-week<=24 && week==7 && hourg>2 && hourg<23){
                               if (lito==0){
                                  perevid=1;
                               }                //перевіряємо останню неділю березня
                           }                    //якщо година з 3 до 22 переходимо на літній
              break;      
                    case 10:
                           if (dayd-week<25){
                             if (lito==0){
                              perevid=1;
                             }
                            }
                           if (dayd-week>24){
                               if (lito==1){
                                  perevid=-1;
                               }
                           }
                           if (dayd-week>=18 && dayd-week<=24 && week==7 && hourg>3 && hourg<24){
                               if (lito==1){
                                  perevid=-1;
                               }
                           }
              break; 
   }
 

}
//--------------------------------------------------------------------
String clockread(bool datati){ //фалсе година тру дата
 
 String chuslo;
dt = clock.getDateTime();
hourg=dt.hour;
minutex=dt.minute ;
dayd=dt.day;
mou=dt.month;
yea=dt.year;
week=dt.dayOfWeek;
//--------------
if (!datati){
if (hourg>9){
      if (minutex>9){
        chuslo=static_cast<String>(hourg)+static_cast<String>(minutex);
      }
      else{
        chuslo=static_cast<String>(hourg)+"0"+static_cast<String>(minutex);
      }
}
else{
      if (minutex>9){
        chuslo="a"+static_cast<String>(hourg)+static_cast<String>(minutex);
      }
      else{
        chuslo="a"+static_cast<String>(hourg)+"0"+static_cast<String>(minutex); 
      }
   
}

} 
//----------------
else{
if (mou>9 && dayd>9){
chuslo=static_cast<String>(dayd)+static_cast<String>(mou);
}

      if (dayd>9 && mou<10){
        chuslo=static_cast<String>(dayd)+"a"+static_cast<String>(mou);
      }
      if (dayd<10 && mou>9){
        chuslo="a"+static_cast<String>(dayd)+static_cast<String>(mou);
      }
  
      if (dayd<10 && mou<10){
        chuslo="a"+static_cast<String>(dayd)+"a"+static_cast<String>(mou);
      } 
}   

return chuslo;
}
//------------------------------------------------------------------------
void krapkaf(){
  krapka=!krapka;
  if (krapka){
    digitalWrite(A1, HIGH);
  }
  else{
    digitalWrite(A1,LOW );
  }
}

//-------------------------------------------------------------------------
void temter(){
  sensor_kimnata.requestTemperatures();
  tkimn = sensor_kimnata.getTempCByIndex(0);
  if (tkimn>9) {
    g_chas=static_cast<String>(tkimn)+'g'+'c';
  }
  else{
    g_chas='p'+static_cast<String>(tkimn)+'g'+'c';
  }
}
//-------------------------------------------------------------------------
void segment(int a){
  switch (g_chas[a]){
                    case '1':
      digitalWrite(4, HIGH); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
                    break;
                    case '2':
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      digitalWrite(A0, LOW);
                    break;
                    case '3':
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, LOW);
                    break;
                    case '4':
      digitalWrite(4, HIGH); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(A0, LOW);
                    break;
                    case '5':
      digitalWrite(4, LOW); 
      digitalWrite(5, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(A0, LOW);
                    break;
                    case '6':
      digitalWrite(4, LOW); 
      digitalWrite(5, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      digitalWrite(A0, LOW);
                    break;
                    case '7':
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
                    break;
                    case '8':
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      digitalWrite(A0, LOW);
                    break;
                    case '9':
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(A0, LOW);
                    break;
                    case '0':
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      digitalWrite(A0, HIGH);
                   break;
                   case '-':// minus
      digitalWrite(4, HIGH); 
      digitalWrite(5, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, LOW);
                    break;
                    case 'g':// gradus
      digitalWrite(4, LOW); 
      digitalWrite(5, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(A0, LOW);
                    break;
                    case 'c':// Celsiy
      digitalWrite(4, LOW); 
      digitalWrite(5, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      digitalWrite(A0, HIGH);
                    break;

                    default:// pusto
      digitalWrite(4, HIGH); 
      digitalWrite(5, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);

                        break;
   }
     
        switch (a){
               
                    case 0:
                        analogWrite(6, yas);
                        delay(inter);
                        analogWrite(6, 0);
                        break;
                    case 1:
                        analogWrite(9, yas);
                        delay(inter);
                        analogWrite(9, 0);
                        break;
                    case 2:
                        analogWrite(11, yas);
                        delay(inter);
                        analogWrite(11, 0);
                        break;
                    case 3:
                        analogWrite(10, yas);
                        delay(inter);
                         analogWrite(10, 0);
                        break;
                   
                     }
}
//----------------------------------------------------------------------
void perevi(){
if (hourg>2 && hourg<23){  // 23
     if (perevid==1){
      EEPROM.update(1, 1);//adres value litofalse
     clock.setDateTime(yea, mou, dayd, hourg+1, minutex, 0);
     lito=1; 
      }
     if (perevid==-1){
      EEPROM.update(1, 0);//adres value litofalse
     clock.setDateTime(yea, mou, dayd, hourg-1, minutex, 0);
     lito=0;
     }  
perevid=0;
}
}

//----------------------------------------------------------------------
void loop() {
if (millis()-sta>500){
  ++pivsec;
       if (pivsec>60){
       pivsec=0;
       }
if (pivsec<48){     
    krapkaf();
}

if (pivsec%5==0 && pivsec<48){
  g_chas=clockread(false);
}
if (pivsec==48){
  g_chas=clockread(true);
}
if (pivsec==54){
  temter();
  if (mou==3 || mou==10){
    zimalito();    
  }
                 if (perevid!=0){
                    perevi();
                 }
}

//Serial.println(analogRead(A6));
        if (analogRead(A6)>1012){
          
          ++dark;
          if (dark>4){
            dark=4;
            yas=4;
          inter=4;
          }
        }
        if (analogRead(A6)<1010){
          
          --dark;
          if (dark<-4){
            dark=-4;
            yas=255;
          inter=1;
          }
        }
sta=millis();
}
//----------------------------
for(int a=0;a<4;++a){ 
  segment(a); 
      
}
}
