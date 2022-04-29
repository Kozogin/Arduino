#ifndef WEB_CONTROL_H
#define WEB_CONTROL_H

#include "state.h"
#include <EEPROM.h>

class WebContlol{

private:    

    char owner1Tel[14];
    char owner2Tel[14];
    char owner3Tel[14];
    char owner4Tel[14];
    char owner5Tel[14];
    char owner6Tel[14];

    String owner1check;
    String owner2check;
    String owner3check;
    String owner4check;
    String owner5check;
    String owner6check;

    char ussAc[8];
    char uss_sumAc[4];

    char zona1Tel[14];
    char zona2Tel[14];
    char zona3Tel[14];
    char zona4Tel[14];
    char zona5Tel[14];

    char password[14];

public:
    WebContlol(){}

String getPage(){

String ptr = "<!DOCTYPE html> <html>\n";

try{

  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<meta charset=\"UTF-8\">\n";
 
 ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 ptr += "<title>Alarm</title>\n";
 ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";

    ptr += "body{margin-top: 20px;background-color: #333;}\n";
    ptr += "table {color: #888;margin: 10px auto 30px;} \n";
    ptr += "h1 {color: #888;margin: 10px auto 30px;}  \n";   
    ptr += "h3 {color: #888;margin-bottom: 30px; padding-bottom: 10px; padding-right: 30px ;}\n";

    ptr += ".owner_ckd{padding: 10px 20px; margin: 0px auto 35px; background-color: #248c9c; }\n";
    ptr += ".owner {display: inline;width: 190px;background-color: #945c0c;border: none;color: white;padding: 10px 30px;text-decoration: none;font-size: 20px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;} \n";   
    ptr += "#post {display: block;width: 200px;background-color: #992121;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Сигналізація</h1> \n";

 ptr += "<form action='/' method='POST'>\n";

  ptr += "<table>\n";
    ptr += "<caption><h2>Номери власників +380671234567</h2></caption>\n";
    ptr += "<tr>  \n";
    ptr += "<th></th>\n";  
    ptr += "<th></th> \n";
    ptr += "<th></th> \n";
    ptr += "</tr>\n";    
    ptr += "<tr> \n"; 
     ptr += "<td></td>\n";  
     ptr += "<td></td>\n"; 
     ptr += "<td>call back</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
      ptr += "<td><h3>telephone main</h3></td>\n";
      ptr += "<td><input type='input' id='owner1' class='owner' name='owner1' value='" + (String)owner1Tel + "' ></td>\n";       
      ptr += "<td><input type='checkbox' "+ owner1check + " id='owner1_ckd' class='owner_ckd' name='owner1_ckd' ></td>\n";     
    ptr += "</tr>\n";

    ptr += "<tr>\n"; 
      ptr += "<td><h3>telephone owner 2</h3></td>\n"; 
      ptr += "<td><input type='input' id='owner2' class='owner' name='owner2' value='" + (String)owner2Tel + "' ></td>\n"; 
      ptr += "<td><input type='checkbox' "+ owner2check + " id='owner2_ckd' class='owner_ckd' name='owner2_ckd' ></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>telephone owner 3</h3></td>\n"; 
      ptr += "<td><input type='input' id='owner3' class='owner' name='owner3' value='" + (String)owner3Tel + "' ></td>\n"; 
      ptr += "<td><input type='checkbox' "+ owner3check + " id='owner3_ckd' class='owner_ckd' name='owner3_ckd' ></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>telephone owner 4</h3></td>\n"; 
      ptr += "<td><input type='input' id='owner4' class='owner' name='owner4' value='" + (String)owner4Tel + "' ></td>\n"; 
      ptr += "<td><input type='checkbox' "+ owner4check + " id='owner4_ckd' class='owner_ckd' name='owner4_ckd' ></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>telephone owner 5</h3></td>\n"; 
      ptr += "<td><input type='input' id='owner5' class='owner' name='owner5' value='" + (String)owner5Tel + "' ></td>\n"; 
      ptr += "<td><input type='checkbox' "+ owner5check + " id='owner5_ckd' class='owner_ckd' name='owner5_ckd' ></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>telephone owner 6</h3></td>\n"; 
      ptr += "<td><input type='input' id='owner6' class='owner' name='owner6' value='" + (String)owner6Tel + "' ></td>\n"; 
      ptr += "<td><input type='checkbox' "+ owner6check + " id='owner6_ckd' class='owner_ckd' name='owner6_ckd' ></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td></td>\n"; 
      ptr += "<td></td>\n"; 
      ptr += "<td></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td></td>\n"; 
      ptr += "<td><h2>Залишок рахунку</h2></td>\n"; 
      ptr += "<td></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>telephone code</h3></td>\n"; 
      ptr += "<td><input type='input' id='uss' class='owner' name='uss' value='" + (String)ussAc + "'></td>\n"; 
      ptr += "<td></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>залишок рахунку</h3></td>\n"; 
     ptr += "<td><input type='input' id='uss_sum' class='owner' name='uss_sum' value='" + (String)uss_sumAc + "' ></td>\n"; 
      ptr += "<td></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td></td>\n"; 
      ptr += "<td><h2>Зони</h2></td> \n";      
      ptr += "<td></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n"; 
      ptr += "<td><h3>zona name 1</h3></td>\n"; 
      ptr += "<td><input type='input' id='zona1' class='owner' name='zona1' value='" + (String)zona1Tel + "'></td>\n"; 
      ptr += "<td></td>\n"; 
    ptr += "</tr>\n"; 
    ptr += "<tr>\n";
      ptr += "<td><h3>zona name 2</h3></td>\n";
      ptr += "<td><input type='input' id='zona2' class='owner' name='zona2' value='" + (String)zona2Tel + "'></td>\n";
      ptr += "<td></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
      ptr += "<td><h3>zona name 3</h3></td>\n";
      ptr += "<td><input type='input' id='zona3' class='owner' name='zona3' value='" + (String)zona3Tel + "'></td>\n";
      ptr += "<td></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
      ptr += "<td><h3>zona name 4</h3></td>\n";
      ptr += "<td><input type='input' id='zona4' class='owner' name='zona4' value='" + (String)zona4Tel + "'></td>\n";
      ptr += "<td></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
      ptr += "<td><h3>zona name 5</h3></td>\n";
      ptr += "<td><input type='input' id='zona5' class='owner' name='zona5' value='" + (String)zona5Tel + "'></td>\n";
      ptr += "<td></td>\n";
    ptr += "</tr>\n";

    ptr += "<tr>\n";
      ptr += "<td></td>\n";
      ptr += "<td><h2>Password</h2></td> \n";     
      ptr += "<td></td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
      ptr += "<td><h3>password</h3></td>\n";
      ptr += "<td><input type='input' id='password' class='owner' name='password' value='" + (String)password + "'></td>\n";
      ptr += "<td></td>\n";
    ptr += "</tr>\n";

   ptr += "</table>\n";

   ptr += "<h3>really change</h3>\n";
   ptr += "<input type='checkbox' id='really_ckd' class='owner_ckd' name='really_ckd' >\n";
   
  ptr += "<br><INPUT TYPE='submit' id='post' VALUE='застосувати'><br><br>\n";
  ptr += "</form>\n";
  ptr += "<br> <br> <br> <br> <br> \n";
 
  ptr +="</body>\n";
  ptr +="</html>\n";

    }
		catch (...)
		{
		}
  return ptr;
}

void refreshData(){

  readData(0, owner1Tel);
  readData(15, owner2Tel);
  readData(30, owner3Tel);
  readData(45, owner4Tel);
  readData(60, owner5Tel);
  readData(75, owner6Tel);

  readCheckBox(); 

  readData(100, ussAc);
  readData(109, uss_sumAc);

  readData(115, zona1Tel);
  readData(130, zona2Tel);
  readData(145, zona3Tel);
  readData(160, zona4Tel);
  readData(175, zona5Tel);

  readData(190, password);
}

void saveOwnerTelephone(){

  saveData((String)owner1Tel, 14, 0);
  saveData((String)owner2Tel, 14, 15);
  saveData((String)owner3Tel, 14, 30);
  saveData((String)owner4Tel, 14, 45);
  saveData((String)owner5Tel, 14, 60);
  saveData((String)owner6Tel, 14, 75);
}

void saveCheckBox(String ownercheck, int addressBegin){

try{
    if(ownercheck == "checked"){
      EEPROM.write(addressBegin, 1);
    } else{
      EEPROM.write(addressBegin, 0);
    }  
  }
  catch (...)
  {
  }
}

void saveAllCheckbox(){

  saveCheckBox(owner1check, 91);
  saveCheckBox(owner2check, 92);
  saveCheckBox(owner3check, 93);
  saveCheckBox(owner4check, 94);
  saveCheckBox(owner5check, 95);
  saveCheckBox(owner6check, 96); 
}

void saveUssZona(){

  saveData((String)ussAc, 8, 100);
  saveData((String)uss_sumAc, 4, 109);
  saveData((String)zona1Tel, 14, 115);
  saveData((String)zona2Tel, 14, 130);
  saveData((String)zona3Tel, 14, 145);
  saveData((String)zona4Tel, 14, 160);
  saveData((String)zona5Tel, 14, 175);

  saveData((String)password, 14, 190);
}


void saveData(String data, int sizeData, int addressBegin){ 
   
  try{
    if(data.length() != 0){
      if(data.length() < sizeData){
        sizeData = data.length(); 
      }
      if(sizeData > 14){
        sizeData = 14;
      }

      EEPROM.write(addressBegin, (byte)sizeData);
      for (size_t i = 0; i < sizeData; i++)
      {            
        EEPROM.write(addressBegin + 1 + i, (byte)(int)data[i]); 
      }
    } else{
      EEPROM.write(addressBegin, 0);
    }
  }
  catch (...)
  {
  }
}
  

void recordArray(char* array, char* m_array){

  try{
    for (size_t i = 0; i < 14; i++)
    {
      m_array[i] = array[i];
      array[i] = 0;
    }
    m_array[13] = 0;
  }
  catch (...)
  {
  }
} 

void readData(int addressBegin, char* array){

  try{
    int sizeData = (int)EEPROM.read(addressBegin);
    if(sizeData > 14){
      sizeData = 14;
    }
    int data;

    if(sizeData != 0){
      for (size_t i = 0; i < sizeData; i++)
        {         
          data = (int)EEPROM.read(addressBegin + 1 + i);        
            array[i] = (char)data;
        }
    }
    array[13] = 0; 
  }
  catch (...)
  {
  }
}

  String getChecked(int data){
      
    return data == 1 ? "checked" : ""; 
  }

void readCheckBox(){

  try{
    int data = (int)EEPROM.read(91);
    owner1check = getChecked(data);

    data = (int)EEPROM.read(92);
    owner2check = getChecked(data);

    data = (int)EEPROM.read(93);
    owner3check = getChecked(data);

    data = (int)EEPROM.read(94);
    owner4check = getChecked(data);

    data = (int)EEPROM.read(95);
    owner5check = getChecked(data);

    data = (int)EEPROM.read(96);
    owner6check = getChecked(data); 
  }
  catch (...)
  {
  }
}

void deleteItem(char* c, int size){

  try{
    for (int i = 0; i < size; i++)
    {
      c[i] = 0;
    }
  }
  catch (...)
  {
  }
}

void deleteAll(){  

  deleteItem(owner1Tel, 14);
  deleteItem(owner2Tel, 14);
  deleteItem(owner3Tel, 14);
  deleteItem(owner4Tel, 14);
  deleteItem(owner5Tel, 14);
  deleteItem(owner6Tel, 14);  

  owner1check = "";
  owner2check = "";
  owner3check = "";
  owner4check = "";
  owner5check = "";
  owner6check = "";

  deleteItem(ussAc, 8);
  deleteItem(uss_sumAc, 4);

  deleteItem(zona1Tel, 14);
  deleteItem(zona2Tel, 14);
  deleteItem(zona3Tel, 14);
  deleteItem(zona4Tel, 14);
  deleteItem(zona5Tel, 14);

  deleteItem(password, 14); 
}

void setOwner1(char* array){
   recordArray(array, owner1Tel);
}

void setOwner2(char* array){
   recordArray(array, owner2Tel);
}

void setOwner3(char* array){
   recordArray(array, owner3Tel);
}

void setOwner4(char* array){
   recordArray(array, owner4Tel);
}

void setOwner5(char* array){
   recordArray(array, owner5Tel);
}

void setOwner6(char* array){
   recordArray(array, owner6Tel);
}
//--------------------------------------------

void setUss(char* array){
   recordArray(array, ussAc);
}

void setUssSum(char* array){
   recordArray(array, uss_sumAc);
}
//------------------------------------------

void setZona1(char* array){
   recordArray(array, zona1Tel);
}

void setZona2(char* array){
   recordArray(array, zona2Tel);
}

void setZona3(char* array){
   recordArray(array, zona3Tel);
}

void setZona4(char* array){
   recordArray(array, zona4Tel);
}

void setZona5(char* array){
   recordArray(array, zona5Tel);
}

void setPassword(char* array){
   recordArray(array, password);
}

void setOwnerCheck(state check1, state check2, state check3, state check4, state check5, state check6){
    
    setCheck(check1, owner1check);
    setCheck(check2, owner2check);
    setCheck(check3, owner3check);
    setCheck(check4, owner4check);
    setCheck(check5, owner5check);
    setCheck(check6, owner6check);
}

void setCheck(state check, String &ownercheck){

  try{
    if(check == ON) {
      ownercheck = "checked";
    } else if(check == OFF){
      ownercheck = "";
    } else{}
  }
  catch (...)
  {
  }
}

void getOwnerTelephones(std::string* array){

  try{
    array[0] = (std::string)owner1Tel;
    array[1] = (std::string)owner2Tel;
    array[2] = (std::string)owner3Tel;
    array[3] = (std::string)owner4Tel;
    array[4] = (std::string)owner5Tel;
    array[5] = (std::string)owner6Tel;
  }
  catch (...)
  {
  } 
}

void getChecks(bool* array){
  
  try{
    array[0] = owner1check == "checked" ? true : false;
    array[1] = owner2check == "checked" ? true : false;
    array[2] = owner3check == "checked" ? true : false;
    array[3] = owner4check == "checked" ? true : false;
    array[4] = owner5check == "checked" ? true : false;
    array[5] = owner6check == "checked" ? true : false;
  }
  catch (...)
  {
  } 
}

bool getCheck(int index){
  
  try{
    switch (index)
    {
      case 0:
      return owner1check == "checked";
      case 1:
      return owner2check == "checked";
      case 2:
      return owner3check == "checked";
      case 3:
      return owner4check == "checked";
      case 4:
      return owner5check == "checked";
      case 5:
      return owner6check == "checked";
    default:
      break;
    }
  }
  catch (...)
  {
  } 
  return false;
}

std::string getUssAc(){  
  return (std::string)ussAc;
}

bool isNumeric(int c){
  if(c > 47 && c < 58){
    return true;
  }
  return false;
}

int getUss_sumAc(){
  
  try{
    for (int i = 0; i < 4; i++)
    {
      if(!isNumeric((int)uss_sumAc[i])){
        return 5;
      }
    } 
    return ((String)uss_sumAc).toInt();

  }
  catch (...)
  {
  } 
  return 5;
}

void getZonas(String* array){

  try{
    array[0] = (String)zona1Tel;
    array[1] = (String)zona2Tel;
    array[2] = (String)zona3Tel;
    array[3] = (String)zona4Tel;
    array[4] = (String)zona5Tel;
  }
  catch (...)
  {
  } 
}

std::string getPassword(){
  return (std::string)password;
}

};


#endif