#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>



#define LED 4  //On board LED
#define LED_COUNT 8 //How many LEDs

//---------------------------------------------------

const char* ssid = "SpectrumSetup-08";
const char* password =  "heartyseed192";

// Declare websocket client class variable
WebSocketsClient webSocket;


//---------------------------------------------------

// Pattern types supported:
enum  pattern { NONE, SOLID_FILL, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, LIGHTNING, BREATH_SYNC };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

#include "neoPatterns.h"
void StickComplete();

NeoPatterns strip(LED_COUNT, LED, NEO_GRB + NEO_KHZ800, &StickComplete);

//---------------------------------------------------/

// Allocate the JSON document
StaticJsonDocument<200> doc;

 
void setup() {
  // Connect to WiFi
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  strip.begin();
  strip.show();

//  // Initialize the screen
//  u8g2.setBusClock(100000);
//  u8g2.begin();
//  u8g2.setFont(u8g2_font_ncenB08_tr);

  // server address, port, and URL path
  webSocket.begin("192.168.1.254", 81, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  // Make sure the screen is clear
  //u8g2.clearBuffer();
  if(type == WStype_TEXT)
  {
    // Deserialize the JSON document
    //ReadBufferingStream bufferedFile(payload, 64);
    //deserializeJson(doc, bufferedFile);
    DeserializationError error = deserializeJson(doc, payload);
  
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    const int s = doc["ledState"];
    const int p = doc["currPattern"];
//    const float p = doc["pressure"];
//    const float g = doc["gas"];
    
    // Display the Data
//    u8g2.drawStr(0,10,"T:");
//    u8g2.drawStr(12,10,String(t).c_str());
//    u8g2.drawStr(0,22,"H:");
//    u8g2.drawStr(12,22,String(h).c_str());
//    u8g2.drawStr(0,34,"P:");
//    u8g2.drawStr(12,34,String(p).c_str());
//    u8g2.drawStr(0,46,"G:");
//    u8g2.drawStr(12,46,String(g).c_str());
    Serial.print("pattern:");
    Serial.println(p);
    
    Serial.print("led State:");
    Serial.println(s);

  if (s == 1)
  {

    if (strip.Color1 == NULL) {
      strip.Color1 = strip.Color(255, 255, 255);
    } else {
      strip.Color1 = strip.Color1;
    }
    strip.ActivePattern = SOLID_FILL;
    Serial.println("handleLED running");
    strip.Update();
  }
  else
  {
    strip.ActivePattern = SOLID_FILL;
    strip.Color1 = strip.Color(0, 0, 0);
    strip.Update();
  }
    // Send acknowledgement to the client
    //webSocket.sendTXT("{\"status\":\"OK\"}");
  }
  //u8g2.sendBuffer();
}

void loop() {
  webSocket.loop();
  strip.Update();
}

void StickComplete()
{
  //Serial.println("Loop complete.");
}
