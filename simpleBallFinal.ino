 /*The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
*/

//tft librarys
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735 
#include <SPI.h>

// gyrometer librarys
#include "Wire.h"
#include <MPU6050_light.h>

//tft variables
#define TFT_CS         10
#define TFT_RST        14 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         12
#define TFT_MOSI 11  // Data out
#define TFT_SCLK 13  // Clock out

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//MPU 6050
MPU6050 mpu(Wire);
unsigned long timer = 0;

//button constants
const int BUTTON_PIN = 2;  // the number of the pushbutton pin

//button variables
int lastState = HIGH; // the previous state from the input pin
int currentState;    // the current reading from the input pin

//colors
#define BLACK   0x0000
#define WHITE 0xFFFF

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Wire.begin();
  //if using a 1.44" TFT:
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab


  //gyrometer
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");


  //button
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

//ball starts in the middle
int currentPlaceY = (tft.height() / 2) - 20;
int currentPlaceX = (tft.width() / 2) - 10;

int AngleX;
int AngleY;


void loop() {
  
  //gyrometer
  mpu.update();
  
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);

  if(lastState == LOW && currentState == HIGH){
    Serial.println("The state changed from LOW to HIGH");
   
    // center ball to middle:
    currentPlaceY = (tft.height() / 2) - 20;
    currentPlaceX = (tft.width() / 2) - 10;
  }
    // save the last state
    lastState = currentState;
    
  //}*/
  
  if((millis()-timer)>10){ // print data every 10ms
  Serial.print("X : ");
  Serial.print(mpu.getAngleX());
  Serial.print("\tY : ");
  Serial.print(mpu.getAngleY());
  Serial.println("\t");
  timer = millis();  
  }

//map X and Y values to
  AngleX = map(mpu.getAngleX(), -15, 15, 0, 128);
  AngleY = map(mpu.getAngleY(), -20, 20, 0, 128);
  
  if(AngleX > currentPlaceX + 2){

    currentPlaceX = currentPlaceX + 2;
  }
  if(AngleX > currentPlaceX + 8){

    currentPlaceX = currentPlaceX + 4;
  }
  
  if(AngleX < currentPlaceX - 2){

    currentPlaceX = currentPlaceX - 2;
  }
  if(AngleX < currentPlaceX - 8){

    currentPlaceX = currentPlaceX - 4;
  }
  
  if(AngleY < currentPlaceY - 2){

    currentPlaceY = currentPlaceY + 2;
  }
  if(AngleY < currentPlaceY - 8){

    currentPlaceY = currentPlaceY + 4;
  }
  if(AngleY > currentPlaceY + 2){

    currentPlaceY = currentPlaceY - 2;
  }
  if(AngleY > currentPlaceY + 8){

    currentPlaceY = currentPlaceY - 4;
  }

  //tft updating locaton of balll
  tft.fillScreen(BLACK);
  tft.fillRoundRect(currentPlaceX, currentPlaceY, 15, 15, 5, WHITE);

//when ball hits edge of screen, out of bounds code
  if(currentPlaceX < 5){
    currentPlaceX = 6;
  }
  if(currentPlaceY < 5){
    currentPlaceY = 6;
  }
  if(currentPlaceX > 103){
    currentPlaceX = 109;
  }
  if(currentPlaceY > 103){
    currentPlaceY = 109;
  }
 }
