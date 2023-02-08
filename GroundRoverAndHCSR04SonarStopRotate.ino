// GroundRoverAndHCSR04SonarStopRotate.ino
// Ground Rover Detecting Collisions with an Ultrasonic Distance Sensor.
#include <AFMotor.h> // Adafruit Motor Shield V1 must be stacked on the UNO.

// Set up instances of each motor
AF_DCMotor motor1(1); // Motors 1 & 2 are located together. Left Side.
AF_DCMotor motor2(3); // Motors 3 & 4 are located together. Right Side.

// Set up ultrasonic sensor variables for HC-SR04 Ultrasonic Sensor.
const int trigPin = A1; //Trigger Pin.
const int echoPin = A0; // Echo Pin.
long pingDuration = 0; // Initialize duration variable.
int distance = 0; // Initialize distance variable.

boolean crash = false; // Initialize crash variable.

void checkDistance()
{
  digitalWrite(trigPin, LOW);// Clears the trigPin
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  pingDuration = pulseIn(echoPin, HIGH);// Reads the echoPin, returns the sound wave travel time in microseconds
  
  // Calculating the distance:
  // Duration x Speed of Sound (0.034cm/microsecond)->343m/s @ 20°C.
  //        --------------
  //                 2         (out and back).
  distance = pingDuration * 0.034 / 2;
  if (distance < 10) // crash distance is 5 cm or less
  {
    crash = true;
  }
}

void goBackward(int speed, int duration)
{
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(duration);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(duration);
}

void spinCounterClockwise(int speed, int duration)
{
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(duration);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(duration);
}

void goForward(int speed, int duration)
{
  long a, b;
  boolean move = true;
  a = millis();
  do
  {
    checkDistance();
    if (crash == false)
    {
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    }
    if (crash == true)
    {
      goBackward(100, 1000);
      spinCounterClockwise(100, 700);
      crash = false;
    }
    b = millis() - a;
    if (b >= duration)
    {
      move = false;
    }
  }
  while (move != false);
  // stop motors
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void setup()
{
  pinMode(trigPin, OUTPUT); //Sets the trigPin as an Output
  pinMode(echoPin, INPUT); //Sets the echoPin as an Input
  delay(5000);
}

void loop()
{
  goForward(100, 1000);
}
