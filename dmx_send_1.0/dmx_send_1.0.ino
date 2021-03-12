/*
  Copyright: Niklas Dubhorn 2020
  Creative Commons Zero v1.0 Universal
  https://github.com/cr4wb4r/dmx-controller 
   
   
  dmxMaxChannel = 32; !!!!!! This is the Reason, why the 1st and 4th lamp are the same: no LampD
  Solution --> DMXSerial.maxChannel(60); ?
*/

#include <DMXSerial.h>
#include <Ramp.h>
#include "effects.h"

#define writeA(r, g, b, w, x)  DMXSerial.write(4, x); DMXSerial.write(5, r); DMXSerial.write(6, g); DMXSerial.write(7, b); DMXSerial.write(8, w);
#define writeB(r, g, b, w, x)  DMXSerial.write(12, x); DMXSerial.write(13, r); DMXSerial.write(14, g); DMXSerial.write(15, b); DMXSerial.write(16, w);
#define writeC(r, g, b, w, x)  DMXSerial.write(20, x); DMXSerial.write(21, r); DMXSerial.write(22, g); DMXSerial.write(23, b); DMXSerial.write(24, w);

/////////////////////////////  RAMP  ////////////////////////////////////
rampByte R1;  rampByte G1;  rampByte BB1; rampByte W1;
rampByte R2;  rampByte G2;  rampByte B2;  rampByte W2;
rampByte R3;  rampByte G3;  rampByte B3;  rampByte W3;

byte rLast[3] = {0, 0, 0};
byte gLast[3] = {0, 0, 0};
byte bLast[3] = {0, 0, 0};
byte wLast[3] = {0, 0, 0};

byte rOut[3] = {0, 0, 0};
byte gOut[3] = {0, 0, 0};
byte bOut[3] = {0, 0, 0};
byte wOut[3] = {0, 0, 0};

//char num[16] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
volatile byte data = 0;
volatile bool newData = false;
int cleanData = 0;
int aVal;
int curFx = 0, lastFx = 0, cntFx = 0;
int cmpVal[2];
bool Fx_state = true;  // * --> start, # --> stop
bool altValues = false; //10 to 18 instead of 1 to 9; D Button
bool inputEnable = true;  //false while a button is still pressed --> no multiple triggering
int loopCnt = 0;

bool autoMode = false;
bool twoTimes = false;  //autoMode: every Fx two times in a row
int strobeSpeed = 220;

byte hexData[25][2] = {{B00000000, B00100100}, //1  ----  B0000000, B00010000 is the DP - is used instead of segment 'L' on pin 13 - not used 
                      {B00000100, B10001101},
                      {B00000000, B10101101},
                      {B01000000, B00101100},
                      {B01000000, B10101001},
                      {B01000100, B10101001},
                      {B00000000, B00100101},
                      {B01000100, B10101101},
                      {B01000000, B10101101}, //9
                      {B01010100, B11100101},
                      {B00010000, B01100100},
                      {B00010100, B11001101},
                      {B00010000, B11101101},
                      {B01010000, B01101100},
                      {B01010000, B11101001},
                      {B01010100, B11101001},
                      {B00010000, B01100101},
                      {B01010100, B11101101},
                      {B01010000, B11101101}, //19
                      {B10011000, B01101101}, //A
                      {B11000101, B10101101}, //B; b = B00010001, B11101000
                      {B10011001, B10101001}, //S
                      {B00110000, B01100110}, //M not used
                      {B10000011, B10000011}, //Z not used
                      {B10011000, B01001101}};//P
                    
//////////////////////////// AUX  &&  MIDI  ////////////////////////////////////
bool soundMode = false;
int inputVal = 0;
int referenceVal = 0;
bool stillHigh = false;  //if high, no pulse is detected

unsigned long time = 0;
unsigned long startTime = 0;

byte midiIn = 0;
int midiCnt = 0;
bool midi = false;  //switch on pin 30
int beatTreshold = 24;  //count to this number before switching
  
//////////////////////////////  DMX  /////////////////////////////////////
byte dim = 0; //A0 - slider 1
int dur = 1000;  //A2 - slider 2
bool longDur = true;  //no slow fading when beatTreshold is small

int all_Val = 0; //A4 - slider 3
int alllast;  //to check if the values have changed
byte R, G, B, W;

bool strobo = false;
bool black = false;
bool all = false;

////////////////////////////////     RAMP &&  KEYPAD  &&  SOUND      //////////////////////////////////

void setRamp(){

    dur = constrain(map(analogRead(A2),0,1023,0,4096), 100, 4000);
    if(!longDur) dur = 100;
    
    R1.go(lamp1[curFx][0][cntFx], dur);  
    G1.go(lamp1[curFx][1][cntFx], dur);
    BB1.go(lamp1[curFx][2][cntFx], dur);
    W1.go(lamp1[curFx][3][cntFx], dur);
         
    R2.go(lamp2[curFx][0][cntFx], dur);
    G2.go(lamp2[curFx][1][cntFx], dur);
    B2.go(lamp2[curFx][2][cntFx], dur);
    W2.go(lamp2[curFx][3][cntFx], dur);
          
    R3.go(lamp3[curFx][0][cntFx], dur);
    G3.go(lamp3[curFx][1][cntFx], dur);
    B3.go(lamp3[curFx][2][cntFx], dur);
    W3.go(lamp3[curFx][3][cntFx], dur);     
} //setRamp()

void pauseRamp(){
if(!BB1.isPaused()){ 
    R1.pause(); R2.pause(); R3.pause();
    G1.pause(); G2.pause(); G3.pause(); 
    BB1.pause(); B2.pause(); B3.pause();
    W1.pause(); W2.pause(); W3.pause();
  }
}

void resumeRamp(){
  if(BB1.isPaused()){
    R1.resume(); R2.resume(); R3.resume();
    G1.resume(); G2.resume(); G3.resume();
    BB1.resume(); B2.resume(); B3.resume();
    W1.resume(); W2.resume(); W3.resume(); 
  }
}

void DATA(){    //ISR
  digitalWrite(4, LOW);

  newData = true;

  data  = (digitalRead(26) * 1);
  data  += (digitalRead(24) * 2);
  data  += (digitalRead(27) * 4);
  data  += (digitalRead(25) * 8);

  digitalWrite(4, HIGH);
}

int getChar(byte dataIn){
  int ret = -1;

  switch (dataIn){
    case 0: ret = 0; break; //1
    case 1: ret = 3; break; //4
    case 2: Fx_state = true; break; //*
    case 3: ret = 6; break; //7
    case 4: ret = 1; break; //2
    case 5: ret = 4; break; //5
    case 6: ret = 9; break;  //0 = 10
    case 7: ret = 7; break; //8
    case 8: ret = 2; break; //3
    case 9: ret = 5; break; //6
    case 10: Fx_state = false; break; //#
    case 11: ret = 8; break; //9
    case 12: all = true; break; //A
    case 13: black = true; break; //B
    case 14: altValues ? altValues = false : altValues = true; break; //D
    case 15: strobo = true; break; //C
    default: break;
  }
  
  return(ret);
}

int check(byte current, byte last){
  if(current < 5) current = 0;
  if(current > 250) current = 255;
  if(current == last){
    return(0);
  }else{
    return(current +1);  
  }
} //check()

bool newInput(){
  bool ret = false;
  if(analogRead(A8) < 1000 || analogRead(A10) < 1000 || analogRead(A12) <1000 || analogRead(A14) < 1000){
    delay(5);
    if(analogRead(A8) < 1000 || analogRead(A10) < 1000 || analogRead(A12) <1000 || analogRead(A14) < 1000) ret = true;  //wait a bit for stability and check again
  }
  return(ret);
}//newInput

void checkSwitch(bool *var, int pin){
  
  if((*var == false) && (digitalRead(pin) == LOW)){
    delay(100);
    *var = true;
  }
  if((*var == true) && (digitalRead(pin) == HIGH)){
    delay(100);
    *var = false;
  }
}

int getBeatTreshold(){
  int bT;
  
  if(analogRead(A6) < 15) bT = 6; //sixteenth note
  else if(analogRead(A6) < 200) bT = 12;  //eighth note
  else if(analogRead(A6) < 600) bT = 24; //quarter note
  else if(analogRead(A6) < 1000) bT = 48; //half note
  else bT = 96; //full note

  (bT < 30) ? longDur = false : longDur = true; //no slow fading if the beatTreshold is to small (switching is to fast)
  return(bT);
}

bool getStep(){  
  bool ret = false;
  
  if(Serial3.available()){
    midiIn = Serial3.read();

    if(midiIn == 248) midiCnt ++; //beat
    else if((midiIn == 250) || (midiIn == 251)) midiCnt = 0;  //start or continue
  }// if
  
  if(midiCnt >= beatTreshold){ 
    midiCnt = 0;
    beatTreshold = getBeatTreshold(); //only change Treshold, when the set value is reached
    
    ret = true;
  }
  
  return(ret);
}

////////////////////////////////     DMX  &&  hexSeg      //////////////////////////////////

void hexSeg(int number){
  digitalWrite(9, 0);
  shift(hexData[number][0], hexData[number][1]);
  digitalWrite(9, 1);
} //hexSeg()

void shift(byte dataA, byte dataB ) {
  int pinState;
  
  digitalWrite(7, 0);
  digitalWrite(8, 0);

  for (int i=7; i>=0; i--)  {
    digitalWrite(8, 0);
    
    if ( dataA & (1<<i) ) {
      pinState= 1;
    }else {  
      pinState= 0;
    }
    
    digitalWrite(7, pinState);
    digitalWrite(8, 1);
    digitalWrite(7, 0);
  }
  
  for (int i=7; i>=0; i--)  {
    digitalWrite(8, 0);
    
    if ( dataB & (1<<i) ) {
      pinState= 1;
    }else {  
      pinState= 0;
    }
    
    digitalWrite(7, pinState);
    digitalWrite(8, 1);
    digitalWrite(7, 0);
  }
  
  digitalWrite(8, 0);
}//shift

bool filter(int current, int last){
  if(current >= last){
    if(current - last <= 2) return(false);
    else return(true);  
  }else{
     if(last - current <= 2) return(false);
     else return(true);
  }
} //filter()

void writeOutput(){
  dim = map(analogRead(A0),0,1023,0,255);  

  rOut[0] = check(R1.update(), rLast[0]);        
  if(rOut[0] > 0) rLast[0] = rOut[0] -1;
  
  gOut[0] = check(G1.update(), gLast[0]);        
  if(gOut[0] > 0) gLast[0] = gOut[0] -1;
  
  bOut[0] = check(BB1.update(), bLast[0]);        
  if(bOut[0] > 0) bLast[0] = bOut[0] -1;

  wOut[0] = check(W1.update(), wLast[0]);
  if(wOut[0] > 0) wLast[0] = wOut[0] -1;
  
  writeA(rLast[0], gLast[0], bLast[0], wLast[0], dim)

  rOut[1] = check(R2.update(), rLast[1]);        
  if(rOut[1] > 0) rLast[1] = rOut[1] -1;
  
  gOut[1] = check(G2.update(), gLast[1]);        
  if(gOut[1] > 0) gLast[1] = gOut[1] -1;
  
  bOut[1] = check(B2.update(), bLast[1]);        
  if(bOut[1] > 0) bLast[1] = bOut[1] -1;
  
  wOut[1] = check(W2.update(), wLast[1]);
  if(wOut[1] > 0) wLast[1] = wOut[1] -1;
  
  writeB(rLast[1], gLast[1], bLast[1], wLast[1], dim)
  
  rOut[2] = check(R3.update(), rLast[2]);        
  if(rOut[2] > 0) rLast[2] = rOut[2] -1;
  
  gOut[2] = check(G3.update(), gLast[2]);        
  if(gOut[2] > 0) gLast[2] = gOut[2] -1;
  
  bOut[2] = check(B3.update(), bLast[2]);        
  if(bOut[2] > 0) bLast[2] = bOut[2] -1;
  
  wOut[2] = check(W3.update(), wLast[2]);
  if(wOut[2] > 0) wLast[2] = wOut[2] -1;
  
  writeC(rLast[2], gLast[2], bLast[2], wLast[2], dim)
} //writeOutput

void strobe(bool onoff){
  if(onoff){
    DMXSerial.write(4, 255); DMXSerial.write(12, 255); DMXSerial.write(20, 255);
  
    DMXSerial.write(1, 255);  
    DMXSerial.write(9, 255);
    DMXSerial.write(17, 255);

    DMXSerial.write(2, 105);  //white color
    DMXSerial.write(10, 105);
    DMXSerial.write(18, 105); //no need to change again
  
    DMXSerial.write(3, strobeSpeed);
    DMXSerial.write(11, strobeSpeed);
    DMXSerial.write(19, strobeSpeed);

    delay(10);
  }else{
    DMXSerial.write(1, 0);
    DMXSerial.write(9, 0);
    DMXSerial.write(17, 0);

    DMXSerial.write(3, 0);
    DMXSerial.write(11, 0);
    DMXSerial.write(19, 0);

    delay(10);
  }//else
}//strobe

/////////////////////////////////////////   SETUP &&  LOOP  /////////////////////////////////////////////
void setup() {
  DMXSerial.init(DMXController);

  pinMode(9, OUTPUT); //latch
  pinMode(7, OUTPUT); //data
  pinMode(8, OUTPUT); //clock

  pinMode(A8, INPUT_PULLUP);  //for the keypad
  pinMode(A10, INPUT_PULLUP);
  pinMode(A12, INPUT_PULLUP);
  pinMode(A14, INPUT_PULLUP);

  pinMode(3, INPUT);  //INTERRUPT
  pinMode(4, OUTPUT); //output enable
  pinMode(25, INPUT);  //data d
  pinMode(27, INPUT);
  pinMode(24, INPUT);
  pinMode(26, INPUT); //data a
  
  pinMode(20, INPUT_PULLUP);  //automatic mode
  pinMode(19, INPUT_PULLUP);  //sound mode

  pinMode(30, INPUT_PULLUP);  //HIGH = AUX, LOW = MIDI

  if(digitalRead(30) == LOW){ //control sound mode in setup, no later change
    midi = true;
    Serial3.begin(31250); //MIDI -- Rx3 on Pin D15
  }// if
  
  DMXSerial.write(1, 0); DMXSerial.write(9, 0); DMXSerial.write(17, 0); //cleanup
  startTime = millis();

  digitalWrite(4, HIGH);  //disable data output
  attachInterrupt(1, DATA, RISING); //pin3 = INT5 = number 1

} //setup()


void loop() {

  if(newData){
    cleanData = getChar(data);
    if(cleanData >= 0){
      curFx = cleanData;
    }
    newData = false;
    if(altValues && (curFx < 9)) curFx += 10;
    if(!altValues && (curFx > 9)) curFx -= 10;  //curFx 9 is Fx 10 --> not altered
  }  
  checkSwitch(&autoMode, 20);
  checkSwitch(&soundMode, 19);
  
//////////////////////////  soundMode  //////////////////////////
  
  if(soundMode){
    if(midi){     //midi  
      if(getStep()){
        cntFx ++;
        if(cntFx > 7) cntFx = 0;
        setRamp();
        hexSeg(curFx);
      }
      
    }
    else{        //aux
      if(longDur) longDur = false;
      time = millis() - startTime;
      inputVal = analogRead(A15) - 200;
      referenceVal = analogRead(A6);

      if(time >= 400) stillHigh = false;    //pulses possible every 0.5 seconds
        
      if((stillHigh == false) && (inputVal > referenceVal)){
        cntFx ++;
        if(cntFx > 7) cntFx = 0;
        setRamp();
        hexSeg(curFx);
        
        startTime = millis();
        stillHigh = true;
      }

      if(time >= 5000){    //simulate a pulse every 5 seconds, if nothing happens
        startTime = millis();
      }//if
    }//else
  }//soundMode

  else if(!longDur) longDur = true;  //slow fading is active outside soundMode
  
//////////////////////////  autoMode  //////////////////////////  
   
  if(autoMode || soundMode){     
    if((cntFx >= 7) && (twoTimes == false)){
      twoTimes = true;
    }
    else if((cntFx >= 7) && (twoTimes == true)){  //running every Fx two times
      curFx = random(0, 14); //random curFx -> main function of autoMode      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! set to 19 !!!!!!!!!!!!!!!!!!!!!!!!!!!
      twoTimes = false;
    }
  }
  
//////////////////////////  black  //////////////////////////
  if(black){
    black = false;
    pauseRamp();
    hexSeg(20);
    delay(100);

    writeA(0, 0, 0, 0, 0);
    writeB(0, 0, 0, 0, 0);
    writeC(0, 0, 0, 0, 0);
    
    while(true){      
      if(midi) if(Serial3.available()) Serial3.read();  //clearing the input buffer

      delay(10);
      if(newData) break; 
    }//while
    inputEnable = true;
  }//black

//////////////////////////  strobe  //////////////////////////
  else if(strobo){
    strobo = false;
    pauseRamp();
    hexSeg(21);
    
    strobe(true);
    delay(100);
    
    while(true){
      if(midi) if(Serial3.available()) Serial3.read();  //clearing the input buffer
      
      delay(10);
      if(newData) break; 
    }//while
    strobe(false);
    inputEnable = true;
  }//strobo

//////////////////////////  all  //////////////////////////
  else if(all){
      all = false;
      alllast = 0;
      pauseRamp();
      hexSeg(19);

      delay(100);
      while(true){      
        if(midi) if(Serial3.available()) Serial3.read();  //clearing the input buffer

        all_Val = analogRead(A4);
        dim = map(analogRead(A0),0,1023,0,255);
                
        if(all_Val < 20){all_Val = 0;}
        
        if(filter(all_Val, alllast)){ 
          if(all_Val < 15){R = 0; G = 0; B = 0;}
          else if(all_Val < 30){R = 255; G = 0; B = 0;}
          else if(all_Val < 300){R = (255 - map(all_Val, 30, 299, 0, 255)); B = (255 - R); G = 0;}
          else if(all_Val < 360){R = 0; G = 0; B = 255;}
          else if(all_Val < 700){B = (255 - map(all_Val, 360, 699, 0, 255)); G = (255 - B); R = 0;}
          else if(all_Val < 760){R = 0; G = 255; B = 0;}
          else if(all_Val < 1001){G = (255 - map(all_Val, 690, 1000, 0, 255)); R = (255 - G); B = 0;}
          else{ R = 255; G = 0; B = 0;}
          W = 0;
      
          writeA(R, G, B, W, dim)
          writeB(R, G, B, W, dim)
          writeC(R, G, B, W, dim)
                
        } //filter all input
        alllast = all_Val;
        
        delay(10);
        if(newData) break; 
      }//while
      inputEnable = true;
    }//all

//////////////////////////  Fx Color  //////////////////////////
    else{
      if(Fx_state){
        resumeRamp();

        if(!soundMode){          
          if(BB1.isFinished()){   //new Fx if the Ramp is finished
            cntFx++;
            if(cntFx > 7) cntFx = 0;
            setRamp();
            hexSeg(curFx);
          }//cntFx
        }

        writeOutput();

        if(!inputEnable) loopCnt ++;   //enable Input, after 25 cycles
        if(loopCnt > 25){
          loopCnt = 0;
          if(!inputEnable && !newInput()) inputEnable = true;
        }
         
      }//color change is active
      else hexSeg(24); //color change is stopped
      pauseRamp();
    }//color change
}//loop
