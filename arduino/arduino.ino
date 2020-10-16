
//Relay stuff
// light
int relay1 = 8;
volatile byte relayState = LOW;  // Start with this HIGH so it enters the 'turn off' state first.
long unsigned timer = 0;
// pump
int relay2 = 7;
volatile byte relayState2 = LOW;

//Sensor code
const int AirValue = 465;
const int WaterValue = 210;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;
const int targetMoisture = 35;

//General
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600 * SECOND;

void setup() {
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);
  Serial.begin(9600);
}

void loop() {
  soilMoistureValue = analogRead(A0);
  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.println(soilMoisturePercent);

  // Prints for debugging
  /*
    Serial.println("Moisture value:");
    Serial.println(soilMoistureValue);
    Serial.println("Moisture percent:");
    Serial.println(soilMoisturePercent);
  */

  // Light
  if ((millis() > timer)) {
    if (relayState == HIGH) {     // turn off
      digitalWrite(relay1, LOW);
      relayState = LOW;
      //timer = millis() + (5 * SECOND);
      timer = millis() + (8 * HOUR);  //TESTING
      Serial.println("Light OFF");
    }
    else if (relayState == LOW) { // turn on
      digitalWrite(relay1, HIGH);
      relayState = HIGH;
      //timer = millis() + (10 * SECOND);
      timer = millis() + (16 * HOUR); //TESTING
      Serial.println("Light ON");
    }
  }

  // Water pump
  //if (soilMoisturePercent > 80) { // Use this one for testing
  if (soilMoisturePercent < 35) {
    digitalWrite(relay2, HIGH);
    relayState2 = HIGH;
    Serial.println("Pump ON");
    delay(1000);
    digitalWrite(relay2, LOW);
    relayState2 = LOW;
    Serial.println("Pump OFF");


  }
  delay(200);
}
