#include <Ultrasonic.h>
Ultrasonic us = Ultrasonic(6, 7);

unsigned long start_time;
unsigned long s = 0;
const unsigned long min_d = 20; // minimum distance to obstacle in cm
unsigned long d;    // distance to obstacle in cm
unsigned int dir;   // 0 - stop, 1 - forward, 2 - backward

// LEDs
const unsigned int fl = 4;  // front left LED
const unsigned int fr = 5;  // front right LED
const unsigned int b = 3;   // back LED

// motor 1
const unsigned int in1 = 8;
const unsigned int in2 = 9;
const unsigned int enA = 10;

// motor 2
const unsigned int in3 = 12;
const unsigned int in4 = 13;
const unsigned int enB = 11;

void setup() {
  pinMode(fl, OUTPUT);
  pinMode(fr, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  Serial.begin(9600);

  delay(1000);
}

void loop() {

  int signal = Serial.read();

  switch (signal) {
    case '1':
      autoMode(1000);
      break;
    case '2':
      goBackward(0);
      break;
    case '3':
      goLeft();
      break;
    case '4':
      goRight();
      break;
    default:
      // autoMode(100);
      break;
  }





  // autoMode(5000);
  // int d = us.read();
  // if (d >= min_d) {
  //   int i = 0;
  //   while (i < 200) {
  //     d = us.read();
  //     if (d <= min_d) {
  //       break;
  //     }
  //     goLeft();
  //     delay(10);
  //     i++;
  //   }
  //   i = 0;
  //   while (i < 200) {
  //     d = us.read();
  //     if (d <= min_d) {
  //       break;
  //     }
  //     goRight();
  //     delay(10);
  //     i++;
  //   }
  // }
}

void autoMode(int time) {

  int x = time / 10;
  int i = 0;

  while (i < x) {
    d = us.read();
    if (d <= min_d) {
      turnLeft(500, 120);
      int dl = us.read();
      turnRight(1000, 120);
      int dr = us.read();

      if (dr > min_d && dr > dl) {
        goForward(0);
      } else if (dr > min_d && dr < dl) {
        turnLeft(950, 120);
        goForward(0);
      } else {
        turnRight(500, 120);
        int db = us.read();

        if (db > min_d) {
          goForward(0);
        } else {
          searchMode();
        }
      }

    } else {
      goForward(0);
    }
    delay(10);
    i++;
  }
}

void searchMode() {

  while (true) {
    digitalWrite(fl, LOW);
    digitalWrite(fr, LOW);
    digitalWrite(b, LOW);

    delay(100);

    digitalWrite(fl, HIGH);
    digitalWrite(fr, HIGH);
    digitalWrite(b, HIGH);

    d = us.read();
    if (d <= min_d) {
      turnRight(0, 200);
      d = us.read();
    } else {
      break;
    }
  }
}

void goForward(int time) {
  dir = 1;
  start_time = millis();

  while (true) {
    d = us.read();
    if (d < 80) {
      analogWrite(enA, 190);
      analogWrite(enB, 190);
    } else {
      analogWrite(enA, 225);
      analogWrite(enB, 225);
    }
    if (d <= min_d) {
      s = millis() - start_time;
      break;
    }
    digitalWrite(fl, HIGH);
    digitalWrite(fr, HIGH);
    digitalWrite(b, LOW);

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    if ((millis() - start_time) >= time && time != 0) {
      // stop();
      break;
    }
  }
}

void goBackward(int time) {
  dir = 2;
  unsigned long x = (s / 225) * 200;

  // while (x > time) {
  while (true) {

    digitalWrite(fl, LOW);
    digitalWrite(fr, LOW);
    digitalWrite(b, HIGH);

    analogWrite(enA, 200);
    analogWrite(enB, 200);

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    if (time != 0) {
      delay(time);
      stop();
      break;
    }
  }
}

void goLeft() {

  if (dir == 0 || dir == 1) {
    digitalWrite(b, LOW);
  } else {

    digitalWrite(b, HIGH);
  }
  digitalWrite(fl, HIGH);
  digitalWrite(fr, LOW);

  analogWrite(enA, 200);
  analogWrite(enB, 180);
}

void goRight() {

  if (dir == 0 || dir == 1) {
    digitalWrite(b, LOW);
  } else {
    digitalWrite(b, HIGH);
  }
  digitalWrite(fl, LOW);
  digitalWrite(fr, HIGH);

  analogWrite(enA, 180);
  analogWrite(enB, 200);
}

void turnLeft(int time, int speed) {
  digitalWrite(fl, HIGH);
  digitalWrite(fr, LOW);
  digitalWrite(b, HIGH);

  analogWrite(enA, speed);
  analogWrite(enB, speed);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  if (time != 0) {
    delay(time);
    stop();
  }
}

void turnRight(int time, int speed) {
  digitalWrite(fl, LOW);
  digitalWrite(fr, HIGH);
  digitalWrite(b, HIGH);

  analogWrite(enA, speed);
  analogWrite(enB, speed);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  if (time != 0) {
    delay(time);
    stop();
  }
}

void stop() {
  dir = 0;

  digitalWrite(fr, LOW);
  digitalWrite(fl, LOW);
  digitalWrite(b, HIGH);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
