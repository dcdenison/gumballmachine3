// This #include statement was automatically added by the Particle IDE.
#include <SparkFun_APDS9960.h>

#include <Wire.h>

#include <blynk.h>

#include <ledmatrix-max7219-max7221.h>

//#define DELAY 7000

#define SYSTEM_NUM 1 //0 for the first, 1 for the second
#if SYSTEM_NUM == 0
    #define GUM_NAME "GUMBALL"
#else
    #define GUM_NAME "GUMBALL1"
#endif

/*
3.3v VCC
GND GND
D0 SDA
D1 SCL

ADD 4.7k pullup resistors to I2C lines for APDS9960!!
*/

SparkFun_APDS9960 apds = SparkFun_APDS9960();

LEDMatrix *led;

int bitmapWidth = 8;            // 8 is default

String text_default = "WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   WAVE HAND OVER PINK BOX FOR GUMBALL FROM FAB@CIC   ";
String text_from_terminal;
String text;

int textLength = text.length();

// default position of the text is outside and then scrolls left
int textX = bitmapWidth;
int fontWidth = 5, space = 1;

// **************

void drawText(String s, int x)
{
  int y = 0;
  for(int i = 0; i < s.length(); i++) {
    // Adafruit_GFX method
    led->drawChar(x + i*(fontWidth+space), y, s[i], true, false, 1);
  }
}



void setup() {
    if(apds.init()){
        Particle.publish("init", "APDS up", PRIVATE);
    }
    if(apds.enableGestureSensor(false)){
        Particle.publish("init", "Gesture up", PRIVATE);
    }
    
    Time.zone(+1.00);   // Berlin
  
  // setup pins and library
  // 1 display per row, 1 display per column
  // optional pin settings - default: CLK = A0, CS = A1, D_OUT = A2
  // (pin settings is independent on HW SPI)
  led = new LEDMatrix(8, 1, D2, D3, D4); // my pins vary from the default  // 4, 1 > 1 Matrix // 8, 1 > 2 Matrix  // 12, 1 > 3 Matrix
  //led = new LEDMatrix(12, 1, D1, D2, D3); // my pins vary from the default  // 4, 1 > 1 Matrix // 8, 1 > 2 Matrix  // 12, 1 > 3 Matrix
    // > add every matrix in the order in which they have been connected <
  // the first matrix in a row, the first matrix in a column
  // vertical orientation (-90°) and no mirroring - last three args optional
  // the Wangdd22 Matrix has 4 matrix elements, arranged side-by-side
  
  // third Matrix  
 // led->addMatrix(11, 0, 0, false, false);
 // led->addMatrix(10, 0, 0, false, false);
 // led->addMatrix(9, 0, 0, false, false);
 // led->addMatrix(8, 0, 0, false, false);
  
  // second Matrix  
led->addMatrix(7, 0, 0, false, false);
led->addMatrix(6, 0, 0, false, false);
led->addMatrix(5, 0, 0, false, false);
led->addMatrix(4, 0, 0, false, false);

  // first Matrix  
  led->addMatrix(3, 0, 0, false, false);
  led->addMatrix(2, 0, 0, false, false);
  led->addMatrix(1, 0, 0, false, false);
  led->addMatrix(0, 0, 0, false, false);
}

int last_dispense = 0;

void loop() {
    
    if (text_from_terminal != "") {
        text = text_from_terminal;
        textLength = text.length();
    }
    else 
    {
        text = text_default;
        textLength = text.length();
    }

  
  if(led != NULL) {
    drawText(text, textX--);
    // text animation is ending when the whole text is outside the bitmap
    if(textX < textLength*(fontWidth+space)*(-1)) {
      // set default text position
      textX = bitmapWidth;
      // show heart
      led->flush();
      delay(333);   // 1000 is default
      // turn all pixels off (takes effect after led->flush())
      led->fillScreen(false);
    }
    // draw text
    led->flush();
    delay(10);   // 250 is default 
  }
  // animations end
  else if (led != NULL) {
    // shutdown all displays
    led->shutdown(true);
    // free memory
    delete led;
    led = NULL;
  }
    
    if (last_dispense==0 or millis()-last_dispense>4000){
        handleGesture();
    }else{
        if(apds.isGestureAvailable()){
            apds.readGesture();
        }
    }
}

char* gesture_names[8] = {"NONE", "LEFT", "RIGHT", "UP", "DOWN", "NEAR", "FAR", "ALL"};

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
        int gesture=apds.readGesture();
        if (gesture!=DIR_NONE){
            Particle.publish("DIR", gesture_names[gesture], PRIVATE);
            Particle.publish(GUM_NAME,"DISPENSE", PRIVATE);
            last_dispense=millis();
        }
        
    }
}
