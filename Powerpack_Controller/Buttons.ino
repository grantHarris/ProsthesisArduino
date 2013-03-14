int PlusPin = 22;
int MinusPin = 23;
int Value = 0;
int StartTime;
bool FastMode = false;

const int SingleTime = 500;
const int RapidTime = 100;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //emulates delayed hold faster increase
  //pressing the button normally will increase value by 1
  //holding button for longer than 1/2 second will increase by 1 again for each .1 second it is held (beyond 1/2 second)

  if( digitalRead(PlusPin) == HIGH ) {
    Value++;
    StartTime = millis();
    Serial.println(Value);
    while( digitalRead(PlusPin) == HIGH ) {
      if( !FastMode ) {
        if( StartTime - millis() > SingleTime ) {
          Value++;
          StartTime = millis();
          FastMode = true;
          Serial.println(Value);
        }
      }
      else {
        if( StartTime - millis() > RapidTime ) {
          Value++;
          StartTime = millis();
          Serial.println(Value);
        }
      }
    }
    FastMode = false;
  }

  if( digitalRead(MinusPin) == HIGH ) {
    Value++;
    StartTime = millis();
    Serial.println(Value);

    while( digitalRead(MinusPin) == HIGH ) {
      if( !FastMode ) {
        if( StartTime - millis() > SingleTime ) {
          Value--;
          StartTime = millis();
          FastMode = true;
          Serial.println(Value);
        }
      }
      else {
        if( StartTime - millis() > RapidTime ) {
          Value--;
          StartTime = millis();
          Serial.println(Value);
        }
      }
    }
    FastMode = false;
  }
}
