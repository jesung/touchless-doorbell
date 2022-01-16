/*
 * touchless-doorbell
 * 
 * Sounds buzzer when an object is detected within 15 cm of ultrasonic sensor.
 * Power-saving measures in place for battery operation.
 * 
 * Created 2022-01-16
 * By Jesung Park
 * 
 * Details can be found here https://github.com/jesung/touchless-doorbell
 */


#include <Adafruit_SleepyDog.h>
#include <NewPing.h>

//digital pins
#define BUZZER_PIN 4
#define ECHO_PIN 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define TRIGGER_PIN 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define MAX_DISTANCE 200

// define variables
int distance = 42; // variable for the distance measurement
int ring = 0;
int count = 10;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  
  // board layout
  // GND D2 D3 5V
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

}
void loop() {
  
  if (ring) {
    ring = 0;
    tone(BUZZER_PIN, 1250);
    delay(1000);
    tone(BUZZER_PIN, 1000);
    delay(1500);
    noTone(BUZZER_PIN);
    delay(500);
    tone(BUZZER_PIN, 1250);
    delay(1000);
    tone(BUZZER_PIN, 1000);
    delay(1500);
    noTone(BUZZER_PIN);
  }
  else if (distance < 50 || count > 0) {
    if (count > 0) {
      count--;
    }
    delay(200);   // measure every 0.2 seconds if we detect an object within 50 cm
  }
  else {
    //go to sleep for 2 seconds
    Serial.println("going to sleep...");
    delay(20);
    int sleepMS = Watchdog.sleep(2000);
    
    Serial.print("I'm awake now! I slept for ");
    Serial.print(sleepMS, DEC);
    Serial.println(" milliseconds.");
    Serial.println();
    delay(50);
  }
  
  distance = sonar.ping_cm();   // measure distance
  
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < 50) {
    count = 10;
  }
  if (distance < 15) {
    ring = 1;    
  }
  else {
    delay(50);
  }
}
