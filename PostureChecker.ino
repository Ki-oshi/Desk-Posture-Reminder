#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define BUZZER_PIN 6
#define LED_PIN 5
#define BUTTON_PIN 7
#define RESET_BUTTON_PIN 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

float goodPostureDistance = 0;
const int minDistance = 10;
const int maxDistance = 150;
const int readingsToAverage = 20;
const int buzzerOnTime = 1000;
const int buzzerOffTime = 3000;
unsigned long lastBuzzTime = 0;

bool calibrated = false;
bool awaitingCalibration = true;

long smoothedDistance = -1;

void setup()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Posture Reminder");
    delay(2000);
    lcd.clear();
}

long getSmoothedDistance()
{
    long readings[readingsToAverage];
    int valid = 0;

    for (int i = 0; i < readingsToAverage; i++)
    {
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(5);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        long duration = pulseIn(ECHO_PIN, HIGH, 50000);
        long distance = duration * 0.034 / 2;

        if (distance >= minDistance && distance <= maxDistance)
        {
            readings[valid++] = distance;
        }
        delay(10);
    }

    if (valid == 0)
        return -1;

    for (int i = 0; i < valid - 1; i++)
    {
        for (int j = i + 1; j < valid; j++)
        {
            if (readings[i] > readings[j])
            {
                long temp = readings[i];
                readings[i] = readings[j];
                readings[j] = temp;
            }
        }
    }

    if (valid % 2 == 0)
    {
        return (readings[valid / 2 - 1] + readings[valid / 2]) / 2;
    }
    else
    {
        return readings[valid / 2];
    }
}

void calibratePosture()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating");

    long total = 0;
    int count = 0;
    int dots = 0;
    unsigned long start = millis();

    while (millis() - start < 10000)
    {
        long d = getSmoothedDistance();
        if (d != -1)
        {
            total += d;
            count++;
        }

        lcd.setCursor(12, 0);
        for (int i = 0; i < dots; i++)
            lcd.print(".");
        dots = (dots + 1) % 4;
        delay(500);
        lcd.setCursor(12, 0);
        lcd.print("   ");
    }

    if (count > 0)
    {
        goodPostureDistance = total / count;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Calib. Complete!");
        delay(2000);
        lcd.clear();
        calibrated = true;
        awaitingCalibration = false;
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Calib Failed");
        delay(2000);
        lcd.clear();
    }
}

void resetCalibration()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Resetting...");
    Serial.println("Resetting...");

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    calibrated = false;
    awaitingCalibration = true;

    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Posture Reminder");
    delay(2000);
    lcd.clear();
}

void loop()
{
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(BUTTON_PIN);
    bool currentResetButtonState = digitalRead(RESET_BUTTON_PIN);

    if (lastButtonState == HIGH && currentButtonState == LOW && awaitingCalibration)
    {
        calibratePosture();
    }

    if (currentResetButtonState == LOW && calibrated)
    {
        resetCalibration();
    }

    lastButtonState = currentButtonState;

    if (!calibrated)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Posture Reminder");
        delay(200);
        return;
    }

    long distance = getSmoothedDistance();
    if (distance != -1)
    {
        smoothedDistance = distance;
    }

    lcd.clear();

    if (smoothedDistance != -1)
    {
        lcd.setCursor(0, 0);
        lcd.print("Dist: ");
        lcd.print(smoothedDistance);
        lcd.print(" cm");
        //leaning forward                                //leaning backward
        if (smoothedDistance < goodPostureDistance - 5 || smoothedDistance > goodPostureDistance + 100)
        {
            lcd.setCursor(0, 1);
            lcd.print("Bad Posture!");
            digitalWrite(LED_PIN, HIGH);

            if (millis() - lastBuzzTime >= buzzerOffTime)
            {
                digitalWrite(BUZZER_PIN, HIGH);
                lastBuzzTime = millis();
            }

            if (millis() - lastBuzzTime >= buzzerOnTime)
            {
                digitalWrite(BUZZER_PIN, LOW);
            }

            Serial.println("⚠️ BAD POSTURE");
        }

        else if (smoothedDistance >= goodPostureDistance - 5 && smoothedDistance <= goodPostureDistance - 2)
        {
            lcd.setCursor(0, 1);
            lcd.print("Slouching...");
            digitalWrite(LED_PIN, HIGH);

            if (millis() - lastBuzzTime >= buzzerOffTime)
            {
                digitalWrite(BUZZER_PIN, HIGH);
                lastBuzzTime = millis();
            }

            if (millis() - lastBuzzTime >= buzzerOnTime)
            {
                digitalWrite(BUZZER_PIN, LOW);
            }

            Serial.println("⚠️ SLOUCHING");
        }

        else
        {
            lcd.setCursor(0, 1);
            lcd.print("Good posture :)");
            digitalWrite(LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            Serial.println("✅ GOOD POSTURE");
        }
    }

    delay(400);
}
