#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <Wire.h>

const char *ssid = "matze";
const char *password = "123456789";
const int serverPort = 80;

int index1 = 0;
int Temparatur[8] = {1,2,3,4,5,6,7,8};
int TempAVG;

#define NODE_ADDRESS 2
#define PAYLOAD_SIZE 2
byte nodePayload[PAYLOAD_SIZE];

AsyncWebServer server(serverPort);

void setup()
{
  Serial.begin(9600);

  Wire.begin(NODE_ADDRESS);
  Wire.onReceive(receiveEvent); // erstelle ein Empfangen-Ereignis
  Wire.onRequest(requestEvent); // erstelle ein Anfrage-Ereignis
  connectToWifi();

  // Mount SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/mainSite.html", "text/html"); });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String output = request->getParam("output")->value();
    int temperature = output.substring(0, output.indexOf("/")).toInt();
    int oben = output.substring(output.indexOf("/") + 1).toInt();
    // Process the received data here
    Serial.println("Received data: temperature = " + String(temperature) + ", oben = " + String(oben));
    request->send(200, "text/plain", "Data received successfully");
  });
  
server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
  String json = "{\"temperatures\": [";
  for (int i = 0; i < 8; i++) {
    json += String(Temparatur[i]);
    if (i < 7) {
      json += ",";
    }
  }
  json += "]}";
  request->send(200, "application/json", json);
});
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/styles.css", "text/css"); });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });
  server.on("/img/pizzaofen.jpeg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/img/pizzaofen.jpeg", "image/jpeg"); });
  server.on("/img/IMG_1494.jpeg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/img/IMG_1494.jpeg", "image/jpeg"); });
  server.on("/img/definition-von-feuer.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/img/definition-von-feuer.jpg", "image/jpeg"); });
  server.on("/img/Oben.jpeg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/img/Oben.jpeg", "image/jpeg"); });
  server.on("/img/Unten.jpeg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/img/Unten.jpeg", "image/jpeg"); });

  server.begin();
}

String processor(const String &var)
{

  Serial.println(var);
  return var;
}

void loop()
{
}

void connectToWifi()
{
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void receiveEvent(int bytes)
{
  int x;
  x = Wire.read(); // lies die gesendeten Daten aus
  Temparatur[index1] = x * 2; // speichert die empfangenen Werte in einem Array ab
  index1++;
  if(index1 == 8) {
    index1=0;
    }
}

void requestEvent()
{
  nodePayload[0] = NODE_ADDRESS;
  nodePayload[1] = analogRead(A0) / 4;
  Wire.write(nodePayload, PAYLOAD_SIZE);
}