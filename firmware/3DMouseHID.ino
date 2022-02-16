// (c) 2022 Julien BOUCARON under MIT License


// Standard Arduino Lib
#include "Keyboard.h"
#include "Mouse.h"

#include "Encoder.h" // https://github.com/PaulStoffregen

#define SERIALBUFLEN 40

const char *MVERSION = "3DMouseHID V0.0";

const int pinLed = LED_BUILTIN;

// JOYSTICK
const int horzPin = A1;         // Pin Analog output of X
const int vertPin = A2;        // Pin Analog output of Y
const int joyButt = 15;         // Pin Joystick Button

// ENCODER
const int encBut = 5;    //Pin encoder Button
const int encDT = 7;       // Pin  Detect
const int encCLK = 6;      // Pin  Clockwise
Encoder myEnc(encDT, encCLK);
long positionEnc  = -999;



void freecadConfiguration(bool);
void basicMouseConfiguration();


/** Sent message */
struct MouseKeyHID {
  uint8_t keyboardEnabled;
  uint8_t  keyboardCode;
  uint16_t keyboardMod;

  uint8_t mouseEnabled;
  uint8_t mouseButton;
  int8_t xAxis, yAxis, wheel;
 


  void print() {
    Serial.print("<<");
    Serial.print(keyboardEnabled); Serial.print(":");
    Serial.print(keyboardCode); Serial.print(":");
    Serial.print(keyboardMod); Serial.print(":");
    Serial.print(mouseEnabled); Serial.print(":");
    Serial.print(mouseButton); Serial.print(":");
    Serial.print(xAxis); Serial.print(":");
    Serial.print(yAxis); Serial.print(":");
    Serial.print(wheel);  Serial.print(":");
    Serial.println(">>");
  }

  void read() {
    keyboardEnabled = Serial.read();
    keyboardCode = Serial.read();
    uint8_t keyboardMod0 = Serial.read();
    uint8_t keyboardMod1 = Serial.read();
    keyboardMod = keyboardMod1;
    keyboardMod << 8;
    keyboardMod += keyboardMod0;

    mouseEnabled = Serial.read();
    mouseButton = Serial.read();
    xAxis = Serial.read();
    yAxis = Serial.read();
    wheel = Serial.read();

  }

  void writeHID(byte up, int XSens, int YSens) {
    if ( up != 0 ) {
      if ( keyboardEnabled != 0 ) {       
// TODO        
      }
      if ( mouseEnabled != 0 ) {
        Mouse.press(mouseButton);
        Mouse.move(xAxis * XSens, yAxis * YSens, wheel);
      }
    }
  }

  void reset() {
    keyboardEnabled = 0;
    mouseEnabled = 0;
  }



};


struct  MouseState {
  byte UpX, DownX;
  byte UpY, DownY;
  byte UpZ, DownZ;
  byte ButtonZ;
  int RotateZ;


  int YZero, XZero;
  int offsetJoyX, offsetJoyY;
  int XValue, YValue;
  int XSens, YSens;

 

  void reset() {
    UpX = 0 ; DownX = 0;
    UpY = 0 ; DownY = 0;
    UpZ = 0 ; DownZ = 0;
    ButtonZ = 0 ;
    RotateZ = 0;
    XSens = 0; YSens = 0;

   
    offsetJoyX = 5;    // set this value if the joystick moves by itself : 15
    offsetJoyY = 5;    // set this value if the joystick moves by itself : 15

  }

  void setup() {
    reset();
    YZero = 0; XZero = 0;
    YZero = analogRead(vertPin);
    XZero = analogRead(horzPin);
  }


  void print() {
    Serial.println("State:");
    Serial.print("UpX:"); Serial.print(UpX); Serial.println();
    Serial.print("DownX:"); Serial.print(DownX); Serial.println();
    Serial.print("UpY:"); Serial.print(UpY); Serial.println();
    Serial.print("DownY:"); Serial.print(DownY); Serial.println();
    Serial.print("UpZ:"); Serial.print(UpZ); Serial.println();
    Serial.print("DownZ:"); Serial.print(DownZ); Serial.println();
    Serial.print("ButtonZ:"); Serial.print(ButtonZ); Serial.println();
    Serial.print("RotateZ:"); Serial.print(RotateZ); Serial.println();
    Serial.print("XZero:"); Serial.print(XZero); Serial.println();
    Serial.print("YZero:"); Serial.print(YZero); Serial.println();    
    Serial.print("OffsetJoyX"); Serial.print(offsetJoyX); Serial.println();
    Serial.print("OffsetJoyY"); Serial.print(offsetJoyY); Serial.println();
    Serial.print("XValue:"); Serial.print(XValue); Serial.println();
    Serial.print("YValue:"); Serial.print(YValue); Serial.println();
    Serial.print("XSens:"); Serial.print(XSens); Serial.println();
    Serial.print("YSens:"); Serial.print(YSens); Serial.println();
  }



};


struct MouseConf {
  MouseKeyHID UpX, DownX;
  MouseKeyHID UpY, DownY;
  MouseKeyHID UpZ, DownZ;

  MouseKeyHID ButtonZ;
  MouseKeyHID RotateZ;

  uint16_t Delay;

  void reset() {
    UpX.reset(); DownX.reset();
    UpY.reset(); DownY.reset();
    UpZ.reset(); DownZ.reset();

    ButtonZ.reset();
    RotateZ.reset();
    Delay = 0;
  }

  void print() {
    Serial.println("Configuration:");
    Serial.print("UpX:"); UpX.print();  Serial.println();
    Serial.print("DownX:"); DownX.print(); Serial.println();
    Serial.print("UpY:"); UpY.print();  Serial.println();
    Serial.print("DownY:"); DownY.print(); Serial.println();
    Serial.print("UpZ:"); UpZ.print(); Serial.println();
    Serial.print("DownZ:"); DownZ.print(); Serial.println();
    Serial.print("ButtonZ:"); ButtonZ.print(); Serial.println();
    Serial.print("RotateZ:"); RotateZ.print(); Serial.println();
    Serial.print("Delay:"); Serial.print(Delay); Serial.println();
  }

  void read() {
    UpX.read(); DownX.read();
    UpY.read(); DownY.read();
    UpZ.read(); DownZ.read();
    ButtonZ.read(); RotateZ.read();

    uint8_t delay0 = Serial.read();
    uint8_t delay1 = Serial.read();
    Delay =  delay1;
    Delay << 8;
    Delay += delay0;
  }

  bool releasedMouseButton(const MouseKeyHID  &in, byte state, byte mouseButton) {
    if ( in.mouseEnabled && ( (in.mouseButton & mouseButton) == mouseButton ) && state != 0 ) {
      return false;
    }
    return true;
  }

  void checkAndReleaseMouseButton(byte mouseButton, const MouseState &state ) {
    if ( releasedMouseButton(UpX, state.UpX, mouseButton) &&
         releasedMouseButton(DownX, state.DownX, mouseButton) &&
         releasedMouseButton(UpY, state.UpY, mouseButton) &&
         releasedMouseButton(DownY, state.DownY, mouseButton) &&
         releasedMouseButton(UpZ, state.UpZ, mouseButton) &&
         releasedMouseButton(DownZ, state.DownZ, mouseButton) && 
         releasedMouseButton(ButtonZ, state.ButtonZ, mouseButton) &&
         releasedMouseButton(RotateZ, state.RotateZ, mouseButton) 
         ) {
      Mouse.release(mouseButton);
         }
  }

  void writeHID(const MouseState &state) {

    // FreeCAD Rotation Test: Refactoring required to have a succession of configuration
    /* if ( (UpX.mouseEnabled && state.UpX != 0) || 
         (DownX.mouseEnabled && state.DownX != 0)  || 
         (DownY.mouseEnabled && state.DownY != 0)  ||
         (UpY.mouseEnabled && state.UpY != 0) 
          ) {
      Mouse.press(MOUSE_MIDDLE);
      // No delay required, but sensitive
     // delay(5);
      Mouse.press(MOUSE_LEFT);
     // delay(5);
    }   */
    
    UpX.writeHID(state.UpX, state.XSens, state.YSens); 
    DownX.writeHID(state.DownX, state.XSens, state.YSens);
    UpY.writeHID(state.UpY, state.XSens, state.YSens);
    DownY.writeHID(state.DownY, state.XSens, state.YSens);
    UpZ.writeHID(state.UpZ, state.XSens, state.YSens); 
    DownZ.writeHID(state.DownZ, state.XSens, state.YSens);

   
    checkAndReleaseMouseButton(MOUSE_LEFT, state);
    checkAndReleaseMouseButton(MOUSE_RIGHT, state);
    checkAndReleaseMouseButton(MOUSE_MIDDLE, state);
    
    ButtonZ.writeHID(state.ButtonZ, state.XSens, state.YSens);
    
    if (  ButtonZ.mouseEnabled &&  ButtonZ.mouseButton != 0 ) {
      // Debounce
      if ( state.ButtonZ != 0 ) {
        delay(150);
   /*   } else {
         Mouse.release(ButtonZ.mouseButton);
      } */
    } 
    RotateZ.writeHID(state.RotateZ, state.XSens, state.YSens);


    // Small delay for lower speed
    if ( (state.XSens >= 1 && state.XSens <= 3) || (state.YSens <= 3 && state.YSens >= 1)  ) {
      if ( state.UpX != 0 || state.DownX != 0 || state.UpY != 0 || state.DownY != 0 ) {
        delay(10); // Basic Mouse        
      }
    }         
    
    }
  }
};

MouseState mouseState;
MouseConf mouseConf;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(joyButt, INPUT_PULLUP);
  pinMode(horzPin, INPUT_PULLUP);
  pinMode(vertPin, INPUT_PULLUP);
  pinMode(encBut, INPUT_PULLUP);
  pinMode(encDT, INPUT);
  pinMode(encCLK, INPUT);

  mouseState.setup();
  mouseConf.reset();
  
  Serial.begin(9600);
  Serial.println(MVERSION);
  Mouse.begin();  
  Keyboard.begin();
 
}

void helpCommand() {
  Serial.println("Commands:");
  Serial.println("V: prints firmware version");
  Serial.println("H: shows this help");
  Serial.println("S: shows current mouse state");
  Serial.println("C: shows current mouse configuration");
  Serial.println("R: read mouse configuration");
  Serial.println("T: dummy test mouse move");
  Serial.println("t: dummy test keyboard type");
  Serial.println("f: FreeCAD Translate Mouse Configuration");
  Serial.println("F: FreeCAD Rotate Mouse Configuration");
  Serial.println("m: Basic Mouse Configuration");
}

void readSerialPort() {
  static char message[SERIALBUFLEN];
  static unsigned int message_pos = 0;

  // Check if any configuration change
  while (Serial.available() > 0) {
    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();
    switch (inByte) {
      case 'V':
        Serial.println(MVERSION);
        break;
      case 'H':
        helpCommand();
        break;
      case 'S':
        mouseState.print();
        break;
      case 'C':
        mouseConf.print();
        break;
      case 'R':
        mouseConf.read();
        break;
      case 'T':
        mouseTest();
        break;
      case 't':
        keyboardTest();
        break;
      case 'f': // Translate
        Serial.println("FreeCAD Translate Mode");
        freecadConfiguration(false);
        break;
      case 'F': // Rotate
        Serial.println("FreeCAD Rotate Mode");
        freecadConfiguration(true);
        break;
      case 'm':
        Serial.println("Basic Mouse Mode");
        basicMouseConfiguration();
        break;          
      default:
        Serial.print("Unknown command:");
        Serial.print(inByte);
        helpCommand();
    }
  }
  // Update configuration
}




void readMouse() {
  // Check if any change
  mouseState.reset();

  // Update for writeKeyboard
  mouseState.YValue = analogRead(vertPin);
  mouseState.XValue = analogRead(horzPin);

  const int myDiv = 100;

  if ( mouseState.YValue -  mouseState.YZero < -mouseState.offsetJoyY ) {
    mouseState.UpY = 1; mouseState.DownY = 0;    
    mouseState.YSens = (mouseState.YZero - mouseState.YValue) / myDiv;
  }
  if (mouseState.YValue - mouseState.YZero > mouseState.offsetJoyY ) {
    mouseState.UpY = 0; mouseState.DownY = 1;
    mouseState.YSens = (mouseState.YValue -  mouseState.YZero) / myDiv;
  }
  if ( mouseState.XValue -  mouseState.XZero < -mouseState.offsetJoyX ) {
    mouseState.UpX = 1; mouseState.DownX = 0;    
    mouseState.XSens = (mouseState.XZero - mouseState.XValue) / myDiv;
  }
  if ( mouseState.XValue -  mouseState.XZero > mouseState.offsetJoyX ) {
    mouseState.UpX = 0; mouseState.DownX = 1;
    mouseState.XSens = (mouseState.XValue -  mouseState.XZero) / myDiv;
  }
  if (digitalRead(joyButt) == HIGH) {
    mouseState.ButtonZ = 0;
  } else {
    mouseState.ButtonZ = 1;
  }

  //Encoder
  long newEPosition = myEnc.read();
  if (newEPosition != positionEnc) {
    bool encoderLeft = false;
    if ( positionEnc - newEPosition > 0 ) {
      encoderLeft = true;
      mouseState.UpZ = 1;
      mouseState.DownZ = 0;
    } else {
      mouseState.UpZ = 0;
      mouseState.DownZ = 1;
    }
    positionEnc = newEPosition;

  }
  if (digitalRead(encBut) == LOW) {
    // mouseState.ButtonZ = 0;
  } else {
    // mouseState.ButtonZ = 1;
  }



}

void writeHID() {
  mouseConf.writeHID(mouseState);
 
/*
  Keyboard.releaseAll();
  Mouse.release(MOUSE_LEFT);
  Mouse.release(MOUSE_RIGHT);
  Mouse.release(MOUSE_MIDDLE); */
  
}

// Simple mouse test
void mouseTest() {
  mouseState.reset();
  mouseState.UpX = 1;
  long a = mouseConf.UpX.mouseEnabled;
  long b = mouseConf.UpX.xAxis;
  mouseConf.UpX.mouseEnabled = 1;
  mouseConf.UpX.xAxis = 100;
  writeHID();
  mouseConf.UpX.mouseEnabled = a;
  mouseConf.UpX.xAxis = b;

  Serial.println("mouseTest()");
}


// Simple keyboard test
void keyboardTest() {

 Keyboard.write('a');
 Keyboard.write('b');
 Keyboard.write('c');
 Keyboard.write('V');
 Keyboard.write('V');
 Serial.println("keyboardTest()");
  
}

// FreeCAD mouse test : CAD Mode
// Translate: Hold Middle Button, Move, Then Release when back on center
// Rotate: Hold Middle then Left Button (not both together), Move, Then Release when back on center
void freecadConfiguration(bool rotate) {

 mouseState.reset();
 byte rotateB = 0;
 if ( rotate != false )  {
    rotateB = MOUSE_LEFT;   
 } 
   
  mouseConf.UpX.mouseEnabled = 1;
  mouseConf.UpX.xAxis = 1;
  mouseConf.UpX.yAxis = 0;
  mouseConf.UpX.wheel = 0;
  mouseConf.UpX.keyboardEnabled = 0;  
  mouseConf.UpX.mouseButton =  MOUSE_MIDDLE | rotateB;

  mouseConf.DownX.mouseEnabled = 1;
  mouseConf.DownX.xAxis = -1;
  mouseConf.DownX.yAxis = 0;
  mouseConf.DownX.wheel = 0;
  mouseConf.DownX.keyboardEnabled = 0;  
  mouseConf.DownX.mouseButton =  MOUSE_MIDDLE | rotateB;


  mouseConf.UpY.mouseEnabled = 1;
  mouseConf.UpY.xAxis = 0;
  mouseConf.UpY.yAxis = -1;
  mouseConf.UpY.wheel = 0;
  mouseConf.UpY.keyboardEnabled = 0;  
  mouseConf.UpY.mouseButton =  MOUSE_MIDDLE | rotateB;

  mouseConf.DownY.mouseEnabled = 1;
  mouseConf.DownY.xAxis = 0;
  mouseConf.DownY.yAxis = 1;
  mouseConf.DownY.wheel = 0;
  mouseConf.DownY.keyboardEnabled = 0;  
  mouseConf.DownY.mouseButton =   MOUSE_MIDDLE | rotateB;

  mouseConf.ButtonZ.mouseEnabled = 1;
  mouseConf.ButtonZ.xAxis = 0;
  mouseConf.ButtonZ.yAxis = 0;
  mouseConf.ButtonZ.wheel = 0;
  mouseConf.ButtonZ.keyboardEnabled = 0;
  mouseConf.ButtonZ.mouseButton = MOUSE_LEFT;

}


// Basic Mouse Configuration
void basicMouseConfiguration() { 
  mouseState.reset();
  
  
  mouseConf.UpX.mouseEnabled = 1;
  mouseConf.UpX.xAxis = 1;
  mouseConf.UpX.yAxis = 0;
  mouseConf.UpX.wheel = 0;
  mouseConf.UpX.keyboardEnabled = 0;  
  mouseConf.UpX.mouseButton = 0;

  mouseConf.DownX.mouseEnabled = 1;
  mouseConf.DownX.xAxis = -1;
  mouseConf.DownX.yAxis = 0;
  mouseConf.DownX.wheel = 0;
  mouseConf.DownX.keyboardEnabled = 0;  
  mouseConf.DownX.mouseButton = 0;


  mouseConf.UpY.mouseEnabled = 1;
  mouseConf.UpY.xAxis = 0;
  mouseConf.UpY.yAxis = -1;
  mouseConf.UpY.wheel = 0;
  mouseConf.UpY.keyboardEnabled = 0;  
  mouseConf.UpY.mouseButton = 0;

  mouseConf.DownY.mouseEnabled = 1;
  mouseConf.DownY.xAxis = 0;
  mouseConf.DownY.yAxis = 1;
  mouseConf.DownY.wheel = 0;
  mouseConf.DownY.keyboardEnabled = 0;  
  mouseConf.DownY.mouseButton = 0;

  mouseConf.ButtonZ.mouseEnabled = 1;
  mouseConf.ButtonZ.xAxis = 0;
  mouseConf.ButtonZ.yAxis = 0;
  mouseConf.ButtonZ.wheel = 0;
  mouseConf.ButtonZ.keyboardEnabled = 0;
  mouseConf.ButtonZ.mouseButton = MOUSE_LEFT;

}



void loop() {
  readSerialPort();
  readMouse();
  writeHID();
}
