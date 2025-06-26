# 🧠🤖 TinyML Object Sorting Robot

An embedded machine learning system for **real-time object detection and sorting**, powered by an **ESP32-CAM** and an **Arduino UNO-controlled robotic arm**. This project demonstrates how a locally deployed neural network can be integrated with robotic movement using inverse kinematics to autonomously classify and sort physical objects on a table.

---

## 🔍 What is it?

This system uses a **TinyML model** running directly on the **ESP32-CAM** to detect and classify simple objects, specifically **cubes** and **balls**, within the camera's field of view. Once an object is detected and centered, its position is translated into a physical coordinate, which is then passed via **UART serial communication** to an **Arduino UNO**.

On the Arduino, **inverse kinematics** (using the `FABRIK2D` algorithm) is used to solve the necessary angles for the robotic arm to move and pick up the object. The arm then places it into a zone depending on whether it’s a cube or ball.

This is a completely **offline**, **real-time**, and **low-cost** solution with no external computation, that demonstrates practical applications of Edge AI and robotics.

---

## 🧰 Hardware Components

- **ESP32-CAM** – Captures images and performs onboard object detection using a deployed TensorFlow Lite model.
- **Arduino UNO** – Receives coordinate data and performs inverse kinematics to control a 5DOF robotic arm.
- **5x Servo Motors** – Power the robotic arm joints and gripper.
- **Sensor Shield V5** – Simplifies servo connections to the Arduino UNO.
- **2x 3.7V Li-ion 18650 Batteries** – Power source for servos and camera.
- **Cubes and Spheres (Table Tennis Balls)** – Test objects used for detection and sorting.
- **3D Printed or Custom Robotic Arm Frame** – Holds and connects servos.

---

## 🗂️ Repository Structure

### `/esp32-cam-inference/`
Arduino sketch for the **ESP32-CAM**. It performs:
- Image capture
- Local inference using the deployed TFLite model
- Bounding box extraction
- Confidence filtering
- Label-based serial output to the Arduino UNO

> ✅ This must be uploaded to the ESP32-CAM to run the final system.

---

### `/inverse_kinematics/`
Arduino sketch for the **Arduino UNO**. It:
- Receives CX/CY data over UART from the ESP32-CAM
- Converts CY to forward distance
- Applies FABRIK2D inverse kinematics
- Moves the robotic arm to the correct pickup point
- Sorts object to a defined drop-off area based on label

> ✅ This must be uploaded to the Arduino UNO to run the final system.

---

### `/Collect_Images_for_EdgeImpulse/`
Scripts used to **capture and save images** from the ESP32-CAM via serial connection or browser interface. These images are used to build the dataset for training the object detection model on Edge Impulse.

> ✅ Only needed for retraining the ML model.

---

### `/old_uno_movement/`
Early test code for moving servos based on **hardcoded positions** (prior to inverse kinematics). Useful for understanding basic servo logic and structure.

> 🧪 For reference only — not used in final build.

---

### `/old_esp32_inference/`
Legacy ESP32 scripts that used **basic color filtering** or simpler detection logic before integrating the Edge Impulse model.

> 🧪 For reference or educational comparison — not needed for final build.

---

## 🛠️ To Run the Final System

1. **Import** the libraries:
   - Install the `FABRIK2D` library from the Arduino Library Manager.
   - Install the `ESP32` board package in Arduino IDE.
   - Ensure you have the `libraries` folder from this repository in your Arduino libraries directory.
2. **Upload** the code from `/esp32-cam-inference/` to the ESP32-CAM using Arduino IDE.
3. **Upload** the code from `/inverse_kinematics/` to the Arduino UNO.
4. **Connect** the ESP32-CAM and Arduino via UART (TX > RX), and power both modules.
5. **Place an object** (cube or ball) in front of the camera — the robot will detect, classify, and sort the object accordingly.

---

## 💡 Key Features

- Edge machine learning inference on low resource device (ESP32-CAM)
- UART communication between modules
- Real-time object classification
- Planar inverse kinematics via FABRIK2D
- 5DOF robotic arm controlled via servos
- Offline, standalone deployment (no cloud or external computation required)

---

## 📸 Demo

![TinyML Robot Demo](https://github.com/JacksoonHayes/public/blob/main/demonstration/PRJ703_robot.gif)

> Demonstration of the object detection and sorting project.


---

## 📚 Credits & Acknowledgements

- Model training powered by [Edge Impulse](https://www.edgeimpulse.com/)
- Inverse kinematics with [FABRIK2D Arduino library](https://github.com/hungtruong/FABRIK2D)
- Designed and developed as part of a final-year Bachelor of I.T. capstone project

---

## 📄 License

MIT License — use and modify freely.

