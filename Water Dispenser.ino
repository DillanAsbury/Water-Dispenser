#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_BusIO_Register.h>

#include <Adafruit_MonoOLED.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -----
// InterruptRotator.ino - Example for the RotaryEncoder library.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// -----

// This example checks the state of the rotary encoder in the loop() function.
// The current position is printed on output when changed.

// Hardware setup:
// Attach a rotary encoder with output pins to A2 and A3.
// The common contact should be attached to ground.
// For bottle filling project you will need a 0.96" OLED Display and a 5V water pump
// Hardware can be found on GitHub page in README

#include <RotaryEncoder.h>
#define touch 12
#define RELAY 8
#define led 13

// Setup a RotaryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);

int touchState;
int lastTouchState;
boolean printed = false;

// 'White on Black copy 2', 128x64px
// My logo
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x07, 0xc0, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x0f, 0xf0, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 0xf8, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x3c, 0x3c, 0x00, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x80, 0x70, 0x1c, 0x01, 0xc0, 0x70, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x60, 0x0f, 0xff, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x18, 0x03, 0x80, 0x60, 0x0f, 0xff, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x60, 0x0f, 0xff, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x80, 0x78, 0x1c, 0x01, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x3c, 0x7c, 0x00, 0xf1, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x7f, 0xf8, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0xff, 0xf0, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0xe3, 0xc0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x07, 0xe0, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x3f, 0xf8, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x00, 0x38, 0x3c, 0x01, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x78, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x60, 0x0e, 0x01, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x60, 0x0e, 0x01, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x00, 0x78, 0x1c, 0x01, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x3f, 0xfc, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x1f, 0xf8, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x0f, 0xe0, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x0f, 0xe1, 0xe0, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x1f, 0xff, 0xc0, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xbf, 0x00, 0x3e, 0xff, 0x80, 0xf9, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x00, 0x38, 0x1f, 0x01, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0xff, 0xf0, 0x0e, 0x01, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x18, 0x03, 0xff, 0xe0, 0x0e, 0x01, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0xff, 0xe0, 0x0e, 0x01, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0xf0, 0x0e, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x80, 0x70, 0x1c, 0x01, 0xc0, 0x78, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x0f, 0x00, 0x38, 0x3c, 0x01, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x3f, 0xf8, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x1f, 0xf0, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x07, 0xe0, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Initialize the arduino with setup (only runs once)
void setup()
{
  //Set the baud rate
  Serial.begin(57600);
  // Initialize display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show my logo and name of the project
  startup_display();
  
  // Setup pin modes
  pinMode(touch, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(led,OUTPUT);
  // Assigning an initial value to lastTouchState
  lastTouchState = digitalRead(touch);
  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.
}

// Used to activate the water pump, function calculates ounces based on pump flow rate
void pour(int val, int ounces)
{
  if(val == 1)
  {
    float onFor = 1000 * ounces * 1.5; //Formula based on pump flow rate
    digitalWrite(RELAY, HIGH);
    digitalWrite(led,HIGH);
    Serial.print("Pouring for ");
    Serial.println(onFor);
    delay(onFor);
    Serial.println("Done");
    digitalWrite(RELAY,LOW);
    digitalWrite(led,LOW);
    
  }
  else
  {
    Serial.println("Switch not at ON position");
  }
}

//Display the number of ounces selected on the OLED Display
void print_ounces(int num, int fontSize, String align)
{
  int letterSize = (fontSize * 5);
  int wordSize = letterSize + fontSize;
  int len = 128;
  int width = 64;
  int x_point = 64 - (wordSize/2);
  int y_point = 32 - ((fontSize * 7) / 2);
  display.setTextSize(fontSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x_point, y_point);
  display.cp437(true);
  display.display();
}

//Accepts string input, size and alignment to format text on the OLED Display
void printOLED(String chars, int fontSize, String align)
{
  if(align.equals("CENTER"))
  {
    int letterSize = (fontSize * 5);
    int wordSize = chars.length() * letterSize + ((chars.length() - 1) * fontSize);
    int len = 128;
    int width = 64;
    int x_point = 64 - (wordSize/2);
    int y_point = 32 - ((fontSize * 7) / 2);
    display.setTextSize(fontSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x_point, y_point);
    display.cp437(true);
    display.print(chars);
    display.display();
  }  
  else if(align.equals("TOP CENTER"))
  {
    int letterSize = (fontSize * 5);
    int wordSize = chars.length() * letterSize + ((chars.length() - 1) * fontSize);
    int len = 128;
    int width = 64;
    int x_point = 64 - (wordSize/2);
    int y_point = 32 - ((fontSize * 7) / 2);
    display.setTextSize(fontSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x_point,0);
    display.cp437(true);
    display.print(chars);
    display.display();
  }
  else if(align.equals("BOTTOM CENTER"))
  {
    int letterSize = (fontSize * 5);
    int wordSize = chars.length() * letterSize + ((chars.length() - 1) * fontSize);
    int len = 128;
    int width = 64;
    int x_point = 64 - (wordSize/2);
    int y_point = width - (fontSize * 7);
    display.setTextSize(fontSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x_point,y_point);
    display.cp437(true);
    display.print(chars);
    display.display();
  }
  
}

// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A2 and A3: exactly where we need to check.
ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
}


// Main loop
void loop()
{
  
  touchState = digitalRead(touch);
  static int pos = 1;

  int newPos = encoder.getPosition();
  if (pos != newPos) 
  {
    // Sanitize input from rotary encoder if larger than 9 ounces (max size of my son's bottles)
    if(newPos > 9)
    {
      encoder.setPosition(9);
      newPos = 9;
      Serial.print(newPos);
      Serial.println();
      pos = newPos;
      printed = false;
    }
    else if(newPos < 1)
    {
      encoder.setPosition(1);
      newPos = 1;
      Serial.print(newPos);
      Serial.println();
      pos = newPos;
      printed = false;
    }
    else{
      Serial.print(newPos);
      Serial.println();
      pos = newPos;
      printed = false;
    }  
    display.clearDisplay();
    char str[4];
    sprintf(str, "%dOZ", newPos);
    printOLED(str, 4, "CENTER");

    // Just to show, that long lasting procedures don't break the rotary encoder:
    // When newPos is 66 the ouput will freeze, but the turned positions will be recognized even when not polled.
    // The interrupt still works.
    // The output is correct 6.6 seconds later.
  }
  if(touchState != lastTouchState && !printed)
  {
    pour(1, pos);
    printed = true;
  }
  
  // Display logo and project title on OLED
  void startup_display()
  {
    display.clearDisplay();
    display.drawBitmap(0,0,myBitmap, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    display.display();
    delay(3000);
    display.clearDisplay();
    printOLED("WATER", 4,"TOP CENTER");
    printOLED("PUMP", 4,"BOTTOM CENTER");
    delay(3000);
    display.clearDisplay();
    display.display();
  }
}
