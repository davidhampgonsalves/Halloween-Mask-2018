#include <Arduino.h>

int EYES_LEDS = 1;
int LUNG_LEDS = 2;

void setup() {
  pinMode(EYES_LEDS, OUTPUT);
  pinMode(LUNG_LEDS, OUTPUT);

  randomSeed(analogRead(0));
}

unsigned long BLINK_INTERVAL = 6000;
unsigned long MULTI_BLINK_INTERVAL = 250;
unsigned long BLINK_DURRATION = 250;
unsigned long lastBlinkChange = 0;
unsigned long nextBlinkInterval = BLINK_INTERVAL;
bool areEyesLit = false;

void performBlink(unsigned long now) {
  if((areEyesLit && now - lastBlinkChange > nextBlinkInterval) ||
      (!areEyesLit && now - lastBlinkChange > BLINK_DURRATION)) {

    // random possibility of multiple blinks
    if(!areEyesLit)
      nextBlinkInterval = random(100) > 75 ? MULTI_BLINK_INTERVAL : BLINK_INTERVAL;

    areEyesLit = !areEyesLit;
    lastBlinkChange = now;
    digitalWrite(EYES_LEDS, areEyesLit ? HIGH : LOW);
  }
}

unsigned long BRIGHTNESS_INTERVAL = 20;
unsigned long lastBreathChange = 0;
int currentBrightness = 0;
bool isFadingIn = true;

void performBreath(unsigned long now) {
  if(now - lastBreathChange < BRIGHTNESS_INTERVAL)
    return;

  if(currentBrightness <= 5)
    isFadingIn = true;
  else if(currentBrightness >= 130)
    isFadingIn = false;

  lastBreathChange = now;
  if(isFadingIn)
    currentBrightness++;
  else
    currentBrightness--;

  int brightness;
  if(currentBrightness > 140)
    brightness = 140;
  else
    brightness = currentBrightness;

  analogWrite(LUNG_LEDS, brightness);
}

void loop() {
  unsigned long now = millis();

  performBlink(now);
  performBreath(now);
}
