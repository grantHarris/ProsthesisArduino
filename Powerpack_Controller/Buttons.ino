int PlusPin = 1;
int MinusPin = 2;
int Value = 5;
double StartTime;
bool FastMode = FALSE;

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
    while( DigitalRead(PlusPin) == HIGH ) {
      if( !FastMode ) {
        if( StartTime - millis() > 500 ) {
          Value++;
          StartTime = millis();
          FastMode = TRUE;
        }
      }
      else {
        if( StartTime - millis() > 100 ) {
        Value++;
        StartTime = millis();
        }
      }
    }
    FastMode = FALSE;
  }

  if( DigitalRead(MinusPin) == HIGH ) {
    Value++;
    StartTime = millis();
    while( DigitalRead(MinusPin) == HIGH ) {
      if( !FastMode ) {
        if( StartTime - millis() > 500 ) {
          Value--;
          StartTime = millis();
          FastMode = TRUE;
        }
      }
      else {
        if( StartTime - millis() > 100 ) {
        Value--;
        StartTime = millis();
        }
      }
    }
    FastMode = FALSE;
  }

