#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
 
//the network credentials ssid(service set identifier-name of the network) and password must be given to connect to a network.
const char* ssid = "Redmi";
const char* password = "abcd1234";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
Servo servo;  //creating an object of class servo
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
  
}
//Function to rotate the servo motor to a specific position so that closing of door latch takes place
 void turnoff()
 {
   Serial.println(0);//printing 0 on serial monitor
   servo.write(0);//this rotates the servo to zero degree position
   server.send(200,"text/html","<html> <h1> off</h1> </html>");//printing off on the webpage created
 }
  void turnon()
 {
   Serial.println(1);
   servo.write(90);
   server.send(200,"text/html","<html> <h1> on</h1> </html>");
 }
void loop(void)
{
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.handleClient();
  }
