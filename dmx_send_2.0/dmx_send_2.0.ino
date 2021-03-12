/*
  Copyright: Niklas Dubhorn 2020
  Creative Commons Zero v1.0 Universal
  https://github.com/cr4wb4r/dmx-controller  

     
  A0-A5: rotating potentiometers -> sub control
  A6-A9: sliding potentiometers -> main control
  D2-D4: rotary encoder
  D5-D7: toggle switches -> black, strobe and pause
  D8-D11, D13, D14: SPI Display
*/
#include "effects.h"
#include <Adafruit_GFX.h>    // Graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>  //Display
#include <Bounce2.h>  //Button debouncing
#include <MIDI.h> //Midi
#include <TeensyDMX.h>  //DMX
#include <Ramp.h> //Ramp

#define TFT_CS        10
#define TFT_RST        8 
#define TFT_DC         9
#define pinA           2
#define pinB           3
#define Switch         4
#define writeA(r, g, b, w, x)  dmx.set(4, x); dmx.set(5, r); dmx.set(6, g); dmx.set(7, b); dmx.set(8, w);
#define writeB(r, g, b, w, x)  dmx.set(12, x); dmx.set(13, r); dmx.set(14, g); dmx.set(15, b); dmx.set(16, w);
#define writeC(r, g, b, w, x)  dmx.set(20, x); dmx.set(21, r); dmx.set(22, g); dmx.set(23, b); dmx.set(24, w);
#define writeD(r, g, b, w, x)  dmx.set(28, x); dmx.set(29, r); dmx.set(30, g); dmx.set(31, b); dmx.set(32, w);
#define writeHead(s, x)        dmx.set(67, x); dmx.set(73, s);
#define writeFog(t, f)        dmx.set(33, t); dmx.set(34, f);
//#define writeBar(y, x)        dmx.set(0000, x); dmx.set(0000, y);

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Bounce debounceSwitch = Bounce();
Bounce debounceStrobe = Bounce();
Bounce debounceBlack = Bounce();
Bounce debouncePause = Bounce();

namespace teensydmx = ::qindesign::teensydmx;
teensydmx::Sender dmx{Serial1};

rampByte R1;  rampByte G1;  rampByte BB1; rampByte W1;
rampByte R2;  rampByte G2;  rampByte B2;  rampByte W2;
rampByte R3;  rampByte G3;  rampByte B3;  rampByte W3;
rampByte R4;  rampByte G4;  rampByte B4;  rampByte W4;

/////////MUSIC/////////////////////
byte clkCount = 0;
byte beatTreshold[5] = {6, 12, 24, 48, 96};
byte beat = 2;  //choose from beatTreshold
int musicTreshold = 400;  //Treshold for musicMode without MIDI
unsigned long prevMillis;
int Time = 0;

///////MAIN OPERATIONS////////////////
bool blackMode = false, strobeMode = false, autoMode = true, musicMode = false, staticMode = false, playPause = true;  //playPause == true --> play
bool MIDIaux = false;
int color = 0, lastColor = 0; //for static
byte curFx = 0, lastFx = 0, cntFx = 0;
byte strobeSpeed = 220, dim = 0;
int dur = 0;
bool speedAdjust = false;
float speedAdjustFactor = 1.0;
bool notNormalUse = false;  //turning strobe off again
bool twoTimes = false;

////////////DISPLAY/////////////
volatile int cursorPos = 0; //has to be int, because byte has no negative numbers
int cursorLastPos = 0;      //the same
volatile bool Anew = true, Bnew = false, Alast = false, Blast = true;
volatile bool encoderChange = false;
volatile bool buttonChange = false;
bool pageChange = false, pageRefresh = false;
bool main_0[3] = {true, false, false}; //auto, music, static
bool main_1[3] = {false, false, false}; 
bool music_0[3] = {true, false, false};
bool auto_0[3] = {false, true, false};
bool setting_2[3] = {false, false, false};

////////////RAMP/////////////////
byte rLast[4] = {0, 0, 0, 0};  //Ramp
byte gLast[4] = {0, 0, 0, 0};
byte bLast[4] = {0, 0, 0, 0};
byte wLast[4] = {0, 0, 0, 0};
byte rOut[4] = {0, 0, 0, 0};
byte gOut[4] = {0, 0, 0, 0};
byte bOut[4] = {0, 0, 0, 0};
byte wOut[4] = {0, 0, 0, 0};

byte R = 0, G = 0, B = 0, W = 0; //for static

////////////not used////////////
byte potVal[6] = {0, 0, 0, 0, 0, 0};
bool potChanged = false;
bool potActive = true;

////////////SUBDEVICES///////////
byte headDim, soundSens;   //moving Head
byte temp, fog; //fogger
//byte a, b;  //led bar


/////////////////////////////////// display ///////////////////////////////////////////////

typedef struct _obj {
  String entryName[3];
  bool entryState[3];
  _obj *next;
} OBJ;

OBJ firstPage, secondPage, musicSetting, autoSetting, setting2;
OBJ * currentPage = &firstPage; //page that is diplayed
//OBJ * lastStatePage = &firstPage; //page, which contains the selected program
byte lastStateElement = 0;
byte currentMainElement = 0;  //auto, music or static

void assignment(){
  firstPage = {{"Automatik", "Musik", "Statisch"}, {true, false, false}, &secondPage};
  secondPage = {{"Auto-Ausw.", "Musik-Ausw.", "Platzhalter"}, {false, false, false}, &firstPage};
  musicSetting = {{"MIDI", "Zufall", "Abbrechen"}, {false, false, false}, &musicSetting};         //Zufall = Aux, Aux is not yet implemented
  autoSetting = {{"Langsam", "Normal", "Schnell"}, {false, false, false}, &musicSetting};
  setting2 = {{"ja", "nein", "Abbrechen"}, {false, false, false}, &setting2};
}

void printText(OBJ *page){
  tft.fillScreen(ST77XX_BLACK);

  for(int a=0; a<3; a++){
    tft.setCursor(20, (40 * a) + 10);
    (page->entryState[a])? tft.setTextColor(ST77XX_GREEN): tft.setTextColor(ST77XX_WHITE);
    tft.print(page->entryName[a]);
  }
  
  printFx(curFx, lastFx);
}

void printCursor(byte current, byte last){
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(0, (40 * cursorLastPos) + 10);
  tft.print(">");

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, (40 * cursorPos) + 10);
  tft.print(">");
}

void printFx(byte current, byte last){
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(130, 110);
  tft.print(String(last));

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(130, 110);
  tft.print(String(current));
}

void mainMenuHandling(byte * oldElement, byte newElement){

  firstPage.entryState[*oldElement] = false;
  firstPage.entryState[newElement] = true;    
  *oldElement = newElement;
  
  for(int a=0; a<3; a++){
    if(main_0[a] != firstPage.entryState[a]) main_0[a] = firstPage.entryState[a];
  }
    
  if(main_0[0]){      //one has to be active; only one can be active
    autoMode = true;
    staticMode = false;
    musicMode = false;
    currentMainElement = 0;
  }
  else{
    autoMode = false;
    if(main_0[1]){
      musicMode = true;
      staticMode = false;
      currentMainElement = 1;
    }
    else{
      musicMode = false;
      staticMode = true;
      currentMainElement = 2;
    }
  }
}

OBJ * subMenuHandling(OBJ * newPage, byte newElement){
    OBJ * retOBJ = &secondPage;
    
    newPage->entryState[newElement] = true;    

    for(int a=0; a<3; a++){
      main_1[a] = secondPage.entryState[a];
      music_0[a] = musicSetting.entryState[a];
      auto_0[a] = autoSetting.entryState[a];
      setting_2[a] = setting2.entryState[a];
    }

    newPage->entryState[newElement] = false;
    
    if(main_1[0]) retOBJ = &autoSetting;
    if(main_1[1]) retOBJ = &musicSetting;
    if(main_1[2]) retOBJ = &setting2;
    
    if(music_0[0]) MIDIaux = false; //Midi on, aux off
    if(music_0[1]) MIDIaux = true; //aux on, midi off 
    if(auto_0[0]){ speedAdjust = true; speedAdjustFactor = 1.5;} //slow
    if(auto_0[1]) speedAdjust = false; //normal
    if(auto_0[2]){ speedAdjust = true; speedAdjustFactor = 0.75;} //rapid
    //if(setting_2[0]){ potActive = false; setPot(true);}
    //if(setting_2[1] || setting_2[2]){ potActive = true; potChanged = true;}
    
    return(retOBJ);
}

////////////////////////////////// input  ///////////////////////////////////////////////////////

void inputHandling(){
  if(buttonChange){
    if(currentPage != &firstPage) currentPage = subMenuHandling(currentPage, cursorPos);  //subMenu
    else mainMenuHandling(&lastStateElement, cursorPos);  //mainMenu
    buttonChange = false;
    pageRefresh = true;
  }
  
  if(encoderChange){
    if(cursorPos > 2){  
      cursorPos = 0;
      pageChange = true;
    }  
    if(cursorPos < 0){
      cursorPos = 2;
      pageChange = true;
    }
         
    printCursor(cursorPos, cursorLastPos);
    cursorLastPos = cursorPos;
    
    encoderChange = false;
  }
  
  if(pageChange){  //for scrolling
    pageChange = false;
    currentPage = currentPage->next;
    printText(currentPage);
    printCursor(cursorPos, cursorLastPos);
  }
  if(pageRefresh){  //for clicking
    pageRefresh = false;
    printText(currentPage);
    printCursor(cursorPos, cursorLastPos);
  }
}

void doA(){
  delay(1);
  Anew = digitalRead(pinA);

  if(Anew != Alast){
    Alast = Anew;
    
    if(Alast && !Blast){
     cursorPos ++;
     encoderChange = true;
    }
  } 
}

void doB(){
  delay(1);
  Bnew = digitalRead(pinB);

  if(Bnew != Blast){
    Blast = Bnew;
    
    if(Blast && !Alast){
     cursorPos --;
     encoderChange = true;
    }
  }
}

/////////////////////////////////// dmx ////////////////////////////////////////////////////

void doAutoMode(byte dimVal){
  if(BB1.isFinished()){
    cntFx ++;
 
    if(cntFx > 7){
      cntFx = 0;
      if(twoTimes){
        curFx = random(0, 20);
        twoTimes = false;
        printFx(curFx, lastFx);
        lastFx = curFx;
      }
      else twoTimes = true;
    }

    setRamp();
  }
  writeMainOutput(dimVal);
}

void doMidiMode(byte dimVal){
  if(clkCount >= beatTreshold[beat]){
    clkCount = 0;
    cntFx ++;

    beat = map(analogRead(A6), 0, 1023, 0, 4);
    if(cntFx > 7){
      cntFx = 0;
      if(twoTimes){
        curFx = random(0, 20);
        printFx(curFx, lastFx);
        lastFx = curFx;
        twoTimes = false;
      }
      else twoTimes = true;
    }
    writeA(lamp1[curFx][0][cntFx], lamp1[curFx][1][cntFx], lamp1[curFx][2][cntFx], lamp1[curFx][3][cntFx], dimVal)
    writeB(lamp2[curFx][0][cntFx], lamp2[curFx][1][cntFx], lamp2[curFx][2][cntFx], lamp2[curFx][3][cntFx], dimVal)
    writeC(lamp3[curFx][0][cntFx], lamp3[curFx][1][cntFx], lamp3[curFx][2][cntFx], lamp3[curFx][3][cntFx], dimVal)
    writeD(lamp4[curFx][0][cntFx], lamp4[curFx][1][cntFx], lamp4[curFx][2][cntFx], lamp4[curFx][3][cntFx], dimVal)
  }

}

void doAuxMode(byte dimVal, int timeVal){
  if(timeVal > musicTreshold){
    musicTreshold = analogRead(A6);
    cntFx ++;
    
    if(cntFx > 7){
      cntFx = 0;
      if(twoTimes){
        curFx = random(0, 20);
        printFx(curFx, lastFx);
        lastFx = curFx;
        twoTimes = false;
      }
      else twoTimes = true;
    }
    writeA(lamp1[curFx][0][cntFx], lamp1[curFx][1][cntFx], lamp1[curFx][2][cntFx], lamp1[curFx][3][cntFx], dimVal)
    writeB(lamp2[curFx][0][cntFx], lamp2[curFx][1][cntFx], lamp2[curFx][2][cntFx], lamp2[curFx][3][cntFx], dimVal)
    writeC(lamp3[curFx][0][cntFx], lamp3[curFx][1][cntFx], lamp3[curFx][2][cntFx], lamp3[curFx][3][cntFx], dimVal)
    writeD(lamp4[curFx][0][cntFx], lamp4[curFx][1][cntFx], lamp4[curFx][2][cntFx], lamp4[curFx][3][cntFx], dimVal)

    prevMillis = millis();
  }
}

void doStaticMode(byte dimVal){
  color = analogRead(A7);

  if(color < 15) color = 0;
        
  if(filter(color, lastColor)){ 
    if(color < 15){R = 0; G = 0; B = 0;}
    else if(color < 30){R = 255; G = 0; B = 0;}
    else if(color < 300){R = (255 - map(color, 30, 299, 0, 255)); B = (255 - R); G = 0;}
    else if(color < 360){R = 0; G = 0; B = 255;}
    else if(color < 700){B = (255 - map(color, 360, 699, 0, 255)); G = (255 - B); R = 0;}
    else if(color < 760){R = 0; G = 255; B = 0;}
    else if(color < 1001){G = (255 - map(color, 690, 1000, 0, 255)); R = (255 - G); B = 0;}
    else{ R = 255; G = 0; B = 0;}
    W = 0;
      
    writeA(R, G, B, W, dimVal)
    writeB(R, G, B, W, dimVal)
    writeC(R, G, B, W, dimVal)
    writeD(R, G, B, W, dimVal)
                
  } //filter all input
  lastColor = color;
}

void writeMainOutput(byte dimVal){
  
  rOut[0] = check(R1.update(), rLast[0]);        
  if(rOut[0] > 0) rLast[0] = rOut[0] -1;
  gOut[0] = check(G1.update(), gLast[0]);        
  if(gOut[0] > 0) gLast[0] = gOut[0] -1;
  bOut[0] = check(BB1.update(), bLast[0]);        
  if(bOut[0] > 0) bLast[0] = bOut[0] -1;
  wOut[0] = check(W1.update(), wLast[0]);
  if(wOut[0] > 0) wLast[0] = wOut[0] -1;
  writeA(rLast[0], gLast[0], bLast[0], wLast[0], dimVal)

  rOut[1] = check(R2.update(), rLast[1]);        
  if(rOut[1] > 0) rLast[1] = rOut[1] -1;
  gOut[1] = check(G2.update(), gLast[1]);        
  if(gOut[1] > 0) gLast[1] = gOut[1] -1;
  bOut[1] = check(B2.update(), bLast[1]);        
  if(bOut[1] > 0) bLast[1] = bOut[1] -1;
  wOut[1] = check(W2.update(), wLast[1]);
  if(wOut[1] > 0) wLast[1] = wOut[1] -1;
  writeB(rLast[1], gLast[1], bLast[1], wLast[1], dimVal)
  
  rOut[2] = check(R3.update(), rLast[2]);        
  if(rOut[2] > 0) rLast[2] = rOut[2] -1;
  gOut[2] = check(G3.update(), gLast[2]);        
  if(gOut[2] > 0) gLast[2] = gOut[2] -1;
  bOut[2] = check(B3.update(), bLast[2]);        
  if(bOut[2] > 0) bLast[2] = bOut[2] -1;  
  wOut[2] = check(W3.update(), wLast[2]);
  if(wOut[2] > 0) wLast[2] = wOut[2] -1;
  writeC(rLast[2], gLast[2], bLast[2], wLast[2], dimVal)

  rOut[3] = check(R4.update(), rLast[3]);        
  if(rOut[3] > 0) rLast[3] = rOut[3] -1;
  gOut[3] = check(G4.update(), gLast[3]);        
  if(gOut[3] > 0) gLast[3] = gOut[3] -1;
  bOut[3] = check(B4.update(), bLast[3]);        
  if(bOut[3] > 0) bLast[3] = bOut[3] -1;  
  wOut[3] = check(W4.update(), wLast[3]);
  if(wOut[3] > 0) wLast[3] = wOut[3] -1;
  writeD(rLast[3], gLast[3], bLast[3], wLast[3], dimVal)
} //writeOutput

void writeSubOutput(){
  soundSens = map(analogRead(A0), 0, 1023, 210, 255);
  if(soundSens > 250) soundSens = 255;
  if(soundSens < 215) soundSens = 210;
  headDim = map(analogRead(A1), 0, 1023, 0, 255);
  if(headDim > 245) headDim = 255;
  if(headDim < 15) headDim = 0;
  writeHead(soundSens ,headDim);

  temp = map(analogRead(A4), 0, 1023, 0, 255); //no need for adjustment at upper and lower boarder
  fog = map(analogRead(A5), 0, 1023, 0, 255);
  writeFog(temp, fog);
}

bool filter(int current, int last){
  
  if(current >= last){
    if(current - last <= 2) return(false);
    else return(true);  
  }else{
     if(last - current <= 2) return(false);
     else return(true);
  }
} //filter()

void doStrobeMode(){
  notNormalUse = true;
  pauseRamp();  
  dmx.set(1, 255); dmx.set(9, 255); dmx.set(17, 255); dmx.set(25, 255);   //strobe mode
  dmx.set(2, 105); dmx.set(10, 105); dmx.set(18, 105); dmx.set(26, 105);  //white color
  dmx.set(3, strobeSpeed); dmx.set(11, strobeSpeed); dmx.set(19, strobeSpeed); dmx.set(27, strobeSpeed);  //strobe speed
}

void doBlackMode(){
  notNormalUse = true;
  pauseRamp();
  dmx.set(4, 0); dmx.set(12, 0); dmx.set(20, 0); dmx.set(28, 0); dmx.set(67, 0); //brigthness = 0
}

void normalUse(){
  notNormalUse = false;
  resumeRamp();
  dmx.set(1, 0); dmx.set(9, 0); dmx.set(17, 0); dmx.set(25, 0);//strobe off
}

///////////////////////////////// music //////////////////////////////////////////////////////

void receiveStartStop(){
  clkCount = 0;
}

void receiveClock(){
  clkCount ++;
}

///////////////////////////////////// ramp  /////////////////////////////////////////////////

void setRamp(){
    dur = constrain(map(analogRead(A8),0,1023,0,4095), 200, 4000);
    if(speedAdjust) dur = dur * speedAdjustFactor; 

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

    R4.go(lamp4[curFx][0][cntFx], dur);
    G4.go(lamp4[curFx][1][cntFx], dur);
    B4.go(lamp4[curFx][2][cntFx], dur);
    W4.go(lamp4[curFx][3][cntFx], dur); 
} //setRamp()

void pauseRamp(){
if(!BB1.isPaused()){ 
    R1.pause(); R2.pause(); R3.pause(); R4.pause();
    G1.pause(); G2.pause(); G3.pause(); G4.pause();
    BB1.pause(); B2.pause(); B3.pause(); B4.pause();
    W1.pause(); W2.pause(); W3.pause(); W4.pause();
  }
}

void resumeRamp(){
  if(BB1.isPaused()){
    R1.resume(); R2.resume(); R3.resume(); R4.resume();
    G1.resume(); G2.resume(); G3.resume(); G4.resume();
    BB1.resume(); B2.resume(); B3.resume(); B4.resume();
    W1.resume(); W2.resume(); W3.resume(); W4.resume();
  }
}

byte check(byte current, byte last){
  if(current < 5) current = 0;
  if(current > 250) current = 255;
  if(current == last){
    return(0);
  }else{
    return(current +1);  
  }
} //check()

////////////////////////////////////  setup & loop  //////////////////////////////////////////////

void setup() {
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setTextSize(2); 
  
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  attachInterrupt(pinA, doA, CHANGE);
  attachInterrupt(pinB, doB, CHANGE);

  dmx.begin();

  debounceSwitch.attach(Switch, INPUT); //encoder, no input_pullup, there is an exeternal 10k resistor
  debounceBlack.attach(5, INPUT_PULLUP);
  debounceStrobe.attach(6, INPUT_PULLUP);
  debouncePause.attach(7, INPUT_PULLUP);
  debounceSwitch.interval(5);
  debounceStrobe.interval(5);
  debounceBlack.interval(5);
  debouncePause.interval(5);

  usbMIDI.setHandleStart(receiveStartStop);
  usbMIDI.setHandleStop(receiveStartStop);
  usbMIDI.setHandleClock(receiveClock);
  
  assignment();
  printText(currentPage);
  printCursor(0, 0);
}

void loop() {
  debounceSwitch.update();
  debounceStrobe.update();
  debounceBlack.update();
  debouncePause.update();
  buttonChange = debounceSwitch.fell();
  strobeMode = !debounceStrobe.read();
  blackMode = !debounceBlack.read();
  playPause = debouncePause.read();
  
  usbMIDI.read();
  dim = map(analogRead(A9),0,1023,0,255);  
  if(dim > 245) dim = 255;
  if(dim < 10) dim = 0;
  
  if(blackMode){ doBlackMode();
  }else{ if(strobeMode){ doStrobeMode();
    }else{
      if(notNormalUse) normalUse(); 
      if(playPause){
        if(autoMode) doAutoMode(dim);
        else if(musicMode){
          if(MIDIaux){
            Time = millis() - prevMillis;
            doAuxMode(dim, Time);
          }
          else doMidiMode(dim);
        }
        else doStaticMode(dim);
      } 
    }
    writeSubOutput();
  }
  
  inputHandling();
  
  //if(readPot() && potActive) setPot(false);
  
  //dmx.set(82, map(analogRead(A2), 0, 1023, 0, 255));  //not working
  //dmx.set(81, 255); //sound mode led bar
}
