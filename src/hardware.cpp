#include "hardware.h"
#include "config.h"

bool isOn = false;

static void turnOffAlert();
static void setPins();
static void setAlertVals(int brightnessRed,int brightnessBlue,int toneVal);
static void checkBtn();
static void alertWave();

void initHardware() {
    setPins();
    turnOffAlert();
}

void handleHardware() {
    checkBtn();
    if(isOn) alertWave();
}

void toggleAlert() {
    isOn = !isOn;
    if(!isOn) turnOffAlert();
}

static void alertWave() {
    static unsigned long lastWaveTime = 0;
    static int angle = 0;
    float sinVal;
    int toneVal;
    int brightnessRed;
    int brightnessBlue;

    if(millis() - lastWaveTime > 10) {
        sinVal = sin(angle * (PI / 180.0));
        brightnessBlue = (int) ((sinVal + 1.0) * 127.5);
        brightnessRed = 255 - brightnessBlue;
        toneVal = 2000 + (int) (sinVal * 500);
        setAlertVals(brightnessRed,brightnessBlue,toneVal);

        if(angle > 360) angle = 0; 
        angle += 10;
        lastWaveTime = millis();
    }
}

static void checkBtn() {
    static unsigned long lastDebounceTime = 0;
    static int lastButtonState = HIGH;
    static int lastStableButtonState = HIGH;
    int currentButtonState = digitalRead(BTN_PIN);

    if(currentButtonState != lastButtonState) {
        lastDebounceTime = millis();
        lastButtonState = currentButtonState;
    }

    if(millis() - lastDebounceTime > 50) {
        if(currentButtonState != lastStableButtonState){
            lastStableButtonState = currentButtonState;

            if(lastStableButtonState == LOW) {
                toggleAlert();
            }
        }
    };
}

static void turnOffAlert() {
    setAlertVals(0,0,0);
}

static void setPins() {
    pinMode(BTN_PIN, INPUT_PULLUP);
}

static void setAlertVals(int brightnessRed,int brightnessBlue,int toneVal) {
    analogWrite(RED_LED_PIN, brightnessRed);
    analogWrite(BLUE_LED_PIN,brightnessBlue);
    toneVal == 0 ? noTone(BUZZER_PIN) : tone(BUZZER_PIN, toneVal);
}
