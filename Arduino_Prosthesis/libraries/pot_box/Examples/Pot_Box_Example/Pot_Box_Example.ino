#include <Arduino.h>
#include "pot_box.h"
//Handy wrapper for using stream-like serial printing
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#define POT_BOX_INTERRUPT_ID 0
#define POT_BOX_CONNECT_PIN 2

#define POT_BOX_P_PIN 0
#define POT_BOX_I_PIN 1
#define POT_BOX_D_PIN 2

bool mBoxConnectionDirty = true;

ProsthesisPotBox mPotBox;

void MarkConnectionDirty()
{
  mBoxConnectionDirty = true;
}

void setup()
{
  Serial.begin(9600);
  
  mPotBox = ProsthesisPotBox(POT_BOX_INTERRUPT_ID, POT_BOX_CONNECT_PIN, POT_BOX_P_PIN, POT_BOX_I_PIN, POT_BOX_D_PIN, 0, 0, 0);
  mPotBox.SetConnectionDirtyCallback(MarkConnectionDirty);
  
  int isConnected;
  mPotBox.AttemptReconnect(&isConnected);
}

void loop()
{  
  if (mBoxConnectionDirty)
  {
    int conn;
    mPotBox.AttemptReconnect(&conn);
  }
  
  int p;
  int i;
  int d;
  
  mPotBox.GetPID(&p, &i, &d);
  
  Serial << "P: " << p << " I: " << i << " D: " << d << "\n";

}
