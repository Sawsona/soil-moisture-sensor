#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==============================
// 5.8.1 Libraries and Constants
// ==============================
#define LCD_ADDRESS 0x27
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

const int sensorPin = 35;

// ==============================
// 5.8.2 Calibration Parameters
// ==============================
int dryValue = 4095;   // Sensor reading in air
int wetValue = 1170;   // Sensor reading in water / very wet soil

// ==============================
// 5.8.3 Moisture Classification Thresholds
// ==============================
const int DRY_MAX = 40;     
const int NORMAL_MAX = 70;  

// ==============================
// 5.8.4 Stability and Smoothing Settings
// ==============================
const int CHANGE_TRIGGER = 6;
const int SETTLE_MS = 800;
const int SAMPLES_STABLE = 15;
const int SAMPLES_QUICK  = 6;

int lastShown = -1;

// ==============================
// 5.8.5 Helper Functions
// ==============================
int readRawAverage(int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(sensorPin);
    delay(5);
  }
  return (int)(sum / samples);
}

int rawToPercent(int raw) {
  long mapped = map(raw, dryValue, wetValue, 0, 100);
  int m = (int)mapped;
  if (m < 0) m = 0;
  if (m > 100) m = 100;
  return m;
}

const char* classify(int p) {
  if (p <= DRY_MAX) return "DRY";
  if (p <= NORMAL_MAX) return "NORMAL";
  return "WET";
}

void showResult(int percent) {
  const char* label = classify(percent);

  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture   ");

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);

  lcd.print(label);
  lcd.print(" ");

  if (label[0] == 'D') lcd.print("   ");
  else if (label[0] == 'W') lcd.print("   ");
  else lcd.print(" ");

  if (percent < 10) lcd.print(" ");
  if (percent < 100) lcd.print(" ");

  lcd.print(percent);
  lcd.print("%");
}

// ==============================
// 5.8.6 setup()
// System Initialization
// ==============================
void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetPinAttenuation(sensorPin, ADC_11db);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(500);

  int raw = readRawAverage(SAMPLES_STABLE);
  lastShown = rawToPercent(raw);
  showResult(lastShown);
}

// ==============================
// 5.8.7 loop()
// Main Processing Logic
// ==============================
void loop() {
  int rawNow = readRawAverage(SAMPLES_QUICK);
  int pctNow = rawToPercent(rawNow);

  if (abs(pctNow - lastShown) >= CHANGE_TRIGGER) {
    delay(SETTLE_MS);

    int rawStable = readRawAverage(SAMPLES_STABLE);
    int pctStable = rawToPercent(rawStable);

    if (pctStable != lastShown) {
      lastShown = pctStable;
      showResult(lastShown);
    }

    Serial.print("Stable Raw: ");
    Serial.print(rawStable);
    Serial.print(" | %: ");
    Serial.println(pctStable);
  }

  delay(250);
}