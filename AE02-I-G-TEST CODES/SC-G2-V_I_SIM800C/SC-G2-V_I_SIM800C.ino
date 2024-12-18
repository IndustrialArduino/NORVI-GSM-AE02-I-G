/*
 * 
 * AE02 GSM I FINAL 
 * RTC Check
 * RS485
 * SIM800C
 * All Output Turn ON Series
 * All input status serial print

 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>

#define ANALOG_PIN_0 36

#define INPUT1 35
#define INPUT2 34
#define INPUT3 21
#define INPUT4 14
#define INPUT5 13
#define INPUT6 4
#define INPUT7 5
#define INPUT8 15

#define OUTPUT1 12
#define OUTPUT2 2

#define RS485_RX 25
#define RS485_TX 19
#define RS485_FC 22

#define GSM_RX 33
#define GSM_TX 32

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float shunt_resistance = 100.0; // Ohms
float load_resistance = 10.0; // Load resistor

Adafruit_ADS1115 ads1;

int analog_value = 0;

int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);

 
  return analog_value     ; //Read analog
  
}

unsigned long int timer1 = 0;


void setup() {
 
  Serial.begin(115200);

  Serial.println("Hello");
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); 
  Serial2.begin(9600, SERIAL_8N1, GSM_TX, GSM_RX); 
 
  pinMode(RS485_FC, OUTPUT);
  digitalWrite(RS485_FC, HIGH);  
  
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);
  
  Wire.begin(16,17);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();

  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }
    ads1.setGain(GAIN_ONE);  

  Serial.println("Testing Modem");
  
  timer1 = millis();
  Serial2.println("AT");
  while(millis()<timer1+10000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+CPIN?");
  while(millis()<timer1+10000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
  timer1 = millis();
  Serial2.println("AT+GSN");
  while(millis()<timer1+10000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
   Serial.println("Testing Modem Done");

  digitalWrite(RS485_FC, HIGH);  
  
}


void loop() {

 int16_t adc0, adc1, adc2, adc3;


 while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }
  delay(200);
  

  Serial.print(digitalRead(INPUT1));Serial.print(digitalRead(INPUT2));Serial.print(digitalRead(INPUT3));Serial.print(digitalRead(INPUT4));Serial.print(digitalRead(INPUT5));Serial.print(digitalRead(INPUT6));Serial.print(digitalRead(INPUT7));Serial.print(digitalRead(INPUT8));
  Serial.println(""); 

  adc0 = ads1.readADC_SingleEnded(0);
  adc1 = ads1.readADC_SingleEnded(1);
  adc2 = ads1.readADC_SingleEnded(2);
  adc3 = ads1.readADC_SingleEnded(3);
 
  float voltage0 = adc0 * 0.155/ 1000*500; 
  float voltage1 = adc1 * 0.155 / 1000*500; 
  float voltage2 = adc2 * 0.155/ 1000*500; 
  float voltage3 = adc3 * 0.155 / 1000*500; 

  float current0 = voltage0 / shunt_resistance;
  float current1 = voltage1 / shunt_resistance;
  float current2 = voltage2 / shunt_resistance;
  float current3 = voltage3 / shunt_resistance;

  Serial.print("Current 0  : "); Serial.print(current0); Serial.println(" mA");
  Serial.print("Current 1  : "); Serial.print(current1); Serial.println(" mA");
  Serial.print("Current 2  : "); Serial.print(current2); Serial.println(" mA");
  Serial.print("Current 3  : "); Serial.print(current3); Serial.println(" mA");

  Serial.println(""); 
  Serial.print("Push button  ");Serial.println(readSwitch());
  Serial.println(""); 

  
  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  delay(150);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);
  delay(150);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
   
  digitalWrite(RS485_FC, HIGH);                   
  delay(100);
  Serial1.println(F("RS485 01 SUCCESS"));   
  delay(100);                                
  digitalWrite(RS485_FC, LOW) ;                    

                                             
  delay(100);     
  
  while (Serial1.available()) {  
    char c = Serial1.read();     
    Serial.write(c);             
  }
   
 delay(100);
 
 Serial.println("-----------------------------------------------------------------------");
 
}
