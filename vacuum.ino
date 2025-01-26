#include <AFMotor.h>
#include <Servo.h>

// this constant won't change. It's the pin number of the sensor's output:
const int echoPin = 12;
const int trigPin = 13;

// Initializing motors
AF_DCMotor motorLeft(2);
AF_DCMotor motorRight(1);

// Initializing Servo
Servo myservo;
int pos = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  motorLeft.setSpeed(250);
  motorRight.setSpeed(250);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myservo.attach(9);
  moveForward(); 
  
  // turnLeft(); 
}

void loop() {
  // moveBackward();
  int forwardDistance = measureDistance();

  
  if (forwardDistance < 20) { // Obstacle detected within 20 cm
    stopMotors();

    // Check left and right distances
    int totalDistance[2];
    rotateServo(totalDistance);

    int leftDistance = totalDistance[0];
    int rightDistance = totalDistance[1];

    // Decide where to go
    if (leftDistance > rightDistance && leftDistance > 20) {
      // moveBackward();
      turnLeft();
    } else if (rightDistance > 20) {
      // moveBackward();
      turnRight();
    } else {
      moveBackward();      
    }
  } else {
    moveForward(); // No obstacle, keep moving forward
  }

  delay(100);

}

long measureDistance() {
  long duration, inches, cm;

 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 30000);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  return cm;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

// Rotate the servo to measure distances at 90-degree angles
void rotateServo(int* distances) {
  // Move to the left (90° → 0°)
  myservo.write(90);
  delay(500); // Wait for the servo to stabilize
  distances[0] = measureDistance(); // Measure distance to the left

  // Move to the right (90° → 180°)
  myservo.write(180);
  delay(500); // Wait for the servo to stabilize
  distances[1] = measureDistance(); // Measure distance to the right
  
  // Return to center (90°)
  myservo.write(90);
  delay(500); // Wait for the servo to stabilize
}

void stopMotors() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}

void moveForward() {
 motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}
void moveBackward() {
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  delay(500);

  // Stop motors
  stopMotors();

 
}

void turnLeft() {
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  delay(1000); // Turn left
  moveForward();
}

// Function to turn right
void turnRight() {
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  delay(1000); // Turn right
  moveForward();
}


