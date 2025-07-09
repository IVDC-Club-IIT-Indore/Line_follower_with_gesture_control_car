#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSN_GPIO    A5
#define CE_GPIO     A2
int enr=5; 
int   enl=3;
int vspeed=90;
int mr1=10;  //motor right 1
int mr2=9;  //motor right 2
int ml1=8;   //motor left 1
int ml2=2; //motor left 2
int tspeed=80;
int tdelay=20;

// Hardware configuration
RF24 radio(CE_GPIO, CSN_GPIO);                           // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

const byte Address[6] = "00001";
unsigned char Received_Command = 0,Speed_index = 0,Run_Stop_Mode = 0;   //  Run_Stop_Mode -> 0 = Stop , 1 = Run;
unsigned char Rx_Array[2];
unsigned int Run_Stop_Counter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(mr1,OUTPUT);
 pinMode(mr2,OUTPUT);
 pinMode(ml1,OUTPUT);
   pinMode(ml2,OUTPUT);
  radio.begin();
  radio.openReadingPipe(0, Address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("START");
}

void loop()
{
  if (radio.available())    // If the NRF240L01 module received data
  {  
    delay(1);
    //radio.read(&Received_Command, 1);
    radio.read(&Rx_Array, sizeof(Rx_Array));
    Received_Command = Rx_Array[0];
    Speed_index = Rx_Array[1];
    Serial.print(Received_Command);
    Serial.print(" , ");
    Serial.println(Speed_index);
  }

/***************** Speed control Logic ********************/
analogWrite (enr,0);
 analogWrite   (enl,0);
  if(Received_Command == 1) // Move forward (all motors rotate in forward direction)
  {
  digitalWrite(mr1,HIGH);
   digitalWrite(mr2,LOW);
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
   analogWrite (enr,vspeed);
  analogWrite (enl,vspeed);
  delay(tdelay);
  }
   if(Received_Command ==2 ) // Move reverse (all motors rotate in reverse direction)
  {
  digitalWrite(mr1,LOW);
  digitalWrite(mr2,HIGH);
  digitalWrite(ml1,LOW);
   digitalWrite(ml2,HIGH);
  analogWrite (enr,vspeed);
  analogWrite (enl,vspeed);
  delay(tdelay);
  }
  else if(Received_Command == 3) // Turn right (left side motors rotate in forward direction, right side motors rotate in backward direction)
  {
  digitalWrite(mr1,LOW);
  digitalWrite(mr2,HIGH);
   digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  analogWrite (enr,tspeed);
   analogWrite (enl,tspeed);
  delay(tdelay);
  }
  else if(Received_Command == 4) // Turn left (right side motors rotate in forward direction, left side motors rotate in backward direction)
  {
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  digitalWrite(ml1,LOW);
   digitalWrite(ml2,HIGH);
  analogWrite (enr,tspeed);
  analogWrite (enl,tspeed);
   delay(tdelay);
  }
  else if(Received_Command == 0) // Stop (all motors stop)
  {      
     analogWrite (enr,0);
  analogWrite   (enl,0);
  }
  else // If any other command is received, stop all motors
  {
     analogWrite (enr,0);
  analogWrite   (enl,0);
  }



}
