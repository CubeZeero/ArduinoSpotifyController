#include <stdint.h>
#include "TouchScreen.h"
#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h> 

#define LCD_CS A3
#define LCD_CD A2 
#define LCD_WR A1
#define LCD_RD A0 
#define LCD_RESET A4 

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A2
#define XM A3
#define YM 8
#define XP 9

#define TS_MINX 170
#define TS_MAXX 940
#define TS_MINY 170
#define TS_MAXY 910

int button_num = 0;

int pp_sw = 0;
int r_sw = 0;
int s_sw = 0;

int init_Set_num[4];
int exit_sw = 0;
int volume = 0;
int mute_sw = 0;
int start_sw = 0;

int i;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void clear_LCD(){
    tft.fillScreen(BLACK);      
}

void screen_init(){

  if(start_sw == 0){
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(35,140);
    tft.print("Please connect");
    tft.setCursor(70,160);
    tft.print("with ASC.");
    tft.setTextSize(1);
    tft.setCursor(82,190);
    tft.print("Start ASC.exe");

    delay(2000);
    Serial.print(0);
    start_sw = 1;
  }

  while(1){
    if (Serial.available() > 0){
      for (i = 0; i < 4; i++){
        init_Set_num[i] = Serial.read();
        delay(300);
        //Serial.print(init_Set_num[i]);
      }
      delay(100);
      break;
    }
  }

  volume = init_Set_num[3];
  
  clear_LCD();

  if(init_Set_num[0] == 0){
       tft.fillRect(100,130,50,75,BLACK);
       tft.fillRect(102,140,10,50,WHITE);
       tft.fillRect(127,140,10,50,WHITE);
       pp_sw = 0;
     }else{
       tft.fillRect(100,130,50,75,BLACK);
       tft.fillTriangle(105,140,105,190,145,165,WHITE);
       pp_sw = 1;
     }

  /*PLAY
  tft.fillTriangle(105,140,105,190,145,165,WHITE);
  tft.fillRect(102,140,10,50,WHITE);
  tft.fillRect(127,140,10,50,WHITE);*/

  //PREVIOS
  tft.fillRect(10,140,10,50,WHITE);
  tft.fillTriangle(50,140,50,190,10,165,WHITE);

  //NEXT
  tft.fillRect(225,140,10,50,WHITE);
  tft.fillTriangle(190,140,190,190,230,165,WHITE);

  if(init_Set_num[2] == 1){
    //Repeat List
    tft.fillRect(50,25,52,52,GREEN);
    tft.drawRect(51,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,33);
    tft.print("R");
    r_sw = 1;
  }else if(init_Set_num[2] == 2){
    //Repeat Current Track
    tft.fillRect(50,25,52,52,GREEN);
    tft.drawRect(51,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,33);
    tft.print("R");
    tft.fillCircle(102,25,10,GREEN);
    tft.setTextSize(2);
    tft.setCursor(98,18);
    tft.print("1");
    r_sw = 2;
  }else if(init_Set_num[2] == 0){
    //Repeat Off
    tft.fillRect(50,25,52,52,WHITE);
    tft.drawRect(51,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,33);
    tft.print("R");
    r_sw = 0;
  }
   
  /*REPEAT
  tft.fillRect(50,45,52,52,WHITE);
  tft.drawRect(51,46,50,50,BLACK);
  tft.setTextColor(BLACK);
  tft.setTextSize(5);
  tft.setCursor(63,53);
  tft.print("R");*/

  if(init_Set_num[1] == 1){
    tft.fillRect(137,25,52,52,GREEN);
    tft.drawRect(138,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(150,33);
    tft.print("S");
    s_sw = 1;
  }else if(init_Set_num[1] == 0){
    tft.fillRect(137,25,52,52,WHITE);
    tft.drawRect(138,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(150,33);
    tft.print("S");
    s_sw = 0;
  }

  /*SHUFFLE
  tft.fillRect(137,45,52,52,WHITE);
  tft.drawRect(138,46,50,50,BLACK);
  tft.setTextColor(BLACK);
  tft.setTextSize(5);
  tft.setCursor(150,53);
  tft.print("S");*/

  //VOLUME
  tft.fillRect(70,245,100,15,WHITE);
  tft.fillRect(70,245,volume,15,GREEN);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(35,242);
  tft.print("-");
  tft.setCursor(190,242);
  tft.print("+");

  //MUTE
  if(volume == 0){
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.setCursor(97,220);
    mute_sw = 1;
  }else{
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(97,220);
  }
  tft.print("Mute");

  //SYNC
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(97,280);
  tft.print("Sync");

  //5SEC NEXT
  tft.fillTriangle(145,115,145,95,160,105,WHITE);

  //10SEC NEXT
  tft.fillTriangle(205,115,205,95,220,105,WHITE);
  tft.fillTriangle(190,115,190,95,205,105,WHITE);

  //5SEC BACK
  tft.fillTriangle(95,115,95,95,80,105,WHITE);

  //10SEC BACK
  tft.fillTriangle(50,115,50,95,35,105,WHITE);
  tft.fillTriangle(35,115,35,95,20,105,WHITE);
}

void touch_detection(){
  
  if (button_num == 0){
    
  while(1){
  
  TSPoint p = ts.getPoint();
    if (p.z > ts.pressureThreshhold) {

     //PLAY / PAUSE
     if(p.x>414 && p.x<588 && p.y>429 && p.y<636)
     {
      Serial.print("pp");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 1;
      if(pp_sw == 0){
        pp_sw = 1;
      }else{
        pp_sw = 0;
      }
      break;
     }

     //PREVIOUS
     if(p.x>414 && p.x<588 && p.y>111 && p.y<320)
     {
      Serial.print("pr");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 2;
      break;
     }

     //NEXT
     if(p.x>414 && p.x<588 && p.y>702 && p.y<912)
     {
      Serial.print("n");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 3;
      break;
     }

     //REPEAT
     if(p.x>751 && p.x<900 && p.y>255 && p.y<466)
     {
      Serial.print("rp");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 4;
      break;
     }

     //SHUFFLE
     if(p.x>748 && p.x<888 && p.y>552 && p.y<754)
     {
      Serial.print("s");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 5;
      break;
     }

      //VOLUME +
      if(p.x>238 && p.x<357 && p.y>722 && p.y<867)
     {
      Serial.print("vp");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 6;
      break;
     }

      //VOLUME -
      if(p.x>247 && p.x<360 && p.y>181 && p.y<342)
     {
      Serial.print("vm");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 7;
      break;
     }

      //MUTE
      if(p.x>322 && p.x<391 && p.y>401 && p.y<633)
     {
      Serial.print("mt");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 8;
      break;
     }

      //SYNC
      if(p.x>174 && p.x<240 && p.y>412 && p.y<627)
     {
      Serial.print("sn");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 9;
      break;
     }

     //5SEC NEXT
     if(p.x>649 && p.x<742 && p.y>592 && p.y<682)
     {
      Serial.print("0n");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 10;
      break;
     }

     //10SEC NEXT
     if(p.x>646 && p.x<722 && p.y>742 && p.y<877)
     {
      Serial.print("1n");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 11;
      break;
     }

     //5SEC BACK
     if(p.x>644 && p.x<728 && p.y>369 && p.y<462)
     {
      Serial.print("0b");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 12;
      break;
     }

     //10SEC BACK
     if(p.x>636 && p.x<723 && p.y>152 && p.y<296)
     {
      Serial.print("1b");
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      button_num = 13;
      break;
     }
     
    }
  }
  }
}

void setup() {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);

  tft.setRotation(2);

  clear_LCD();

  screen_init();

}

void loop() {

/*TSPoint p = ts.getPoint();
if (p.z > ts.pressureThreshhold) {
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
  }*/

touch_detection();

if(button_num == 1){
   if(pp_sw == 0){
        tft.fillTriangle(105,140,105,190,145,165,GREEN);
        delay(200);
        tft.fillRect(100,130,50,75,BLACK);
        tft.fillRect(102,140,10,50,WHITE);
        tft.fillRect(127,140,10,50,WHITE);
        pp_sw = 0;
      }else{
        tft.fillRect(102,140,10,50,GREEN);
        tft.fillRect(127,140,10,50,GREEN);
        delay(200);
        tft.fillRect(100,130,50,75,BLACK);
        tft.fillTriangle(105,140,105,190,145,165,WHITE);
        pp_sw = 1;
      }
}

if(button_num == 2){
  tft.fillRect(10,140,10,50,GREEN);
  tft.fillTriangle(50,140,50,190,10,165,GREEN);
  delay(200);
  tft.fillRect(10,140,10,50,WHITE);
  tft.fillTriangle(50,140,50,190,10,165,WHITE);
  tft.fillRect(100,130,50,75,BLACK);
  tft.fillRect(102,140,10,50,WHITE);
  tft.fillRect(127,140,10,50,WHITE);
  pp_sw = 0;

  if(r_sw == 2){
    tft.fillRect(50,25,70,72,BLACK);
    tft.fillRect(50,45,52,52,GREEN);
    tft.drawRect(51,46,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,53);
    tft.print("R");
    r_sw = 1;
  }
}

if(button_num == 3){
  tft.fillRect(225,140,10,50,GREEN);
  tft.fillTriangle(190,140,190,190,230,165,GREEN);
  delay(200);
  tft.fillRect(225,140,10,50,WHITE);
  tft.fillTriangle(190,140,190,190,230,165,WHITE);
  tft.fillRect(100,130,50,75,BLACK);
  tft.fillRect(102,140,10,50,WHITE);
  tft.fillRect(127,140,10,50,WHITE);
  pp_sw = 0;

  if(r_sw == 2){
    tft.fillRect(50,25,70,72,BLACK);
    tft.fillRect(50,45,52,52,GREEN);
    tft.drawRect(51,46,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,53);
    tft.print("R");
    r_sw = 1;
  }
}

if(button_num == 4){
  tft.fillRect(50,5,70,72,BLACK);
  
  if(r_sw == 0){
    //Repeat List
    tft.fillRect(50,25,52,52,GREEN);
    tft.drawRect(51,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,33);
    tft.print("R");
    r_sw = 1;
  }else if(r_sw == 1){
    //Repeat Current Track
    tft.fillRect(50,25,52,52,GREEN);
    tft.drawRect(51,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,33);
    tft.print("R");
    tft.fillCircle(102,25,10,GREEN);
    tft.setTextSize(2);
    tft.setCursor(98,18);
    tft.print("1");
    r_sw = 2;
  }else if(r_sw == 2){
    //Repeat Off
    tft.fillRect(50,25,52,52,WHITE);
    tft.drawRect(51,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(63,33);
    tft.print("R");
    r_sw = 0;
  }
  delay(200);
}

if(button_num == 5){
  tft.fillRect(137,25,52,52,BLACK);
  
  if(s_sw == 0){
    tft.fillRect(137,25,52,52,GREEN);
    tft.drawRect(138,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(150,33);
    tft.print("S");
    s_sw = 1;
  }else if(s_sw == 1){
    tft.fillRect(137,25,52,52,WHITE);
    tft.drawRect(138,26,50,50,BLACK);
    tft.setTextColor(BLACK);
    tft.setTextSize(5);
    tft.setCursor(150,33);
    tft.print("S");
    s_sw = 0;
  }
  delay(200);
}

if(button_num == 6){

  volume = volume+5;
  
  if(volume<0 || volume>100){
    volume = volume-5;
  }

   if(mute_sw == 1){
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(97,220);
    tft.print("Mute");
    mute_sw = 0;
    volume = 0;
  }
  
  tft.fillRect(70,245,100,15,WHITE);
  tft.fillRect(70,245,volume,15,GREEN);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.setCursor(190,242);
  tft.print("+");
  delay(200);
  tft.setTextColor(WHITE);
  tft.setCursor(190,242);
  tft.print("+");
}

if(button_num == 7){

  if(mute_sw == 0){
     volume = volume-5;
  }

  if(volume<0 || volume>100){
    volume = volume+5;
  }

  if(volume == 0){
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.setCursor(97,220);
    tft.print("Mute");
    mute_sw = 1;
  }
  
  tft.fillRect(70,245,100,15,WHITE);
  if(mute_sw == 0){
    tft.fillRect(70,245,volume,15,GREEN);
  }
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.setCursor(35,242);
  tft.print("-");
  delay(200);
  tft.setTextColor(WHITE);
  tft.setCursor(35,242);
  tft.print("-");
}

if(button_num == 8){
  if(mute_sw == 1){
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(97,220);
    tft.print("Mute");
    tft.fillRect(70,245,100,15,WHITE);
    tft.fillRect(70,245,volume,15,GREEN);
    mute_sw = 0;
  }else{
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.setCursor(97,220);
    tft.print("Mute");
    tft.fillRect(70,245,100,15,WHITE);
    tft.fillRect(70,245,0,15,GREEN);
    mute_sw = 1;
  }
  delay(400);
}

if(button_num == 9){
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.setCursor(97,280);
  tft.print("Sync");
  delay(200);
  clear_LCD();
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.setCursor(67,160);
  tft.print("Resyncing Settings");
  screen_init();
}

if(button_num == 10){
  tft.fillTriangle(145,115,145,95,160,105,GREEN);
  delay(200);
  tft.fillTriangle(145,115,145,95,160,105,WHITE);
}

if(button_num == 11){
  tft.fillTriangle(205,115,205,95,220,105,GREEN);
  tft.fillTriangle(190,115,190,95,205,105,GREEN);
  delay(200);
  tft.fillTriangle(205,115,205,95,220,105,WHITE);
  tft.fillTriangle(190,115,190,95,205,105,WHITE);
}

if(button_num == 12){
  tft.fillTriangle(95,115,95,95,80,105,GREEN);
  delay(200);
  tft.fillTriangle(95,115,95,95,80,105,WHITE);
}

if(button_num == 13){
  tft.fillTriangle(50,115,50,95,35,105,GREEN);
  tft.fillTriangle(35,115,35,95,20,105,GREEN);
  delay(200);
  tft.fillTriangle(50,115,50,95,35,105,WHITE);
  tft.fillTriangle(35,115,35,95,20,105,WHITE);
}

button_num = 0;


}
