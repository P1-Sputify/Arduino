// 0 get 0V,  255 ger 5v. 8 bitar är D/A
// Måste vara byte för att bli analog (8bitar är en byte)
//Inkomande bytes är -128 till 127, behövs inte omvandlas till 0-255

// Adruino Clock Speed	16 MHz

// The default configuration for the Bluetooth module is: • Serial port 115,200 Kbps baud rate, 8 bits, no parity, 1 stop bit • Serial port flow control disabled

// Sändningshastighet = 1440 BytesPerSecond to 115200BitsPersSecond »

//for (i=0; i< 64;i++){data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); // this gets the absolute value of the values in the array, so we're only dealing with positive numbers};

/*
 * This is a class that takes music amplitude data from a bluetooth and makes the light shine in rythm with a song,
 */
#include <SoftwareSerial.h>  

//int test[200] = { 21065, 17990, 1478, 256, 22337, 22085, 26221, 29728, 4096, 0, 256, 256, 32706, 0, 125, 0, 512, 4096, 25697, 29793, 10694, 256, 22272, 26627, 17402, 23305, 29195, 31476, 13812, 1013, 24586, 5881, 5884, 32513, 254, 23302, 19977, 23285, 10765, 13555, 28685, 10764, 3082, 16647, 1277, 32512, 2301, 6919, 17399, 24565, 23796, 18164, 12277, 4106, 31737, 28676, 1, 32259, 12538, 20727, 5877, 20749, 13555, 21005, 6133, 20471, 12794, 32515, 769, 27396, 30201, 2570, 10741, 19444, 23028, 25077, 16887, 6919, 3069, 32256, 765, 16391, 2826, 11276, 28429, 13555, 10765, 23029, 19465, 24070, 1022, 32257, 6140, 5369, 23562, 780, 16116, 28660, 26123, 26121, 19962, 30467, 512, 30980, 20473, 25610, 26636, 29683, 15859, 269, 23051, 5368, 6651, 32766, 258, 23301, 21256, 24566, 12556, 14836, 29964, 10763, 3337, 16646, 766, 32513, 2812, 6664, 17398, 25076, 23539, 19443, 11508, 3083, 31224, 27397, 770, 31998, 13307, 19704, 5878, 21004, 13300, 21260, 6134, 19704, 13051, 32510, 514, 27397, 30968, 2827, 11508, 19187, 23795, 24564, 18166, 7688, 4092, 31999, 510, 16902, 2825, 11787, 28428, 13044, 10764, 22774, 19976, 23813, 258, 32254, 7675,  };
//int test[200] = {1, 4, 5, 6, 7, 8, 9, 12 ,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12,1, 4, 5, 6, 7, 8, 9, 12  };
int bluetoothTx = 2;  // TransMit Pin
int bluetoothRx = 3;  // Reciever Pin

byte incomming; // Bluetooths sending data

// Makes pin 2 and 3 act like Rx and Tx and we are able to se whats happening with the serial monitor
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

//int ledPins[6] = { 4, 5, 6, 7, 8, 9, };
// !!!!!!!!!!!!!!!!!!!!BUUUUUUUUUUUUUUUUUFFFFFFFFEEEEERRRRRRRRRRRRTTTTTTTTTTT
//The buffert that will store the Wav Songs
int const buffertSize = 100; 
byte musicArray[buffertSize]; 
int frontCounter = 0; 
int backCounter = -1;
int sizeCounter = 0; // How many samples there are in the array
int sampleCounter = 0; //How many samples that have men handled
int sum = 0; // The sum of all the samples in the array
// !!!!!!!!!!!!!!!!!!!!BUUUUUUUUUUUUUUUUUFFFFFFFFEEEEERRRRRRRRRRRRTTTTTTTTTTT

byte Lys = 10; // Decide if te LED will shine or not.


void setup()
{
  Serial.begin(9600);  // Ger Serial 9600bps
  delay(250);
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);
  delay(250);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}
/*
 * A Method that returns true if the buffert is full with samples
 */
boolean isBuffertFull()
{
  return sizeCounter >= buffertSize;
}
/*
 * A Method that returns true if the buffert us empty
 */
boolean isBuffertEmpty()
{
  return sizeCounter == 0;
}
/*
 * a Method that return 0 to the backCounter or frontCounter if they have reached the buffertSize
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
 * a Method that removes the first sample that was queued in the musicArray "buffert"
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
 * A Method that puts a sample in the musicArray "buffert" if the buffert isn't alredy ful
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
  //Serial.println(data);
  if(isBuffertFull())
  {
    dequeue();
  }
  enqueue(incomming); 
  sum += incomming; 
  sampleCounter++;

  if(sampleCounter % 25 == 0) // 1500
  {
    //delay(25);
    int middleValue = (sum/sizeCounter);
    ligthShow(middleValue);
  }
}
//Här kommer LEDsen att lyssa beoronde på musikens frekvens
void ligthShow(int sampleMiddleValue)
{
  // delay(5);
  int musicByte = (sampleMiddleValue); 
  //LED 1
  if( musicByte > Lys*2) // !!! VILKEN FREKVENS FÖR VILKEN LAMPA SOM SKA LYSA
    digitalWrite(4, HIGH);
  else 
    digitalWrite(4,LOW);
  //LED2
  if( musicByte > Lys*4) 
    digitalWrite(5, HIGH);
  else 
    digitalWrite(5,LOW);
  //LED3
  if( musicByte > Lys*6) 
    digitalWrite(6, HIGH);
  else 
    digitalWrite(6,LOW);
  //LED4
  if(musicByte > Lys*8) 
    digitalWrite(7, HIGH);
  else 
    digitalWrite(7,LOW);
  //LED5
  if( musicByte > Lys*10) 
    digitalWrite(8, HIGH);
  else 
    digitalWrite(8,LOW);
  //LED6
  if( musicByte > Lys*12) 
    digitalWrite(9, HIGH);
  else 
    digitalWrite(9,LOW);
  delay(100);
}
void loop()
{
  //delay(1000);
  //lagra = bluetooth.read();
  //Serial.println(lagra);
  //serial = Serial.read();
  //bluetooth.print(serial);
  //bluetooth.println("Saafft");
  // Serial.print("HEj");
  if(bluetooth.available() )  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    incomming = bluetooth.read();
   // Serial.println(incomming);
   // delay(1);
 //  incomming = test[sampleCounter];
   ligthShow(incomming);  
   // delay(2);  
//char apa = bluetooth.read();
//Serial.println(apa);
  // buffert(incomming);
   // if(Serial.available())  // If stuff was typed in the serial monitor
  //  {
      // Send any characters the Serial monitor prints to the bluetooth
  //    bluetooth.print((char)Serial.read());
 //   }
  }





}



