// (c) 2022 Julien BOUCARON under MIT License


// Standard Arduino Lib
#include "Keyboard.h"
#include "Mouse.h"

#include "Encoder.h" // https://github.com/PaulStoffregen

#define SERIALBUFLEN 40
#define MOUSEBUFLEN 40

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
void blenderConfiguration(bool);



enum MouseKeyHIDMode {
  NOTSET,
  KEYBOARD_PRESS,
  KEYBOARD_RELEASE,
  MOUSE_PRESS,
  MOUSE_RELEASE
};

struct MouseHIDData {
  uint8_t mouseButton;
  int8_t xAxis, yAxis, wheel;
};

struct KeyHIDData {
  uint8_t keyboardCode;
};

union HIDData {
  MouseHIDData mouse;
  KeyHIDData keyboard;
};


/** Sent message */
struct MouseKeyHID {
  MouseKeyHIDMode mode;
  HIDData data;
  


  void print() {
    Serial.print("<<");
    switch (mode) {
       case NOTSET: Serial.print("NOTSET"); break;
       case KEYBOARD_PRESS: Serial.print("KEYBOARD_PRESS"); break;
       case KEYBOARD_RELEASE: Serial.print("KEYBOARD_RELEASE"); break;
       case MOUSE_PRESS: Serial.print("MOUSE_PRESS"); break;
       case MOUSE_RELEASE: Serial.print("MOUSE_RELEASE"); break;
       default: Serial.print("Unknown mode!"); break;
    }
    Serial.print(":");
    if ( mode == KEYBOARD_PRESS || mode == KEYBOARD_RELEASE ) {
      Serial.print(data.keyboard.keyboardCode); Serial.print(":");     
    }
    else if ( mode == MOUSE_PRESS || mode == MOUSE_RELEASE ) {
     Serial.print(data.mouse.mouseButton); Serial.print(":");
     Serial.print(data.mouse.xAxis); Serial.print(":");
     Serial.print(data.mouse.yAxis); Serial.print(":");
     Serial.print(data.mouse.wheel);  Serial.print(":");
    }
    Serial.println(">>");
  }

  void read() {
    int mmode = Serial.parseInt();
    switch ( mmode ) {
      case 0: mode = NOTSET; break;
      case 1: mode = KEYBOARD_PRESS; break;
      case 2: mode = KEYBOARD_RELEASE; break;
      case 3: mode = MOUSE_PRESS; break;
      case 4: mode = MOUSE_RELEASE; break;
      default: Serial.print("Unknown mode!"); break;
  }
    if ( mode == KEYBOARD_PRESS || mode == KEYBOARD_RELEASE ) {
      data.keyboard.keyboardCode = Serial.parseInt();
    } else if ( mode == MOUSE_PRESS || mode == MOUSE_RELEASE ) {
      data.mouse.mouseButton = Serial.parseInt();
      data.mouse.xAxis = Serial.parseInt();
      data.mouse.yAxis = Serial.parseInt();
      data.mouse.wheel = Serial.parseInt();
    }    
  }

  void writeHID(byte up, int XSens, int YSens) {
    if ( up != 0 ) {
      switch ( mode ) {       
        case KEYBOARD_PRESS:
          Keyboard.press(data.keyboard.keyboardCode); 
              // DEBUG
             // Serial.println("Keyboard press");   
          break;
        case KEYBOARD_RELEASE:
          Keyboard.release(data.keyboard.keyboardCode); 
          // DEBUG
          // Serial.println("Keyboard release");   
          break;
        case MOUSE_PRESS: 
          if ( data.mouse.mouseButton != 0 ) {
              Mouse.press(data.mouse.mouseButton);  
              // DEBUG
              // Serial.println("Mouse press");            
          }
          Mouse.move(data.mouse.xAxis * XSens, data.mouse.yAxis * YSens, data.mouse.wheel);
         //  Serial.println("Mouse move = "); Serial.print(data.mouse.xAxis * XSens);
          break;
        case MOUSE_RELEASE:
          if ( data.mouse.mouseButton != 0 ) {
            Mouse.release(data.mouse.mouseButton);
          }
          Mouse.move(data.mouse.xAxis * XSens, data.mouse.yAxis * YSens, data.mouse.wheel);
          break;
      }
     
    }
  }

  void reset() {
    mode = NOTSET;
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

  // If already released, prevent a loop annoying for debug
  int mouseReleased;

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
    mouseReleased = 0;
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


// STATIC BUFFER CONTAININGALL THE CONF
MouseKeyHID mouseBuf[MOUSEBUFLEN];

/**
 * List of moves to be performed on each "key" in sequence
 */
struct MouseKeyConf { 
  int from; // Index in mouseBuf for the move
  int to; // Index in mouseBuf for the move
  
  void reset() { 
    from = -1;
    to  = -1;
  }

  void print() {
    Serial.println(to-from); Serial.println(" moves");
    for(int i = from ; i < to ; i++ ) {      
      mouseBuf[i].print();
    }
  }

  void read() {
    from = Serial.parseInt();
    to = Serial.parseInt();
    for(int i = from ; i < to ; i++ ) {
      mouseBuf[i].read();
    }
  }

  void writeHID(byte up, int XSens, int YSens) {
    if ( from == -1 || to == -1 ) {
      return;
    } else {
      for(int i = from ; i < to ; i++ ) {
        mouseBuf[i].writeHID(up, XSens, YSens);
      }
    }
  }
};


struct MouseConf {
  MouseKeyConf UpX, DownX;
  MouseKeyConf UpY, DownY;
  MouseKeyConf UpZ, DownZ;

  MouseKeyConf ButtonZ;
  MouseKeyConf RotateZ;

// Run before and after performing any "moves"
  MouseKeyConf before, after;

  void reset() {
    UpX.reset(); DownX.reset();
    UpY.reset(); DownY.reset();
    UpZ.reset(); DownZ.reset();

    ButtonZ.reset();
    RotateZ.reset();

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
    Serial.print("before:"); before.print(); Serial.println();
    Serial.print("after:"); after.print(); Serial.println();
  }

  void read() {
    UpX.read(); DownX.read();
    UpY.read(); DownY.read();
    UpZ.read(); DownZ.read();
    ButtonZ.read(); RotateZ.read();    
    before.read(); after.read();
  }


  

  void checkAndReleaseMouseButton(byte mouseButton, const MouseState &state, const MouseState &previousState) {
    if ( state.UpX == 0 && state.UpY == 0 && state.UpZ == 0 && 
         state.DownX == 0 && state.DownY == 0 && state.DownZ == 0 && 
         state.ButtonZ == 0 && state.RotateZ == 0 ) { 
          if ( previousState.UpX != 0 || previousState.UpY != 0 || previousState.UpZ != 0 || 
                previousState.DownX != 0 || previousState.DownY != 0 || previousState.DownZ != 0 || 
                previousState.ButtonZ != 0 || previousState.RotateZ !=  0 ) {         
            Mouse.release(mouseButton);
            // DEBUG
            // Serial.println("Mouse release button = ");
            // Serial.println(mouseButton);
          }      
         }
  }
  
  void checkAndReleaseKeyboard(const MouseState &state, const MouseState &previousState) {
    if ( state.UpX == 0 && state.UpY == 0 && state.UpZ == 0 && 
         state.DownX == 0 && state.DownY == 0 && state.DownZ == 0 && 
         state.ButtonZ == 0 && state.RotateZ == 0 ) { 
          if ( previousState.UpX != 0 || previousState.UpY != 0 || previousState.UpZ != 0 || 
                previousState.DownX != 0 || previousState.DownY != 0 || previousState.DownZ != 0 || 
                previousState.ButtonZ != 0 || previousState.RotateZ !=  0 ) {                                  
            Keyboard.releaseAll();    
            // DEBUG
            // Serial.println("Keyboard.releaseAll()");
            }      
         }
  } 

  bool atLeastAMove(const MouseState &state ) {
    if ( state.UpX != 0 || state.UpY != 0 || state.UpZ != 0 ||
        state.DownX != 0 || state.DownZ != 0 || state.DownY != 0 ||
        state.ButtonZ != 0 || state.RotateZ != 0 ) {
      return true;
    }
    return false;
  }

  bool atLeastAButtonPressed(const MouseState &state) {
    if ( state.ButtonZ != 0 ) {
       return true;
    }
    return false;
  }

  void writeHID(const MouseState &state, MouseState &previousState) {    

    if ( atLeastAMove(state) ) {
      before.writeHID(1, state.XSens, state.YSens);
    }
    
    UpX.writeHID(state.UpX, state.XSens, state.YSens); 
    DownX.writeHID(state.DownX, state.XSens, state.YSens);
    UpY.writeHID(state.UpY, state.XSens, state.YSens);
    DownY.writeHID(state.DownY, state.XSens, state.YSens);
    UpZ.writeHID(state.UpZ, state.XSens, state.YSens); 
    DownZ.writeHID(state.DownZ, state.XSens, state.YSens);

      
    ButtonZ.writeHID(state.ButtonZ, state.XSens, state.YSens);
    RotateZ.writeHID(state.RotateZ, state.XSens, state.YSens);

    if ( atLeastAMove(state) ) {
      after.writeHID(1, state.XSens, state.YSens);
    }


    checkAndReleaseMouseButton(MOUSE_LEFT, state, previousState);
    checkAndReleaseMouseButton(MOUSE_RIGHT, state, previousState);
    checkAndReleaseMouseButton(MOUSE_MIDDLE, state, previousState);
    checkAndReleaseKeyboard(state, previousState);
    

     // Debounce
    if ( atLeastAButtonPressed(state) ) {
       delay(100);
    }

    // Small delay for lower speed
    if ( (state.XSens >= 1 && state.XSens <= 3) || (state.YSens <= 3 && state.YSens >= 1)  ) {
      if ( state.UpX != 0 || state.DownX != 0 || state.UpY != 0 || state.DownY != 0 ) {
        delay(10); // Basic Mouse        
      }
    } 

    previousState = state;
    
    
  }
};

MouseState mouseState, previousMouseState;
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
  previousMouseState.setup();
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
  Serial.println("b: Blender Translate Mouse Configuration");
  Serial.println("B: Blender Rotate Mouse Configuration");
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
      case 'b':
        Serial.println("Blender Translate Mode");
        blenderConfiguration(false);
        break;
      case 'B':
        Serial.println("Blender Rotate Mode");
        blenderConfiguration(true);
        break; 
      case 'c':
        mouseKeyboardTest();
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
  mouseConf.writeHID(mouseState, previousMouseState);
}

// Simple mouse test
void mouseTest() {
  mouseState.reset();
  mouseState.UpX = 1;
  mouseState.XSens = 1;
  mouseState.YSens = 1;
  mouseConf.UpX.from = 0;
  mouseConf.UpX.to = 1;
  mouseBuf[0].mode = MOUSE_PRESS;  
  mouseBuf[0].data.mouse.mouseButton = 0;
  mouseBuf[0].data.mouse.xAxis = 100;
  mouseBuf[0].data.mouse.yAxis = 0;
  mouseBuf[0].data.mouse.wheel = 0;
  writeHID();
  
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


void mouseKeyboardTest() {
  mouseState.reset();

 mouseConf.before.from = -1;
 mouseConf.before.to = -1;
 mouseConf.after.from = -1;
 mouseConf.after.to = -1;
  
  mouseState.UpX = 1;
  mouseState.XSens = 1;
  mouseState.YSens = 1;
  mouseConf.UpX.from = 0;
  mouseConf.UpX.to = 3;
  mouseBuf[0].mode =  KEYBOARD_PRESS; 
  mouseBuf[0].data.keyboard.keyboardCode = KEY_LEFT_SHIFT;  
  mouseBuf[1].mode = MOUSE_PRESS;  
  mouseBuf[1].data.mouse.mouseButton = 0;
  mouseBuf[1].data.mouse.xAxis = 10;
  mouseBuf[1].data.mouse.yAxis = 0;
  mouseBuf[1].data.mouse.wheel = 0;
  mouseBuf[2].mode =  KEYBOARD_PRESS; 
  mouseBuf[2].data.keyboard.keyboardCode = 'a';  
    
  writeHID();
  Keyboard.releaseAll();
  Serial.println("mouseKeyboardTest()");
  
  
}

// FreeCAD mouse test : CAD Mode
// Translate: Hold Middle Button, Move, Then Release when back on center
// Rotate: Hold Middle then Left Button (not both together), Move, Then Release when back on center
void freecadConfiguration(bool rotate) {

 mouseState.reset();


 mouseConf.before.from = -1;
 mouseConf.before.to = -1;
 mouseConf.after.from = -1;
 mouseConf.after.to = -1;
 
 byte rotateB = 0;
 if ( rotate != false )  {
    rotateB = MOUSE_LEFT;   
 } 
   
  mouseConf.UpX.from = 0; 
  mouseConf.UpX.to = 1;
  mouseBuf[0].mode =  MOUSE_PRESS;  
  mouseBuf[0].data.mouse.xAxis = 1;
  mouseBuf[0].data.mouse.yAxis = 0;
  mouseBuf[0].data.mouse.wheel = 0;
  mouseBuf[0].data.mouse.mouseButton =  MOUSE_MIDDLE | rotateB;

  mouseConf.DownX.from = 1;
  mouseConf.DownX.to = 2;
  mouseBuf[1].mode =  MOUSE_PRESS;  
  mouseBuf[1].data.mouse.xAxis = -1;
  mouseBuf[1].data.mouse.yAxis = 0;
  mouseBuf[1].data.mouse.wheel = 0; 
  mouseBuf[1].data.mouse.mouseButton =  MOUSE_MIDDLE | rotateB;


  mouseConf.UpY.from = 2;
  mouseConf.UpY.to = 3;
  mouseBuf[2].mode =  MOUSE_PRESS;  
  mouseBuf[2].data.mouse.xAxis = 0;
  mouseBuf[2].data.mouse.yAxis = -1;
  mouseBuf[2].data.mouse.wheel = 0;
  mouseBuf[2].data.mouse.mouseButton =  MOUSE_MIDDLE | rotateB;

  mouseConf.DownY.from = 3;
  mouseConf.DownY.to = 4;
  mouseBuf[3].mode =  MOUSE_PRESS; 
  mouseBuf[3].data.mouse.xAxis = 0;
  mouseBuf[3].data.mouse.yAxis = 1;
  mouseBuf[3].data.mouse.wheel = 0;
  mouseBuf[3].data.mouse.mouseButton =   MOUSE_MIDDLE | rotateB;

  mouseConf.ButtonZ.from = 4;
  mouseConf.ButtonZ.to = 5;
  mouseBuf[4].mode=  MOUSE_PRESS; 
  mouseBuf[4].data.mouse.xAxis = 0;
  mouseBuf[4].data.mouse.yAxis = 0;
  mouseBuf[4].data.mouse.wheel = 0;  
  mouseBuf[4].data.mouse.mouseButton = MOUSE_LEFT;

  if ( rotate != false )  {
    mouseConf.before.from = 5;
    mouseConf.before.to = 7;
  }
  else {
    mouseConf.before.from = -1;
    mouseConf.before.to = -1;
  }
  mouseBuf[5].mode=  MOUSE_PRESS; 
  mouseBuf[5].data.mouse.xAxis = 0;
  mouseBuf[5].data.mouse.yAxis = 0;
  mouseBuf[5].data.mouse.wheel = 0;  
  mouseBuf[5].data.mouse.mouseButton = MOUSE_MIDDLE;
  mouseBuf[6].mode=  MOUSE_PRESS; 
  mouseBuf[6].data.mouse.xAxis = 0;
  mouseBuf[6].data.mouse.yAxis = 0;
  mouseBuf[6].data.mouse.wheel = 0;  
  mouseBuf[6].data.mouse.mouseButton = MOUSE_LEFT;
}


// Basic Mouse Configuration
void basicMouseConfiguration() { 
  mouseState.reset();

  mouseConf.before.from = -1;
  mouseConf.before.to = -1;
  mouseConf.after.from = -1;
  mouseConf.after.to = -1;
  
  
  mouseConf.UpX.from = 0; 
  mouseConf.UpX.to = 1;
  mouseBuf[0].mode =  MOUSE_PRESS;  
  mouseBuf[0].data.mouse.xAxis = 1;
  mouseBuf[0].data.mouse.yAxis = 0;
  mouseBuf[0].data.mouse.wheel = 0;
  mouseBuf[0].data.mouse.mouseButton =  0;

  mouseConf.DownX.from = 1;
  mouseConf.DownX.to = 2;
  mouseBuf[1].mode =  MOUSE_PRESS;  
  mouseBuf[1].data.mouse.xAxis = -1;
  mouseBuf[1].data.mouse.yAxis = 0;
  mouseBuf[1].data.mouse.wheel = 0; 
  mouseBuf[1].data.mouse.mouseButton =  0;


  mouseConf.UpY.from = 2;
  mouseConf.UpY.to = 3;
  mouseBuf[2].mode =  MOUSE_PRESS;  
  mouseBuf[2].data.mouse.xAxis = 0;
  mouseBuf[2].data.mouse.yAxis = -1;
  mouseBuf[2].data.mouse.wheel = 0;
  mouseBuf[2].data.mouse.mouseButton =  0;

  mouseConf.DownY.from = 3;
  mouseConf.DownY.to = 4;
  mouseBuf[3].mode =  MOUSE_PRESS; 
  mouseBuf[3].data.mouse.xAxis = 0;
  mouseBuf[3].data.mouse.yAxis = 1;
  mouseBuf[3].data.mouse.wheel = 0;
  mouseBuf[3].data.mouse.mouseButton =  0;

  mouseConf.ButtonZ.from = 4;
  mouseConf.ButtonZ.to = 5;
  mouseBuf[4].mode=  MOUSE_PRESS; 
  mouseBuf[4].data.mouse.xAxis = 0;
  mouseBuf[4].data.mouse.yAxis = 0;
  mouseBuf[4].data.mouse.wheel = 0;  
  mouseBuf[4].data.mouse.mouseButton = MOUSE_LEFT;

}


void blenderConfiguration(bool rotate) {
 mouseState.reset();


 mouseConf.before.from = -1;
 mouseConf.before.to = -1;
 mouseConf.after.from = -1;
 mouseConf.after.to = -1;
  
   
  mouseConf.UpX.from = 0; 
  mouseConf.UpX.to = 1;
  mouseBuf[0].mode =  MOUSE_PRESS;  
  mouseBuf[0].data.mouse.xAxis = 1;
  mouseBuf[0].data.mouse.yAxis = 0;
  mouseBuf[0].data.mouse.wheel = 0;
  mouseBuf[0].data.mouse.mouseButton =  MOUSE_MIDDLE;

  mouseConf.DownX.from = 1;
  mouseConf.DownX.to = 2;
  mouseBuf[1].mode =  MOUSE_PRESS;  
  mouseBuf[1].data.mouse.xAxis = -1;
  mouseBuf[1].data.mouse.yAxis = 0;
  mouseBuf[1].data.mouse.wheel = 0; 
  mouseBuf[1].data.mouse.mouseButton =  MOUSE_MIDDLE;


  mouseConf.UpY.from = 2;
  mouseConf.UpY.to = 3;
  mouseBuf[2].mode =  MOUSE_PRESS;  
  mouseBuf[2].data.mouse.xAxis = 0;
  mouseBuf[2].data.mouse.yAxis = -1;
  mouseBuf[2].data.mouse.wheel = 0;
  mouseBuf[2].data.mouse.mouseButton =  MOUSE_MIDDLE;

  mouseConf.DownY.from = 3;
  mouseConf.DownY.to = 4;
  mouseBuf[3].mode =  MOUSE_PRESS; 
  mouseBuf[3].data.mouse.xAxis = 0;
  mouseBuf[3].data.mouse.yAxis = 1;
  mouseBuf[3].data.mouse.wheel = 0;
  mouseBuf[3].data.mouse.mouseButton =   MOUSE_MIDDLE;

  mouseConf.ButtonZ.from = 4;
  mouseConf.ButtonZ.to = 5;
  mouseBuf[4].mode=  MOUSE_PRESS; 
  mouseBuf[4].data.mouse.xAxis = 0;
  mouseBuf[4].data.mouse.yAxis = 0;
  mouseBuf[4].data.mouse.wheel = 0;  
  mouseBuf[4].data.mouse.mouseButton = MOUSE_LEFT;

  if ( rotate == false )  {
    mouseConf.before.from = 5;
    mouseConf.before.to = 7;
    mouseConf.after.from = -1;
    mouseConf.after.to = -1;
  }
  else {
    mouseConf.before.from = -1;
    mouseConf.before.to = -1;
    mouseConf.after.from = -1;
    mouseConf.before.to = -1;
  }
 
  mouseBuf[5].mode =  KEYBOARD_PRESS; 
  mouseBuf[5].data.keyboard.keyboardCode = KEY_LEFT_SHIFT;  
  mouseBuf[6].mode=  MOUSE_PRESS; 
  mouseBuf[6].data.mouse.xAxis = 0;
  mouseBuf[6].data.mouse.yAxis = 0;
  mouseBuf[6].data.mouse.wheel = 0;  
  mouseBuf[6].data.mouse.mouseButton = MOUSE_MIDDLE;
 

}



void loop() {
  readSerialPort();
  readMouse();
  writeHID();
}
