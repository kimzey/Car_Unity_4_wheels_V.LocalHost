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
