//========================================================
//ESP32 Cloud Server
//========================================================
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

#include "webpage.h"

#define LED 4  //On board LED
#define LED_COUNT 8 //How many LEDs

//---------------------------------------------------
const char* ssid = "SpectrumSetup-08";
const char* password = "heartyseed192";


WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

//---------------------------------------------------

// Pattern types supported:
enum  pattern { NONE, SOLID_FILL, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, LIGHTNING, BREATH_SYNC };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

#include "neoPatterns.h"
void StickComplete();

NeoPatterns strip(LED_COUNT, LED, NEO_GRB + NEO_KHZ800, &StickComplete);

//const char* ledState = "OFF";
String json;

#include "handleFunctions.h"
//===================================================

void setup()
{
  Serial.begin(115200);
  //-------------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  strip.begin();
  strip.show();
  
  while(WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(500); Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //-------------------------------------------------
  server.on("/", handleRoot);
//  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
//    Serial.println("Request received");
//    request->send(200, "text/plain", "Hello from server");
//  });
  server.on("/setLED", handleLED);
  server.on("/readLED", isLit);
  server.on("/setColor", colorUpdate);
  server.on("/readColor", colorIs);
  server.on("/triggerPattern", patternUpdate);
  server.begin();
  Serial.println("HTTP server started");
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
//===================================================

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if(type == WStype_TEXT)
  {
    // processs any returned data
    Serial.printf("payload [%u]: %s\n", num, payload);
    String message = String((char*)( payload));
    Serial.println(message);

    DynamicJsonDocument doc(200);

    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("deserializationJson()  failed: ");
        Serial.println(error.c_str());
        return;
      }
    JsonObject obj = doc.as<JsonObject>();

      String json = "{\"ledState\":";
      json += obj["ledState"].as<String>();
      json += ",\"currPattern\":";
      json += obj["currPattern"].as<String>();
      json += "}";
      Serial.print("json:");
      Serial.println(json);
      webSocket.broadcastTXT(json.c_str(), json.length());
  }
}

void loop(void)
{
  webSocket.loop();
  server.handleClient(); delay(1);
  


//   StaticJsonDocument<192> doc;
//
//    doc["ledState"] = "OFF";
//    doc["currPattern"] = "activePattern";
//
//    JsonObject cloud = doc.createNestedObject("cloud");
//    cloud["number"] = 2;
//
//    JsonObject cloud_colors = cloud.createNestedObject("colors");
//    cloud_colors["color1"] = "clor1hex";
//    cloud_colors["color2"] = "color2hex";
//    cloud_colors["color3"] = "color3hex";
//
//    serializeJson(doc, server);

  
    
    strip.Update();
}

void StickComplete()
{
  //Serial.println("Loop complete.");
}
