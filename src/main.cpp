#include <Arduino.h>

/*

*/

float recordTime = 50; //fréquence d'enregistrement

int potPin = A0; //potentiomètre

int outA = 6; //channel A out PWM -> need a low pass
int recA = 2; //channel A record button
int recAstate = 0; //mise à 0 du bouton record A
int arraySizeA = 0; //mise à 0 de la taille du tableau des voltages de A
int countArray = 0; //comptage des steps à playback pour A

int outB = 9; 
int recB = 4;
int recBstate = 0;
int arraySizeB = 0;
int countBrray = 0;

byte channelAcontent[100] = {0}; //stockage des voltages A
byte channelBcontent[100] = {0};

void setup()
{
  pinMode(potPin, INPUT);
  pinMode(outA, OUTPUT);
  pinMode(recA, INPUT_PULLUP);
  pinMode(recB, INPUT_PULLUP);
  pinMode(outB, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  recAstate = digitalRead(recA); //lit le state pour sortir des loops
  recBstate = digitalRead(recB);

  for (int i = 0; recAstate == 1; i++)                         //rec de A
  {
    recAstate = digitalRead(recA);                             //relecture du state du bouton record
    byte writevalA = map(analogRead(potPin), 0, 1023, 0, 255); //lit le pot sur A0 et le map en 8 bits
    channelAcontent[i] = writevalA;                            //remplit channel A du tableau
    arraySizeA++;                                              //incrémente le arraySize
    analogWrite(outA, channelAcontent[i]);                     //reprends la valeur crée pour la sortir thru
    countArray = 0;                                            //remets le compte des steps du playback à 0
    delay(recordTime);
  }

  for (int i = 0; recBstate == 1; i++)
  {
    recBstate = digitalRead(recB);
    byte writevalB = map(analogRead(potPin), 0, 1023, 0, 255); 
    channelBcontent[i] = writevalB;
    arraySizeB++;
    analogWrite(outB, channelBcontent[i]);
    countBrray = 0;
    delay(recordTime);
  }

  analogWrite(outA, channelAcontent[countArray]);              //playback la valeur du step du array
  if (countArray <= arraySizeA)
  {
    countArray++;                                              //incrémente pour passer au step d'après si ce n'étais pas le dernier step
  }
  else
  {
    countArray = 0;                                            //si dernier step, remise à 0 pour playback en boucle 
  }

  analogWrite(outB, channelBcontent[countBrray]);
  if (countBrray <= arraySizeB)
  {
    countBrray++;
  }
  else
  {
    countBrray = 0;
  }

  delay(recordTime);
}