#include "esp_camera.h"  // ไลบรารีสำหรับกล้อง AI-Thinker
#include <WiFi.h>        // ไลบรารีสำหรับการเชื่อมต่อ WiFi

const char* ssid = "rccar";           // ชื่อ WiFi
const char* password = "12345678";   // รหัสผ่าน WiFi
#define CAMERA_MODEL_AI_THINKER      // รุ่นของกล้อง AI-Thinker
#define PWDN_GPIO_NUM     32         // ขา GPIO สำหรับกำหนดการปิด
#define RESET_GPIO_NUM    -1         // ขา GPIO สำหรับการรีเซ็ต (ไม่ได้ใช้)
#define XCLK_GPIO_NUM      0         // ขา GPIO สำหรับคล็อกภายนอก
#define SIOD_GPIO_NUM     26         // ขา GPIO สำหรับการสื่อสารข้อมูล (SDA)
#define SIOC_GPIO_NUM     27         // ขา GPIO สำหรับการสื่อสารข้อมูล (SCL)

#define Y9_GPIO_NUM       35         // ขา GPIO สำหรับสี Y9 (จุดสีเขียว)
#define Y8_GPIO_NUM       34         // ขา GPIO สำหรับสี Y8 (จุดสีเขียว)
#define Y7_GPIO_NUM       39         // ขา GPIO สำหรับสี Y7 (จุดสีเขียว)
#define Y6_GPIO_NUM       36         // ขา GPIO สำหรับสี Y6 (จุดสีเขียว)
#define Y5_GPIO_NUM       21         // ขา GPIO สำหรับสี Y5 (จุดสีเขียว)
#define Y4_GPIO_NUM       19         // ขา GPIO สำหรับสี Y4 (จุดสีเขียว)
#define Y3_GPIO_NUM       18         // ขา GPIO สำหรับสี Y3 (จุดสีเขียว)
#define Y2_GPIO_NUM        5         // ขา GPIO สำหรับสี Y2 (จุดสีเขียว)
#define VSYNC_GPIO_NUM    25         // ขา GPIO สำหรับ VSYNC (Vertical sync)
#define HREF_GPIO_NUM     23         // ขา GPIO สำหรับ HREF (Horizontal reference)
#define PCLK_GPIO_NUM     22         // ขา GPIO 
WiFiServer server(80);  // สร้างเซิร์ฟเวอร์บนพอร์ต 80
bool connected = false; // ตัวแปรสถานะการเชื่อมต่อ

WiFiClient live_client; // ตัวแปรไคลเอนต์สำหรับเชื่อมต่อการสตรีมวิดีโอ

// HTML สำหรับการแสดงผล
String index_html = "<meta charset=\"utf-8\"/>\n" \
                    "<style>\n" \
                    "#content {\n" \
                    "display: flex;\n" \
                    "flex-direction: column;\n" \
                    "justify-content: center;\n" \
                    "align-items: center;\n" \
                    "text-align: center;\n" \
                    "min-height: 100vh;}\n" \
                    "</style>\n" \
                    "<body bgcolor=\"#000000\"><div id=\"content\">\"<img src=\"video\"></div></body>";
void configCamera(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 25000000;เพิ่มความลื่น
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_HD;//ขนาเฟรมของภาพ
  config.jpeg_quality = 59;/ความคมชัดของภาพยิ่งปรับน้อยภาพยิ่งชัด
  config.fb_count = 2;
 
  esp_err_t err = esp_camera_init(&config);  // กำหนดค่าการเชื่อมต่อกล้องและเริ่มต้นการทำงาน
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}
// ส่งภาพวิดีโอต่อเนื่อง
void liveCam(WiFiClient &client){
  camera_fb_t * fb = esp_camera_fb_get();  // เรียกใช้ข้อมูลภาพจากกล้อง
  if (!fb) {
      Serial.println("Frame buffer could not be acquired");
      return;
  }
  client.print("--frame\n");  // ส่งส่วนหัวและเปลี่ยนตัวเลือกเป็นภาพ
  client.print("Content-Type: image/jpeg\n\n");
  client.flush();
  client.write(fb->buf, fb->len);  // ส่งข้อมูลภาพวิดีโอ
  client.flush();
  client.print("\n");
  esp_camera_fb_return(fb);  // คืนทรัพยากรของภาพวิดีโอ
}
void setup() {
  Serial.begin(115200);  // เปิด Serial Monitor
  WiFi.begin(ssid, password);  // เชื่อมต่อ WiFi
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {  // รอการเชื่อมต่อ WiFi
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  String IP = WiFi.localIP().toString();  // ดึง IP ของ ESP32
  Serial.println("IP address: " + IP);
  index_html.replace("server_ip", IP);  // แทนที่ค่า IP ใน HTML
  server.begin();  // เริ่มต้นเซิร์ฟเวอร์
  configCamera();  // กำหนดค่าการเชื่อมต่อกล้อง
}
void http_resp(){
  WiFiClient client = server.available();  // รอรับคำขอจากผู้ใช้                           
  if (client.connected()) {     
      String req = "";
      while(client.available()){
        req += (char)client.read();
      }
      Serial.println("request " + req);
      int addr_start = req.indexOf("GET") + strlen("GET");
      int addr_end = req.indexOf("HTTP", addr_start);
      if (addr_start == -1 || addr_end == -1) {
          Serial.println("Invalid request " + req);
          return;
      }
      req = req.substring(addr_start, addr_end);
      req.trim();
      Serial.println("Request: " + req);
      client.flush();
  
      String s;
      if (req == "/")
      {
          s = "HTTP/1.1 200 OK\n";
          s += "Content-Type: text/html\n\n";
          s += index_html;
          s += "\n";
          client.print(s);  // ส่งหน้าเว็บไปยังผู้ใช้
          client.stop();
      }
      else if (req == "/video")
      {
          live_client = client;
          live_client.print("HTTP/1.1 200 OK\n");
          live_client.print("Content-Type: multipart/x-mixed-replace; boundary=frame\n\n");
          live_client.flush();
          connected = true;
      }
      else
      {
          s = "HTTP/1.1 404 Not Found\n\n";
          client.print(s);
          client.stop();
      }
    }       
}
  String s;
  if (req == "/")
  {
      s = "HTTP/1.1 200 OK\n";
      s += "Content-Type: text/html\n\n";
      s += index_html;
      s += "\n";
      client.print(s);  // ส่งหน้าเว็บไปยังผู้ใช้
      client.stop();
  }
  else if (req == "/video")
  {
      live_client = client;
      live_client.print("HTTP/1.1 200 OK\n");
      live_client.print("Content-Type: multipart/x-mixed-replace; boundary=frame\n\n");
      live_client.flush();
      connected = true;
  }
  else
  {
      s = "HTTP/1.1 404 Not Found\n\n";
      client.print(s);
      client.stop();
  }
void loop() {
  http_resp();
  if(connected == true){
    liveCam(live_client);
  }
}
