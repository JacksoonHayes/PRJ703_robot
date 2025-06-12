#include <FABRIK2D.h>
#include <Servo.h>

#define RAD_TO_DEG 57.2958

// Servo objects
Servo baseServo, shoulderServo, elbowServo, wristServo;

// Real-world segment lengths in mm (from user measurements)
int lengths[] = {55, 60, 100};  // shoulder→elbow, elbow→wrist, wrist→gripper tip
Fabrik2D fabrik2D(4, lengths);  // 4 joints (3 links)

void setup() {
  Serial.begin(115200);

  // Attach servos to appropriate pins
  baseServo.attach(10);    // base
  shoulderServo.attach(9); // shoulder
  elbowServo.attach(8);    // elbow
  wristServo.attach(7);    // wrist

  // Set precision tolerance
  fabrik2D.setTolerance(0.5);

  // Simulated target in 3D space (mm)
  float x = 100;
  float y = 30;
  float z = 60;

  // Tool angle: -PI / 2.5 ≈ -72°, safer than -90°
  float toolAngle = -M_PI / 2.5;

  // Solve for joint angles
  uint8_t result = fabrik2D.solve2(x, y, z, toolAngle, 0, lengths);

  // Read and convert angles
  float base = fabrik2D.getBaseAngle() * RAD_TO_DEG;
  float shoulder = fabrik2D.getAngle(0) * RAD_TO_DEG;
  float elbow = fabrik2D.getAngle(1) * RAD_TO_DEG;
  float wrist = fabrik2D.getAngle(2) * RAD_TO_DEG;

  // Send angles to servos (tune offsets as needed for physical orientation)
  baseServo.write(constrain(base + 90, 0, 180));         // base rotation
  shoulderServo.write(constrain(shoulder + 90, 0, 180));        // shoulder
  elbowServo.write(constrain(elbow + 90, 0, 180));          // elbow (inverted)
  wristServo.write(constrain(wrist + 90, 0, 180));          // wrist (stable offset)

  // Debugging
  Serial.print("Shoulder: "); Serial.println(s);
  Serial.print("Elbow: "); Serial.println(e);
  Serial.print("Wrist: "); Serial.println(w);
  Serial.print("Base: "); Serial.println(base);
  Serial.print("Solver result: "); Serial.println(result);
}

void loop() {
  // No continuous movement in this test sketch
}
