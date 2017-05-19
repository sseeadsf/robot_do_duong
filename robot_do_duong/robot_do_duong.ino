#include <EEPROM.h>
#include <Servo.h>
#include<LiquidCrystal.h>                                          
#include <stdlib.h>
#include <AutoDIY.h>

LiquidCrystal lcd(1, 0, A0, A1, A2, A3);    


#define trigger 2
#define echo 3

//LiquidCrystal lcd(RS, E, D4, D5, D6, D7); 

AutoDIY robot;
Servo servo;
int ntcount,pos;
int j;
int state;
int min_adc[4], ng_adc[4], max_adc[4], adc[4];

unsigned long duration;

void lui(){
  robot.control_motor(0,1,100);
  robot.control_motor(1,0,85);
}

void tien(){
  robot.control_motor(0,0,100);
  robot.control_motor(1,1,90);
}
void quay_phai(){
  robot.control_motor(0,1,100);
  robot.control_motor(1,1,85);
}

void quay_trai(){
  robot.control_motor(0,0,100);
  robot.control_motor(1,0,85);
}

void display_adc(int a,int b, int c, int d){
lcd.setCursor(0, 1);  lcd.print(a);    
lcd.setCursor(4, 1);  lcd.print(b);   
lcd.setCursor(8, 1);  lcd.print(c);   
lcd.setCursor(12, 1);  lcd.print(d);                    
}

void read_adc(){
  int i;
  for(i=0;i<4;i++)
    adc[i]=analogRead(i+4);
  display_adc(adc[0],adc[1],adc[2],adc[3]);
}

void set_adc(){
  /*if(digitalRead(11)==0){
    read_adc();
    for(j=0;j<4;j++)
      min_adc[j] = adc[j];
    display_adc(min_adc[0],min_adc[1], min_adc[2], min_adc[3]);
    lcd.setCursor(10,0);
    lcd.print("Black");
  }*/
  if(digitalRead(12)==0 && state==0){
    delay(500);
    read_adc();
    for(j=0;j<4;j++)
      min_adc[j] = adc[j];
    display_adc(min_adc[0],min_adc[1], min_adc[2], min_adc[3]);
    lcd.setCursor(10,0);
    lcd.print("Black");
    state = 1;
  }
  if(digitalRead(12)==0 && state==1){
    delay(500);
    read_adc();
    for(j=0;j<4;j++)
      max_adc[j] = adc[j];
    display_adc(max_adc[0],max_adc[1], max_adc[2], max_adc[3]);
    
    for(j=0;j<4;j++)
      ng_adc[j]=(min_adc[j]+max_adc[j])/2;
    display_adc(ng_adc[0],ng_adc[1], ng_adc[2], ng_adc[3]);
    lcd.setCursor(10,0);
    lcd.print("White");
    state = 0;
  }
}

float get_distance()
{
  long duration, distanceCm;
   
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  duration = pulseIn(echo, HIGH, 5000);
 
  // convert to distance
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}



void setup() {

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  lcd.begin(16,2);
  servo.attach(8);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);
  robot.init_pin();
  robot.init_adc();
  robot.ngatu=0;
  robot.clearr();
  ntcount=0;
  servo.write(0);

}

void loop() {
  lcd.clear();
  set_adc();
  read_adc();
  long distance = get_distance();
  lcd.setCursor(0, 0);
  lcd.print(distance);
  tien();
  if(distance > 0 && distance < 8){
     quay_phai();
     delay(350);
  }
  /*if(adc[0] < ng_adc[0] && adc[1] < ng_adc[1] && adc[2] < ng_adc[2] && adc[3] < ng_adc[3]){
    lcd.setCursor(5,0);
    lcd.print("BBBBBB");
    servo.write(90);
    delay(200);
    servo.write(170);
  }*/
  if(adc[0] < ng_adc[0] && adc[1] < ng_adc[1] && adc[2] < ng_adc[2] && adc[3] < ng_adc[3]){
    servo.write(90);
    delay(200);
    servo.write(170);
    delay(200);
  }
  else if(adc[0] < ng_adc[0] || adc[1] < ng_adc[1] || adc[2] < ng_adc[2] || adc[3] < ng_adc[3]){ //lon hon nguong la mau trang nho hon nguong la mau den
    lcd.setCursor(5,0);
    lcd.print("AAAAAA");
    quay_trai();
    delay(350);
  }
}
