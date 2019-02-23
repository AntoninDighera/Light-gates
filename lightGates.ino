// reads two IR proximity/line-detectors.  reports time between triggers.
// uses interrupts.  First one should go high for detect on 2
// second detector should go high on detect on pin 3
// created by antonin.dighera@gmail.com 6/8/2018
// Last revised 2/22/2019

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128                                                  // OLED display width, in pixels
#define SCREEN_HEIGHT 32                                                  // OLED display height, in pixels                 
#define OLED_RESET    -1                                                  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

#define LOGO_HEIGHT   16                                //Stuff that the library came with that looks cool
#define LOGO_WIDTH    16                                //Stuff that the library came with that looks cool
static const unsigned char PROGMEM logo_bmp[] =         //Stuff that the library came with that looks cool
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
float x;
float y;
float z;

void sens1() { if ((t1==0) && (t2==0)) t1=micros(); } 
void sens2() { if ((t2==0) && (t1!=0)) t2=micros(); }

void setup() {
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2),sens1,RISING);
  attachInterrupt(digitalPinToInterrupt(3),sens2,RISING);
  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //Serial.begin(115200); //initializes serial connection at 115200 baud
  Serial.begin(57600); //initializes serial connection at 57600 baud
  
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);                                        // Don't proceed, loop forever
    }
    
  display.display();       // Updates display buffer 
  delay(2000);             // Pause for 2 seconds
  display.clearDisplay();  // Clear the buffer
  sass();                  // Initialize sass

}

void loop() {
  digitalWrite(4,t1?HIGH:LOW);      
  if (t2>0)
    {
        x = t2-t1;                              // Assign x to the difference between the variables t1 and t2, result will be in microseconds
        y = 1000000;                            // Assign y as 1,000,000
        z = x / y;                              // converts time from microseconds to seconds
        /*
        Serial.println("*******************");
        Serial.println(z,6);                    // Prints time in seconds with 6 decimal places
        Serial.println("SECONDS");              // Prints the string
        Serial.println("*******************");  // uncomment this if you want to use the serial monitor as well as the OLED display 
        */ 
        display.clearDisplay();                 // Clears display buffer
        display.setTextSize(2);                 // Normal 1:1 pixel scale
        display.setTextColor(WHITE);            // Draw white text
        display.setCursor(0,0);                 // Start at top-left corner
        display.println(z,6);                   // Prints time in seconds with 6 decimal places
        display.println(F( "SECONDS"));         // Prints the string
        display.display();                      // Updates display buffer
        x = 0;                                  // Sets variable to 0
        y = 0;                                  // Sets variable to 0
        z = 0;                                  // Sets variable to 0
        t1=t2=0;                                // Sets variables to 0
    }
}

void sass(void){
  
  display.clearDisplay();             // Clears display buffer
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F( "WAITING"));     // Prints the string
  display.println(F( "ON YOU..."));   // Prints the string
  display.display();                  // Updates display buffer

}
