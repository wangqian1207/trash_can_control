#include <ESP8266WiFi.h>
#include <Servo.h>

Servo myservo; // create servo object to control a servo
Servo myservo2;

const char* ssid = "wangq";
const char* password = "peterwang";
int val;
int val2;
  
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
  Serial.begin(9600); //9600
  delay(10);

  // prepare GPIO2
  //pinMode(2, OUTPUT);
  //digitalWrite(2, 0);
  myservo.attach(2);
  myservo2.attach(4);
  //pinMode(12, OUTPUT); //yellow
  pinMode(13, OUTPUT); //green
  pinMode(14, OUTPUT); //red
  //digitalWrite(12, HIGH); //initialization
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started @ ");

  // Print the IP address & instructions
  Serial.println(WiFi.localIP());
  Serial.println("To control GPIO, open your web browser.");
  Serial.println("To set GPIO 0 high, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/1");
  Serial.println("To set GPIO 0 low, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/0");
  Serial.println("To toggle GPIO 0, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/0");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    delay(100);
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request

  if (req.indexOf("/gpio/0") != -1) {
    val = 90;
    val2 = 90;
    //digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  else if (req.indexOf("/gpio/1") != -1) {
    val = 170;
    val2 = 10;
    digitalWrite(13, HIGH);
    //digitalWrite(12, LOW);
    digitalWrite(14, LOW);
  }
  else if (req.indexOf("/gpio/2") != -1) {
    val = 10;
    val2 = 170;
    digitalWrite(14, HIGH);
    digitalWrite(13, LOW);
    //digitalWrite(12, LOW);
  }
  else {
    val = 90;
    val2 = 90;
    //digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  //else {
    //Serial.println("invalid request");
    //client.print("HTTP/1.1 404\r\n");
    //client.stop();
   // return;
  //}

  // Set GPIO2 according to the request
  //digitalWrite(2, val);
  myservo.write(val);
  myservo2.write(val2);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  if (val == 90)
    s += "stowed";
  else if (val == 170)
    s += "wet";
  else if (val == 10)
    s += "dry";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(200);
  Serial.println("Client disonnected");
} 
