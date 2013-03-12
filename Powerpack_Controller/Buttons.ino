int PlusPin = 1;
int MinusPin = 2;
int Value = 5;
int StartTime;
bool FastMode = FALSE;

const int SingleTime = 500;
const int RapidTime = 100;

void setup() {
  Serial.begin(9600);
}

void loop() {
//emulates delayed hold faster increase
//pressing the button normally will increase value by 1
//holding button for longer than 1/2 second will increase by 1 again for each .1 second it is held (beyond 1/2 second)

  if( DigitalRead(PlusPin) == HIGH ) {
    Value++;
    StartTime = millis();
    Serial.println("Value = %i", Value)
    while( DigitalRead(PlusPin) == HIGH ) {
      if( !FastMode ) {
        if( StartTime - millis() > SingleTime ) {
          Value++;
          StartTime = millis();
          FastMode = TRUE;
          Serial.println("Value = %i", Value)
        }
      }
      else {
        if( StartTime - millis() > RapidTime ) {
        Value++;
        StartTime = millis();
        Serial.println("Value = %i", Value)
        }
      }
    }
    FastMode = FALSE;
  }

  if( DigitalRead(MinusPin) == HIGH ) {
    Value++;
    StartTime = millis();
    Serial.println("Value = %i", Value)

    while( DigitalRead(MinusPin) == HIGH ) {
      if( !FastMode ) {
        if( StartTime - millis() > SingleTime ) {
          Value--;
          StartTime = millis();
          FastMode = TRUE;
          Serial.println("Value = %i", Value)
        }
      }
      else {
        if( StartTime - millis() > RapidTime ) {
        Value--;
        StartTime = millis();
        Serial.println("Value = %i", Value)
        }
      }
    }
    FastMode = FALSE;
  }
}
