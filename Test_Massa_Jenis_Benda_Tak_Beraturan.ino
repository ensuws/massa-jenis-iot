#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D7;
const int LOADCELL_SCK_PIN = D8;

#define echoPin D6
#define triggerPin D5

LiquidCrystal_I2C lcd(0x27, 16, 2);

HX711 scale;

float beratAsli;
float beratBenda;
float volume;
float massaJenis;
float volumeBenda;

float ketinggian = 0;
float air = 0;

void setup() {
  Serial.begin(115200);
  
  lcd.begin();
  lcd.backlight();
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  //memanggil atau menginisialisasi pin
  scale.set_scale(106.83);                           //faktor kalibrasi
  scale.tare(25);                                    //reset the scale to 0

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  float massa= scale.get_units(25);
  beratAsli = massa - 10.0;
  beratBenda = beratAsli - 28;      //massa benda dalam air
//  if(beratAsli<=0.1)
//  {
//    beratAsli=0.0;
//  }
  if(beratBenda<=0.1)
  {
    beratBenda=0.0;
  }

   //sensor ultrasonik
  float duration, ketinggian, air, volume;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);         
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);       
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  ketinggian = (duration/2) / 29.1;
//  air = ketinggian+0.6;
  air = ketinggian*0;
//  volume = air*10.812067;
  volumeBenda = air+8; 

//  
//  if(volume>=85)
//  {
//    volume=0.0;
//  }

//  if(volumeBenda>=53)
//  {
//    volumeBenda=0.0;
//  }

    if(air>=8.5)
  {
    air=0.0;
  }

  Serial.print("Massa :"); 
  Serial.print(beratBenda); 
  Serial.println(" gram");

//  Serial.print("Tinggi Air :"); 
//  Serial.print(air); 
//  Serial.println(" cm^3");

  Serial.print("Volume :"); 
  Serial.print(volumeBenda); 
  Serial.println(" cm^3");
  
//  Serial.print("Volume :"); 
//  Serial.print(volumeBenda); 
//  Serial.println(" cm^3"); 
//
  Serial.print("Massa Jenis :"); 
  Serial.print(massaJenis); 
  Serial.println(" g/cm^3"); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("m:");
  lcd.setCursor(2, 0);
  lcd.print(beratBenda,2);       //massa dengan 1 angka dibelakang koma      
//  lcd.print("g");

   //lcd
  lcd.setCursor(8, 0);
  lcd.print("V:");
  lcd.setCursor(10, 0);
  lcd.print(volumeBenda,2);
//  lcd.print(ketinggian,1);
//  lcd.setCursor(9, 0);
//  lcd.print("cm^3");
//  delay(1000);

//  massa jenis
//  massaJenis = beratAsli/volume;
  massaJenis = beratBenda/volumeBenda;
//  if(massaJenis<=0.1)
//  {
//    massaJenis=0.0;
//  }
  lcd.setCursor(0, 1);
  lcd.print("rho:");
  lcd.setCursor(4, 1);
  lcd.print(massaJenis,2);
  lcd.setCursor(10, 1);
  lcd.print("g/cm^3");
  delay(1000);
}
