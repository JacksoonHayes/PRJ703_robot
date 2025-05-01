#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
String input;

void setup() {
  Serial.begin(115200);
  servo1.attach(6); // Gripper
  servo2.attach(7); // Wrist
  servo3.attach(8); // Elbow
  servo4.attach(9); // Hinge
  servo5.attach(10); // Base

  // Initial positions
  servo1.write(10);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);
}

void loop() {
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');
    input.trim();

    Serial.print("Received: ");
    Serial.println(input);

    if (input == "CUBE DETECTED") {
      placeCube();
    } else if (input == "BALL DETECTED") {
      placeBall();
    }
  }
}

void placeCubeFast() {
  servo5.write(90);
  servo4.write(160);
  servo1.write(90);
  servo2.write(75);
  servo3.write(170);
  delay(300);
  servo1.write(10);

  delay(300);

  servo4.write(90);
  servo5.write(0);

  delay(300);

  servo4.write(120);
  servo3.write(130);
  servo2.write(50);
  servo1.write(30);

  delay(300);

  servo1.write(10);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);
}

void placeCube() {
  // Pick up and place cube to right
  smoothMove(servo5, 90, 90);     // Base: centered
  smoothMove(servo4, 90, 160);    // Hinge down
  servo1.write(90);               // Gripper open
  smoothMove(servo2, 90, 75);     // Wrist down
  smoothMove(servo3, 90, 170);    // Elbow down

  delay(300);

  servo1.write(10);               // Gripper close
  delay(500);

  smoothMove(servo2,75, 90);
  smoothMove(servo3, 170, 90);
  smoothMove(servo4, 160, 90);
  delay(300);

  smoothMove(servo5, 90, 0);      // Base rotate right
  delay(500);

  smoothMove(servo4, 90, 120);
  smoothMove(servo3, 90, 130);
  smoothMove(servo2, 90, 50);
  delay(300);

  servo1.write(30);               // Gripper open (release)
  delay(500);

  smoothMove(servo2, 50, 90);
  servo1.write(0);
  smoothMove(servo3, 130, 90);
  smoothMove(servo4, 120, 90);
  smoothMove(servo5, 0, 90);      // Return base to center
}

void placeBall() {
  // Pick up and place ball to left
  smoothMove(servo5, 90, 90);     // Base: centered
  smoothMove(servo4, 90, 180);    // Hinge down
  servo1.write(90);               // Gripper open
  smoothMove(servo2, 90, 110);     // Wrist down
  smoothMove(servo3, 90, 170);    // Elbow down

  delay(300);

  servo1.write(10);               // Gripper close
  delay(500);

  smoothMove(servo3, 170, 90);
  smoothMove(servo2, 110, 90);
  smoothMove(servo4, 180, 90);
  delay(300);

  smoothMove(servo5, 90, 180);    // Base rotate left
  delay(500);

  smoothMove(servo4, 90, 120);
  smoothMove(servo3, 90, 130);
  smoothMove(servo2, 90, 50);
  delay(300);

  servo1.write(50);               // Gripper open (release)
  delay(500);

  smoothMove(servo2, 50, 90);
  servo1.write(0);
  smoothMove(servo3, 130, 90);
  smoothMove(servo4, 120, 90);
  smoothMove(servo5, 180, 90);    // Return base to center
}

void smoothMove(Servo &servo, int from, int to) {
  int step = (to > from) ? 1 : -1;
  for (int pos = from; pos != to; pos += step) {
    servo.write(pos);
    delay(15);
  }
  servo.write(to);
  delay(250);
}
