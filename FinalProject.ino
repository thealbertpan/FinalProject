int leds [4] = {33, 35, 37, 39};

int potPin_speed = A13; //potentiometer pin for speed of lights
int potVal_speed = 0;
int mappedPotVal_speed = 0;
int midiNote[4] = {2793, 3136, 164, 440};

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

boolean channelButtonState[2] = {LOW, LOW};
boolean lastChannelButtonState[2] = {LOW, LOW};

boolean ledState[3][4] = {
  { LOW, HIGH, LOW, HIGH },
  { LOW, HIGH, HIGH, HIGH },
  { LOW, LOW, LOW, HIGH }
};

void setup() {
  for(int ledPin = 33; ledPin < 40; ledPin = ledPin+2)
    pinMode(ledPin, OUTPUT);
  pinMode(nextChannelButtonPin, INPUT);
  pinMode(prevChannelButtonPin, INPUT);
}

void loop() {
  nextChannel();
  prevChannel();
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
    usbMIDI.sendNoteOff(midiNote[channelDisplayed], 0, 1);
    digitalWrite(leds[currentStep], LOW);
      
    currentStep += 1;
    if(currentStep > 3)
      currentStep = 0;
    for(int channelNumber = 0; channelNumber < 3; channelNumber ++)
    {
      if(ledState[channelNumber][currentStep] == HIGH)
        usbMIDI.sendNoteOn(midiNote[channelNumber], 127, 1);
      digitalWrite(leds[currentStep], HIGH);
    }
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
      if(ledState[channelDisplayed][i] == LOW)
        ledState[channelDisplayed][i] = HIGH;
      else if(ledState[channelDisplayed][i] == HIGH)
        ledState[channelDisplayed][i] = LOW;
    }
  }
}

void setLEDStates()
{
  for(int ledNumber = 0; ledNumber < 4; ledNumber = ledNumber + 1)
  {
  if(ledState[channelDisplayed][ledNumber] == HIGH || currentStep == ledNumber)
    digitalWrite(leds[ledNumber], HIGH);
  else if(ledState[channelDisplayed][ledNumber] == LOW)
    digitalWrite(leds[ledNumber], LOW);
  }
}

void nextChannel()
{
  lastChannelButtonState[0] = channelButtonState[0];
  channelButtonState[0] = digitalRead(nextChannelButtonPin);
  if(channelButtonState[0] == HIGH && lastChannelButtonState[0] == LOW)
  {
    channelDisplayed += 1;
    if(channelDisplayed > 2)
      channelDisplayed = 0;
    Serial.println(channelDisplayed);
  }
}
void prevChannel()
{
  lastChannelButtonState[1] = channelButtonState[1];
  channelButtonState[1] = digitalRead(prevChannelButtonPin);
  if(channelButtonState[1] == HIGH && lastChannelButtonState[1] == LOW)
  {
    channelDisplayed -= 1;
    if(channelDisplayed < 0)
      channelDisplayed = 2;
    Serial.println(channelDisplayed);
  }
}
