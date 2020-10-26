//Relay stuff
// light
int relay1 = 8;
volatile byte relayState = HIGH;  // Start with this HIGH so it enters the 'turn off' state first.
long unsigned timer = 0;

// pump
int relay2 = 7;
volatile byte relayState2 = LOW;  // Start with this LOW so it is off at the start

//Sensor code
const int AirValue = 465;
const int WaterValue = 210;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;
const int targetMoistureLow = 35;
const int targetMoistureHigh = 45;

//Light on startup
int lightpin = 3;

//General
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600 * SECOND;

int getSoilMoisturePercent(){
  int soilMVal = analogRead(A0); // Get soil moisture from senseor
  return(map(soilMVal, AirValue, WaterValue, 0, 100)); // Convert this to a percent
}

void setup() {
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);

  pinMode(lightpin, INPUT);

  if(digitalRead(lightpin) == LOW){  // If our switch is open (jumper not in place)
    relayState = LOW;             // We set this to HIGH so our light will enter on state at start
  }
  else{
    relayState = HIGH;
  }
  
  Serial.begin(9600);

}

void loop() {
  soilMoisturePercent = getSoilMoisturePercent(); // Convert this to a percent
  Serial.println(soilMoisturePercent);

  // Prints for debugging
  /*
    Serial.println("Moisture value:");
    Serial.println(soilMoistureValue);
    Serial.println("Moisture percent:");
    Serial.println(soilMoisturePercent);
  */

  /*
    Light turns on for 12 hours and off for 12 hours. 
    When changing state 'timer' is set to the current time + 12 hours. When that time is hit, we check the state and flip it. Rinse and repeat. 
  */

  // Light
  if ((millis() > timer)) {
    if (relayState == HIGH) {     // turn off
      digitalWrite(relay1, LOW);
      relayState = LOW;
      //timer = millis() + (5 * SECOND);
      timer = millis() + (12 * HOUR);  //TESTING
      Serial.println("Light OFF");
    }
    else if (relayState == LOW) { // turn on
      digitalWrite(relay1, HIGH);
      relayState = HIGH;
      //timer = millis() + (10 * SECOND);
      timer = millis() + (12 * HOUR); //TESTING
      Serial.println("Light ON");
    }
  }

  // Water pump
  /*
    Water pump works in 1 secon bursts. Pump I own is way too powerful so we have to water, wait a second for it to soak in, then check again.
    Checks moisture percent, if low, waters for 1 second then turns off again.
  */
  //if (soilMoisturePercent > 80) { // Use this one for testing
  if (soilMoisturePercent < targetMoistureLow) { // If we high our lower moisture boundry 
    digitalWrite(relay2, HIGH);
    relayState2 = HIGH;
    Serial.println("Pump ON");
    do{
      delay(500);
      soilMoisturePercent = getSoilMoisturePercent();
    }while (soilMoisturePercent < targetMoistureHigh);  // Keep going until we hit our high %
    digitalWrite(relay2, LOW);
    relayState2 = LOW;
    Serial.println("Pump OFF");

  }
  delay(2000);  // Pause and then we start the loop again in 2 seconds
}
