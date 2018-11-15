int leds [4] = {33, 35, 37, 39};
//boolean ledState[4] = {LOW, LOW, LOW, LOW};
int potPin_speed = A13; //potentiometer pin for speed of lights
int potVal_speed = 0;
int mappedPotVal_speed = 0;
int midiNote[4] = {461, 470, 490, 440};

int tempo = 0;
int currentStep = 0;
unsigned long lastStepTime = 0;

int potPin_setNote = A12;
int potPin_setNote_value = 0;
int potPin_setNote_lastvalue = 0;
int potPin_setNote_map = 0;

int nextChannelButtonPin = 30;
int prevChannelButtonPin = 29;
int channelDisplayed = 0;

boolean ledState[3][4] = {
  { LOW, HIGH, LOW, HIGH },
  { LOW, HIGH, HIGH, HIGH },
  { LOW, LOW, LOW, HIGH }
};

void setup() {

  for(int ledPin = 33; ledPin < 40; ledPin = ledPin+2)
    pinMode(ledPin, OUTPUT);
}

void loop() {
  sequence();
  checkPotSet();
  setLEDStates();
}

void sequence()
{
  potVal_speed = analogRead(potPin_speed);
  mappedPotVal_speed = map(potVal_speed, 0, 1023, 0, 1000);
  tempo = mappedPotVal_speed;
  if(millis() > lastStepTime + tempo)
  {
    digitalWrite(leds[currentStep], LOW);
    if(ledState[0][currentStep] == LOW)
      usbMIDI.sendNoteOff(midiNote[currentStep], 0, 1);
    currentStep += 1;
    if(currentStep > 3)
      currentStep = 0;
    digitalWrite(leds[currentStep], HIGH);
    if(ledState[0][currentStep] == HIGH)
      usbMIDI.sendNoteOn(midiNote[currentStep], 127, 1);
    lastStepTime = millis();
  }
}

void checkPotSet()
{
  for(int i = 0; i < 4; i++)
  {
    potPin_setNote_lastvalue = potPin_setNote_value;
    potPin_setNote_value = analogRead(potPin_setNote);
    potPin_setNote_map = map(potPin_setNote_value, 0, 1023, 0, 100);
  
    if(potPin_setNote_value >= 50 && potPin_setNote_lastvalue < 50)
    {
      if(ledState[0][i] == LOW)
        ledState[0][i] = HIGH;
      else if(ledState[0][i] == HIGH)
        ledState[0][i] = LOW;
    }
  }
}

void setLEDStates()
{
  for(int ledNumber = 0; ledNumber < 4; ledNumber = ledNumber + 1)
  {
  if(ledState[0][ledNumber] == HIGH || currentStep == ledNumber)
    digitalWrite(leds[ledNumber], HIGH);
  else if(ledState[0][ledNumber] == LOW)
    digitalWrite(leds[ledNumber], LOW);
  }
}
