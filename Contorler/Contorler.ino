void Contorler(int x,int y){
//  Serial.println(x);
//  Serial.println(y);

  int speed_x = abs(x) ;
  int speed_y = abs(y) ;
  
//สำหรับ 0-1023
//  int speed_Sum = CalSpeed(speed_x,speed_y);
//สำหรับ 0-255
  int speed_Sum = map(CalSpeed(speed_x,speed_y), 0, 1023, min_speed, max_speed);

    Serial.println("sum = " + String(speed_Sum));
    if ( (x >= -event_joy && x <= event_joy) && y >= 1 ) {
      Serial.println("Forward");
      digitalWrite(MotorA_1, LOW);
      digitalWrite(MotorA_2, HIGH);
      analogWrite(MotorA_PWM, speed_Sum);
      
      digitalWrite(MotorB_1, LOW);
      digitalWrite(MotorB_2, HIGH);
      analogWrite(MotorB_PWM, speed_Sum);
    }
    else if ( (x >= -event_joy && x <= event_joy) && y  <= -1 ) {
      Serial.println("Backward");
      digitalWrite(MotorA_1, HIGH);
      digitalWrite(MotorA_2, LOW);
      analogWrite(MotorA_PWM, speed_Sum);
      
      digitalWrite(MotorB_1, HIGH);
      digitalWrite(MotorB_2, LOW);
      analogWrite(MotorB_PWM, speed_Sum); 
    }
    else if (x <= -1 && (y >= -event_joy && y <= event_joy) ) {
      Serial.println("Left");
      digitalWrite(MotorA_1, LOW);
      digitalWrite(MotorA_2, LOW);
      analogWrite(MotorA_PWM, 0);
      
      digitalWrite(MotorB_1, LOW);
      digitalWrite(MotorB_2, HIGH);
      analogWrite(MotorB_PWM, speed_Sum); 
     }
    else if (x >= 1 && (y >= -event_joy && y <= event_joy) ) {
      Serial.println("Right");
      digitalWrite(MotorA_1, LOW);
      digitalWrite(MotorA_2, HIGH);
      analogWrite(MotorA_PWM,speed_Sum);
      
      digitalWrite(MotorB_1, LOW);
      digitalWrite(MotorB_2, LOW);
      analogWrite(MotorB_PWM,0);
    }
    else if (x <= -event_joy && y  >= 1 ) {
      Serial.println("Forward_Left");
      digitalWrite(MotorA_1, LOW);
      digitalWrite(MotorA_2, HIGH);
      analogWrite(MotorA_PWM, speed_Sum/turn_system);
      
      digitalWrite(MotorB_1, LOW);
      digitalWrite(MotorB_2, HIGH);
      analogWrite(MotorB_PWM, speed_Sum);
    }
    //อาจจะต้องเปลี่ยน y >= even_joy
    else if (x >= event_joy && y  >= 1 ) {
      Serial.println("Forward_Right");
      digitalWrite(MotorA_1, LOW);
      digitalWrite(MotorA_2, HIGH);
      analogWrite(MotorA_PWM, speed_Sum);
      
      digitalWrite(MotorB_1, LOW);
      digitalWrite(MotorB_2, HIGH);
      analogWrite(MotorB_PWM, speed_Sum/turn_system);
    }
    else if (x <= -event_joy && y  <= -1 ) {
      Serial.println("Backward_Left");
      digitalWrite(MotorA_1, HIGH);
      digitalWrite(MotorA_2, LOW);
      analogWrite(MotorA_PWM, speed_Sum/turn_system);
      
      digitalWrite(MotorB_1, HIGH);
      digitalWrite(MotorB_2, LOW);
      analogWrite(MotorB_PWM, speed_Sum);
    }
    else if (x >= event_joy && y  <= -1 ) {
      Serial.println("Backward_Right");
      digitalWrite(MotorA_1, HIGH);
      digitalWrite(MotorA_2, LOW);
      analogWrite(MotorA_PWM, speed_Sum);
      
      digitalWrite(MotorB_1, HIGH);
      digitalWrite(MotorB_2, LOW);
      analogWrite(MotorB_PWM, speed_Sum/turn_system);
    }
    else if (x == 0 && y  == 0 ) {
      Serial.println("Stay");
      digitalWrite(MotorA_1, LOW);
      digitalWrite(MotorA_2, LOW);
      analogWrite(MotorA_PWM, 0);
      
      digitalWrite(MotorB_1, LOW);
      digitalWrite(MotorB_2, LOW);
      analogWrite(MotorB_PWM, 0);
}
//    else if (command == "brake") {
//      //ต้องลองดูอาจจะมีปัญหา
//      Serial.println("brake");
//      digitalWrite(MotorA_1, HIGH);
//      digitalWrite(MotorA_2, HIGH);
//      digitalWrite(MotorA_PWM, speed);
//      
//      digitalWrite(MotorB_1, HIGH);
//      digitalWrite(MotorB_2, HIGH);
//      digitalWrite(MotorB_PWM, speed);
//}
 }
