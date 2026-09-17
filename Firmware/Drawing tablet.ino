#include <SPI.h>
#include <TFT_eSPI.h>

// 
#define USE_WIFI    // whatever you want to use/ wifi or bluetooth just uncomment 
// #define USE_BT

#ifdef USE_WIFI
  #include <WiFi.h>
  const char* WIFI_SSID = "YOUR_WIFI_SSID";
  const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
  const char* PC_IP     = "192.168.1.50";   // pc lan ip adress 
  const uint16_t PC_PORT = 5005;
  WiFiClient client;
#endif

#ifdef USE_BT
  #include "BluetoothSerial.h"
  BluetoothSerial SerialBT;
#endif

TFT_eSPI tft = TFT_eSPI();


uint16_t calData[5] = { 458, 3383, 331, 3407, 7 };


#define SCR_W 320
#define SCR_H 240

#define ROW1_H 24   // colors + thickness
#define ROW2_H 16   // brightness slider
#define ROW3_H 16   // clear / export / status
#define MENU_H (ROW1_H + ROW2_H + ROW3_H)   // 56
#define CANVAS_Y MENU_H
#define CANVAS_H (SCR_H - MENU_H)

uint16_t palette[] = {
  TFT_BLACK, TFT_RED, TFT_GREEN, TFT_BLUE,
  TFT_YELLOW, TFT_ORANGE, TFT_WHITE   
};
const int numColors = 7;
const int thickBoxW = 54;
int swatchW = (SCR_W - thickBoxW) / numColors;

uint16_t currentColor = TFT_BLACK;
int thickness = 3;          
int lastX = -1, lastY = -1;
bool wasTouching = false;


#define BL_PIN 5
#define BL_PWM_FREQ 5000
#define BL_PWM_RES  8
#define BL_CHANNEL  0
int brightness = 200;       

const int SLIDER_X1 = 10;
const int SLIDER_X2 = SCR_W - 10;
const int SLIDER_Y  = ROW1_H;
const int SLIDER_TRACK_H = ROW2_H;

struct BtnZone { int x1, x2; };
BtnZone Z_CLEAR  = {5,   75};
BtnZone Z_EXPORT = {80,  160};

void setBacklight(int val) {
  brightness = constrain(val, 10, 255);
  #if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(BL_PIN, brightness);
  #else
    ledcWrite(BL_CHANNEL, brightness);
  #endif
}


void drawTopRow() {
  tft.fillRect(0, 0, SCR_W, ROW1_H, TFT_DARKGREY);
  for (int i = 0; i < numColors; i++) {
    int x = i * swatchW;
    tft.fillRect(x + 2, 2, swatchW - 4, ROW1_H - 4, palette[i]);
    if (palette[i] == currentColor) {
      tft.drawRect(x, 0, swatchW, ROW1_H, TFT_CYAN);
      tft.drawRect(x + 1, 1, swatchW - 2, ROW1_H - 2, TFT_CYAN);
    }
  }
  int tx = numColors * swatchW;
  tft.fillRect(tx, 0, thickBoxW, ROW1_H, TFT_NAVY);
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("-", tx + thickBoxW / 4, ROW1_H / 2, 4);
  tft.drawString("+", tx + 3 * thickBoxW / 4, ROW1_H / 2, 4);
}

void drawSlider() {
  tft.fillRect(0, SLIDER_Y, SCR_W, SLIDER_TRACK_H, TFT_BLACK);
  int trackW = SLIDER_X2 - SLIDER_X1;
  int fillW = map(brightness, 10, 255, 0, trackW);
  tft.fillRect(SLIDER_X1, SLIDER_Y + 3, trackW, 6, TFT_DARKGREY);
  tft.fillRect(SLIDER_X1, SLIDER_Y + 3, fillW, 6, TFT_YELLOW);
  int knobX = SLIDER_X1 + fillW;
  tft.fillCircle(knobX, SLIDER_Y + 6, 6, TFT_WHITE);
  tft.drawCircle(knobX, SLIDER_Y + 6, 6, TFT_CYAN);
}

void drawBottomRow() {
  int y = SLIDER_Y + ROW2_H;
  tft.fillRect(0, y, SCR_W, ROW3_H, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("CLEAR", Z_CLEAR.x1, y, 2);
  tft.drawString("EXPORT", Z_EXPORT.x1, y, 2);
  tft.drawString("T:" + String(thickness), 170, y, 2);
  tft.drawString("B:" + String(brightness), 220, y, 2);
  tft.fillCircle(SCR_W - 8, y + 8, 4,
    #ifdef USE_WIFI
      (WiFi.status() == WL_CONNECTED) ? TFT_GREEN : TFT_RED
    #else
      (SerialBT.hasClient()) ? TFT_GREEN : TFT_RED
    #endif
  );
}

void drawMenu() {
  drawTopRow();
  drawSlider();
  drawBottomRow();
}

void clearCanvas() {
  tft.fillRect(0, CANVAS_Y, SCR_W, CANVAS_H, TFT_WHITE);
  sendCommand("C\n");
}


void sendCommand(const String &s) {
  #ifdef USE_WIFI
    if (client.connected()) client.print(s);
  #endif
  #ifdef USE_BT
    SerialBT.print(s);
  #endif
}

void sendStroke(int x1, int y1, int x2, int y2, uint16_t color, int th) {
  uint8_t r = ((color >> 11) & 0x1F) * 255 / 31;
  uint8_t g = ((color >> 5) & 0x3F) * 255 / 63;
  uint8_t b = (color & 0x1F) * 255 / 31;
  String msg = "D," + String(x1) + "," + String(y1) + "," +
               String(x2) + "," + String(y2) + "," +
               String(r) + "," + String(g) + "," + String(b) + "," +
               String(th) + "\n";
  sendCommand(msg);
}


void setup() {
  Serial.begin(115200);
  delay(300);

  #if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcAttach(BL_PIN, BL_PWM_FREQ, BL_PWM_RES);
    ledcWrite(BL_PIN, brightness);
  #else
    ledcSetup(BL_CHANNEL, BL_PWM_FREQ, BL_PWM_RES);
    ledcAttachPin(BL_PIN, BL_CHANNEL);
    ledcWrite(BL_CHANNEL, brightness);
  #endif

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);

  tft.setTouch(calData);  
  Serial.println("Touch calibration applied");

  drawMenu();

  #ifdef USE_WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi");
    unsigned long wifiStart = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 8000) {
      delay(300);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
      client.connect(PC_IP, PC_PORT);
    } else {
      Serial.println("\nWiFi FAILED - continuing without network");
    }
  #endif

  #ifdef USE_BT
    SerialBT.begin("ESP32_DrawTablet");
    Serial.println("Bluetooth started");
  #endif

  drawMenu();
  Serial.println("Setup complete - entering loop()");
}


void loop() {
  uint16_t x, y;
  bool touched = tft.getTouch(&x, &y);

  #ifdef USE_WIFI
    if (WiFi.status() == WL_CONNECTED && !client.connected()) {
      client.connect(PC_IP, PC_PORT);
    }
  #endif

  if (touched) {
    if (y < MENU_H) {
      handleMenuTouch(x, y);
      wasTouching = false;
      return;
    }

    if (!wasTouching) {
      lastX = x; lastY = y;
      wasTouching = true;
    }

    for (int o = -thickness / 2; o <= thickness / 2; o++) {
      tft.drawLine(lastX, lastY + o, x, y + o, currentColor);
    }
    tft.fillCircle(x, y, thickness / 2, currentColor);

    sendStroke(lastX, lastY, x, y, currentColor, thickness);

    lastX = x;
    lastY = y;
  } else {
    wasTouching = false;
  }

  delay(5);
}


void handleMenuTouch(int x, int y) {
  if (y < ROW1_H) {
    if (x < numColors * swatchW) {
      int idx = x / swatchW;
      currentColor = palette[idx];
      drawTopRow();
      return;
    }
    int tx = numColors * swatchW;
    if (x >= tx && x < tx + thickBoxW) {
      if (x < tx + thickBoxW / 2) thickness = max(1, thickness - 1);
      else thickness = min(10, thickness + 1);
      drawBottomRow();
    }
    return;
  }

  if (y < ROW1_H + ROW2_H) {
    int clampedX = constrain(x, SLIDER_X1, SLIDER_X2);
    int trackW = SLIDER_X2 - SLIDER_X1;
    int val = map(clampedX - SLIDER_X1, 0, trackW, 10, 255);
    setBacklight(val);
    drawSlider();
    drawBottomRow();
    return;
  }

  if (x >= Z_CLEAR.x1 && x < Z_CLEAR.x2) {
    clearCanvas();
  } else if (x >= Z_EXPORT.x1 && x < Z_EXPORT.x2) {
    sendCommand("E\n");
    tft.fillCircle(SCR_W - 8, ROW1_H + ROW2_H + 8, 4, TFT_CYAN);
    delay(150);
    drawBottomRow();
  }
}
