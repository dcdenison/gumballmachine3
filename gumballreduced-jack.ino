// This #include statement was automatically added by the Particle IDE.
#include <AccelStepper.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>



#define HOLECOUNT 2 //How many holes are in gumball machine dispenser plate

#if HOLECOUNT == 1
    #define STEPSTOTAKE 200
#elif HOLECOUNT == 2
    #define STEPSTOTAKE 100 //100 steps
#endif

#define SYSTEM_NUM 0 //0 for the first, 1 for the second
#if SYSTEM_NUM == 0
    #define NAME "GUMBALL"
#else
    #define NAME "GUMBALL1"
#endif

#define PIXEL_PIN D6
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812

AccelStepper dispense_stepper(1,3,2);

int setbrightness=60;
int wait=5; // delay wait ms
int wait_onther_delay=40;

int current_setting = 0; //0-12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup()
{
    
    Particle.subscribe(NAME,handle_gumball,MY_DEVICES);

    dispense_stepper.setMaxSpeed(500);
    dispense_stepper.setSpeed(200);
    dispense_stepper.setAcceleration(150);
    strip.begin();
    strip.setBrightness(setbrightness);
    strip.show();
}

void loop()
{
    rainbow(wait);
}

void handle_gumball(const char *event, const char *data){
    if(String(data)=="DISPENSE"){
        Particle.publish("dispense","dispensing",PRIVATE);
        dispense();
    }
}

void dispense(){
    theaterChaseRainbow();
    dispense_stepper.move(STEPSTOTAKE);
    dispense_stepper.runToPosition();
    theaterChaseRainbow();
    //delay(2000);
}

void rainbow(uint8_t wait)
{
  static int lastUpdateMillis = 0;
  static int direction = -1;
  static int pixelIndex = 0;

  if (millis() - lastUpdateMillis >= wait)
  {
    if (pixelIndex <= 0 or pixelIndex >= 255)
    {
      direction *= -1;
    }
    pixelIndex += direction;
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor( i, Wheel((i + pixelIndex) & 255));  // not sure about this...
    }
    strip.show();
    lastUpdateMillis = millis();  //  Whoopsie!!
  }
}



//Theatre-style crawling lights with rainbow effect

void theaterChaseRainbow(void) {
  for (int j=0; j < 25; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j*10) % 255));    //turn every third pixel on
        }
        strip.show();
        delay(wait_onther_delay);
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

uint32_t Wheel(byte WheelPos) {

  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

