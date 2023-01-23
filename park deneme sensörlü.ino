#include <Servo.h>
#include <Ultrasonic.h>

// Create the servo object
Servo myservo;
Servo back_right;  // define servo for rrb
Servo back_left;
Ultrasonic ultrasonic_back(5, 6);           // ultrasonic for back
Ultrasonic ultrasonic_right_back(7, 8);     // ultrasonic for right back
Ultrasonic ultrasonic_right_front(16, 17);  // ultrasonic for right front
Ultrasonic ultrasonic_front(11, 12);        // ultrasonic for front
Ultrasonic ultrasonic_right_cross(14, 15);  // cross back
// Setup section to run once
bool parking_find = false;
bool parking_first = false;
bool parking_first2 = false;
bool parking_second = false;
bool parking_third = false;
bool parking_done = false;
void setup() {
  Serial.begin(9600);
  myservo.attach(4);     // attach the servo to our servo object
  back_right.attach(2);  // right servo connection
  back_left.attach(3);
  myservo.write(85);
  back_right.write(80);
  back_left.write(105);
  long right_front_sensor = ultrasonic_right_front.distanceRead(CM);
  long right_back_sensor = ultrasonic_right_back.distanceRead(CM);
  while (right_front_sensor < 10 && right_back_sensor < 10) {
    Serial.println("inside");

      break;
    
  }
}

// Loop to keep the motor turning!
void loop() {
  long front_sensor = ultrasonic_front.distanceRead(CM);
  long right_front_sensor = ultrasonic_right_front.distanceRead(CM);
  long right_back_sensor = ultrasonic_right_back.distanceRead(CM);
  long back_sensor = ultrasonic_back.distanceRead(CM);
  long right_cross = ultrasonic_right_cross.distanceRead(CM);

  if (front_sensor < 5 && parking_done == false) {
    parking_find = false;
    parking_first = false;
    parking_second = false;
    parking_third = false;
    parking_done = false;
    Serial.println("all false");
    Serial.println("--------------------------");
  }

  // park alanı bulundu
  if (right_front_sensor < 10 && right_back_sensor < 10 && parking_find == false && parking_done == false) {
    Serial.println("park başladı bulundu");
    parking_find = true;
    back_right.write(90);
    back_left.write(90);
    myservo.write(130);
    Serial.println("park bulundu");
    Serial.println("--------------------------");
  }

  back_sensor = ultrasonic_back.distanceRead(CM);
  right_cross = ultrasonic_right_cross.distanceRead(CM);
  // parka başla sağa dön ve geri git
  if (parking_find == true && back_sensor > 5 && parking_first == false && right_cross > 5 && parking_done == false) {
    if (parking_first2 == false) {
      Serial.println("geri sağ yapılıyor başladı");
      myservo.write(120);
      back_left.write(80);
      //back_right.write(105);
      delay(2550);
      parking_first2 = true;
      Serial.println("geri sağ yapılıyor");
      Serial.println("--------------------------");
    }
    myservo.write(85);
    back_left.write(85);
    back_right.write(105);
    delay(2000);
    back_sensor = ultrasonic_back.distanceRead(CM);
    right_cross = ultrasonic_right_cross.distanceRead(CM);
    // çok yakınsa işlemi durdur
    Serial.println(back_sensor);
    if (back_sensor < 10) {
      parking_first = true;
      parking_done == true;
      Serial.println("geri sağ çok yakın hatası bitir");
      Serial.println("--------------------------");
    }
  }
  // sola dön ve geri git
  if (parking_find == true && parking_first == true && parking_second == false && parking_done == false) {
    Serial.println("geri sol yapılıyor başladı");
    myservo.write(50);
    back_right.write(105);
    //delay(3500);
    Serial.println("geri sol yapılıyor");
    Serial.println("--------------------------");
    back_sensor = ultrasonic_back.distanceRead(CM);
    // çok yakınsa işlemi durdur
    if (back_sensor < 5) {
      parking_second = true;
      parking_done == true;

      Serial.println("geri sol çok yakın hatası bitir");
      Serial.println("--------------------------");
      while (true) {
        myservo.write(85);
        back_right.write(90);
        back_left.write(90);
      }
    }
  }

  front_sensor = ultrasonic_front.distanceRead(CM);
  back_sensor = ultrasonic_back.distanceRead(CM);

  if (parking_find == true && parking_first == true && parking_second == true && parking_third == false && parking_done == false) {
    if (front_sensor > back_sensor) {
      myservo.write(85);
      back_right.write(85);
      back_left.write(100);
      Serial.println("geri düz yapılıyor");
      Serial.println("--------------------------");
      front_sensor = ultrasonic_front.distanceRead(CM);
      back_sensor = ultrasonic_back.distanceRead(CM);
      if (front_sensor <= back_sensor) {
        parking_third = true;
        parking_done = true;
        Serial.println("geri düz hatası bitir");
        Serial.println("--------------------------");
        while (true) {
          myservo.write(85);
          back_right.write(90);
          back_left.write(90);
        }
      }
    }
  }
}