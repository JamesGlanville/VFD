#include "TimerOne.h"
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

int load = 5;
int clk = 4;
int din = 6;
int number=0;
int place=0;
int bleh=3;
int button=8;

volatile int waspressed=0;

volatile int hour=0;
volatile int minute=0;
volatile int seconds=0;
volatile boolean kill=false;

volatile  int disp[]={
  0,0,0,10,0,0,10,0,0};

void setup() {         
  Wire.begin();
  RTC.begin();
  if (0==1){
    DateTime now = DateTime(__DATE__, __TIME__);
    DateTime future(now.unixtime()+12+3600);
    RTC.adjust(future);
    RTC.setSqwOutLevel(HIGH);
    RTC.setSqwOutSignal();
}
  pinMode(load, OUTPUT);     
  pinMode(clk, OUTPUT);     
  pinMode(din, OUTPUT); 
  pinMode(bleh, OUTPUT); 
  
  analogReference(EXTERNAL);
  analogRead(0);
  analogRead(0);
  analogRead(0);
  analogRead(0);
  analogRead(0);

  pinMode(button, INPUT_PULLUP); 

  digitalWrite(load,LOW);  
  digitalWrite(clk,LOW);  
  digitalWrite(din,LOW);  
  analogWrite(bleh,35);
  Serial.begin(57600);
  Timer1.initialize(1000000);        
  Timer1.pwm(3, 512);
  //Timer1.attachInterrupt(callback);
  getRTCtime();
  tick();
  
  attachInterrupt(0,tick,RISING);

}

void getRTCtime(){
         DateTime now = RTC.now();
       seconds=now.second();
       minute=now.minute();
       hour=now.hour();
 
}

void tick(){
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
  /*  if(seconds==1)
    {
      getRTCtime();
   }*/
    disp[1]=hour/10;
    disp[2]=hour%10;
    disp[4]=minute/10;
    disp[5]=minute%10;
    disp[7]=seconds/10;
    disp[8]=seconds%10;
}
void loop() {
  static int temperaturereading;
  float temp;
  temp=0;
  
  int t_tens;
  int t_ones;
  int t_tenths;
  
  
  if(!digitalRead(button))
  {
    if(!waspressed)
    {
      waspressed=1;
    for(int i=0;i<100;i++)
    {
      temp += analogRead(0);
    }
  temperaturereading=(int)(temp/100.0);
  
  }
  float temperature = ((temperaturereading*0.4882825))-273.15;  
  
  t_tens=((int)(temperature/10))%10;
  t_ones=((int)temperature)%10;
  t_tenths=((int)(temperature*10))%10;
  
  if(t_tenths>=7){t_ones++;t_tenths=0;}
  else if(t_tenths<=3){t_tenths=0;}
  else {t_tenths=5;}
  
    RefreshIV18(t_tens,2,0);
    RefreshIV18(t_ones,3,1);
    RefreshIV18(t_tenths,4,0);
    RefreshIV18(12,6,0);
    RefreshIV18(13,7,0);
  }
  
  else
  {
    waspressed=0;
    for (int i=0;i<9;i++)
    {
      RefreshIV18(disp[i],i,0);
    }
  }

}

void RefreshIV18(int number,int place, int dot) //decimal point is out14 (i think, wibbly bottom one)
{ 
  long data=0;
  long fuck=262144L;
  long segments[]={
    907,258,654,782,263,781,909,266,911,271,4,1<<5,15,649      }; // 0->9, then -(dash), then dot
  long places[]={
    0,16,1<<14,1<<13,1<<15,1<<11,0,1024,0      };// left->right
  data^=segments[number];
  data^=places[place];
  if(dot)
  {
   data|=(1<<5); 
  }

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


