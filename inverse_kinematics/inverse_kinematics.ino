#include <FABRIK2D.h>
#include <Servo.h>

#define RAD_TO_DEG 57.2958

// Servo objects
Servo baseServo, shoulderServo, elbowServo, wristServo, gripperServo;

// Segment lengths in mm (base excluded)
int lengths[] = {60, 60, 100};  
Fabrik2D fabrik2D(4, lengths);  

// Camera input size
const int IMG_WIDTH = 96;
const int IMG_HEIGHT = 96;

// Servo base angle (start at 90°)
int baseAngle = 90;

// Target detection
int cx = -1, cy = -1;
bool centered = false;

float estimateDistanceFromCY(int cy) {
  if (cy < 40) return 30.0;

  if (cy < 55) {
    float t = (cy - 40) / 15.0;
    return 30.0 + t * (55.0 - 30.0);  // 30 → 58
  }

  if (cy < 68) {
    float t = (cy - 55) / 13.0;
    return 55.0 + t * (66.0 - 55.0);  // lower range overall

  }

  return 65.0;
}






String last_detected_label = "";

void setup() {
  Serial.begin(115200);
  
  baseServo.attach(10);
  shoulderServo.attach(9);
  elbowServo.attach(11);
  wristServo.attach(12);
  gripperServo.attach(13);
  
  fabrik2D.setTolerance(0.5);
  resetServo();

}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    Serial.print("Received: ");
    Serial.println(line);

    if (line.startsWith("LABEL:")) {
      // Extract and store the label
      int labelEnd = line.indexOf(',');
      if (labelEnd > 6) {
        last_detected_label = line.substring(6, labelEnd);
        Serial.print("Detected Label: ");
        Serial.println(last_detected_label);
      }

      // Extract CX and CY
      int cxIndex = line.indexOf("CX:");
      int cyIndex = line.indexOf("CY:");

      if (cxIndex > 0 && cyIndex > cxIndex) {
        cx = line.substring(cxIndex + 3, line.indexOf(',', cxIndex)).toInt();
        cy = line.substring(cyIndex + 3).toInt();

        Serial.print("Parsed CX: "); Serial.println(cx);
        Serial.print("Parsed CY: "); Serial.println(cy);

        // === STEP 1: Base alignment ===
        int error = cx - 48;
        Serial.print("Centering error: "); Serial.println(error);

        if (abs(error) <= 8) {
          Serial.println("✅ Object centered.");
          centered = true;
        }
        else if (abs(error) <= 12) {
          baseAngle += (error > 0) ? -5 : 5;
          Serial.println("🟦 Moderate correction (5°)");
          centered = false;
        }
        else {
          baseAngle += (error > 0) ? -10 : 10;
          Serial.println("🟥 Large correction (10°)");
          centered = false;
        }

        baseAngle = constrain(baseAngle, 0, 180);
        baseServo.write(baseAngle);

        Serial.print("Base moved to angle: ");
        Serial.println(baseAngle);

        // === STEP 2: Arm movement ===
        if (centered) {
          float forwardDistance = estimateDistanceFromCY(cy);

          Serial.print("Actual CY: ");
          Serial.println(cy);
          Serial.print("Estimated distance: ");
          Serial.println(forwardDistance);

          float y = (forwardDistance <= 40.0) ? -18.0 : -36.0;
          Serial.print("Y: ");         
          Serial.println(y);
          float baseOffsetRad = radians(baseAngle - 90);
          float x = cos(baseOffsetRad) * forwardDistance;
          float z = sin(baseOffsetRad) * forwardDistance;

          float toolAngle = (forwardDistance <= 40.0) ? (PI / 3) : (PI / 4);
          uint8_t result = fabrik2D.solve2(x, y, z, toolAngle, 0, lengths);
          float base = fabrik2D.getBaseAngle() * RAD_TO_DEG;
          float shoulder = fabrik2D.getAngle(0) * RAD_TO_DEG;
          float elbow = fabrik2D.getAngle(1) * RAD_TO_DEG;
          float wrist = fabrik2D.getAngle(2) * RAD_TO_DEG;

          baseServo.write(constrain(base + 90, 0, 180));
          shoulderServo.write(constrain(-shoulder + 90, 0, 180));
          elbowServo.write(constrain(elbow + 90, 0, 180));
          wristServo.write(constrain(wrist - 90, 0, 180));
          delay(1000);
          gripperServo.write(50);
          delay(500);

          Serial.println("IK SOLVED, MOVED TO TARGET.");

          placeObject(last_detected_label);
          delay(500);
          resetServo();
          centered = false;
        }
      }
    }
  }
}


void resetServo() {
  baseServo.write(90);
  shoulderServo.write(95);
  elbowServo.write(110);
  wristServo.write(60);
  gripperServo.write(90);
}

void placeObject(String label) {
  // Step 1: Move shoulder to current baseAngle (centering angle)
  shoulderServo.write(constrain(baseAngle, 0, 180));
  delay(500);

  // Step 2: Rotate base depending on label
  if (label == "cube") {
    baseServo.write(180);
  } else if (label == "ball") {
    baseServo.write(0);
  }
  delay(1000); // Allow time to rotate base

  // Step 3: Lower elbow and close gripper
  elbowServo.write(150);
  wristServo.write(40);
  gripperServo.write(90);

  Serial.print("Placed: ");
  Serial.println(label);
}

