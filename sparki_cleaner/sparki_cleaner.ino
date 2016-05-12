/*
 * Sparki Cleaner
 * Made by Nanohenry on 5.5.2016
 * It collects the "trash" on the table and drops it off the table while avoiding edges.
 */

#include <Sparki.h> // Include the library

int l = 100; // Declare the variables
int cl = 100;
int c = 100;
int cr = 100;
int r = 100;

int left = 0;
int right = 0;
const int threshold1 = 200;

const int threshold2 = 40;
int dis = 0;

long timer = 0;

bool succeed = 0;

void setup() { // Setup (runs once)
  sparki.servo(SERVO_CENTER); // Center the servo
  sparki.moveForward(); // Start to move forward
  sparki.clearLCD(); // Clear the LCD
  sparki.drawString(0, 0, "Searching..."); // Print stuff to the LCD
  sparki.updateLCD(); // Make the printed text visible
}

void loop() { // Loop (runs forever)
  sparki.moveForward(); // Move forward
  check(0); // Check for the table edge (the "0" is just for the function for knowing that where is the code running)
  if (millis() - timer > 1500) { // If 1500 milliseconds (1.5 seconds) have passed
    scan(); // Scan/look for objects
    timer = millis(); // Reset the timer
  }
  getIfPossible(); // If seen something, get it
  delay(20); // Wait 20 milliseconds for stability
}

void scan() { // The scan/look function
  sparki.clearLCD(); // Clear the LCD
  sparki.drawString(0, 0, "Scanning..."); // Print text to the LCD
  sparki.updateLCD(); // Make the text visible
  sparki.moveStop(); // Stop
  sparki.servo(SERVO_LEFT); delay(300); save(0); // Look left and save results
  delay(200); // Wait 200 milliseconds for stability
  sparki.servo(-45); delay(300); save(1); // Look between center and left and save results
  delay(200); // Wait 200 milliseconds for stability
  sparki.servo(SERVO_CENTER); delay(300); save(2); // Look center and save results
  delay(200); // Wait 200 milliseconds for stability
  sparki.servo(45); delay(300); save(3); // Look between center and right and save results
  delay(200); // Wait 200 milliseconds for stability
  sparki.servo(SERVO_RIGHT); delay(300); save(4); // Look right and save results
  delay(200); // Wait 200 milliseconds for stability
  sparki.servo(SERVO_CENTER); // Center the servo
  sparki.moveForward(); // Start to move forward
  sparki.clearLCD(); // Clear the LCD
  sparki.drawString(0, 0, "Searching..."); // Print text to the LCD
  sparki.updateLCD(); // Make the changes visible on the LCD
}

void save(int w) { // The saving function
  switch (w) { // Switch for "w"
    case 0: l = sparki.ping(); if (l == -1) {
        l = 99; // Save the reading (but if reading is -1 (the "error"/"unknown" reading) then it will be 99)
      } break; // Break is needed after a switch case or it will "drop" to the case below
    case 1: cl = sparki.ping(); if (cl == -1) {
        cl = 99; // Save the reading (but if reading is -1 (the "error"/"unknown" reading) then it will be 99)
      } break;
    case 2: c = sparki.ping(); if (c == -1) {
        c = 99; // Save the reading (but if reading is -1 (the "error"/"unknown" reading) then it will be 99)
      } break;
    case 3: cr = sparki.ping(); if (cr == -1) {
        cr = 99; // Save the reading (but if reading is -1 (the "error"/"unknown" reading) then it will be 99)
      } break;
    case 4: r = sparki.ping(); if (r == -1) {
        r = 99; // Save the reading (but if reading is -1 (the "error"/"unknown" reading) then it will be 99)
      } break;
  }
}

void check(int r) { // The "check for edge" function
  if (r == 0) {
    left = sparki.edgeLeft(); // Get the edgeLeft reading
    right = sparki.edgeRight(); // Get the edgeRight reading
    if (right < threshold1 || left < threshold1) { // Check if the left or right sensor is over edge
      sparki.moveStop(); // Stop
      sparki.beep(); // Beep
      sparki.clearLCD(); // Clear the LCD
      sparki.drawString(0, 0, "Edge detected!"); // Print text on the LCD
      sparki.updateLCD(); // Make the printed text visible on the LCD
      sparki.moveBackward(6); // Move a bit backward
      sparki.moveLeft(90); // Turn left 90 degrees
      sparki.moveForward(); // Start moving forward
    }
  } else if (r == 1) {
    left = sparki.edgeLeft(); // Do as same as above, except print something else and don't move after reaching the edge
    right = sparki.edgeRight();
    if (right < threshold1 || left < threshold1) {
      sparki.moveStop();
      sparki.clearLCD();
      sparki.drawString(0, 0, "Edge! Not possible to reach the object!");
      sparki.updateLCD();
      sparki.beep(200, 100); // Beep a low sound (200) for 100 milliseconds (100)
    }
  }
}

void getIfPossible() { // The if seen something, catch it function
  if (l < threshold2 || cl < threshold2 || c < threshold2 || cr < threshold2 || r < threshold2) { // Check that if any of the saved values are less than threshold2 (which is the threshold for the ultrasonic distance sensor)
    sparki.beep(); // Beep
    sparki.moveStop(); // Stop
    sparki.clearLCD(); // Clear the LCD
    sparki.println("Found one! Location: "); // Print something to the LCD
    sparki.updateLCD(); // Make the changes on the LCD visible

    if (l < threshold2) { // If left reading was less than threshold2
      sparki.println("left"); // Print to the LCD
      sparki.updateLCD(); // "Update" the LCD (so stuff gets visible)
      sparki.moveLeft(90); // Turn left 90 degrees
      dis = sparki.ping(); // Get the ultrasonic distance sensor reading
      if (dis == -1) { // Exclude the bad results
        dis = 99;
        dis = sparki.ping(); // Try again
        if (dis == -1) { // Exclude the bad results
          dis = 99;
        }
      }
      if (dis < threshold2) { // Check that if the object is still there
        getIt(); // Catch it
      } else {
        sparki.clearLCD(); // Clear the LCD
        sparki.drawString(0, 0, "Lost it!"); // Print "Lost it!" to the LCD
        sparki.updateLCD(); // Make the text visible on the LCD
        sparki.beep(100, 200); // Beep a low sound
        delay(500); // Wait for 500 milliseconds (half second)
      }
    } else if (cl < threshold2) { // Do the same as above but turn to a different direction
      sparki.println("center-left");
      sparki.updateLCD();
      sparki.moveLeft(45);
      dis = sparki.ping();
      if (dis == -1) {
        dis = 99;
      }
      if (dis < threshold2) {
        getIt();
      } else {
        sparki.clearLCD();
        sparki.drawString(0, 0, "Lost it!");
        sparki.updateLCD();
        sparki.beep(100, 750);
        delay(750);
      }
    } else if (c < threshold2) { // Same as above, except don't turn
      sparki.println("center");
      sparki.updateLCD();
      dis = sparki.ping();
      if (dis == -1) {
        dis = 99;
      }
      if (dis < threshold2) {
        getIt();
      } else {
        sparki.clearLCD();
        sparki.drawString(0, 0, "Lost it!");
        sparki.updateLCD();
        sparki.beep(100, 750);
        delay(750);
      }
    } else if (cr < threshold2) { // Same as above, except turn right 45
      sparki.println("center-right");
      sparki.updateLCD();
      sparki.moveRight(45);
      dis = sparki.ping();
      if (dis == -1) {
        dis = 99;
      }
      if (dis < threshold2) {
        getIt();
      } else {
        sparki.clearLCD();
        sparki.drawString(0, 0, "Lost it!");
        sparki.updateLCD();
        sparki.beep(100, 750);
        delay(750);
      }
    } else if (r < threshold2) { // Same as above but turn a bit differently
      sparki.println("right");
      sparki.updateLCD();
      sparki.moveRight(90);
      dis = sparki.ping();
      if (dis == -1) {
        dis = 99;
      }
      if (dis < threshold2) {
        getIt();
      } else {
        sparki.clearLCD();
        sparki.drawString(0, 0, "Lost it!");
        sparki.updateLCD();
        sparki.beep(100, 500);
        delay(750);
      }
    }

  }
}

void getIt() { // The catch it function
  
  sparki.moveForward(); // Start moving forward

  sparki.clearLCD(); // Wipe the LCD
  sparki.drawString(0, 0, "Reaching it..."); // Print stuff to the LCD
  sparki.updateLCD(); // Make the LCD show the text

  left = sparki.edgeLeft(); // Get the edge sensor readings
  right = sparki.edgeRight();
  dis = sparki.ping(); // Get the distance sensor reading
  while (dis > 3 && left > threshold1 && right > threshold1) { // Repeat while distance is over 3 and Sparki is not at an edge
    dis = sparki.ping(); // Get the distance sensor reading
    if (dis == -1) { // Fork out the bad results
      dis = 99;
    }
    check(1); // Get the edge sensor readings (the 1 is kind of explaned above in the check() function)
  }

  if (left > threshold1 && right > threshold1) { // Check if the sensors are not over an edge
    succeed = 1; // Set succeed to 1
  }

  delay(100); // Wait 100milliseconds for stability
  if (succeed == 1) { // If none of the sensors were over the edge
    sparki.moveForward(1); // Move a bit (1 centimeter) forward

    sparki.clearLCD(); // Clear the LCD
    sparki.drawString(0, 0, "Cought it!"); // Print text to the LCD
    sparki.updateLCD(); // Update the LCD

    sparki.gripperClose(4); // Close the gripper
    delay(1000); // Wait one second (the "first part" of the delay) for the gripper to close

    sparki.clearLCD(); // Clear the LCD for printing new text
    sparki.drawString(0, 0, "Grabbing it..."); // Print stuff to the LCD
    sparki.updateLCD(); // Show the text on the LCD

    delay(3000); // Wait 3 seconds (the "second part" of the delay) for the gripper to close
    getToEdge(); // Get to the edge
    sparki.gripperOpen(4); // Open the gripper
    delay(1000); // Wait one second (the "first part" of the delay) for the gripper to close

    sparki.clearLCD(); // Empty the LCD
    sparki.drawString(0, 0, "Releasing it..."); // Print "Releasing it..."
    sparki.updateLCD(); // Make the text visible on the LCD

    delay(3000); // Wait 3 seconds (the "second part" of the delay) for the gripper to close

    sparki.clearLCD(); // Clear the LCD
    sparki.drawString(0, 0, "Reversing and turning..."); // Print text
    sparki.updateLCD(); // Update the LCD

    sparki.moveBackward(3); // Move backward 3 centimeters
    sparki.moveLeft(180); // Turn around (full 180 degrees)

    sparki.clearLCD(); // Clear the LCD
    sparki.drawString(0, 0, "Done!"); // Print "Done!"
    sparki.updateLCD(); // Update the LCD
  }
  sparki.beep(); // Beep
  delay(500); // Wait half a second (500 milliseconds = 0,5 second)
  l = 99; // Reset the reading values
  cl = 99;
  c = 99;
  cr = 99;
  r = 99;
  sparki.clearLCD(); // "Reset" the LCD back to the LCD's screen which was displayed at the beginning
  sparki.drawString(0, 0, "Searching...");
  sparki.updateLCD();
  sparki.moveForward(); // Start moving forward
}

void getToEdge() { // The get to an edge function
  sparki.clearLCD(); // Clear the LCD to be ready for the next text
  sparki.drawString(0, 0, "Looking for edge..."); // Print stuff on the LCD
  sparki.updateLCD(); // Update the LCD
  sparki.moveForward(); // Start to move forward
  left = sparki.edgeLeft(); // Get the sensor readings
  right = sparki.edgeRight();
  while (right > threshold1 && left > threshold1) { // Wait until the sensor detects an edge
    left = sparki.edgeLeft();
    right = sparki.edgeRight();
  }
  sparki.clearLCD(); // Clear the LCD
  sparki.drawString(0, 0, "Found edge!"); // Print text to the LCD
  sparki.updateLCD(); // Make the text visible on the LCD
  sparki.moveStop(); // Stop moving
}







