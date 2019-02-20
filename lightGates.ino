// reads two IR proximity/line-detectors.  reports time between triggers.
// uses interrupts.  First one should go high for detect on 2
// second detector should go high on detect on pin 3
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

unsigned long t1=0;
unsigned long t2=0;
void sens1() { if ((t1==0) && (t2==0)) t1=micros(); } 
void sens2() { if ((t2==0) && (t1!=0)) t2=micros(); }

void setup() {
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2),sens1,RISING);
  attachInterrupt(digitalPinToInterrupt(3),sens2,RISING);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //Serial.begin(115200); //initializes serial connection at 57600 baud
  Serial.begin(57600); //initializes serial connection at 57600 baud
  
  
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    
  display.display();
  delay(2000);             // Pause for 2 seconds
  display.clearDisplay();  // Clear the buffer
  sass();

}

void loop() {
  digitalWrite(4,t1?HIGH:LOW);      
  if (t2>0)
    {
//      Serial.println("*******************");
//      Serial.println(t2-t1);
//      Serial.println("Microseconds");
//      Serial.println("*******************"); // uncomment this if you want to use the serial monitor as well as the OLED display 
        display.clearDisplay();             // Clears display buffer
        display.setTextSize(2);             // Normal 1:1 pixel scale
        display.setTextColor(WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(t2-t1);             // Prints time in microseconds (SAVE THIS FUNTIONALITY)
        display.println(F( "SECONDS"));     // Prints the string
        display.display();                  // Updates display buffer
        //delay(2000);                      // I just left this here to take up space.
        t1=t2=0;                            // Sets variables to 0
    }
}

void sass(void){
  
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F( "WAITING"));
  display.println(F( "ON YOU..."));
  display.display();

}
