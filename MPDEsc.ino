#include <ESP8266WiFi.h>
#include <FastLED.h>
#include "ClickButton.h"

#define MPD_HOST "10.208.42.101"
#define MPD_PORT 6600
#define WIFI_SSID "NurdSpace"
#define WIFI_KEY "wiljewelwetenhe"

WiFiClient client;
ClickButton button = ClickButton(D1, LOW, CLICKBTN_PULLUP);
CRGB led[1];

bool nextLight = false;
int nextLightTriggerd = 0;
int nextLightTriggerdFade = 0;
int nextLightTriggerdFadeStep = 255;

void setup()
{
  FastLED.addLeds<NEOPIXEL, D3>(led, 1);
  
  pinMode(D1, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_KEY);
  
  led[0] = CRGB::Red; FastLED.show();
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  led[0] = CRGB::Green; FastLED.show();
}

void loop() {
 button.Update();

  // Fade from 255 to 0 after single button press
  if (nextLight) {
     EVERY_N_MILLISECONDS( 10 ) { 
    
      if (nextLightTriggerdFadeStep - 1 != 0) {
         nextLightTriggerdFadeStep = nextLightTriggerdFadeStep - 1;
         led[0].red = nextLightTriggerdFadeStep;
         led[0].green = nextLightTriggerdFadeStep;
         led[0].blue = nextLightTriggerdFadeStep;
         
      } else {
        nextLightTriggerdFadeStep = 255;
        nextLight = false;
        nextLightTriggerd = 0;
        led[0] = CRGB::Black;
      }
   }
  } else { 
    // Leipe space effect
    EVERY_N_MILLISECONDS( 100 ) led[0] = CHSV(random8(), random8(), random8());
  }
 
 if (button.clicks == 1) { // One click = next song
  
  led[0] = CRGB::White;

  if (!client.connect(MPD_HOST, MPD_PORT)) {
    led[0] = CRGB::Red;
    return;
  }
    Serial.println("CLICK"); 
    client.print("next\r\n");
    nextLightTriggerd = millis();
    nextLight= true;   
  }

  FastLED.show();
}
