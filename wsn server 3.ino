#include "WiFi.h"

// thông tin wifi
const char* ssid = "Bi";
const char* password =  "bibi@@@@";

// cấu hình port
WiFiServer wifiServer(80);

// thông số để thiết lập ip tĩnh cho server
IPAddress ip(192, 168, 43, 4);
IPAddress gateway(192, 168, 43, 183);
IPAddress subnet(255, 255, 255, 0);

//const uint16_t port = 2000;
//const char * host = "192.168.43.5";
 


void setup() {
 
  Serial.begin(115200);
 
  delay(1000);
  // cấu hình ip tĩnh
  WiFi.config(ip, gateway, subnet);
  // kết nối wifi
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
 

}

String data;
 
void loop() {
  // tạo đối tượng wifiClient kết nối với wifi
  WiFiClient client = wifiServer.available();
  // đọc thông số khí gas
  float gas = analogRead(15);
  data = "{\"gas\": " + String(gas) + "}\n";

  // kiểm tra kết nối
  if (client) {
    // gửi tin nhắn đến client
    client.print("Hello from ESP32 server! \n");
    client.print(data);
 
    while (client.connected()) {
 
      while (client.available()>0) {
        // đọc dữ liệu được gửi từ client
        char c = client.read();
        // in dữ liệu được gửi ra Monitor
        Serial.write(c);
      }
      delay(10);
      // hủy kết nối với client
      client.stop();
      Serial.println("Client disconnected");
    }

    
  }
    //delay(10000);
}
    
