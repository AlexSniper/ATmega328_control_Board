#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_MCP23X17.h>

// ---------------- LCD wiring (unchanged) ----------------
#define LCD_RS  8    // PB0
#define LCD_E   13   // PB5
#define LCD_D4  10   // PB2
#define LCD_D5  11   // PB3
#define LCD_D6  7    // PD7
#define LCD_D7  4    // PD4

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// ---------------- MCP23017 ----------------
Adafruit_MCP23X17 mcp;
static constexpr uint8_t MCP_ADDR = 0x20;

// Button on MCP: GPA4 (J9-5) -> GND
static constexpr uint8_t BTN_PIN = 4;   // GPA4

// Motor OUT1 gate is PB1 -> Arduino D9
static constexpr uint8_t MOTOR_GATE_PIN = 9; // PB1 (controls Q4 / J10)

// Debounce (non-blocking)
static constexpr uint32_t DEBOUNCE_MS = 25;

bool motorOn = false;

// For edge detect + debounce
bool lastRaw = HIGH;       // raw button level (HIGH released, LOW pressed)
bool stable = HIGH;        // debounced stable level
uint32_t lastChangeMs = 0; // time raw last changed

void showStatus() {
  lcd.setCursor(0, 0);
  lcd.print("AL_Robotics     ");
  lcd.setCursor(0, 1);
  lcd.print("Motor: ");
  lcd.print(motorOn ? "ON " : "OFF");
  lcd.print("        ");
}

void setup() {
  delay(100);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("AL_Robotics");
  lcd.setCursor(0, 1);
  lcd.print("Init...");

  Wire.begin();

  if (!mcp.begin_I2C(MCP_ADDR)) {
    lcd.clear();
    lcd.print("MCP23017 FAIL");
    lcd.setCursor(0, 1);
    lcd.print("Check I2C");
    while (true) {}
  }

  // Button input + enable MCP pull-up
  mcp.pinMode(BTN_PIN, INPUT);
  mcp.digitalWrite(BTN_PIN, HIGH); // enables pull-up

  // Recommended: GPB0..GPB7 outputs LOW
  for (uint8_t p = 8; p <= 15; p++) {
    mcp.pinMode(p, OUTPUT);
    mcp.digitalWrite(p, LOW);
  }

  // Motor gate
  pinMode(MOTOR_GATE_PIN, OUTPUT);
  digitalWrite(MOTOR_GATE_PIN, LOW);

  // Init debounce state
  stable = mcp.digitalRead(BTN_PIN);
  lastRaw = stable;
  lastChangeMs = millis();

  motorOn = false;
  showStatus();
}

void loop() {
  const uint32_t now = millis();

  // Read raw button
  const bool raw = mcp.digitalRead(BTN_PIN); // HIGH=release, LOW=pressed

  // Track raw changes
  if (raw != lastRaw) {
    lastRaw = raw;
    lastChangeMs = now;
  }

  // If raw has been stable long enough, accept as debounced state
  if ((now - lastChangeMs) >= DEBOUNCE_MS && raw != stable) {
    stable = raw;

    // Trigger on PRESS event (transition to LOW)
    if (stable == LOW) {
      motorOn = !motorOn;
      digitalWrite(MOTOR_GATE_PIN, motorOn ? HIGH : LOW);
      showStatus();
    }
  }
}
