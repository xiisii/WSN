#include <WiFi.h>
#include <DHT.h>

// Thu vien bo loc
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter bo_loc(29.5, 0.1, 0.01);

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// thông tin wifi
const char* ssid = "Bi";
const char* password =  "bibi@@@@";

IPAddress ip(192, 168, 43, 5);
IPAddress gateway(192, 168, 43, 183);
IPAddress subnet(255, 255, 255, 0);


//WiFiServer wifiServer(2000);

// thông tin server 
const uint16_t port = 80;
const char * host = "192.168.43.4";
 
void setup()
{
 
  Serial.begin(115200);
  // cấu hình ip tĩnh
  WiFi.config(ip, gateway, subnet);
  // kết nối esp32 với wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
 
  Serial.print("WiFi connected with IP: ");
  // in ra địa chỉ ip
  Serial.println(WiFi.localIP());
 
}
 
void loop()
{
  // Đọc dữ liệu nhiệt độ từ cảm biến DHT
  float temperature = dht.readTemperature();
  // Áp dụng bộ lọc kalman cho nhiệt độ
  float K_temperature = bo_loc.updateEstimate(temperature);
   K_temperature = bo_loc.updateEstimate(K_temperature);
   K_temperature = bo_loc.updateEstimate(K_temperature);
   K_temperature = bo_loc.updateEstimate(K_temperature);

  // Đọc dữ liệu từ đọ ẩm cảm biến DHT
  float humidity = dht.readHumidity();
  // Áp dụng bộ lọc kalman cho độ ẩm
  float K_humidity = bo_loc.updateEstimate(humidity);
   K_humidity = bo_loc.updateEstimate(K_humidity);
   K_humidity = bo_loc.updateEstimate(K_humidity);
   K_humidity = bo_loc.updateEstimate(K_humidity);


  // Serial.println(K_temperature);
  // Serial.println(K_humidity);

  String data;
  // Kiểm tra nhiệt độ và độ ảm đã đo được phải là số
  if (isnan(temperature) == false && isnan(humidity) == false) {
    data = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}\n";
  

    WiFiClient client;
    // kết nối với địa chỉ IP và port của server
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
 
        delay(1000);
        return;
    }
    Serial.println("Connected to server successful!");

    // gửi dữ liệu từ client tới server
    client.print("Hello from ESP32 Client! \n" + data + "\n");
    //client.print(data);
    while (client.connected()) {
 
      while (client.available()>0) {
        // đọc dữ liệu được gửi từ server
        char c = client.read();
        // in dữ liệu được gửi ra Monitor
        Serial.write(c);
      }
      delay(10);
    }
    // hủy kết nối với server
    Serial.println("Disconnecting...");
    client.stop();
  }
 
    delay(10000);
}