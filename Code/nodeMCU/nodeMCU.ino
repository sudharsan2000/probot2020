
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<Servo.h>
 
// Replace with your network credentials
const char* ssid = "Redmi";
const char* password = "abcd1234";
int relay = LED_BUILTIN;
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
Servo servo;
String page = "<html> \n <body>";
void setup(void){
  servo.attach(D2);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/turnoff",turnoff);
  server.on("/turnon",turnon);
  server.begin();
  Serial.println("Web server started!");
  digitalWrite(relay,HIGH);
}
 void turnoff()
 {
   Serial.println(0);
   servo.write(0);
   server.send(200,"text/html","<html> <h1> Door locked</h1> </html>");
 }
  void turnon()
 {
   Serial.println(1);
   servo.write(90);
   server.send(200,"text/html","<html> <h1> Door unnlocked</h1> </html>");
 }
void loop(void)
{
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.handleClient();
  }
