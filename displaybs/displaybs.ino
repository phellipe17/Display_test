
#include "SevSeg.h"

#define BUZZER   10
#define LED_LANE_L 2
#define LED_LANE_R 3
#define LED_COLLISION 4
#define LED_GO A5
#define LED_TLR 1000
#define LED_GPS 12
#define LED_SPEED 11

#define LED_DBG_LTE  13 

unsigned long permitted_lane_timer;
unsigned long prohibited_lane_timer;
unsigned long collision_timer;
unsigned long tlr_timer;
unsigned long go_timer;
unsigned long speed_timer;
unsigned long distance_timer;
unsigned long id_timer;
unsigned long to_signal_l_timer;
unsigned long to_signal_r_timer;
unsigned long to_signal_l2_timer;
unsigned long to_signal_r2_timer;


int permitted_lane_timer_set = 0;
int prohibited_lane_timer_set = 0;
int collision_timer_set = 0;
int tlr_timer_set = 0;
int speed_timer_set = 0;
int go_timer_set = 0;

int collision_level = 0;
int speed_level = 0;

int id_timer_set = 0;
int distance_timer_set = 0;

int to_signal_l_timer_set = 0;
int to_signal_r_timer_set = 0;
int to_signal_l2_timer_set = 0;
int to_signal_r2_timer_set = 0;

SevSeg sevseg;
char c;
String rString;
static float collision_distance;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_LANE_L, OUTPUT);
  pinMode(LED_LANE_R, OUTPUT);
  pinMode(LED_COLLISION, OUTPUT);
  pinMode(LED_GPS, OUTPUT);
  pinMode(LED_SPEED, OUTPUT);
  pinMode(LED_TLR, OUTPUT);
  pinMode(LED_GO, OUTPUT);
  pinMode(LED_DBG_LTE, OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(LED_LANE_L, LOW);
  digitalWrite(LED_LANE_R, LOW);
  digitalWrite(LED_COLLISION, LOW);
  digitalWrite(LED_GPS, LOW);
  digitalWrite(LED_SPEED, LOW);
  digitalWrite(LED_TLR, LOW);
  digitalWrite(LED_GO, LOW);
  digitalWrite(LED_DBG_LTE, LOW);

  byte numDigits = 2;   //Números de dígitos do display, é válido para qualquer display (1, 2, 3, 4, n dígitos);

  byte digitPins[] = {7, 8}; //Pino dos ânodos ou cátodos.
  byte segmentPins[] = {5, 6, A4, A1, A2, 9, A0, A3}; //Pinos dos segmentos do A ao G e DP
 
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins); //Aqui se seu display for cátodo é preciso trocar COMMON_ANODE para COMMON_CATHODE
  sevseg.setBrightness(100); //Define o brilho do display
       
  permitted_lane_timer=0;
  prohibited_lane_timer=0;
  collision_timer=0;
  speed_timer = 0;
  go_timer = 0;
  tlr_timer = 0;
  distance_timer = 0;
  rString="";
}

// the loop function runs over and over again forever
void loop() {
  static byte prev_c;
  
  while (Serial.available()) {
    c = Serial.read();
    rString += c;
    //delay(2);
      
    if(c == 'r')
    {
      
      Serial.println(rString);

      if(rString == "tlr" && tlr_timer_set == 0)
      {
        tlr_timer = millis() + 750;
        tlr_timer_set = 1;
      }
      
      if(rString == "gor" && go_timer_set == 0)
      {
        go_timer = millis() + 500;
        go_timer_set = 1;
      }
      
      if(rString == "lar" && permitted_lane_timer_set == 0)
      {
        permitted_lane_timer = millis() + 500;
        permitted_lane_timer_set = 1;
      }
      
      if(rString == "lpr" && prohibited_lane_timer_set == 0)
      {
        prohibited_lane_timer = millis() + 1000;
        prohibited_lane_timer_set = 1;
      }
      
      if(rString == "c1r" && collision_timer_set == 0)
      {
        collision_timer = millis() + 1000;
        collision_timer_set = 1;
        collision_level = 1;
      }
      
      if(rString == "c2r" && collision_timer_set == 0)
      {
        collision_timer = millis() + 1000;
        collision_timer_set = 1;
        collision_level = 2;
      }
      
      if(rString == "c3r" && collision_timer_set == 0)
      {
        collision_timer = millis() + 750;
        collision_timer_set = 1;
        collision_level = 3;
      }
      
      if(rString == "sr" && speed_timer_set == 0)
      {
        speed_timer = millis() + 1000;
        speed_timer_set = 1;
      }
        
      if(rString == "gr")
      {
        digitalWrite(LED_GPS, HIGH);
      }
      
      if(rString == "gfr")
      {
        digitalWrite(LED_GPS, LOW);
      }
      
      if(rString == "lteonr")
      {
        digitalWrite(LED_DBG_LTE, HIGH);
      }
      
      if(rString == "lteoffr")
      {
        digitalWrite(LED_DBG_LTE, LOW);
      }
            
      if(rString == "idr" && id_timer_set == 0)
      {
        id_timer = millis() + 1000;
        id_timer_set = 1;
      }
      
      if(rString == "lefr" && to_signal_l_timer_set == 0)
      {
        to_signal_l_timer = millis() + 1000;
        to_signal_l_timer_set = 1;
      }

      if(rString == "lef2r" && to_signal_l2_timer_set == 0)
      {
        to_signal_l2_timer = millis() + 1000;
        to_signal_l2_timer_set = 1;
      }

      if(rString == "gir" && to_signal_r_timer_set == 0)
      {
        to_signal_r_timer = millis() + 1000;
        to_signal_r_timer_set = 1;
      }

      if(rString == "gi2r" && to_signal_r2_timer_set == 0)
      {
        to_signal_r2_timer = millis() + 1000;
        to_signal_r2_timer_set = 1;
      } 


      char rStringArray[64];
      rString.toCharArray(rStringArray, 64);
      if(rStringArray[0] == 'n' && distance_timer_set == 0)
      { 
        String number = "";
        number += rStringArray[1];
        number += rStringArray[2];
        number += rStringArray[3];
    
        char numberArray[64];
        number.toCharArray(numberArray, 64);
        
        collision_distance = atof(numberArray);   
        distance_timer = millis() + 2000;
        distance_timer_set == 1;
      }
      
      rString="";
    }
  }  

   if( millis() < distance_timer)
   {
     
     if(collision_distance >= 10.0)
     {
       sevseg.setNumber(collision_distance, 0); // Seta o número que será mostrado no display
     }
     else
     {
       sevseg.setNumber(collision_distance, 1); // Seta o número que será mostrado no display       
     }

     sevseg.refreshDisplay(); // Executar repetidamente
   }
   else if (distance_timer_set)
   {
     distance_timer_set = 0;
   }

   if(millis() < id_timer)
   {
     sevseg.setChars("id");
     sevseg.refreshDisplay(); // Executar repetidamente
     analogWrite(BUZZER, 80);
   }
   else if (id_timer_set) {
     analogWrite(BUZZER, 0);
     id_timer_set = 0;
   }

   if(millis() < to_signal_l_timer)
   {
     byte segs[] = {B00011000,B00000000};
     sevseg.setSegments(segs);
     sevseg.refreshDisplay(); // Executar repetidamente
     analogWrite(BUZZER, 80);
   }
   else if (to_signal_l_timer_set) {
     analogWrite(BUZZER, 0);
     to_signal_l_timer_set = 0;
   }

   if(millis() < to_signal_l2_timer)
   {
     byte segs[] = {B00011000,B00000000};
     sevseg.setSegments(segs);
     sevseg.refreshDisplay(); // Executar repetidamente
    //  analogWrite(BUZZER, 80);
   }
   else if (to_signal_l2_timer_set) {
     analogWrite(BUZZER, 0);
     to_signal_l2_timer_set = 0;
   }  
   
   if(millis() < to_signal_r_timer)
   {
     byte segs[] = {B00000000,B00001100};
     sevseg.setSegments(segs);
     sevseg.refreshDisplay(); // Executar repetidamente
     analogWrite(BUZZER, 80);
   }
   else if (to_signal_r_timer_set) {
     analogWrite(BUZZER, 0);
     to_signal_r_timer_set = 0;
   }

   if(millis() < to_signal_r2_timer)
   {
     byte segs[] = {B00000000,B00001100};
     sevseg.setSegments(segs);
     sevseg.refreshDisplay(); // Executar repetidamente
    //  analogWrite(BUZZER, 80);
   }
   else if (to_signal_r2_timer_set) {
     analogWrite(BUZZER, 0);
     to_signal_r2_timer_set = 0;
   }

   if(!distance_timer_set && !id_timer_set && !to_signal_l_timer_set && !to_signal_r_timer_set && !to_signal_l2_timer_set && !to_signal_r2_timer_set) {
     sevseg.blank();
     sevseg.refreshDisplay();
   }
   
   if(millis() < tlr_timer)
   {
     static int toggle = 1;
     static long prev_time = millis();

     if(toggle)     
     {    
        digitalWrite(LED_TLR,HIGH);       
        analogWrite(BUZZER, 80);
     }
     else 
     {
       digitalWrite(LED_TLR,LOW);
       analogWrite(BUZZER, 0);
     }
     
     long curr_time = millis();
     if((curr_time - prev_time) >= 126)
     {
       prev_time = curr_time;
       toggle = !toggle;
     }
   }
   else if(tlr_timer_set)
   {
     digitalWrite(LED_TLR, LOW);
     tlr_timer_set = 0;
   }
   
   if(millis() < go_timer)
   {
     static int toggle = 1;
     static long prev_time = millis();

     if(toggle)     
     {    
        digitalWrite(LED_GO,HIGH);       
        analogWrite(BUZZER, 80);
     }
     else 
     {
       digitalWrite(LED_GO,LOW);
       analogWrite(BUZZER, 0);
     }
     
     long curr_time = millis();
     if((curr_time - prev_time) >= 126)
     {
       prev_time = curr_time;
       toggle = !toggle;
     }
   }
   else if(go_timer_set)
   {
     digitalWrite(LED_GO, LOW);
     go_timer_set = 0;
   }
   
   if (millis() < permitted_lane_timer)
   {
     digitalWrite(LED_LANE_L,HIGH);
     digitalWrite(LED_LANE_R,HIGH);
     //if(collision_timer_set == 0 && prohibited_lane_timer_set == 0)
      // analogWrite(BUZZER, 80);    
   }
   else if(permitted_lane_timer_set)
   {
     digitalWrite(LED_LANE_L,LOW);
     digitalWrite(LED_LANE_R,LOW);
     if(collision_timer_set == 0 && prohibited_lane_timer_set == 0)
       analogWrite(BUZZER, 0);
       
     permitted_lane_timer_set = 0;   
   }
  
   if (millis() < prohibited_lane_timer)
   { 
     static int toggle = 1;
     static long prev_time = millis();

       if(toggle)     
       {
         digitalWrite(LED_LANE_L,HIGH);       
         digitalWrite(LED_LANE_R,HIGH);       
         if(collision_timer_set == 0)
           analogWrite(BUZZER, 80);
       }
       else 
       {
         digitalWrite(LED_LANE_L,LOW);
         digitalWrite(LED_LANE_R,LOW);
         if(collision_timer_set == 0)
           analogWrite(BUZZER, 0);
       }
     
     long curr_time = millis();
     if((curr_time - prev_time) >= 125)
     {
       prev_time = curr_time;
       toggle = !toggle;
     }
   }
   else if(prohibited_lane_timer_set)
   {
     digitalWrite(LED_LANE_L,LOW);
     digitalWrite(LED_LANE_R,LOW);
     analogWrite(BUZZER, 0);
     prohibited_lane_timer_set = 0;   
   }
                    
    if (millis() < collision_timer)
    {
      static int toggle = 1;
      static long prev_time = millis();

      if(toggle)     
      {
        analogWrite(BUZZER, 80);
        digitalWrite(LED_COLLISION,HIGH);
      }
      else 
      {
        analogWrite(BUZZER, 0);
        digitalWrite(LED_COLLISION,LOW);
      }
     
      long curr_time = millis();
      if((curr_time - prev_time) >= (collision_level * 125))
      {
        prev_time = curr_time;
        toggle = !toggle;
      }
    }
    else if(collision_timer_set)
    {
      digitalWrite(LED_COLLISION,LOW);
      analogWrite(BUZZER, 0);
      collision_timer_set = 0;
    }
    
    if (millis() < speed_timer)
    {
      digitalWrite(LED_SPEED,HIGH);
      
      if(collision_timer_set == 0 && prohibited_lane_timer_set == 0 && permitted_lane_timer_set == 0)
        analogWrite(BUZZER, 80);
    }
    else if(speed_timer_set)
    {
      digitalWrite(LED_SPEED,LOW);
      
      if(collision_timer_set == 0 && prohibited_lane_timer_set == 0 && permitted_lane_timer_set == 0)
        analogWrite(BUZZER, 0);
      speed_timer_set = 0;
    }
}
