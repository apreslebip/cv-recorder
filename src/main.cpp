#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

float recordTime = 25; //recording frequency

int potPin = A0;  //potentiometer
int resetPin = 2; //reset jack
int recPin = 4;   //record button

int arraySize;  //fake array size
int countArray; //count for the playback
bool recState; //is it recording

byte channelContent[1400] = {0}; //array of recorded values

void reset()
{
  if (recState != 0)
  {
    countArray = 0;
  }
}

void setup()
{
  pinMode(potPin, INPUT);
  pinMode(resetPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(resetPin), reset, RISING);
  pinMode(recPin, INPUT_PULLUP);
  dac.begin(0x60);
  Serial.begin(9600);
}

void loop()
{
  recState = digitalRead(recPin); //read state for break the loop

  if (recState == 0)
  {
    arraySize = 0;
    memset(channelContent, 0, sizeof(channelContent)); //empty the array from previous values
    countArray = 0; //playback count reset
  }

  for (int i = 0; recState == 0; i++)
  {
    recState = digitalRead(recPin);
    byte writeval = map(analogRead(potPin), 0, 1023, 0, 255);
    channelContent[i] = writeval;
    arraySize++;
    dac.setVoltage((channelContent[i] * 16), false);
    delay(recordTime);
  }

  dac.setVoltage((channelContent[countArray] * 16), false); //shoot playback

  if (countArray <= arraySize)
  {
    countArray++; //if not last value of the array, increment
  }
  else
  {
    countArray = 0; //if last value of the array, reset
  }

  delay(recordTime);
}