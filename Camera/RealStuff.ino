//LED Stuff

//LED Constants
int dataPin = 10; //Pin 14 on the 75HC595
int latchPin = 9; //Pin 12 on the 75HC595
int clockPin = 8; //Pin 11 on the 75HC595
int digs [9] = {0,0,0,0,0,0,0,0,0};
int digits [3] [3] = {
  //x
  {0, 0, 0},
  //y
  {0, 0, 0},
  //z
  {0, 0, 0}
};
boolean numbers [10] [7] = {
  //0
  {LOW, LOW, LOW, LOW, LOW, LOW, HIGH},
  //1
  {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH},
  //2
  {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW},
  //3
  {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW},
  //4
  {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW },
  //5
  {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW},
  //6
  {LOW, HIGH, LOW, LOW, LOW, LOW, LOW},
  //7
  {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH},
  //8
  {LOW, LOW, LOW, LOW, LOW, LOW, LOW},
  //9
  {LOW, LOW, LOW, LOW, HIGH, LOW, LOW}
};
#define shifterNum 4
#define shifterPins shifterNum * 8
boolean registers[shifterPins];

//LED Reset
void clearRegisters() {
  for (int i = shifterPins - 1; i >=  0; i--)
    registers[i] = LOW;
  writeRegisters();
}

//Display Values
void writeRegisters() {
  digitalWrite(latchPin, LOW);
  for (int i = shifterPins - 1; i >=  0; i--) {
    digitalWrite(clockPin, LOW);
    boolean val = registers[i];
    digitalWrite(dataPin, val);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
}

//Individual Pin Setter
void setRegisterPin(int index, int value) {
  registers[index] = value;
}

//Digit Splitter
void digiSplice (int val, int loc) {
  for (int i = 0; i < 3; i ++)
  {
    digits [loc][i] = val;
    if (i == 0)
    {
      digits [loc][i] = val / 100;
      val %= 100;
    }
    else if (i == 1)
    {
      digits [loc][i] = val / 10;
      val %= 10;
    }
    else if (i == 2)
    {
      digits [loc][i] = val;
      val %= 1;
    }
  }
}



//Display Setter
void setVal () {
  for (int dig = 0; dig < 3; dig ++)
  {
    for (int loc = 0; loc < 3; loc++)
    {
      for (int seg = 0; seg < 7; seg++)
        setRegisterPin (seg + loc * 10, numbers [digits[loc][dig]][seg]);
      if (dig == 0)
      {
        setRegisterPin(7 + loc * 10, HIGH);
        setRegisterPin(8 + loc * 10, LOW);
        setRegisterPin(9 + loc * 10, LOW);
      }
      else if (dig == 1)
      {
        setRegisterPin(7 + loc * 10, LOW);
        setRegisterPin(8 + loc * 10, HIGH);
        setRegisterPin(9 + loc * 10, LOW);
      }
      else if (dig == 2)
      {
        setRegisterPin(7 + loc * 10, LOW);
        setRegisterPin(8 + loc * 10, LOW);
        setRegisterPin(9 + loc * 10, HIGH);
      }
      writeRegisters();
    }
  }
}


//Stepper Stuff

//Stepper Constants
#include <Stepper.h>
const int steps = 100;
int inPin0 = A0;
int pPin1 = 12;
int dir = 2;
int hmax = 300;
int vmax = 300;
int htrack = 0;
int vtrack = 0;
Stepper vertigo (steps, 4, 5, 6,7);
Stepper horizon (steps, 0,1,2,3);

//Stepper Reset
void reset() {
  delay (1000);
  dir = 2;
  for (int d = htrack; d > 0; d--)
  {
    horizon.step(-1);
    htrack--;
  }

  for (int c = vtrack; c > 0; c--)
  {
    vertigo.step(-1);
    vtrack--;
  }
  delay (1000);
}

//Light Sensor Stuff

//Light Sensor Constants
#define inPin0 0
int pinRead0;
float pVolt0;

void lightMeUp() {
  pinRead0 = analogRead(inPin0);
  pVolt0 = pinRead0 / 1024.0 * 5.0*510;
  Serial.println(pVolt0);
}

//General Stuff

//Program Setup
void printDigs() {
  for (int loc = 0; loc < 3; loc++)
  {
    for (int dig = 0; dig < 3; dig++)
    {
      if (dig == 0)
        Serial.print(digits[loc][dig]);
    }
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  //LED Setup
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  clearRegisters();
  writeRegisters();

  //Stepper Setup
  pinMode (pPin1, OUTPUT);

  vertigo.setSpeed (150);
  horizon.setSpeed (200);

  //Serial
  Serial.begin(9600);
}

//Main Program
void loop() {
  for (int a = 0; a < vmax - 1; a++)
  {
    for (int b = 0; b < hmax - 1; b++)
    {
      digiSplice(htrack+1, 0);
      lightMeUp();
      digiSplice((int) pVolt0, 2);
      setVal();
      htrack += dir/(abs(dir));
        horizon.step(dir);
    }

    digiSplice(vtrack+1, 1);
    setVal();
    
    vertigo.step(-2);
    vtrack += 1;
    dir *= -1;
  }
  reset();
}

