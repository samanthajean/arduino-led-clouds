// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
  public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2, Color3;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t StepsBtwn;  // total number of steps in the transition
    uint16_t Index;  // current step within the pattern
    uint16_t Quarter = float(numPixels() / 4);  // quarter of total LED pixels

    uint32_t noColor = Color(0, 0, 0);
    uint32_t lightningFlash = Color(255, 175, 50);
    uint32_t turquoise = Color(43, 180, 109);
    uint32_t magenta = Color(255, 0, 255);
    uint32_t tulip = Color(255, 0, 150);
    uint32_t deepViolet = Color(150, 0, 255);

    uint16_t isComplete;
    uint16_t nextStep;
    uint16_t lastCycle;
    uint16_t startCycle;

    void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
      : Adafruit_NeoPixel(pixels, pin, type)
    {
      OnComplete = callback;
    }

    // Update the pattern
    void Update()
    {
      if ((millis() - lastUpdate) > Interval) // time to update
      {
        //Serial.println("Update Index:");
        //Serial.println(Index);
        //Serial.println("Last Update:");
        //Serial.println(lastUpdate);
      
        lastUpdate = millis();
        switch (ActivePattern)
        {
          case SOLID_FILL:
            SolidFillUpdate();
            break;
          case RAINBOW_CYCLE:
            RainbowCycleUpdate();
            break;
          case THEATER_CHASE:
            TheaterChaseUpdate();
            break;
          case COLOR_WIPE:
            ColorWipeUpdate();
            break;
          case SCANNER:
            ScannerUpdate();
            break;
          case FADE:
            FadeUpdate();
            break;
          case LIGHTNING:
            LightningUpdate();
            break;
          case BREATH_SYNC:
            BreathSyncUpdate();
            break;
          default:
            break;
        }
      }
    }

    // Increment the Index and reset at the end
    void Increment()
    {
      if (Direction == FORWARD)
      {
        Index++;
        if (Index >= TotalSteps)
        {
          Index = 0;
          if (OnComplete != NULL)
          {
            OnComplete(); // call the comlpetion callback
          }
        }
      }
      else // Direction == REVERSE
      {
        --Index;
        if (Index <= 0)
        {
          Index = TotalSteps - 1;
          if (OnComplete != NULL)
          {
            OnComplete(); // call the comlpetion callback
          }
        }
      }
    }

    // Reverse pattern direction
    void Reverse()
    {
      if (Direction == FORWARD)
      {
        Direction = REVERSE;
        Index = TotalSteps - 1;
      }
      else
      {
        Direction = FORWARD;
        Index = 0;
      }
    }

    // Initialize for a SolidFill
    void SolidFill(uint32_t color1, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = SOLID_FILL;
      Color1 = color1;
      Interval = interval;
      TotalSteps = numPixels();
      Index = 0;
    }

    // Update the Solid Fill Pattern
    void SolidFillUpdate()
    {
      ColorSet(Color1);
      show();
      Increment();
    }

    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = RAINBOW_CYCLE;
      Interval = interval;
      TotalSteps = 255;
      Index = 0;
      Direction = dir;
    }

    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
      
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        //Serial.print("Rainbow Index:");
        //Serial.println(Index);
      }
      show();
      Increment();
    }

    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = THEATER_CHASE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
    }

    // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if ((i + Index) % 3 == 0)
        {
          setPixelColor(i, Color1);
        }
        else
        {
          setPixelColor(i, Color2);
        }
      }
      show();
      Increment();
    }

    // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = COLOR_WIPE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color;
      Index = 0;
      Direction = dir;
    }

    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
      setPixelColor(Index, Color1);
      show();
      Increment();
    }

    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval)
    {
      ActivePattern = SCANNER;
      Interval = interval;
      TotalSteps = (numPixels() - 1) * 2;
      Color1 = color1;
      Index = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if (i == Index)  // Scan Pixel to the right
        {
          setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - Index) // Scan Pixel to the left
        {
          setPixelColor(i, Color1);
        }
        else // Fading tail
        {
          setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }
      show();
      Increment();
    }

    // Initialize for a Fade
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = FADE;
      Interval = interval;
      TotalSteps = steps;
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
    }

    // Update the Fade Pattern
    void FadeUpdate()
    {
      ColorSet(FadeColor(Color1,Color2, TotalSteps, Index));
      show();
      Increment();
    }

    void Lightning(uint32_t color1, uint16_t steps, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = LIGHTNING;
      Interval = interval;
      TotalSteps = steps;
      Color1 = color1;
      Index = 0;
      Direction = dir;
    }

    void LightningUpdate()
    {
      uint16_t delayMultiplier = random(6, 10);
      Serial.println("Lightning Index:");
      Serial.println(Index);
      
      //uint16_t n = random(0,3);
      uint16_t n = SwitchPicker(isComplete, lastCycle, nextStep);
      Serial.print("N after Switch Picker:");
      Serial.println(n);

      if(n != lastCycle) {
         startCycle = millis();  
      }
      
      switch(n) {
        case 0: 
          Serial.println("lighting case 0");
          ColorSet(noColor);
          isComplete = 0;
          lastCycle = n;
          Serial.print("millis:");
            Serial.println(millis());
            Serial.print("startCycle:");
            Serial.println(startCycle);
          if((millis() - startCycle) < (Interval * delayMultiplier)) {
            nextStep = 0;
          } else {
            nextStep = 1;  
          }
            
          break;
        case 1:
          if(nextStep % 2 == 0) {
            Serial.println("lightning case 1.2");
            setPixelColor((Quarter - 1), Color1);
            setPixelColor((Quarter + 2), Color1);
            isComplete = 1;
            
            lastCycle = n;
            //nextStep++;
          } else {
            Serial.println("lighting case 1.1");
            fill(Color1, Quarter, Quarter);
            lastCycle = n;
            isComplete = 0;
            if((millis() - startCycle) > (Interval * 2)) {
                nextStep++;
            }
          }
          break;
        case 2:
          switch(nextStep) {
            case 1: 
              Serial.println("lighting case 2.1");
              fill(Color1, 0, (numPixels()/2));
              isComplete = 0;
              lastCycle = n;
              nextStep++;
              break;
            case 2: 
              Serial.println("lighting case 2.2");
              fill(Color1, 0, (numPixels()));
              isComplete = 0;
              lastCycle = n;
              if((millis() - startCycle) > (Interval * 2)) {
                nextStep++;
              }
              break;
            case 3: 
              Serial.println("lighting case 2.3");
              fill(noColor, (numPixels() - Quarter), numPixels());
              isComplete = 0;
              lastCycle = n;
              nextStep++;
              break;
            case 4: 
              Serial.println("lighting case 2.4");
              fill(noColor, ((numPixels()/2) - 1), numPixels());
              isComplete = 0;
              lastCycle = n;
              nextStep++;
              break;
            case 5: 
              Serial.println("lighting case 2.5");
              fill(noColor, (Quarter / 2), numPixels());
              isComplete = 1;
              lastCycle = n;
              //nextStep++;
              break;
            default:
              break;
          }
          
          break;
        default:
          break;
      }
//      ColorSet(noColor);
//      show();
//      delay(Interval / 2);
//      BadaBoom(Color1);
//      ColorSet(noColor);
//      show();
//      Serial.println(delayMultiplier);
//      delay(Interval * delayMultiplier);
//      Rumble(Color1);
//      ColorSet(noColor);
//      show();
//      Serial.println(delayMultiplier);
//      delay(Interval * delayMultiplier);

      Serial.print("isComplete:");
      Serial.println(isComplete);
      Serial.print("nextStep:");
      Serial.println(nextStep);
      Serial.print("n:");
      Serial.println(n);
      
      show();
      Increment();
    }

     uint16_t SwitchPicker(uint16_t isComplete, uint16_t lastCycle, uint16_t nextStep) {
      uint16_t switchNum;
      Serial.print("Switch isComplete:");
      Serial.println(isComplete);
      Serial.print("Switch lastCycle:");
      Serial.println(lastCycle);
      Serial.print("Switch nextStep:");
      Serial.println(nextStep);
      if((Index == 0) || (isComplete == 1)) {
          switchNum = 0;
//          Serial.println("SwitchPicker If");
        } else if ((lastCycle == 0) && (nextStep == 0)) {
            switchNum = lastCycle;
        } else if ((lastCycle == 1) && (nextStep < 3)) {
          switchNum = lastCycle;
//          Serial.println("SwitchPicker If Else case 1");
        } else if ((lastCycle == 2) && (nextStep < 6)) {
          switchNum = lastCycle;
//          Serial.println("SwitchPicker If Else case 2");
        } else {
          switchNum = random(0,3);
//          Serial.println("SwitchPicker Else");
        }
      return switchNum;
    }

    void BadaBoom(uint32_t color)
    {
      Serial.println("BadaBoom running");
      Rumble(color);
      ColorSet(noColor);
      show();
      delay(300);
      fill(color, 0, (numPixels() / 2));
      show();
      delay(50);
      fill(color, 0, (numPixels()));
      show();
      delay(100);
      fill(noColor, 6, 2);
      show();
      delay(50);
      fill(noColor, 3, 3);
      show();
      delay(50);
      fill(noColor, 1, 2);
      show();
    }

    void Rumble(uint32_t color)
    {
      Serial.println("Rumble running");
      uint32_t noColor = Color(0, 0, 0);
      uint16_t rmblCount = random(0, 10);
      for (int i = 0; i < rmblCount; i++)
      {
        Serial.println(rmblCount);
        fill(color, Quarter, Quarter);
        delay(100);
        setPixelColor((Quarter - 1), color);
        setPixelColor((Quarter + 2), color);
        show();
        delay(50);
        setPixelColor((Quarter - 1), noColor);
        setPixelColor((Quarter + 2), noColor);
        show();
      }
    }


    void BreathSync(uint32_t color1, uint32_t color2, uint32_t color3, uint16_t steps, uint16_t transition, uint8_t interval)
    {
      ActivePattern = BREATH_SYNC;
      Interval = interval;
      TotalSteps = steps;
      StepsBtwn = transition;
      Color1 = color1;
      Color2 = color2;
      Color3 = color3;
      Index = 0;
    }

    void BreathSyncUpdate()
    {
       if( Index < StepsBtwn ) {
        //Fade between Color1 and Color2
        ColorSet(FadeColor(Color1, Color2, StepsBtwn, Index));
        Serial.println("CYCLE 1");
       }
       else if ( Index >= (StepsBtwn * 3) && Index < (StepsBtwn * 4) ) {
        //Fade between Color2 and Color3
        ColorSet(FadeColor(Color2, Color3, StepsBtwn, Index - (StepsBtwn * 3)));
        Serial.println("CYCLE 2");
       }
       else if ( Index >= (StepsBtwn * 6) && Index < (StepsBtwn * 7) ) {
        //Fade between Color3 and Color2
        ColorSet(FadeColor(Color3, Color2, StepsBtwn, Index - (StepsBtwn * 6)));
        Serial.println("CYCLE 3");
       }
       else if ( Index >= (StepsBtwn * 9) && Index < (StepsBtwn * 10) ) {
        //Fade between Color2 and Color1
        ColorSet(FadeColor(Color2, Color1, StepsBtwn, Index - (StepsBtwn * 9)));
        Serial.println("CYCLE 4");
       }
       else {
        // Hold current color
        ColorSet(getPixelColor(0));
        Serial.println("HOLD COLOR");
       }
       
       Serial.println("Breath Sync Index:");
       Serial.println(Index);
       show();
       Increment();
    }

    uint32_t FadeColor(uint32_t color1, uint32_t color2, uint16_t steps, uint16_t index)
    {
      // Calculate linear interpolation between Color1 and Color2
      // Optimise order of operations to minimize truncation error
      uint8_t red = ((Red(color1) * (steps - index)) + (Red(color2) * index)) / steps;
      uint8_t green = ((Green(color1) * (steps - index)) + (Green(color2) * index)) / steps;
      uint8_t blue = ((Blue(color1) * (steps - index)) + (Blue(color2) * index)) / steps;

      uint32_t fadeColor = Color(red, green, blue);
      return fadeColor;
    }
    

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
      // Shift R, G and B components one bit to the right
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, color);
      }
      show();
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
      WheelPos = 255 - WheelPos;
      if (WheelPos < 85)
      {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      else if (WheelPos < 170)
      {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      else
      {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
    }
};
