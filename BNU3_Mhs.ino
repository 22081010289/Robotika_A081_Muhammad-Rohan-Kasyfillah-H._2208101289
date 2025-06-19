/*************************************
 * Program : ROBOT BNU Generasi 3
 * ***********************************/
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

String Topic;
String Payload;

const char* ssid = "Wifi";
const char* password = "Password_Wifi";

// Motor Kiri
int motor1Pin1 = 22; 
int motor1Pin2 = 23; 
int enable1Pin = 21; 

// Motor Kanan
int motor2Pin1 = 17; 
int motor2Pin2 = 18; 
int enable2Pin = 16; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int pwmChanne2 = 0;
const int resolution = 8;
const int dutyCycle = 255;

#define mqttServer "broker.emqx.io"
#define mqttPort 1883

WiFiServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);


void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  /* we got '0' -> Stop */
  if ((char)payload[0] == '0') {
    Serial.println("Robot Berhenti Rek.. !!!");  
    // Stop
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle);
  } 
  /* we got '1' -> Maju */
  if ((char)payload[0] == '1') {
    Serial.println("Robot Maju Rek !!!");  
    // Maju
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  } 
  /* we got '2' -> Kanan */
  if ((char)payload[0] == '2') {
    Serial.println("Robot Belok Kanan Rek!!!");  
    // Belok kanan
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  } 
  /* we got '3' -> Kiri */
  if ((char)payload[0] == '3') {
    Serial.println("Robot belok Kiri Rek..!!!");  
    // Belok kiri
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);  
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  } 
  /* we got '4' -> Mundur */
  if ((char)payload[0] == '4') {
    Serial.println("Robot Mundur !!!");  
    // Mundur 
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);  
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  } 
}
  
void setup() {
  Serial.begin(115200);
  delay(10);

  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // Konfigurasikan fungsi PWM
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChanne2, freq, resolution);

  // menentukan saluran ke GPIO untuk dikontrol
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);

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
 
  server.begin();
  Serial.println("Server started");
 
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 

  // Connect to Server IoT (CloudMQTT)

  client.setServer(mqttServer, mqttPort);
  client.setCallback(receivedCallback);
 
  while (!client.connected()) {
    Serial.println("Connecting to CLoudMQTT...");
 
//    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    if (client.connect("ke Broker")){
 
      Serial.println("connected");
 
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }

  }
  client.subscribe("Maju");
  client.subscribe("Kanan");
  client.subscribe("Kiri");
  client.subscribe("Mundur");
  client.subscribe("Stop");
}
 
void loop() { 

  client.loop();
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  } 
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  } 
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  if (request.indexOf("/IN_1on") > 0)  {
    Serial.println("Robot Maju Rek !!!");  
    // Maju
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  }
  if (request.indexOf("/IN_1off") >0)  {
    Serial.println("Robot Berhenti Rek.. !!!");  
    // Stop
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle);
  }
   if (request.indexOf("/IN_2on") > 0)  {
    Serial.println("Robot Belok Kanan Rek!!!");  
    // Belok kanan
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  }
  if (request.indexOf("/IN_2off") >0)  {
    Serial.println("Robot Berhenti Rek.. !!!");  
    // Stop
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle);
  }
   if (request.indexOf("/IN_3on") > 0)  {
    Serial.println("Robot belok Kiri Rek..!!!");  
    // Belok kiri
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);  
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  }
  if (request.indexOf("/IN_3off") >0)  {
    Serial.println("Robot Berhenti Rek.. !!!");  
    // Stop
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle);
  }
   if (request.indexOf("/IN_4on") > 0)  {
    Serial.println("Robot Mundur !!!");  
    // Mundur 
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);  
    delay(200);
    ledcWrite(pwmChannel, dutyCycle);
  }
  if (request.indexOf("/IN_4off") >0)  {
    Serial.println("Robot Berhenti Rek.. !!!");  
    // Stop
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle);
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("</head>");
  client.println("<body bgcolor = \"#f7e6ec\">"); 
  client.println("<hr/><hr>");
  client.println("<h4><center> Robot BNU 4.0  </center></h4>");
  client.println("<hr/><hr>");
  client.println("<br><br>");
  client.println("<br><br>");
  client.println("<center>");
  client.println("ROBOT");
  client.println("<a href=\"/IN_1on\"\"><button>Maju </button></a>");
  client.println("<a href=\"/IN_2on\"\"><button>Belok Kanan </button></a><br />");
  client.println("</center>"); 
  client.println("<br><br>");
  client.println("<center>"); 
  client.println("ROBOT");
  client.println("<a href=\"/IN_3on\"\"><button>Belok Kiri </button></a>");
  client.println("<a href=\"/IN_4on\"\"><button>Mundur </button></a><br />");
  client.println("</center>"); 
  client.println("<br><br>");
  client.println("<center>"); 
  client.println("<a href=\"/IN_1off\"\"><button>Stop </button></a><br />");
  client.println("</center>"); 
  client.println("<br><br>");
  client.println("<center>"); 


  client.println("<table border=\"5\">");
  client.println("<tr>");
  
  //=====================
  if (digitalRead(motor1Pin1))
         { 
           client.print("<td>Maju = ON</td>");        
         }
  else
          {
            client.print("<td>Maju = OFF</td>");
          }     
  client.println("<br />");
  //======================           
  if (digitalRead(motor1Pin2))
          { 
           client.print("<td>Mundur = ON</td>");
          }
  else
          {
            client.print("<td>Mundur = OFF</td>");
          }
  client.println("</tr>");
  //=======================
  if (digitalRead(motor2Pin1))
          { 
           client.print("<td>Belok Kanan = ON</td>");
          }
  else
          {
            client.print("<td>Belok Kanan = OFF</td>");
          }
  client.println("<br />");
  //=======================
  if (digitalRead(motor2Pin2))
          { 
           client.print("<td>Belok Kiri = ON</td>");
          }
  else
          {
            client.print("<td>Belok Kiri = OFF</td>");
          }
  client.println("</tr>");
  //=======================

  client.println("<tr>"); 
  client.println("</table>");
  client.println("</center>");
  client.println("</html>"); 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println(""); 

}
