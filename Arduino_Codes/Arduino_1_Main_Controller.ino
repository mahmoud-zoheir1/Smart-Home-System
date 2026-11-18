#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h>
#include <Keypad.h>

#define DHTPIN 5 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int IN2 = 4;
const int BUTTON_PIN = A3;     // Security Button
const int WINDOW_PIN = A1;     // Magnetic Window Switch
const int ALARM_PIN = 2;       // Alarm Indicator
const int PIR_PIN = A2;        // Motion sensor
const int MOTION_LED_PIN = 3;  // motion indicator LED

const float TEMP_THRESHOLD = 30.0; 

bool securityMode = false;
bool lastButtonState = LOW;
bool currentButtonState = LOW;

Servo myServo;
const int SERVO_PIN = A0;     
const int NEUTRAL_ANGLE = 0;   
const int UNLOCKED_ANGLE = 90; 

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; 
byte colPins[COLS] = {9, 8, 7, 6}; 
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Password
const String CORRECT_PASSWORD = "1234"; 
String inputPassword = "";

unsigned long lastSensorUpdateTime = 0;
float t = 0;
int windowState = HIGH;

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  lcd.init();
  lcd.backlight();

  pinMode(IN2, OUTPUT);
  digitalWrite(IN2, LOW);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(WINDOW_PIN, INPUT);
  digitalWrite(ALARM_PIN, LOW);
  pinMode(PIR_PIN, INPUT);
  pinMode(MOTION_LED_PIN, OUTPUT);
  digitalWrite(MOTION_LED_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Smart home");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  
  myServo.attach(SERVO_PIN);
  myServo.write(NEUTRAL_ANGLE);
}

void loop() {
    // KEYPAD INPUT
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      inputPassword = ""; 
      lcd.clear();
    } 
    else if (key == '#') {
      if (inputPassword == CORRECT_PASSWORD) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted!");
        
        myServo.write(UNLOCKED_ANGLE); 
        delay(4000);
        myServo.write(NEUTRAL_ANGLE);  
        
        inputPassword = "";
        lcd.clear();
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password!");
        delay(1500);
        inputPassword = "";
        lcd.clear();
      }
    } 
    else {
      inputPassword += key; 
    }
  }

  // Temp. READING
  if (millis() - lastSensorUpdateTime >= 2000) {
    lastSensorUpdateTime = millis();
    t = dht.readTemperature();
    
    // Fan
    if (!isnan(t) && t > TEMP_THRESHOLD) {
      digitalWrite(IN2, HIGH);
    } else {
      digitalWrite(IN2, LOW);
    }
  }

  // Security & Alarm
  windowState = digitalRead(WINDOW_PIN);
  currentButtonState = digitalRead(BUTTON_PIN);
  
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    securityMode = !securityMode; 
    digitalWrite(ALARM_PIN, LOW); 
    delay(250);
  }
  lastButtonState = currentButtonState;

  int motionState = digitalRead(PIR_PIN);

  if (securityMode) {
    if (windowState == LOW) {
      digitalWrite(ALARM_PIN, HIGH); 
    } 
    else {
      digitalWrite(ALARM_PIN, LOW);
    }

     if (motionState == HIGH) {
      digitalWrite(MOTION_LED_PIN, HIGH);
    } else {
      digitalWrite(MOTION_LED_PIN, LOW);
    }
  } 
  else {
    digitalWrite(ALARM_PIN, LOW);  
    digitalWrite(MOTION_LED_PIN, LOW); 
  }

  // LCD DISPLAY 
  if (inputPassword.length() > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Enter Pass:     ");
    lcd.setCursor(0, 1);
    
    String maskedOutput = "";
    for (int i = 0; i < inputPassword.length(); i++) {
      maskedOutput += "*";
    }
    lcd.print(maskedOutput + "                "); // Trailing spaces clean old layout
  } 
  else {
    lcd.setCursor(0, 0);
    if (securityMode && (windowState == LOW || motionState == HIGH)) {
      if (windowState == LOW) {
        lcd.print("ALERT: WINDOW!  ");
      } else {
        lcd.print("ALERT: MOTION!  ");
      }
    } else {
      lcd.print("Door: closed    ");
    }

    lcd.setCursor(0, 1);
    if (!isnan(t)) {
      lcd.print("T: ");
      lcd.print(t, 1);
      lcd.print(" C  ");
    } else {
      lcd.print("T: Sensor Error ");
    }
    
    lcd.setCursor(12, 1);
    lcd.print(securityMode ? "[A]" : "[D]"); 
  }
}
