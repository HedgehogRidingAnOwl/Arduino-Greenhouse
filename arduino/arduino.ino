//////////////////////////////
// START USER CONFIG 
//////////////////////////////

// light
int relayLight =                8;    // Pin controlling relay connect to light
volatile byte relayLightState = LOW;  // Start with this LOW so it enters the 'turn off' state first.

//Light on startup
int lightPin =                  3;    // Pin that inverses behavior of light on startup when it reads LOW

// pump
int relayPump =                 7;    // Pin controlling relay connected to water pump

//Sensor code
const int AirValue =            465;  // Value found by suspending soil moisture sensor in air
const int WaterValue =          210;  // Value found by suspending soil moisture sensor in water
const int targetMoistureLow =   35;   // Lower bound of target range for soil moisture
const int targetMoistureHigh =  45;   // Upper bound of target range for soil moisture


//////////////////////////////
// END USER CONFIG
//////////////////////////////

//General
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600 * SECOND;

long unsigned timer = 0;
volatile byte relayPumpState = LOW;  // Start with this LOW so it is off at the start

int soilMoistureValue = 0;
int soilMoisturePercent = 0;

int getSoilMoisturePercent(){
  int soilMVal = analogRead(A0); // Get soil moisture from senseor
  return(map(soilMVal, AirValue, WaterValue, 0, 100)); // Convert this to a percent
}

void setup() {
  pinMode(relayLight, OUTPUT);
  digitalWrite(relayLight, LOW);
  pinMode(relayPump, OUTPUT);
  digitalWrite(relayPump, LOW);

  pinMode(lightPin, INPUT);

  if(digitalRead(lightPin) == HIGH){    // If our switch is closed (jumper in place) 
    if(relayLightState == HIGH){
      relayLightState = LOW;            // We set this to HIGH so our light will enter on state at start
    }
    else{
      relayLightState = HIGH;
    }
  }
  
  Serial.begin(9600);

}

void loop() {
  soilMoisturePercent = getSoilMoisturePercent(); // Convert this to a percent
  Serial.println(soilMoisturePercent);

  /*
    Light turns on for 12 hours and off for 12 hours. 
    When changing state 'timer' is set to the current time + 12 hours. When that time is hit, we check the state and flip it. Rinse and repeat. 
  */

  // Light
  if ((millis() > timer)) {
    if (relayLightState == HIGH) {     // turn off
      digitalWrite(relayLight, LOW);
      relayLightState = LOW;
      //timer = millis() + (5 * SECOND);
      timer = millis() + (12 * HOUR);  //TESTING
      Serial.println("Light OFF");
    }
    else if (relayLightState == LOW) { // turn on
      digitalWrite(relayLight, HIGH);
      relayLightState = HIGH;
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
    digitalWrite(relayPump, HIGH);
    relayPumpState = HIGH;
    Serial.println("Pump ON");
    do{
      delay(500);
      soilMoisturePercent = getSoilMoisturePercent();
    }while (soilMoisturePercent < targetMoistureHigh);  // Keep going until we hit our high %
    digitalWrite(relayPump, LOW);
    relayPumpState = LOW;
    Serial.println("Pump OFF");

  }
  delay(2000);  // Pause and then we start the loop again in 2 seconds
}
