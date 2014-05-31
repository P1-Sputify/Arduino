//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Project BlinkePartaj
// Authors : Patrik Larsson, Andreas Stridh, Sebastian Aspegren,
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * This is a class that takes music amplitude data from a bluetooth and store it in a buffer,
 * that calculate the middle value of the amlpitudes then send it to the diods so they shine
 */
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TransMit Pin
int bluetoothRx = 3;  // Reciever Pin

byte incomming; // Bluetooths sending data

// Makes pin 2 and 3 act like Rx and Tx and we are able to se whats happening with the serial monitor
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);


int const buffertSize = 100;
byte musicArray[buffertSize]; 
int frontCounter = 0; 
int backCounter = -1;
int sizeCounter = 0; // How many samples there are in the array
int sampleCounter = 0; //How many samples that have men handled
int sum = 0; // The sum of all the samples in the array

byte Lys = 10; // Decide if te LED will shine or not.

void setup()
{
  Serial.begin(9600);  // Gives Serialmonitor 9600bps
  delay(250);
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600); // Gives the bluetooth 9600bps
  delay(250);

  // Makes the arduino pins to outputs, so they can light the diods
  for(int i = 4; i < 14; i++)
  {
    pinMode(i, OUTPUT);
  }

}
/*
 * A Method that returns true if the buffert is full with samples
 */
boolean isBuffertFull()
{
  return sizeCounter >= buffertSize;
}
/*
 * A Method that returns true if the buffert is empty
 */
boolean isBuffertEmpty()
{
  return sizeCounter == 0;
}
/*
 * A method that return 0 to the backCounter or frontCounter if they have reached the buffertSize
 * or return the current possistion of the backCounter or frontCouter.
 */
int buffertWrap(int a)
{
  if(a >= buffertSize)
    return 0;
  else
    return a;
}
/*
 * A method that removes the first sample that was queued in the musicArray "buffert"
 * or return 0 if the musicArray is empty
 */
byte dequeue()
{
  if(isBuffertEmpty())
    return 0;
  else
  {
    byte temp = musicArray[frontCounter]; 
    sizeCounter--;
    sum -= temp;
    frontCounter = buffertWrap(frontCounter + 1);
    return temp; 
  }
}
/*
 * A Method that puts a sample in the musicArray "buffert" if the buffert isn't alredy full
 */
boolean enqueue(int sample)
{
  if(isBuffertFull())
    return false;
  else
  {
    backCounter = buffertWrap(backCounter + 1);
    musicArray[backCounter] = sample;
    sizeCounter++;
    return true;
  }
}
/*
 * A Method that puts sample in a array and then send the samples to the LEDs to shine.
 */
void buffert(int data)
{
  if(isBuffertFull())
  {
    dequeue();
  }
  enqueue(incomming); 
  sum += incomming; 
  sampleCounter++;
  
  // Takes 25 samples and turn it to 1
  if(sampleCounter % 25 == 0) 
  {
    int middleValue = (sum/sizeCounter);
    ligthShow(middleValue);
    sampleCounter = 0; // Reset the sampleCounter so it wont be full.
  }
}
/*
 * A Method that makes the diod shine depending on the incomming data value
 */
void ligthShow(int sampleMiddleValue)
{
  
  int musicByte = (sampleMiddleValue); 
  //LED 1
  if( musicByte > Lys*4) // !!! VILKEN FREKVENS FÖR VILKEN LAMPA SOM SKA LYSA
    digitalWrite(4, HIGH);
  else 
    digitalWrite(4,LOW);
  //LED2
  if( musicByte > Lys*6) 
    digitalWrite(5, HIGH);
  else 
    digitalWrite(5,LOW);
  //LED3
  if( musicByte > Lys*8) 
    digitalWrite(6, HIGH);
  else 
    digitalWrite(6,LOW);
  //LED4
  if(musicByte > Lys*10) 
    digitalWrite(7, HIGH);
  else 
    digitalWrite(7,LOW);
  //LED5
  if( musicByte > Lys*12) 
    digitalWrite(8, HIGH);
  else 
    digitalWrite(8,LOW);
  //LED6
  if( musicByte > Lys*14) 
    digitalWrite(9, HIGH);
  else 
    digitalWrite(9,LOW);
  //LED7  
    if( musicByte > Lys*16) 
    digitalWrite(10, HIGH);
  else 
    digitalWrite(10,LOW);
 //Led8
    if( musicByte > Lys*18) 
    digitalWrite(11, HIGH);
  else 
    digitalWrite(11,LOW);
 //LED9  
    if( musicByte > Lys*20) 
    digitalWrite(12, HIGH);
  else 
    digitalWrite(12,LOW);
  //LED10
    if( musicByte > Lys*22) 
    digitalWrite(13, HIGH);
  else
    digitalWrite(13,LOW);
  
}
/*
 * The main loop will repeat it self as long as the adruino is on
 */
void loop()
{

  if(bluetooth.available() )  // If the bluetooth sent any characters
  {
    incomming = bluetooth.read();// Stores incomming bytes
    
    // We don't use the buffert because the arduino is a slow defice and the lights will not shine in so great sync
    // if the arduino calculate the midde amplitude of the frequency.
    // Instead the andriod calculate the middle amlpitude value
    //buffert(incomming); 
    
    ligthShow(incomming); // Sends the data to the diod method  

   }
  }
