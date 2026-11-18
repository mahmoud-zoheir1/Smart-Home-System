const int LDR_PIN = A0;
const int LIGHT_LED_PIN = 13;
const int LIGHT_THRESHOLD = 400; //depending on room darkness (0-1023)

unsigned long lastSensorUpdateTime = 0;
int ldrValue = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(LIGHT_LED_PIN, OUTPUT);
  digitalWrite(LIGHT_LED_PIN, LOW);
}

void loop() {
  //SENSOR READING (Updates every 2 seconds) ===
  if (millis() - lastSensorUpdateTime >= 2000) {
    lastSensorUpdateTime = millis();
    ldrValue = analogRead(LDR_PIN);
    
    // Light automation execution
    if (ldrValue < LIGHT_THRESHOLD) {
      digitalWrite(LIGHT_LED_PIN, HIGH);
    } else {
      digitalWrite(LIGHT_LED_PIN, LOW);
    }
  }
}
