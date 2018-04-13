#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);


String sodt = "";
String tinnhan = "";
String mang = "";
String reg = "";
boolean goithanhcong=false;
boolean dareset=false;
String checkcpin="";
String cuphapnhantin="Noi dung tin nhan";
String sdtnt="9231";
int led13=13;
int pin_reset=7;
String sim1="";
String sim2="";
char ch; //


void setup()
{
  //reset();
  digitalWrite(led13, LOW);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  mySerial.println("ATE0\r\n");
  delay(500);
  Serial.println("Da Khoi dong OK.");
  
 }


void loop()
{
  goithanhcong= false;

  mang = "";
  reg = "";  
  Serial.println("bat dau loop");

  checkcpin=sendData("AT+CPIN?\r\n",500,false);
  checkcpin=sendData("AT+CPIN?\r\n",500,false);
  delay(2000);
  if (!chechAT(checkcpin,"READY") )
  {
       Serial.println("Chua nhan duoc sim.. dang tien hanh reset wating....");
         reset();           
  }else
  {

      
      Serial.println("bat dau loop check mang mobifone");
      
  
          mang = sendData("AT+COPS?\r\n",1000,false);
          reg = sendData("AT+CREG?\r\n",1000,false);
          if (chechAT(mang,"MOBIFONE") && chechAT(reg,"+CREG: 1,1") )
          {
            Serial.println("=====Da nhan mang Mobifone======");
            Serial.println("=====Dang kiem tra so dien thoai wating .... ");
            // In ra số ĐT
            sendData("AT+CMGF=1\r\n",1000,false);
            sendData("AT+CUSD=1,\"*3#\"\r\n",1000,false);
            sodt=sendData("\032\r\n",5000,false);
            sim1=sodt.substring(sodt.indexOf(':',20)+1,sodt.indexOf('"',20));
            Serial.print("So dien thoai ban la:");
            Serial.println(sim1); 
            Serial.println(sim2);
            Serial.println("Dang tien hanh goi tin nhan wating.....");
            if (sim1 != sim2)
            {           
                if (goitin())
                {
                    Serial.println("********************");
                    Serial.println("Goi thanh cong");
                    Serial.println("********************");
                    sim2=sim1;               
                    goithanhcong=true;
                }else
                {
                    Serial.println("Goi khong thanh cong");
                     goithanhcong=false;
                }
            
            
            } else {
               Serial.println("So dien thoai nay da goi tin nhan roi");
               goithanhcong=true;
            }
         }else
         {
            digitalWrite(led13, LOW);        
         }


      while(goithanhcong)
      {
          dareset=false;
          digitalWrite(led13, HIGH);
          delay(100);
          
              checkcpin=sendData("AT+CPIN?\r\n",1000,false);
              if (!chechAT(checkcpin,"READY") )
              {
                  Serial.println("Dang tien hanh reset wating....");
                  goithanhcong = false;
                  digitalWrite(led13, LOW);
                  reset();   
                  dareset=true;        
              }else{
                 delay(1000);
                 Serial.println("Vui long dua sim vao"); 
              }
  
            if(dareset){

              checkcpin=sendData("AT+CPIN?\r\n",500,false);
              if (chechAT(checkcpin,"READY") )
              {
                    goithanhcong = false;
                    digitalWrite(led13, LOW);
                    sim1 ="";
              
              }
              
            }
                   
        
      }

     
     digitalWrite(led13, LOW);
      Serial.println("Dang tien hanh nhan sim");

  }

}

void reset() {
  Serial.println("Start Reset");
  pinMode(pin_reset, OUTPUT);
  digitalWrite(pin_reset, LOW);
  delay(1000);
  digitalWrite(pin_reset, HIGH);
  delay(4000);
  Serial.println("End Reset");  
}

boolean chechAT(const String& command,String data)
{ 
    if(command.indexOf(data) != -1)
    { 
        return true;
    }
    else
    { 
        return false;
    }  

}

boolean goitin(){
  
  sendData("AT+CMGF=1\r\n",1000,false);
  sendData("AT+CMGS=\""+sdtnt+"\"\r\n",1000,false);
  sendData(cuphapnhantin,100,false);
  tinnhan = sendData("\032\r\n",5000,false);

if(tinnhan.indexOf("+CMGS:") != -1){
  return true;
}else{
  return false;
}
 
}
String sendData(String command, const int timeout, boolean debug)
    {
        String response = "";
        mySerial.print(command); // send the read character to the esp8266
        long int time = millis();
        while( (time+timeout) > millis())
        {
          while(mySerial.available())
          {
           // The esp has data so display its output to the serial window 
            char c = mySerial.read(); // read the next character.
            response+=c;
          }  
        }
      
        if(debug)
        {
          Serial.print(response);
        }
        return response;
    }


