#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "rccar";
const char* password = "12345678";

ESP8266WebServer server(80);

int MotorA_PWM = D1;
int MotorA_1 = D2;
int MotorA_2 = D3 ;

int MotorB_1 = D5;
int MotorB_2 = D6;
int MotorB_PWM = D7;

//ปรับตรงนี้ถ้าอยากให้เลี้ยวได้เยอะขึ้น
double turn_system = 3;

//ปรับตรงนี้หากอยากให้ขอบเขตระยะของคำสั่งในจอยกว้างขึ้นหรือลดลง
int event_joy = 200;

//ค่าขั้นต่ำที่มอเตอร์จะหมุน
int min_speed = 40;
int max_speed = 255;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(MotorA_1,OUTPUT);
  pinMode(MotorA_2,OUTPUT);
  pinMode(MotorA_PWM,OUTPUT);
  pinMode(MotorB_1,OUTPUT);
  pinMode(MotorB_2,OUTPUT);
  pinMode(MotorB_PWM,OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/control", handleControl);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleControl() {
  int x = server.arg("x").toInt();
  int y = server.arg("y").toInt();

    Serial.println("x = " + String(x));
    Serial.println("y = " + String(y));
    
  //ส่งไปควบคุมรถ
  Contorler(x,y);
  
  //server.send(200, "text/plain", "OK");
  
}

int CalSpeed (int input_X,int input_Y){
  int x = abs(input_X);
  int y = abs(input_Y);
        if(x <= y){
            return y;
          }
         else {
            return x;
          }
}


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
