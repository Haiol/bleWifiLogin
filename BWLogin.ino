#include "BluetoothSerial.h"
#include <WiFi.h>
#include <WiFiClient.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

char *ssid = "yourAP";
char *password = "yourPassword";
String id,pw;

boolean idFlag = false;
boolean pwFlag = false;
boolean Flag = false;


BluetoothSerial SerialBT;



char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

String detectString(){
  String myString="";
  while(SerialBT.available())
  {
    char myChar = (char)SerialBT.read();  //mySerial int형식의 값을 char형식으로 변환
    myString+=myChar;   //수신되는 문자열을 myString에 모두 붙임 (1바이트씩 전송되는 것을 모두 붙임)
    delay(5);           //수신 문자열 끊김 방지
  }
 return myString;
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  //data init pormet is pw:[password] id:[id]
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
 
  
  if (SerialBT.available()) {
    String temp = detectString();
    if(temp.charAt(0)=='p'&&temp.charAt(1)=='w'){
      Serial.println("init Password!");
      pw = temp.substring(3,temp.length());
      password = (char *)malloc(sizeof(char)*pw.length());
      pw.toCharArray(password, pw.length());
      pwFlag=true;
    }
    if(temp.charAt(0)=='i'&&temp.charAt(1)=='d'){
      Serial.println("init ID!");
      id = temp.substring(3,temp.length());
      ssid = (char *)malloc(sizeof(char)*id.length());
      id.toCharArray(ssid, id.length());
      idFlag=true;
    }
    Serial.println(temp);

  }
  if(pwFlag==true && idFlag==true && Flag==false){
    Serial.println(id.length());
    Serial.println(pw.length());
    Serial.println(ssid);
    Serial.println(password);
    WiFi.disconnect(true);
  //  SerialBT.close;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);
//    while (WiFi.status() != WL_CONNECTED) {
//        delay(500);
//        
//        Serial.print(".");
//    }
    Serial.println("");
    Serial.println("Successs");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Flag =true;
  }
  else if(pwFlag == true && idFlag==false && Flag == true){
    Serial.println("잘못 입력됐습니다.!!!");
  }

}
