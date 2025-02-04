#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DHT.h"
#define DHTTYPE DHT11  // Changed to DHT11

unsigned long delayTime;
uint8_t DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
float Temp_Fahrenheit;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int m1in1 = 8;
int m1in2 = 9;

int m2in1 = 10;
int m2in2 = 11;

void setup()
{
  pinMode(m1in1, OUTPUT);
  pinMode(m1in2, OUTPUT);

  pinMode(m2in1, OUTPUT);
  pinMode(m2in2, OUTPUT);

  // initialize the LCD
  lcd.begin();

  // Turn on the backlight and print a message.
  lcd.backlight();
  lcd.print("pro title 1");
  lcd.setCursor(0, 1);  // Move cursor to character 2 on line 1
  lcd.print("pro title 2");
  delay(3000);
  Serial.begin(9600);
  pinMode(DHTPin, INPUT);
  dht.begin();
}

void loop()
{
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  Temp_Fahrenheit = dht.readTemperature(true);

  if (isnan(Humidity) || isnan(Temperature) || isnan(Temp_Fahrenheit))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT11 ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Check connection");
    delay(300);
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP: ");
  lcd.print(Temperature);
  lcd.setCursor(0, 1);
  lcd.print("HUM: ");
  lcd.print(Humidity);
  delay(1000);

  // Control motor 1 (M1) based on temperature
  if (Temperature >= 28)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print(Temperature);
    lcd.print(" | M1 FORWARD");
    lcd.setCursor(0, 1);
    lcd.print("M1 RUN FORWARD");
    m1_forward(3000);  // Run motor 1 forward for 3 seconds
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print(Temperature);
    lcd.print(" | M1 REVERSE");
    lcd.setCursor(0, 1);
    lcd.print("M1 RUN REVERSE");
    m1_reverse(3000);  // Run motor 1 reverse for 3 seconds
  }

  // Control motor 2 (M2) based on humidity
  if (Humidity >= 42)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("H: ");
    lcd.print(Humidity);
    lcd.print(" | M2 FORWARD");
    lcd.setCursor(0, 1);
    lcd.print("M2 RUN FORWARD");
    m2_forward(3000);  // Run motor 2 forward for 3 seconds
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("H: ");
    lcd.print(Humidity);
    lcd.print(" | M2 REVERSE");
    lcd.setCursor(0, 1);
    lcd.print("M2 RUN REVERSE");
    m2_reverse(3000);  // Run motor 2 reverse for 3 seconds
  }

  delay(3000);  // Delay before the next reading
}

// Motor control functions with delay and stop
void m1_forward(int duration)
{
  digitalWrite(m1in1, HIGH);
  digitalWrite(m1in2, LOW);
  delay(duration);  // Run forward for the given duration
  m1_stop();
}

void m1_reverse(int duration)
{
  digitalWrite(m1in1, LOW);
  digitalWrite(m1in2, HIGH);
  delay(duration);  // Run reverse for the given duration
  m1_stop();
}

void m1_stop()
{
  digitalWrite(m1in1, LOW);
  digitalWrite(m1in2, LOW);
}

void m2_forward(int duration)
{
  digitalWrite(m2in1, HIGH);
  digitalWrite(m2in2, LOW);
  delay(duration);  // Run forward for the given duration
  m2_stop();
}

void m2_reverse(int duration)
{
  digitalWrite(m2in1, LOW);
  digitalWrite(m2in2, HIGH);
  delay(duration);  // Run reverse for the given duration
  m2_stop();
}

void m2_stop()
{
  digitalWrite(m2in1, LOW);
  digitalWrite(m2in2, LOW);
}
