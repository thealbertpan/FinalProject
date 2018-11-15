int leds [4] = {33, 35, 37, 39};
boolean ledState[4] = {LOW, LOW, LOW, LOW};
int buttonPins[4] = {13, 14, 15, 16};
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
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

void setup() {

  for(int ledPin = 33; ledPin < 40; ledPin = ledPin+2)
    pinMode(ledPin, OUTPUT);
  for(int buttonPin = 13; buttonPin < 17; buttonPin +=1)
    pinMode(buttonPin, INPUT);
}

void loop() {
  sequence();
  //checkButtons();
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
    if(ledState[currentStep] == LOW)
      usbMIDI.sendNoteOff(midiNote[currentStep], 0, 1);
    currentStep += 1;
    if(currentStep > 3)
      currentStep = 0;
    digitalWrite(leds[currentStep], HIGH);
    if(ledState[currentStep] == HIGH)
      usbMIDI.sendNoteOn(midiNote[currentStep], 127, 1);
    lastStepTime = millis();
  }
}
void checkButtons()
{
  for(int buttonNumber = 0; buttonNumber < 4; buttonNumber = buttonNumber + 1)
  {
  lastButtonState[buttonNumber] = buttonState[buttonNumber];
  buttonState[buttonNumber] = digitalRead(buttonPins[buttonNumber]);

  if(buttonState[buttonNumber] == HIGH && lastButtonState[buttonNumber] == LOW)
  {
    if(ledState[buttonNumber] == LOW)
      ledState[buttonNumber] = HIGH;
    else if(ledState[buttonNumber] == HIGH)
      ledState[buttonNumber] = LOW;
  }
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
      if(ledState[i] == LOW)
        ledState[i] = HIGH;
      else if(ledState[i] == HIGH)
        ledState[i] = LOW;
    }
  }
}

void setLEDStates()
{
  for(int ledNumber = 0; ledNumber < 4; ledNumber = ledNumber + 1)
  {
  if(ledState[ledNumber] == HIGH || currentStep == ledNumber)
    digitalWrite(leds[ledNumber], HIGH);
  else if(ledState[ledNumber] == LOW)
    digitalWrite(leds[ledNumber], LOW);
  }
}
