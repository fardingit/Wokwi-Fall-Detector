#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include <string.h>
#include <dht.h>
#include <ctype.h>
int indexH,indexH1;
int state1count =0 ;
const int buttonPin = 2;
int count;
int fallstate =0;
sensors_event_t a, g, temp;
float range1avg,  rangemidavg, range2avg, range1sum,rangemidsum, range2sum, diff1 , diff2;
int range1start, range1end, range2start,range2end, index1, index2,index3;
float accX, accY, accZ, resultAcc;
const int time_reading = 50;// change number before *10 tp change reading time
float _circularBufferAcc[time_reading];
int _curWriteIndexAcc;
float _circularBufferHeight[time_reading];
int _curWriteIndexHeight;
bool walkstate;
long startmilli;
float X;
float Y;
float Z;
float dispX;
float dispY;
float dispZ;
float  circleX, circleY, circleZ;
unsigned long last_t = 0 ;
unsigned long new_t;
unsigned long dt;
int state=0;
int address =0;
const String pass="EEE20003";
int choice;
int xadd = 15;
int yadd = 16;
int zadd = 17;
int humidadd=20;
int tempadd=21;
bool menu = true;
int R =10;
int G = 9;
int B=8;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4
#define GRAPH_WIDTH 60 // OLED display width, in pixels
#define GRAPH_HEIGHT 22 // OLED display height, in pixels
#define DHT22_PIN 5
struct {
  uint32_t total;
  uint32_t ok;
  uint32_t crc_error;
  uint32_t time_out;
  uint32_t connect;
  uint32_t ack_l;
  uint32_t ack_h;
  uint32_t unknown;
} stat = { 0, 0, 0, 0, 0, 0, 0, 0};
int IMUX,IMUY,IMUZ;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);Adafruit_MPU6050 mpu;
dht DHT;

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);

  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = "\0"; 
  return String(data);
}
void but();

const unsigned char myBitmap [] PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x86, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x61, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x0c, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x31, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x11, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x80, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x1f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x3f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x78, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x3c, 0x3f, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x1b, 0xff, 0x8f, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x1f, 0xff, 0xc7, 0xff, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe1, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x1f, 0xff, 0xe3, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf3, 0xff, 0xe0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xff, 0xfc, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0xff, 0xf8, 0x70, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x70, 0x00, 0x00, 
	0x00, 0x11, 0xff, 0xff, 0xf0, 0x70, 0x00, 0x00, 0x00, 0x1b, 0xf0, 0xff, 0xe0, 0x70, 0x00, 0x00, 
	0x00, 0x1f, 0xe0, 0x7f, 0xc0, 0x30, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x1f, 0x80, 0x30, 0x00, 0x00, 
	0x00, 0x1f, 0x00, 0x07, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

const unsigned char walk1 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 
	0x00, 0x0f, 0xc0, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x0f, 0x80, 0x00, 
	0x00, 0x3f, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 
	0x00, 0xff, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 
	0x01, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 
	0x01, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 
	0x00, 0x78, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 
	0x00, 0xfc, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 
	0x01, 0xfe, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x00, 0x07, 0xcf, 0x00, 0x00, 
	0x0f, 0x8f, 0x00, 0x00, 0x1f, 0x0f, 0x00, 0x00, 0x1f, 0x0f, 0x00, 0x00, 0x3e, 0x0f, 0x00, 0x00, 
	0x1c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char walk2 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0e, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 
	0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 
	0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 
	0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xbf, 0xe0, 0x00, 0x00, 0x07, 
	0x3f, 0xf0, 0x00, 0x00, 0x0f, 0x3f, 0xf8, 0x00, 0x00, 0x1e, 0x3f, 0xfc, 0x00, 0x00, 0x1e, 0x3f, 
	0xbe, 0x00, 0x00, 0x3c, 0x3f, 0x9f, 0x00, 0x00, 0x38, 0x3f, 0x8f, 0x80, 0x00, 0x78, 0x3f, 0x87, 
	0x80, 0x00, 0x70, 0x3f, 0x01, 0x80, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 
	0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3d, 0xe0, 0x00, 0x00, 0x00, 
	0x79, 0xe0, 0x00, 0x00, 0x00, 0x79, 0xe0, 0x00, 0x00, 0x00, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0xf0, 
	0xf0, 0x00, 0x00, 0x01, 0xe0, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x78, 0x00, 0x00, 0x07, 0xc0, 0x78, 
	0x00, 0x00, 0x0f, 0x80, 0x78, 0x00, 0x00, 0x0f, 0x80, 0x3c, 0x00, 0x00, 0x1f, 0x00, 0x3c, 0x00, 
	0x00, 0x1e, 0x00, 0x3c, 0x00, 0x00, 0x0c, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

  uint32_t start;
  int chk ;
  uint32_t stop ;






void setup() {

  
  // EEPROM.write(10,pass);
  Serial.begin(115200);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), but, RISING);

  
   display.display();
   delay(1000);

  
   display.clearDisplay();

   display.setTextSize(1);
  display.setTextColor(WHITE);
  Serial.println("begin");

  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }
   writeStringToEEPROM(address, pass);
     
    //display.clearDisplay();  
    display.setCursor(10,15);
    display.print("Please enter the \n  password: ");
    display.display();
    String input;

   
   
 bool passinput=false;
    
   int count=0;
 while(passinput == false)
 {

    while (Serial.available() == 0) 
    {
    }
    input=Serial.readString(); 
    //Serial.println(input.length());
    String password = readStringFromEEPROM( address);
  
    for(int i=0;i<input.length();i++){
      if (input[i]==password[i]){
        count++;  
      }
    
    if(count==8){
        //Serial.println("hmm";
        display.setCursor(20,15);
        display.clearDisplay();
        display.println("correct Password");
        display.setCursor(20,30);
        display.println("Please select an");
        display.setCursor(35,40);
        display.print("option");
        display.display();
        passinput=true;
        }
        else{
        passinput=false;
        }
          
        }
        if (passinput == false){
        display.setCursor(15,15);
        display.clearDisplay();
        display.println("incorrect Password");
        display.setCursor(0,30);
        display.println(" enter password again");

        display.display();
        }
      }

    Serial.println("MENU ");
    Serial.println("-----------------------");
    Serial.println("1. Initial position of IMU ( press button to save current IMU data )");
    Serial.println("2. Data from additional sensor ( press button to save current temperture and Humidity data )");
    Serial.println("3. Interrrupt driven program");
        cli();
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A =6249;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 256 prescaler
  TCCR1B |= (1 << CS12) ;  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
 }
    

ISR(TIMER1_COMPA_vect){
   
  //Serial.print("dfghj");
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  resultAcc = pow((pow(accX,2)+pow(accY,2)+pow(accZ,2)),0.5);
 _circularBufferAcc[_curWriteIndexAcc++] = resultAcc;
if(_curWriteIndexAcc >= time_reading){
    _curWriteIndexAcc = 0;
  }
 _circularBufferHeight[_curWriteIndexHeight++] = dispZ;
if(_curWriteIndexHeight >= time_reading){
    _curWriteIndexHeight = 0;
  }
  count = count + 1 ;
 // Serial.print(resultAcc);


}
    

void loop() {
    new_t = millis();
  dt = (new_t -last_t)*0.001;
  //sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float Xaccel=a.acceleration.x;
  float Yaccel=a.acceleration.y;
  float Zaccel=a.acceleration.z;
    dispX= dispX + (0.5*Xaccel*0.005*dt*dt)*0.01 ;
    dispY= dispY + (0.5*Yaccel*0.005*dt*dt)*0.01 ;   
    dispZ= dispZ + (0.5*Zaccel*0.005*dt*dt)*0.01 ; 

  start = micros();
  chk = DHT.read22(DHT22_PIN);
  stop = micros();
  if (DHT.temperature<10){
        analogWrite(R, 0);
        analogWrite(G,0);
        analogWrite(B,254);
        delay(5);
  }
  else if ( DHT.temperature >35){
        analogWrite(R, 254);
        analogWrite(G,0);
        analogWrite(B,0);
        delay(5);
  }
else{
        analogWrite(R, 0);
        analogWrite(G,254);
        analogWrite(B,0);
        delay(5);


}

    
      if (Serial.available()){
    
    choice=Serial.parseInt();
      }
    
    if (choice==1)
    {
      state = 1;
    }
    else if(choice==2)
    {
      state=2;
    }
    else if (choice ==3)
    {
      state=3;
    }
    
  
   if (state==1)

  {
    
    
                IMUX= EEPROM.read(xadd);
                IMUY = EEPROM.read(yadd);
                IMUZ = EEPROM.read(zadd);
                if (IMUX == 255 & IMUY == 255 & IMUZ == 255){
                     display.clearDisplay();
                     display.setCursor(30,30);
                     display.print("calibrating");
                     display.display();
                     delay(1000);
                    EEPROM.write(xadd,dispX);
                    EEPROM.write(yadd,dispY);
                    EEPROM.write(zadd,dispZ);
                }
                else{
                // new_t = millis();
                // dt = (new_t -last_t)*0.001;
                // sensors_event_t a, g, temp;
                // mpu.getEvent(&a, &g, &temp);
                // float Xaccel=a.acceleration.x;
                // float Yaccel=a.acceleration.y;
                // float Zaccel=a.acceleration.z;
                //   dispX= dispX + (0.5*Xaccel*0.005*dt*dt)*0.1 ;
                //   dispY= dispY + (0.5*Yaccel*0.005*dt*dt)*0.1 ;   
                //   dispZ= dispZ + (0.5*Zaccel*0.005*dt*dt)*0.1 ; 

                  display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0,5);
                  display.print("Saved position of IMU- ");
                  display.setCursor(50,20);
                  display.print(" X:");
                  display.print(IMUX);
                  display.setCursor(50,35);
                  display.print(" Y:");
                  display.print(IMUY);
                  display.setCursor(50,45);
                  display.print(" Z:");
                  display.print(IMUZ);
                  display.display();
                  state= 0 ;
              
                }

                
  }



  else if (state==2){


                
                if (EEPROM.read(humidadd) == 255 & EEPROM.read(tempadd) == 255){
                     display.clearDisplay();
                     display.setCursor(30,30);
                     display.print("calibrating");
                     display.display();
                     delay(1000);
                     EEPROM.write(humidadd,DHT.humidity);
                     EEPROM.write(tempadd,DHT.temperature);
                }
                int temp = EEPROM.read(tempadd);
                if (temp > 200){
                  temp =  temp-255;
                }
                
                  display.clearDisplay();


                stat.total++;
                display.setCursor(10,10);
                display.print("Humidity: ");
                display.setCursor(85,10);
                display.print(EEPROM.read(humidadd));
                display.setCursor(10,40);
                display.print("Temperature: ");
                display.setCursor(85,40);

                
                display.print(temp);
                //display.display();
                //delay(3000);
                if (EEPROM.read(humidadd)>80){
                  //display.clearDisplay();
                  display.setCursor(25,20);
                  display.print("too wet");
                  //display.display();
                  //delay(3000);
                }
                if (EEPROM.read(humidadd)<20){
                  //display.clearDisplay();
                  display.setCursor(25,20);
                  display.print("too dry");
                  //display.display();
                  //delay(3000);

                }
                if (temp<0){
                  //display.clearDisplay();
                  display.setCursor(25,50);
                  display.print("too cold");
                  //display.display();
                  //delay(3000);

                }
                if (temp>40){
                  //display.clearDisplay();
                  display.setCursor(25,50);
                  display.print("too warm");
                  //display.display();
                // delay(3000);
                }
                display.display();

                state = 4;
  }

  else if (state == 3){

                    mpu.getEvent(&a, &g, &temp);  

                    range1start= _curWriteIndexAcc+1;
                    if (range1start > 49){
                      range1start = range1start-50;
                    }

                    range1end=range1start + 15;
                    if (range1end > 49){
                      range1end = range1end-50;
                    }
                    range2start=range1end + 20;
                    if (range2start > 49){
                      range2start = range2start-50;
                    }
                    range2end =range2start + 14;
                    if (range2end > 49){
                      range2end = range2end-50;
                    }
                    index1  = range1start;
                    for (int i = 0;  i <= 15; i++){
                      range1sum = range1sum + abs(_circularBufferAcc[index1]);
                      index1 =index1+1;
                      if (index1 > 49){
                        index1=index1-49;
                      }

                    }
                    range1avg= range1sum/15;
                    index2 = range1end;
                    for (int i = 0;  i <=20;i++){
                      rangemidsum=rangemidsum+abs(_circularBufferAcc[index2]);
                      index2=index2+1;
                      if (index2>49){
                        index2=index2-50;
                      }

                      
                    }
                    rangemidavg= rangemidsum/20;

                    index3 = range2start;
                    for (int i = 0;  i <=15;i++){
                        range2sum=range2sum+abs(_circularBufferAcc[index3]);
                        index3 = index3+1;
                      if (index3>49){
                        index3=index3-50;
                      }

                    }
                    //Serial.println("");
                    range2avg= range2sum/15;


                    diff1 = rangemidavg -range1avg;
                    diff2 = rangemidavg - range2avg;
                    // Serial.print(diff1);
                    // Serial.print("   ,  ");
                    // Serial.println(diff2);  

                    if (diff1 > 5 & diff2 > 5){
                      // for (int i = 0 ; i<=49;i++){
                      //   Serial.println(_circularBufferHeight[i]);
                      // }
                      indexH =_curWriteIndexAcc;
                      indexH1 = _curWriteIndexAcc+1;
                      if (indexH1 > 49){
                        indexH1 = indexH1-50;
                      }
                      if (_circularBufferHeight[indexH] >  _circularBufferHeight[indexH1]){
                      //Serial.print("!!!!!!!!!!!!!!!!!!!!!!!! fall detected !!!!!!!!!!!!!!!!!!!!!!!!!11");
                      fallstate = 1;
                      }
                    }

                    delay(100);
                    range1sum = 0;
                    rangemidsum = 0;
                    range2sum = 0;


  }

  if (state == 3 ){
    startmilli= millis();
    if ((startmilli-millis())%2000 == 0 ){
      walkstate = !walkstate;
    }
    if (fallstate == 0){
                      if (walkstate == 0){
                      display.clearDisplay();
                      display.drawBitmap(0, 0, walk1, 27, 50, WHITE);
                      //display.display();
                      }
                      if (walkstate == 1){
                      display.clearDisplay();
                      display.drawBitmap(0, 0, walk2, 33, 50, WHITE);
                      //display.display();
                      }  
                      display.setCursor(60,20);
                      display.print("no fall");
                      display.setCursor(60,39);
                      display.print("detected");
                      display.display()  ;                           
    }
    else{
                      display.clearDisplay();
                      display.drawBitmap(0, 0, myBitmap, 60, 55, WHITE);
                       display.setCursor(60,20);
                      display.print("fall");
                      display.setCursor(60,39);
                      display.print("detected");
                      display.setCursor(0,55);
                      display.print("press button to reset");
                      display.display()  ; 
                      
  }
}
if (state == 5 ){
            display.clearDisplay();
         display.setCursor(40,20);
        display.print("resetting");
        display.display()  ; 
        delay(3000);
        state = 3;
}
}

void but(){
  if (state == 3 & fallstate == 1){//Serial.println("ertyui");
 
        //   display.clearDisplay();
        //  display.setCursor(40,20);
        // display.print("resetting");
        // display.display()  ; 
        // delay(3000);
         fallstate = 0; 
         state = 5 ;

  }
   if (state == 0){
                EEPROM.write(xadd,dispX);
                EEPROM.write(yadd,dispY);
                EEPROM.write(zadd,dispZ);
                state = 1;
  }
  if (state == 4){

                EEPROM.write(humidadd,DHT.humidity);
               EEPROM.write(tempadd,DHT.temperature);

                state = 2;
  }

}
