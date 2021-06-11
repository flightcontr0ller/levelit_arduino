/*
 ************************************************
 * project:      	LevelIT
 * file:         	external_sensor_v1.1
 * dev:          	Malte Schoenert
					        Jack Kittelmann	
 * created on:   	2021-02-07
 ************************************************
*/

// libs
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Buzzer.h>
#include <Bounce2.h>
#include <MPU6050.h>

// vars
int gX = 0, gY, gZ;
const int Button_Pin = 8;
const int Buzzer_Signal = 9;
const int Bluetooth_RX = 10;
const int Bluetooth_TX = 11;
int count, myData[20];
float offsetX, sumX;

// init
MPU6050 mpu;
SoftwareSerial BT(Bluetooth_RX, Bluetooth_TX);
Buzzer buzzer(Buzzer_Signal);
Bounce myButton = Bounce();

void setup() 
{
  BT.begin(9600);
  mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  Serial.begin(9600);
  
  pinMode(Button_Pin,INPUT_PULLUP);
  myButton.attach(Button_Pin);
  myButton.interval(5); // 5ms zum Entprellen
  
  Serial.println("init");
}


void loop() 
{
  //Offset beim drücken des Tasters S1 bestimmen
  myButton.update();
  if(myButton.fallingEdge()){
      buzzer.sound(NOTE_D4, 500);
      count = 0;
      //2s Werte einlesen
      while (count < 20) {
        Vector rawAccel = mpu.readRawAccel();
        myData[count] = rawAccel.XAxis/200;
        Serial.println((rawAccel.XAxis/200));
        Serial.println(count);
        count = count+1;
        delay(100); 
      }
      //Mittelwert bilden
      count = 0;
      sumX = 0;
      while (count < 20) {
        sumX = sumX + myData[count];
        count = count+1;
      }
      offsetX = sumX/20;
      Serial.print("Offset:  ");
      Serial.println(offsetX);
      delay(1000);
      buzzer.sound(NOTE_D4, 500);
  }
  
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();

  BT.println((rawAccel.XAxis/200)-offsetX);
  Serial.println((rawAccel.XAxis/200)-offsetX);

  delay(500);

}
