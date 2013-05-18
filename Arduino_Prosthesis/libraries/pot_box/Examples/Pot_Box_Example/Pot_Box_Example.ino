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

void MarkConnectionDirty()
{
  mBoxConnectionDirty = true;
}

void setup()
{
  Serial.begin(9600);
  
  ProsthesisPotBox::Initialize(POT_BOX_INTERRUPT_ID, POT_BOX_CONNECT_PIN, POT_BOX_P_PIN, POT_BOX_I_PIN, POT_BOX_D_PIN, 0, 0, 0);
  ProsthesisPotBox::SetConnectionDirtyCallback(MarkConnectionDirty);
}

void loop()
{  
  delay(100);
  if (mBoxConnectionDirty)
  {
    int conn;
    ProsthesisPotBox::AttemptReconnect(&conn);
  }
  
  int p;
  int i;
  int d;
  
  ProsthesisPotBox::GetPID(&p, &i, &d);
  
  Serial << "P: " << p << " I: " << i << " D: " << d << "\n";

}
