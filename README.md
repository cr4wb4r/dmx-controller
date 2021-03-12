# dmx-controller
Self made DMX controller using a Teensy 3.2 and the Arduino IDE.


This is the newest and best version of my self-made DMX controller. As stated, the heart of the project is a Teensy 3.2. 
This little microcontroller is quite fast and very powerful. Combined with better software has this controller version a nice and somewhat professional feel to it.


Functions:
The controller sends DMX data to 4 LED PAR lights, to a fog maschine and to a moving head. The latter two are just improvised for now.
My main focus is on the LED lights, wich have an eight channel data input.
I control following channels with my project:

  CH1 - macro functions: set to 0, for strobing set to 255
  CH2 - color (for macro): set to 105 aka white light for the strobing
  CH3 - strobe speed: set to 220
  CH4 - master dim: controlled with an sliding potentiometer
  CH5 - red: controlled via software
  CH6 - green: controlled via software
  CH7 - blue: controlled via software
  CH8 - white: controlled via software
  
Every light has it's own address, beginning at 1 and stopping at 25 (1-8; 9-16; 17-24; 25-32).


Inputs:
The controller has four sliding potentiometers, three switches, six regular potentiometers and a rotary encoder located all on the top panel.
Starting wiht the simplest part, the switches: one controls the blackout (all lights off), one controls the strobing and the last one pauses the effect.
The effects are chosen with the rotary encoder. Their speed is manipulated with the second slider (from left). The first slider sets the master brightness, the third choses
a color and the fourth slider changes the amount of beats required for the MIDI input. More on the specialized topics is found later in the readme.
For the experimental outputs of the DMX controller are the six potentiometers needed. Two control the moving head, two the fog maschine and two are currently not used.

Another input is the MIDI data over USB. The Teensy luckily supports this kind of communication. I am just using the beat clock part of the midi protocoll in this project.


Outputs:
The main DMX output is through an industry standartized three pin XLR connector. Convertion is done with a MAX485 "serial to RS485" converter module.
Visually the operator can see the used effects and the current selected operation on a SPI color LCD. I choose an easy three row output to be displayed on this screen.
There are just two pages or six menu's to select from (two pages x three rows = six options). The options on the front page are the operating modes and
the options on the second page are settings, each with a submenu page.

The currently selected operating mode is highlighted green.


Usage - automatic mode:
  1. Connect the power and the DMX data cable, set all sliding potentiometers to the top (maximum) and turn on the power switch. Turn off all toggle Switches.
  2. Adjust the preferred brightness and speed with the corresponding sliders (1 and 2).

Usage - music mode:
  1. Connect the power and the DMX data cable, set all sliding potentiometers to the top (maximum) and turn on the power switch. Turn off all toggle Switches.
  2. Select the music mode in page one. Select the preferred music input in page two. MIDI is preselected.
  3. Adjust the preferred brightness with the corresponding slider (1).
  4. In MIDI mode: Adjust the number of beats required for an effect change with the corresponding slider (4).

Usage - static mode:
  1. Connect the power and the DMX data cable, set all sliding potentiometers to the top (maximum) and turn on the power switch. Turn off all toggle Switches.
  2. Select the static mode in page one.
  3. Adjust the preferred brightness and the static color with the corresponding sliders (1 and 3). 
  
Usage - special effects:
  While on: Toggle on of the three switches to activate their effect. A green LED lights up, if the effect is active.
  
  
Effects:
All the effects are stored in the effects.h file. There are 21 effects with eight scenes each. While in automatic or music mode, the controller cycles trough the effects.
Each scene of one effect is "played" in order, before the effect changes.

In automatic mode the effects ramp up and down to smoothly change the given colors. The music mode on the other hand switches abruptly. This is down for a disco like feeling.




Software:
I have used the TeensyDMX, Ramp, SPI, MIDI, Bounce2, Adafruit_GFX and Adafruit_ST7735 librarys. All can be easyly be found in the web or in github.
The menu and submenu handling, as well as all the effects and input methods are developed by me.


Pictures can be found in the files of this project.
  
