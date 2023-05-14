#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <stdio.h>
#include <string.h>
ESP8266WebServer server(80);
String ssid = "";
String password = "";
String server_ip = "http://172.16.29.209:3001";
String IDdevice = "";
const int pos_ssid = 0;
const int pos_pass = 150;
const int pos_server = 300;
const int pos_IDdevice = 450;
////
String name_wifi = "BUBUCHACHA";
String ssid_pass;


String Separate = "@";

unsigned long timerDelay = 5000;
String jsonString;
JSONVar myObject;
unsigned long lastTime = 0;
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);
void setup() {
  Serial.begin(115200);
  //đọc từ eerom
  delay(200);
  read(pos_ssid);
  delay(300);
  read(pos_pass);
  delay(300);
  read(pos_server);
  delay(300);
  read(pos_IDdevice);
  ////
  //Thiết lập wifi  
  WiFi.begin(ssid, password);
  delay(2000);
  if(WiFi.status() == WL_CONNECTED)
  {
      Serial.println("");
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());
  }
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(name_wifi) ? "Setup OK" : "Setup Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

// thiết lập server  
  server.on(F("/set_ssid"), set_ssid);
  server.on(F("/set_password"), set_password);

  server.on(F("/set_server"), set_server);
  server.on(F("/set_id_device"), set_IDdevice);
  server.on(F("/"),hello);
  server.begin();
}
void set_ssid(){
  String  ssid_ = server.arg("plain");
  save(pos_ssid,ssid_);
  delay(200);
  server.send(200, "text/plain", "ok! "+server_ip ); 
}
void set_password(){
  String pas_ = server.arg("plain");
  save(pos_pass,pas_);
  delay(200);
  read(pos_ssid);
  delay(200);
  read(pos_pass);
  delay(200);
  WiFi.begin(ssid, password);
  delay(2000);
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    server.send(200, "text/plain", "ok! "+server_ip ); 
  }
  else
  {
    server.send(200, "text/plain", "not_connected" + server_ip);
  }
}
void set_server()
{
  String ser_ip = server.arg("plain");
  Serial.println(server_ip);
  save(pos_server, ser_ip);
  delay(200);
  read(pos_server);
  delay(200);
  server.send(200, "text/plain", "ok! "+ser_ip ); 
}
void set_IDdevice()
{
  String ser_ip = server.arg("plain");
  Serial.println(server_ip);
  save(pos_IDdevice, ser_ip);
  delay(200);
  read(pos_IDdevice);
  delay(200);
  server.send(200, "text/plain", "ok! "+server_ip ); 
}
void save(int start_position, String str){
  EEPROM.begin(512);
  char buffer[32];
  itoa(str.length(),buffer,10);
  String str2 = buffer+Separate+str;
  Serial.println(str2);
  for(int i =0;i<str2.length();i++)
  {
    EEPROM.write(0x0F+i+start_position, str2[i]);   
  }
  EEPROM.commit();
}
void read(int start_position)
{
  EEPROM.begin(512);
  char a[10];
  int c = 0; 
  for(int i = start_position;i<start_position+10;i++)
   {
         if(char(EEPROM.read(0x0F+i)) =='@')
         {
           String x ="";
           for(int j = i+1;j<i +atoi(a)+1; j++)
           {
             x=x+char(EEPROM.read(0x0F+j));
           }

          Serial.println(x);
          switch(start_position){            
                          case pos_ssid:
                            //ssid = x.c_str();
                            ssid = x;
                            Serial.println("read_ssid "+x);
                            break;
                          case pos_pass:
                            password = x;
                            Serial.println("read_password "+x);
                            break;  
                          case pos_server:
                            server_ip = x;
                            Serial.println("set_server "+x);
                            break;   
                          case pos_IDdevice:
                            IDdevice = x;
                            Serial.println("IDdevice "+x);
                            break;                              
                                  }
         }
         else
         {
          a[c] = char(EEPROM.read(0x0F+i));
          c=c+1;
         }
   }
}
void hello()
{
  Serial.println("helllllllllllllo");
}

void loop() {
  Serial.print(ssid);
  Serial.print(password);
  Serial.print(server_ip);
  server.handleClient();
  delay(1000);
  if (WiFi.status() == WL_CONNECTED && millis()-lastTime>=timerDelay)
  {
    lastTime = millis();
    delay(200);
    WiFiClient client;
    HTTPClient https;
    String post_data = server_ip+"/data";

    https.begin(client, post_data);
    myObject["ssid"] = ssid;
    myObject["password"] = password;
    myObject["id_device"] = IDdevice;
    myObject["server_ip"] = server_ip;
    Serial.print(post_data);
    jsonString = JSON.stringify(myObject);
    Serial.print(jsonString);
    int httpCode = https.POST(jsonString);
    String payload = https.getString();
    delay(200);
    Serial.println(httpCode);
    Serial.println(payload);
    https.end();
  }
  else if(WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
    Serial.print("Connecting.......");
    delay(2000);
  }
}