//=============================================
//Handle functions executed upon client request
//=============================================
void handleRoot()
{
  server.send(200, "text/html", webpageCode);
}
//---------------------------------------
//void handlePOT()
//{
// String POTval = String(analogRead(A0));
// server.send(200, "text/plane", POTval);
//}


void handleLED()
{
  String ledState = "OFF";
  String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  Serial.println(t_state);
  if (t_state == "1")
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
}

void isLit()
{
  String ledState = "OFF";

  for (int i = 0; i < strip.numPixels(); i++)
  {
    if (strip.getPixelColor(i)) {
      ledState = "ON";
      break;
    } else {
      ledState = "OFF";
    }
  }
  server.send(200, "text/plain", ledState);
}

void colorIs() {
  String color1 = String(strip.Color1, HEX);
  String color2 = String(strip.Color2, HEX);
  String color3 = String(strip.Color3, HEX);

  // Serial.println(color1, HEX);
  // Serial.println(color1);
  // color1State = "#" + color1State;
//
//   StaticJsonDocument<192> doc;
//
//    doc["ledState"] = "OFF";
//    doc["currPattern"] = "activePattern";
//
//    JsonObject cloud = doc.createNestedObject("cloud");
//    cloud["number"] = 2;
//
//    JsonObject cloud_colors = cloud.createNestedObject("colors");
//    cloud_colors["color1"] = "color1hex";
//    cloud_colors["color2"] = "color2hex";
//    cloud_colors["color3"] = "color3hex";
//
//    serializeJson(doc, server);
  
  // String colors[] = {color1, color2, color3};
  // var blob = new Blob([color1, color2, color3], {type: 'text/plain'});
  server.send(200, "text/plain", "?color1=" + color1 + "&" + "color2=" + color2 + "&" + "color3=" + color3 );
  // server.send(200, "application/json", doc );
}

void colorUpdate() {

  String n = server.arg("Color"); //Refer  xhttp.open("GET", "setColor?Color="+n, true);
  Serial.println(n);
  String hex = server.arg("Hex");

  // Convert hex to integer
  int number = (int) strtol( &hex[0], NULL, 16);
  int x = n.toInt();

  // Split them up into r, g, b values
  uint8_t b = number & 0xFF;
  uint8_t g = (number >> 8) & 0xFF;
  uint8_t r = (number >> 16) & 0xFF;

  uint32_t color = strip.Color(r, g, b);
  switch (x) {
    case 1:
      Serial.println("case 1 running");
      for (int i = 0; i < strip.numPixels(); i++)
      {
        if (strip.getPixelColor(i)) {
          strip.Color1 = color;
          strip.Update();
        }
      }
      //Update hex
      hex = "#" + hex;
      server.send(200, "text/plain", hex);
      break;
    case 2:
      Serial.println("case 2 running");
      for (int i = 0; i < strip.numPixels(); i++)
      {
        if (strip.getPixelColor(i)) {
          strip.Color2 = color;
          strip.Update();
        }
      }
      hex = "#" + hex;
      server.send(200, "text/plain", hex);
      break;
    case 3:
      Serial.println("case 3 running");
      for (int i = 0; i < strip.numPixels(); i++)
      {
        if (strip.getPixelColor(i)) {
          strip.Color3 = color;
          strip.Update();
        }
      }
      hex = "#" + hex;
      server.send(200, "text/plain", hex);
      break;
    default:
      break;
  }
  Serial.println(hex);
}

void patternUpdate() {
  String pattern = server.arg("Pattern");
  int x = pattern.toInt();

  switch (x) {
    case 0:
      strip.ActivePattern = SOLID_FILL;
      strip.Interval = 100;
      strip.Update();
      break;
    case 1:
      strip.ActivePattern = RAINBOW_CYCLE;
      strip.TotalSteps = 255;
      strip.Interval = 10;
      strip.Update();
      break;
    case 2:
      strip.ActivePattern = COLOR_WIPE;
      strip.Color1 = strip.Wheel(random(255));
      strip.TotalSteps = strip.numPixels();
      strip.Interval = 40;
      strip.Update();
      break;
    case 3:
      strip.ActivePattern = LIGHTNING;
      strip.Interval = 50;
      strip.TotalSteps = 144;
      strip.Color1 = strip.lightningFlash;
      strip.Update();
      break;
    case 4:
      strip.ActivePattern = BREATH_SYNC;
      if(strip.Color2 == 0) {
        strip.Color2 = strip.magenta;
      }
      if(strip.Color3 == 0) {
        strip.Color3 = strip.deepViolet;
      }
      strip.Interval = 100;
      strip.StepsBtwn = 12;
      strip.TotalSteps = 12 * strip.StepsBtwn;
      strip.Update();
      break;
    default:
      break;
  }



  Serial.println(strip.ActivePattern);
}

void jsonSync() {
      String json = "{\"ledState\":";
    json += server.arg("LEDstate");
    json += ",\"currPattern\":";
    json += strip.ActivePattern;
//    json += ",\"cloud\":";
//    json += bme.pressure;
//    json += ",\"gas\":";
//    json += bme.gas_resistance / 1000.0;
    json += "}";
    //Serial.println(json); // DEBUGGING
    webSocket.broadcastTXT(json.c_str(), json.length());
}
