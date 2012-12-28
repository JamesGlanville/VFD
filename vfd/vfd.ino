#include "TimerOne.h"


int load = 5;
int clk = 4;
int din = 6;
int number=0;
int place=0;
int bleh=3;

volatile int hour=0;
volatile int minute=0;
volatile int seconds=0;
volatile boolean kill=false;
boolean lock =false;

volatile  int disp[]={
  0,0,0,10,0,0,10,0,0};

void setup() {                
  pinMode(load, OUTPUT);     
  pinMode(clk, OUTPUT);     
  pinMode(din, OUTPUT); 
  pinMode(bleh, OUTPUT); 
  digitalWrite(load,LOW);  
  digitalWrite(clk,LOW);  
  digitalWrite(din,LOW);  
  analogWrite(bleh,35);
  Serial.begin(57600);
  Timer1.initialize(1000000);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(3, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(callback);  // attaches callback() as 
  callback();
}


void callback(){
  if (!lock){
    if (disp[0]==0){
      disp[0]=11;
    }
    else{
      disp[0]=0;
    }
    seconds++;
    if (seconds==60){
      seconds=0;
      minute++;
    }
    if (minute==60){
      minute=0;
      hour++;
    }
    if (hour==24){
      hour=0;
    }
    disp[1]=hour/10;
    disp[2]=hour%10;
    disp[4]=minute/10;
    disp[5]=minute%10;
    disp[7]=seconds/10;
    disp[8]=seconds%10;
  }
}
void loop() {
  for (int i=0;i<9;i++)
  {
    RefreshIV18(disp[i],i);
  }
  if ( Serial.available() )
  {
    lock=true;
    char c = toupper(Serial.read());

    if ( c == 'T')
    {
      hour = 10 * (Serial.read()-'0');
      hour += Serial.read()-'0';
      minute = 10 * (Serial.read()-'0');
      minute += Serial.read()-'0';
      seconds = 10 * (Serial.read()-'0');
      seconds += Serial.read()-'0';
    }
  }
  lock=false;
}

void RefreshIV18(int number,int place) //decimal point is out14 (i think, wibbly bottom one)
{ 
  long data=0;
  long fuck=262144L;
  long segments[]={
    907,258,654,782,263,781,909,266,911,271,4,1<<5      }; // 0->9, then -(dash), then dot
  long places[]={
    0,16,1<<14,1<<13,1<<15,1<<11,0,1024,0      };// left->right
  data^=segments[number];
  data^=places[place];

  digitalWrite(load, LOW); 

  for(byte i=0; i<16; i++) 
  { 
    digitalWrite(clk, LOW); //data on rising edge
    if (data & (1 << i)) digitalWrite(din, HIGH);
    else digitalWrite(din, LOW);      
    digitalWrite(clk, HIGH); 
  }
  digitalWrite(clk, LOW); //data on rising edge
  if (place==6) digitalWrite(din, HIGH);
  else digitalWrite(din, LOW);      
  digitalWrite(clk, HIGH); 

  digitalWrite(clk, LOW); //data on rising edge
  if (place==8) digitalWrite(din, HIGH);
  else digitalWrite(din, LOW);      
  digitalWrite(clk, HIGH); 


  digitalWrite(clk, LOW); //data on rising edge
  if (place==0) digitalWrite(din, HIGH);
  else digitalWrite(din, LOW);      
  digitalWrite(clk, HIGH); 

  digitalWrite(clk, LOW); //data on rising edge
  if (1==1) digitalWrite(din, HIGH);
  else digitalWrite(din, LOW);      
  digitalWrite(clk, HIGH); 



  digitalWrite(load, HIGH);

} 
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
    case 1: 
      mode = 0x01; 
      break;
    case 8: 
      mode = 0x02; 
      break;
    case 64: 
      mode = 0x03; 
      break;
    case 256: 
      mode = 0x04; 
      break;
    case 1024: 
      mode = 0x05; 
      break;
    default: 
      return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } 
    else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } 
  else if(pin == 3 || pin == 11) {
    switch(divisor) {
    case 1: 
      mode = 0x01; 
      break;
    case 8: 
      mode = 0x02; 
      break;
    case 32: 
      mode = 0x03; 
      break;
    case 64: 
      mode = 0x04; 
      break;
    case 128: 
      mode = 0x05; 
      break;
    case 256: 
      mode = 0x06; 
      break;
    case 1024: 
      mode = 0x7; 
      break;
    default: 
      return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

