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
  servo.attach(D2);//assigning pin D2 to input pin of servo motor
  Serial.begin(115200);//Begins the serial communication at baud rate=115200(bits per second)
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
 
  // Wait for connection to the hotspot
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN,HIGH);//the LED_BUILTIN in NodeMCU is made high
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());//the IP address of the network to which it is connected is printed on serial monitor
  server.on("/turnoff",turnoff);
  server.on("/turnon",turnon);
  server.begin();//Starting the web server
  Serial.println("Web server started!");
  
}
//Function to rotate the servo motor to a specific position so that one movement of door latch takes place
 void turnoff()
 {
   Serial.println(0);//printing 0 on serial monitor
   servo.write(0);//this rotates the servo to zero degree position
   server.send(200,"text/html","<html> <h1> off</h1> </html>");//printing off in the webpage created
 }

//Function to rotate the servo motor to a specific position so that opposite movement of door latch takes place(as compared to turnoff) 
  void turnon()
 {
   Serial.println(1);//printing 1 on serial monitor
   servo.write(90);//this rotates the servo to ninety degree position
   server.send(200,"text/html","<html> <h1> on</h1> </html>");//printing on in the webpage created
 }
void loop(void)
{
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.handleClient();// to handle the actual incoming of HTTP requests, we need to call the handleClient method on the server object
  }
