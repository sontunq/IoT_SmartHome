#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <time.h>
#include "web_page.h"
#include "faces.h"

// Kieu du lieu cho mat robot: phai khai bao o dau file (Arduino IDE tu chen khai bao ham len tren)
// Tat ca la so thuc de mat chuyen dong bang lo xo (xem phan OLED ben duoi)
struct Blob  { float k, x, y, w, h, ti, to, bc, tc, s; };   // 1 mat
struct Mouth { float k, x, y, w, c, h, rx, ry, a; };        // mieng
struct Part  { float p[10], v[10], nx[10]; float sq, ph; bool sw, init; };   // 1 bo phan dang chuyen dong

// ===== WiFi + Telegram + Web: thong tin bi mat nam trong secrets.h (KHONG dua len GitHub) =====
// Lan dau: chep secrets.example.h thanh secrets.h roi dien WiFi, token Telegram, mat khau web.
#include "secrets.h"
const char* MDNS_NAME  = "cerberus";              // -> http://cerberus.local
const bool  USE_WIFI   = true;   // false = TAT WiFi/Telegram/web (do sut ap), dieu khien bang trang cerberus_usb.html qua cap USB

// ===== Chan =====
const int PIN_BUZZER    = 17;
const int PIN_SERVO     = 13;
const int PIN_RC522_SS  = 5;
const int PIN_RC522_RST = 27;
const int PIN_TRIG      = 16;
const int PIN_ECHO      = 34;
const int PIN_VIB       = 39;
const int PIN_SDA       = 21;
const int PIN_SCL       = 22;
const int PIN_ARD_RX    = 26;                 // nhan tu Arduino D1 (qua chia ap)
const int PIN_ARD_TX    = 25;                 // gui sang Arduino D0
HardwareSerial ARD(2);

// ===== The =====
const char* OWNER_UID  = "DE:DF:14:05";      // the chu nha
const char* OWNER_NAME = "Chu nha";
const char* SEED_UID   = "C0:E7:F3:25";      // the thanh vien co san (chi them o lan chay dau tien)
const char* SEED_NAME  = "Thanh vien 1";

// ===== Cau hinh =====
const int  SERVO_CLOSED = 0;
const int  SERVO_OPEN   = 90;
const bool SERVO_REVERSE = true;               // true = dao chieu quay (cua mo nguoc huong thi doi true/false)
const int  SERVO_STEP_MS = 8;                 // quay tu tu: 8 ms moi do
const float PRESENCE_CM = 5.0;                // nguoi dung gan hon muc nay la "co nguoi" (mo hinh: 5 cm)
const unsigned long OPEN_MS          = 5000;  // cua mo bao lau roi tu dong
const unsigned long READY_TIMEOUT_MS = 5000;
const unsigned long GREET_MS         = 3000;  // thoi gian hien loi chao
const unsigned long DENIED_MS        = 2600;  // du thoi gian cho hieu ung gian
const int           MAX_FAILS        = 3;
const unsigned long LOCK_MS          = 30000;
const unsigned long OWNER_HELLO_MS   = 3200;  // man chao chu nha truoc khi vao che do cap quyen
const unsigned long GRANT_MS         = 15000; // che do cap quyen
const unsigned long FLASH_MS         = 2600;  // mat "da them" (an the) / "da thu hoi"

const int           VIB_HITS        = 5;     // dap 5 lan trong 4 giay -> bao dong
const unsigned long VIB_WINDOW_MS   = 4000;
const unsigned long VIB_DEBOUNCE_MS = 150;   // 1 cu go chi tinh 1 lan
const unsigned long KNOCK_GAP_MS    = 1500;  // go 1-4 lan roi ngung 1,5 giay -> bao "co nguoi go cua"
const unsigned long KNOCK_REPEAT_MS = 10000; // 2 lan bao go cua cach nhau it nhat 10 giay
const unsigned long VIB_SERVO_IGNORE_MS = 1500;
const unsigned long ALARM_MS        = 30000;

const unsigned long TG_POLL_MS        = 3000;
const unsigned long REMOTE_CONFIRM_MS = 30000;

// ===== Doi tuong =====
MFRC522 rfid(PIN_RC522_SS, PIN_RC522_RST);
Servo doorServo;
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Preferences prefs;
bool oledOk = false;

// ===== Danh sach thanh vien (luu flash) =====
const int MAX_MEMBERS = 10;
String memUid[MAX_MEMBERS], memName[MAX_MEMBERS];
int memCount = 0;

// ===== Trang thai =====
enum DoorState { D_IDLE, D_READY, D_GREET, D_OWNER, D_GRANT, D_DENIED, D_LOCKED, D_ALARM };
DoorState state = D_IDLE;
unsigned long stateT = 0, lastPresenceT = 0, lastDistT = 0, grantUntil = 0;
float distanceCm = 999;
int failCount = 0;
String greetText = "";          // chu duoi mat robot khi chao
int flashFace = 0;              // 0 khong, 1 "da them", 2 "da thu hoi"
unsigned long flashUntil = 0;
String flashText = "";
bool sysLock = false;           // khoa he thong: the thanh vien khong mo duoc cua
int rcVersion = 0;
volatile bool tgOk = false;     // lan gui/nhan Telegram gan nhat co thanh cong khong
uint16_t cardsVer = 1;          // tang moi khi danh sach the thay doi (de trang web tai lai)

// ===== Nhat ky su kien cho trang web (luu trong RAM, 50 su kien gan nhat, mat khi mat dien) =====
enum EvKind : uint8_t { K_OWNER, K_OK, K_REMOTE, K_GRANT, K_REVOKE, K_UNKNOWN, K_LOCKED, K_INTRUSION, K_SILENCE, K_SYS,
                        K_HFIRE, K_HRAIN, K_HHOT, K_HLIGHT, K_HMODE, K_FIREDOOR, K_HSYS, K_HINTRUDE };
const char* KIND_STR[] = {"owner", "ok", "remote", "grant", "revoke", "unknown", "locked", "intrusion", "silence", "sys",
                          "h_fire", "h_rain", "h_hot", "h_light", "h_mode", "fire_door", "h_sys", "h_intrude"};
struct Ev { uint32_t id; unsigned long ms; uint8_t kind; char uid[24]; char who[56]; };
const int LOG_N = 50;
Ev evLog[LOG_N];
uint32_t evNext = 1;            // id cua su kien tiep theo

void copyUtf8(char* dst, const String &src, size_t n) {   // chep chuoi, khong cat ngang ky tu co dau
  strlcpy(dst, src.c_str(), n);
  if (src.length() >= n) {
    int len = strlen(dst), k = len - 1;
    while (k > 0 && ((uint8_t)dst[k] & 0xC0) == 0x80) k--;
    uint8_t lead = (uint8_t)dst[k];
    int need = (lead >= 0xF0) ? 4 : (lead >= 0xE0) ? 3 : (lead >= 0xC0) ? 2 : 1;
    if (k + need > len) dst[k] = 0;
  }
}
void logEv(uint8_t kind, const String &uid, const String &who) {
  Ev &e = evLog[evNext % LOG_N];
  e.id = evNext++; e.ms = millis(); e.kind = kind;
  copyUtf8(e.uid, uid, sizeof(e.uid));
  copyUtf8(e.who, who, sizeof(e.who));
}

// ===== Cua (servo quay tu tu, khong chan chuong trinh) =====
int servoPos = SERVO_CLOSED, servoTarget = SERVO_CLOSED;
unsigned long servoStepT = 0, doorCloseAt = 0, servoIdleT = 0;
bool servoAttached = false;

// ===== Rung =====
int vibLastLevel = 0, hitCount = 0;
unsigned long firstHitT = 0, lastHitT = 0, vibIgnoreUntil = 0, knockAt = 0;

const char* stateName(DoorState s) {
  switch (s) {
    case D_IDLE:   return "đang chờ, chưa có ai đến gần";
    case D_READY:  return "có người đến gần, sẵn sàng đọc thẻ";
    case D_GREET:  return "vừa mở cửa cho người dùng";
    case D_OWNER:  return "đang chào chủ nhà";
    case D_GRANT:  return "đang ở chế độ cấp quyền";
    case D_DENIED: return "vừa từ chối một thẻ";
    case D_LOCKED: return "đang KHÓA TẠM";
    case D_ALARM:  return "ĐANG BÁO ĐỘNG";
  }
  return "?";
}
unsigned long lastAct = 0;      // lan cuoi co hoat dong (de robot buon ngu / ngu)
void setState(DoorState s) { state = s; stateT = millis(); lastAct = stateT; }

// =====================================================================
//                       TELEGRAM (chay o nhan 0)
// =====================================================================
struct Msg { char text[320]; };
QueueHandle_t msgQ = nullptr;
WiFiMulti wifiMulti;
volatile bool reqOpen = false, reqSilence = false, reqStatus = false, reqList = false;
long tgOffset = 0;
unsigned long pendingOpenT = 0;

String nowStr() {
  struct tm t;
  if (!getLocalTime(&t, 0)) return String("chưa có giờ");
  char buf[24];
  strftime(buf, sizeof(buf), "%H:%M:%S %d/%m/%Y", &t);
  return String(buf);
}

void notify(const String &s) {                  // dua tin vao hang doi, cat dung ranh gioi UTF-8
  if (!msgQ) return;
  Msg m;
  strlcpy(m.text, s.c_str(), sizeof(m.text));
  if (s.length() >= sizeof(m.text)) {
    int n = strlen(m.text), k = n - 1;
    while (k > 0 && ((uint8_t)m.text[k] & 0xC0) == 0x80) k--;
    uint8_t lead = (uint8_t)m.text[k];
    int len = (lead >= 0xF0) ? 4 : (lead >= 0xE0) ? 3 : (lead >= 0xC0) ? 2 : 1;
    if (k + len > n) m.text[k] = 0;
  }
  xQueueSend(msgQ, &m, 0);
}

enum { EV_DENY, EV_ALARM, EV_LOCK, EV_COUNT };
unsigned long lastSent[EV_COUNT] = {0, 0, 0};
bool sentOnce[EV_COUNT] = {false, false, false};
void notifyLimited(int ev, unsigned long minMs, const String &s) {
  unsigned long now = millis();
  if (sentOnce[ev] && now - lastSent[ev] < minMs) return;
  sentOnce[ev] = true; lastSent[ev] = now;
  notify(s);
}

String jsonEscape(const char* s) {
  String o;
  for (; *s; s++) {
    char c = *s;
    if (c == '"') o += "\\\"";
    else if (c == '\\') o += "\\\\";
    else if (c == '\n') o += "\\n";
    else if ((uint8_t)c < 0x20) o += ' ';
    else o += c;
  }
  return o;
}

bool tgSend(const char* text) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = String("https://api.telegram.org/bot") + BOT_TOKEN + "/sendMessage";
  if (!http.begin(client, url)) return false;
  http.setTimeout(8000);
  http.addHeader("Content-Type", "application/json; charset=utf-8");
  String body = String("{\"chat_id\":\"") + CHAT_ID + "\",\"text\":\"" + jsonEscape(text) + "\"}";
  int code = http.POST(body);
  tgOk = (code == 200);
  if (code != 200) { Serial.print("[TG] gui loi, ma "); Serial.println(code); }
  http.end();
  return code == 200;
}

void tgHandleCommand(String cmd) {
  cmd.trim(); cmd.toLowerCase();
  int at = cmd.indexOf('@');
  if (at > 0) cmd = cmd.substring(0, at);
  if (cmd == "/start" || cmd == "/help") {
    notify("🛡️ Mình là Cerberus, hệ thống canh cửa của bạn.\n\n"
           "/trang_thai – báo cáo nhanh tình hình cửa\n"
           "/danh_sach_the – xem các thẻ được vào nhà\n"
           "/mo_cua – mở cửa từ xa (cần xác nhận)\n"
           "/tat_bao_dong – tắt còi báo động");
  } else if (cmd == "/trang_thai") {
    reqStatus = true;
  } else if (cmd == "/danh_sach_the") {
    reqList = true;
  } else if (cmd == "/mo_cua") {
    pendingOpenT = millis();
    notify("🔐 Gõ /xacnhan trong 30 giây để mở cửa.\nMình không mở cửa chỉ với một lệnh.");
  } else if (cmd == "/xacnhan") {
    if (pendingOpenT && millis() - pendingOpenT < REMOTE_CONFIRM_MS) { pendingOpenT = 0; reqOpen = true; }
    else notify("Chưa có yêu cầu mở cửa nào đang chờ. Gõ /mo_cua trước nhé.");
  } else if (cmd == "/tat_bao_dong") {
    reqSilence = true;
  } else {
    notify("Mình chưa hiểu lệnh này. Gõ /help để xem những gì mình làm được nhé.");
  }
}

String extractNumberAfter(const String &p, const char* key) {
  int i = p.indexOf(key);
  if (i < 0) return "";
  i += strlen(key);
  String r;
  while (i < (int)p.length() && (isDigit(p[i]) || p[i] == '-')) r += p[i++];
  return r;
}

void tgPoll() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = String("https://api.telegram.org/bot") + BOT_TOKEN + "/getUpdates?limit=1&timeout=0&offset=" + String(tgOffset);
  if (!http.begin(client, url)) return;
  http.setTimeout(8000);
  int code = http.GET();
  tgOk = (code == 200);
  if (code == 200) {
    String p = http.getString();
    int u = p.indexOf("\"update_id\":");
    if (u >= 0) {
      tgOffset = p.substring(u + 12).toInt() + 1;
      String chat = extractNumberAfter(p, "\"chat\":{\"id\":");
      int t = p.indexOf("\"text\":\"");
      if (chat == String(CHAT_ID) && t >= 0) {
        t += 8;
        int e = p.indexOf('"', t);
        if (e > t) tgHandleCommand(p.substring(t, e));
      } else Serial.println("[TG] bo qua tin nhan tu nguoi khac hoac khong phai van ban");
    }
  }
  http.end();
}

void tgTask(void*) {
  vTaskDelay(2500 / portTICK_PERIOD_MS);   // doi servo, OLED, RC522 on dinh roi moi bat WiFi (tranh sut ap)
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_11dBm);       // giam cong suat phat WiFi: do hut dong dot ngot, van du song trong nha
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);   // chi dung hotspot SonTung, tu ket noi lai neu bi rot
  bool timeSet = false;
  unsigned long lastPoll = 0;
  for (;;) {
    if (wifiMulti.run(10000) == WL_CONNECTED) {
      if (!timeSet) {
        configTime(7 * 3600, 0, "pool.ntp.org", "time.google.com");
        timeSet = true;
        Serial.print("[WiFi] da ket noi, IP: "); Serial.println(WiFi.localIP());
      }
      Msg m;
      while (xQueueReceive(msgQ, &m, 0) == pdTRUE) tgSend(m.text);
      if (millis() - lastPoll >= TG_POLL_MS) { lastPoll = millis(); tgPoll(); }
    } else {
      Serial.println("[WiFi] chua ket noi duoc: kiem tra ten/mat khau, ESP32 chi bat WiFi 2.4GHz");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// =====================================================================
//                       COI (chay nen theo mau bat/tat)
// =====================================================================
const uint16_t PAT_GRANT[]  = {70, 50, 70, 50, 300};
const uint16_t PAT_DENY[]   = {100, 100, 100, 100, 100};
const uint16_t PAT_LOCK[]   = {500, 150, 500};
const uint16_t PAT_HELLO[]  = {40, 60, 40};
const uint16_t PAT_OWNER[]  = {60, 60, 60, 60, 60, 60, 250};
const uint16_t PAT_ADD[]    = {70, 50, 70, 50, 300};
const uint16_t PAT_REMOVE[] = {250, 100, 60};
const uint16_t PAT_EXIT[]   = {150, 80, 150};
const uint16_t PAT_BELL[]   = {300, 180, 600};          // kinh... coong: bao khach biet da bao vao trong nha
const uint16_t PAT_GUIDE[]  = {120};                    // tit moi 2 giay khi mo cua thoat hiem: chi duong ra cua
const uint16_t* bzPat = nullptr;
uint8_t bzLen = 0, bzIdx = 0;
unsigned long bzT = 0;
void bzStart(const uint16_t* p, uint8_t n) { bzPat = p; bzLen = n; bzIdx = 0; bzT = millis(); digitalWrite(PIN_BUZZER, HIGH); }
void bzUpdate() {
  if (!bzPat) return;
  if (millis() - bzT >= bzPat[bzIdx]) {
    bzIdx++; bzT = millis();
    if (bzIdx >= bzLen) { bzPat = nullptr; digitalWrite(PIN_BUZZER, LOW); }
    else digitalWrite(PIN_BUZZER, (bzIdx % 2 == 0) ? HIGH : LOW);
  }
}

// =====================================================================
//                       CUA: SERVO QUAY TU TU
// =====================================================================
void servoWrite(int deg) { doorServo.write(SERVO_REVERSE ? 180 - deg : deg); }   // moi lenh quay servo deu qua day
void servoGo(int target) {
  servoTarget = target;
  if (!servoAttached) { doorServo.attach(PIN_SERVO, 500, 2400); servoAttached = true; }
  vibIgnoreUntil = millis() + VIB_SERVO_IGNORE_MS + abs(target - servoPos) * SERVO_STEP_MS;
}
bool fireOpen = false;                 // dang mo cua thoat hiem vi chay
unsigned long fireCloseAt = 0;
void openDoor(unsigned long ms) { servoGo(SERVO_OPEN); doorCloseAt = fireOpen ? 0 : millis() + ms; }
void closeDoorNow() { if (fireOpen) return; servoGo(SERVO_CLOSED); doorCloseAt = 0; }
void ardSend(const String &s);
void servoUpdate() {
  unsigned long now = millis();
  if (doorCloseAt && now >= doorCloseAt) closeDoorNow();
  if (servoPos != servoTarget) {
    if (now - servoStepT >= SERVO_STEP_MS) {
      servoStepT = now;
      servoPos += (servoTarget > servoPos) ? 1 : -1;
      servoWrite(servoPos);
      servoIdleT = now;
    }
  } else if (servoAttached && now - servoIdleT > 400) {
    doorServo.detach(); servoAttached = false;            // nha servo: het rung, do ton dien
  }
}
bool doorIsOpen() { return servoPos != SERVO_CLOSED || servoTarget != SERVO_CLOSED; }

// =====================================================================
//                       THE: LUU / DOC / TIM
// =====================================================================
void saveMembers() {
  prefs.begin("door", false);
  prefs.putUChar("n", (uint8_t)memCount);
  for (int i = 0; i < memCount; i++) {
    char k[5];
    snprintf(k, sizeof(k), "u%d", i); prefs.putString(k, memUid[i]);
    snprintf(k, sizeof(k), "m%d", i); prefs.putString(k, memName[i]);
  }
  prefs.end();
  cardsVer++;
}
void loadMembers() {
  prefs.begin("door", false);
  if (!prefs.getBool("seeded", false)) {               // lan chay dau tien: them the thanh vien co san
    prefs.putBool("seeded", true);
    prefs.putUChar("n", 1); prefs.putString("u0", SEED_UID); prefs.putString("m0", SEED_NAME);
  }
  memCount = min((int)prefs.getUChar("n", 0), MAX_MEMBERS);
  for (int i = 0; i < memCount; i++) {
    char k[5];
    snprintf(k, sizeof(k), "u%d", i); memUid[i] = prefs.getString(k, "");
    snprintf(k, sizeof(k), "m%d", i); memName[i] = prefs.getString(k, "");
    if (memName[i].length() == 0) memName[i] = "Thanh vien " + String(i + 1);
  }
  sysLock = prefs.getBool("lock", false);
  prefs.end();
}
void saveSysLock() { prefs.begin("door", false); prefs.putBool("lock", sysLock); prefs.end(); }
int findMember(const String &uid) { for (int i = 0; i < memCount; i++) if (memUid[i] == uid) return i; return -1; }
bool isOwner(const String &uid) { return uid.equals(OWNER_UID); }
String nextMemberName() {
  for (int n = 1; n < 100; n++) {
    String s = "Thanh vien " + String(n);
    bool used = false;
    for (int i = 0; i < memCount; i++) if (memName[i] == s) used = true;
    if (!used) return s;
  }
  return "Thanh vien";
}

bool readCard(String &uid) {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return false;
  uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (i) uid += ':';
    if (rfid.uid.uidByte[i] < 0x10) uid += '0';
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  return true;
}

void updateDistance() {
  if (millis() - lastDistT < 100) return;
  lastDistT = millis();
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long dur = pulseIn(PIN_ECHO, HIGH, 25000);
  distanceCm = dur ? dur * 0.0343f / 2.0f : 999;
}

// =====================================================================
//                       HANH DONG
// =====================================================================
// remoteSrc = nullptr khi mo bang the; "Telegram" / "trang web" khi mo tu xa
void grantDoor(const String &name, const char* remoteSrc) {
  failCount = 0;
  openDoor(OPEN_MS);
  bzStart(PAT_GRANT, 5);
  greetText = remoteSrc ? String("Mo tu xa") : String("Xin chao ") + name;
  setState(D_GREET);
  if (remoteSrc) {
    Serial.println(String("-> MO CUA TU XA qua ") + remoteSrc);
    notify(String("🔓 Đã mở cửa từ xa qua ") + remoteSrc + ". Cửa tự đóng sau 5 giây.\n🕐 " + nowStr());
  } else {
    ardSend("ENTRY," + name.substring(0, 8));
    Serial.println("-> MO CUA cho " + name);
    notify(String("✅ Đã mở cửa cho ") + name + ". Mời vào nhé!\n🕐 " + nowStr());
  }
}

void ownerArrive() {
  failCount = 0;
  openDoor(OPEN_MS);
  bzStart(PAT_OWNER, 7);
  setState(D_OWNER);
  ardSend("ENTRY,Chu nha");
  Serial.println("-> CHU NHA: mo cua, sap vao che do cap quyen");
  notify(String("👑 Chủ nhà đã về. Cửa đã mở, chế độ cấp quyền bật trong 15 giây.\n🕐 ") + nowStr());
}

void lockDown() {
  bzStart(PAT_LOCK, 3);
  setState(D_LOCKED);
  logEv(K_LOCKED, "", String("Khóa ") + (LOCK_MS / 1000) + " giây sau " + MAX_FAILS + " lần sai");
  Serial.println("-> KHOA TAM (quet sai qua nhieu lan)");
  notifyLimited(EV_LOCK, 60000, String("🔒 Quẹt sai ") + MAX_FAILS + " lần liên tiếp nên mình tạm khóa đầu đọc thẻ 30 giây.\n"
                "Thẻ chủ nhà mở khóa được ngay.\n🕐 " + nowStr());
}

void denyDoor(const String &uid) {
  failCount++;
  logEv(K_UNKNOWN, uid, "Không có trong danh sách");
  Serial.print("Sai lan thu "); Serial.println(failCount);
  notifyLimited(EV_DENY, 10000, String("⚠️ Thẻ lạ ") + uid + " vừa thử quẹt (lần " + failCount + "/" + MAX_FAILS + ").\n"
                "Cửa vẫn đóng, mình đã ghi lại.\n🕐 " + nowStr());
  if (failCount >= MAX_FAILS) { lockDown(); return; }
  bzStart(PAT_DENY, 5);
  setState(D_DENIED);
}

void triggerAlarm() {
  bzPat = nullptr;
  closeDoorNow();
  setState(D_ALARM);
  hitCount = 0;
  logEv(K_INTRUSION, "", "Rung mạnh ở cửa (SW-420)");
  Serial.println("!!! CANH BAO XAM NHAP !!! (quet the chu nha/thanh vien hoac /tat_bao_dong de tat)");
  notifyLimited(EV_ALARM, 60000, String("🚨 Phát hiện va đập mạnh vào cửa!\nMình đã đóng cửa và bật còi.\n"
                "Nhắn /tat_bao_dong khi bạn đã kiểm tra an toàn.\n🕐 ") + nowStr());
}

void stopAlarm(const String &why) {
  logEv(K_SILENCE, "", "Tắt báo động · " + why);
  digitalWrite(PIN_BUZZER, LOW);
  if (oledOk) oled.invertDisplay(false);
  failCount = 0; hitCount = 0;
  vibIgnoreUntil = millis() + 3000;
  bzStart(PAT_EXIT, 3);
  setState(D_IDLE);
  Serial.println("-> DA TAT CANH BAO");
  notify(String("✅ Đã tắt báo động. Mình vẫn đang canh, mọi thứ ổn.\n🕐 ") + nowStr());
}

void updateVibration() {
  unsigned long now = millis();
  int v = digitalRead(PIN_VIB);
  if (v == 1 && vibLastLevel == 0) {
    bool ignore = (state == D_ALARM || state == D_GRANT || state == D_OWNER || doorIsOpen()
                   || now < vibIgnoreUntil || bzPat != nullptr);
    if (!ignore && now - lastHitT >= VIB_DEBOUNCE_MS) {
      if (hitCount == 0 || now - firstHitT > VIB_WINDOW_MS) { hitCount = 0; firstHitT = now; }
      lastHitT = now;
      hitCount++;
      Serial.print("Go cua lan "); Serial.println(hitCount);
      if (hitCount >= VIB_HITS) triggerAlarm();          // dap 5 lan -> bao dong
    }
  }
  vibLastLevel = v;
  // go 1-4 lan roi ngung -> bao vao trong nha: co nguoi go cua
  if (hitCount > 0 && hitCount < VIB_HITS && now - lastHitT > KNOCK_GAP_MS) {
    int n = hitCount; hitCount = 0;
    if (state != D_ALARM && !doorIsOpen() && (!knockAt || now - knockAt > KNOCK_REPEAT_MS)) {
      knockAt = now;
      ardSend("BELL");                                   // Arduino: LCD "Co nguoi go cua" + chuong
      bzStart(PAT_BELL, 3);                              // coi ngoai cua: bao khach biet da bao vao trong
      logEv(K_HSYS, "", "Có người gõ cửa " + String(n) + " lần · đã báo trong nhà");
    }
  }
}

String cardList() {
  String s = "🪪 Danh sách thẻ\n• Chủ nhà: " + String(OWNER_UID) + "\n";
  if (memCount == 0) s += "• Chưa có thẻ thành viên nào";
  for (int i = 0; i < memCount; i++) s += "• " + memName[i] + ": " + memUid[i] + "\n";
  return s;
}

void handleRemote() {
  if (reqSilence) {
    reqSilence = false;
    if (state == D_ALARM) stopAlarm("Telegram");
    else notify("Hiện không có báo động nào nên mình không cần tắt gì cả.");
  }
  if (reqOpen) {
    reqOpen = false;
    if (state == D_ALARM) notify("Mình đang báo động nên chưa mở cửa. Hãy /tat_bao_dong trước nhé.");
    else if (doorIsOpen()) notify("Cửa đang mở rồi nên mình không cần làm gì thêm.");
    else { logEv(K_REMOTE, "", "Telegram"); grantDoor("", "Telegram"); }
  }
  if (reqStatus) {
    reqStatus = false;
    String s = String("📋 Báo cáo nhanh\n• Hiện tại: ") + stateName(state) + "\n";
    s += String("• Cửa: ") + (doorIsOpen() ? "đang mở" : "đóng") + "\n";
    if (sysLock) s += "• Hệ thống đang KHÓA: thẻ thành viên không mở được cửa\n";
    if (WiFi.status() == WL_CONNECTED) s += "• Trang web: http://" + WiFi.localIP().toString() + "\n";
    s += "• Thẻ thành viên: " + String(memCount) + "\n";
    s += "• Sóng WiFi: " + String(WiFi.RSSI()) + " dBm\n";
    s += "• Giờ: " + nowStr();
    notify(s);
  }
  if (reqList) { reqList = false; notify(cardList()); }
}

// ===== Xu ly the theo trang thai =====
void onCard(const String &uid) {
  Serial.print("The "); Serial.println(uid);
  if (state == D_GRANT) {                               // ---- che do cap quyen
    unsigned long now = millis();
    if (isOwner(uid)) {
      bzStart(PAT_EXIT, 3); setState(D_IDLE);
      Serial.println("-> THOAT CHE DO CAP QUYEN");
      return;
    }
    grantUntil = now + GRANT_MS;                        // moi lan quet tinh lai 15 giay
    int idx = findMember(uid);
    if (idx >= 0) {
      String nm = memName[idx];
      for (int i = idx; i < memCount - 1; i++) { memUid[i] = memUid[i + 1]; memName[i] = memName[i + 1]; }
      memCount--; saveMembers();
      logEv(K_REVOKE, uid, nm + " · thu hồi bởi chủ nhà");
      bzStart(PAT_REMOVE, 3);
      flashFace = 2; flashUntil = now + FLASH_MS; flashText = "DA THU HOI";
      Serial.println("  -> DA THU HOI " + nm);
      notify(String("➖ Đã thu hồi thẻ của ") + nm + " (" + uid + "). Thẻ này không mở được cửa nữa.");
    } else if (memCount >= MAX_MEMBERS) {
      bzStart(PAT_DENY, 5);
      notify("Danh sách thẻ đã đầy 10 thẻ. Hãy thu hồi bớt một thẻ cũ nhé.");
    } else {
      String nm = nextMemberName();
      memUid[memCount] = uid; memName[memCount] = nm; memCount++; saveMembers();
      logEv(K_GRANT, uid, nm + " · cấp bởi chủ nhà");
      bzStart(PAT_ADD, 5);
      flashFace = 1; flashUntil = now + FLASH_MS; flashText = "+ " + nm;
      Serial.println("  -> DA THEM " + nm);
      notify(String("➕ Đã thêm ") + nm + " (" + uid + "). Thẻ này mở được cửa từ bây giờ.");
    }
    return;
  }
  if (state == D_ALARM) {
    if (isOwner(uid)) stopAlarm(String("thẻ ") + OWNER_NAME);
    else if (findMember(uid) >= 0) stopAlarm("thẻ " + memName[findMember(uid)]);
    return;
  }
  if (fireOpen && isOwner(uid)) {                       // chu nha quet the: dong cua thoat hiem
    fireOpen = false; fireCloseAt = 0; closeDoorNow(); bzStart(PAT_EXIT, 3);
    logEv(K_FIREDOOR, uid, "Chủ nhà đóng cửa thoát hiểm"); return;
  }
  if (isOwner(uid)) { logEv(K_OWNER, uid, OWNER_NAME); ownerArrive(); return; }   // chu nha: ca khi dang khoa tam
  if (state == D_LOCKED) { logEv(K_LOCKED, uid, "Quẹt khi đang khóa tạm"); return; }
  int idx = findMember(uid);
  if (idx >= 0 && sysLock) {                             // khoa he thong: thanh vien khong vao duoc
    logEv(K_LOCKED, uid, memName[idx] + " · hệ thống đang khóa");
    bzStart(PAT_DENY, 5);
    setState(D_DENIED);
    Serial.println("-> TU CHOI: he thong dang khoa");
    return;
  }
  if (idx >= 0) { logEv(K_OK, uid, memName[idx]); grantDoor(memName[idx], nullptr); }
  else denyDoor(uid);
}

// =====================================================================
//                OLED: MAT ROBOT KIEU THACH (MOCHI)
// =====================================================================
// Mat la 2 khoi bo tron to (sieu elip), mieng la 1 net cuoi day nho o giua.
// Moi thong so (rong, cao, vat goc, khoet day...) chay bang LO XO hoi long nen chuyen dong nay nhu thach.
// Doi kieu hinh (khoi <-> dau > <, mieng cuoi <-> mieng ha): hinh cu thu lai roi hinh moi bat ra.
// Ve hoan toan bang lenh (khong dung anh), 24 khung hinh/giay.
//   Blob : k (0 khoi, 1 dau > <, 2 mat gian: long may xeo + nua mat rong, 3 mat tron rong, 4 trai tim, 5 mat cuoi nham ∩), x, y tam; w, h;
//          ti/to vat goc tren phia trong/ngoai; bc khoet day (mat cuoi ∩); tc khoet dinh (mat ngu ∪); s cao dau > <
//   Mouth: k (0 net cong, 1 mieng D dac, 2 tron o, 3 rang cua, 4 cuoi ha to co luoi, 5 dau ^); c > 0 cuoi, c < 0 meu
enum FaceId { F_IDLE, F_CURIOUS, F_ATTN, F_HAPPY, F_REMOTE, F_OWNER, F_GUARD, F_EXCITED, F_SAD,
              F_ANGRY, F_LOCKED, F_SHOCKED, F_SLEEPY, F_ASLEEP, F_WAKE };
const float LX = 36, EYE_RX = 92, EYC = 31;             // vi tri do tu anh mau (thu ve 128x64)
const float CURIOUS_CM = 15.0;                          // 5..15 cm: to mo nghieng dau
const unsigned long WAKE_MS = 1180;                     // het khoa: bong bong vo, mo mat chop (28 khung)
const unsigned long FRAME_MS = 42;                      // 24 khung hinh/giay
const int NB = sizeof(Blob) / sizeof(float), NM = sizeof(Mouth) / sizeof(float);

Part pL, pR, pM;
float gX = 0, gY = 0, gVX = 0, gVY = 0, gZ = 1, gVZ = 0, lineMul = 1;   // gZ: phong to ca mat (luc gian)
int face = -1, lastFace = -1;
unsigned long faceT = 0, blinkAt = 0, blinkNext = 0, idleNext = 4000, idleUntil = 0, lastFrame = 0, seenFlash = 0, wakeUntil = 0;
int idleSub = 0, lookDir = 1;
bool oledInv = false;

float rnd() { return random(1000) / 1000.0f; }
int R_(float v) { return (int)lroundf(v); }
Blob eyeT(float x) { Blob e = {0, x, EYC, 22, 33, 0, 0, 0, 0, 13}; return e; }
Mouth mouthT() { Mouth m = {0, 64, 45, 10, 4, 9, 3.5f, 4, 2}; return m; }

// ---------- ve co ban ----------
void fillQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, uint16_t c) {
  oled.fillTriangle(R_(x0), R_(y0), R_(x1), R_(y1), R_(x2), R_(y2), c);
  oled.fillTriangle(R_(x0), R_(y0), R_(x2), R_(y2), R_(x3), R_(y3), c);
}
void thickLine(float x0, float y0, float x1, float y1, float w, uint16_t c) {   // net day dau tron
  w *= lineMul;
  float dx = x1 - x0, dy = y1 - y0, L = sqrtf(dx * dx + dy * dy);
  if (L > .01f) { float nx = -dy / L * w / 2, ny = dx / L * w / 2; fillQuad(x0 + nx, y0 + ny, x1 + nx, y1 + ny, x1 - nx, y1 - ny, x0 - nx, y0 - ny, c); }
  int r = w > 6 ? 3 : w > 4 ? 2 : 1; oled.fillCircle(R_(x0), R_(y0), r, c); oled.fillCircle(R_(x1), R_(y1), r, c);
}
void span(float xl, float xr, int row, uint16_t c) { int a = R_(xl), b = R_(xr); if (b >= a) oled.drawFastHLine(a, row, b - a + 1, c); }
void ellipseArc(float cx, float cy, float rx, float ry, float rot, float a0, float a1, float w) {   // net elip (co the xoay), tu goc a0 den a1
  float cr = cosf(rot), sr = sinf(rot), px = 0, py = 0; int n = 20;
  for (int i = 0; i <= n; i++) {
    float a = a0 + (a1 - a0) * i / n, ex = cosf(a) * rx, ey = sinf(a) * ry, x = cx + ex * cr - ey * sr, y = cy + ex * sr + ey * cr;
    if (i) thickLine(px, py, x, y, w, SSD1306_WHITE);
    px = x; py = y;
  }
}
void ptext(const String &s, int x, int y) { oled.setTextSize(1); oled.setTextColor(SSD1306_WHITE); oled.setCursor(x, y); oled.print(s); }
void ctext(String s, int y) { if (s.length() > 21) s = s.substring(0, 21); ptext(s, (128 - (int)s.length() * 6 + 1) / 2, y); }

// ---------- mat ----------
void heartShape(float x, float y, float s) {           // trai tim dac, rong s
  int r = R_(s * .28f);
  oled.fillCircle(R_(x - s * .24f), R_(y - s * .12f), r, SSD1306_WHITE); oled.fillCircle(R_(x + s * .24f), R_(y - s * .12f), r, SSD1306_WHITE);
  oled.fillTriangle(R_(x - s * .5f), R_(y - s * .04f), R_(x + s * .5f), R_(y - s * .04f), R_(x), R_(y + s * .5f), SSD1306_WHITE);
}
void drawBlob(const Blob &e, int side) {                // side = +1 mat trai (phia trong o ben phai)
  if (e.k > .5f && e.k < 1.5f) {                       // dau > < (cuoi to)
    float s = e.s, d = side, hw = e.w / 2;
    thickLine(e.x - d * hw, e.y - s, e.x + d * hw, e.y, 4.5f, SSD1306_WHITE);
    thickLine(e.x + d * hw, e.y, e.x - d * hw, e.y + s, 4.5f, SSD1306_WHITE);
    return;
  }
  if (e.k > 1.5f && e.k < 2.5f) {                       // gian: long may xeo xuong phia trong + nua mat rong ben duoi
    float r = side * .38f, ux = cosf(r), uy = sinf(r);
    thickLine(e.x - side * ux * e.w * .55f, e.y - side * uy * e.w * .55f, e.x + side * ux * e.w * .85f, e.y + side * uy * e.w * .85f, 3, SSD1306_WHITE);
    ellipseArc(e.x, e.y, e.w / 2, e.h, r, 0, PI, 3);
    return;
  }
  if (e.k > 4.5f) {                                     // mat cuoi nham hinh ∩ (2 chan thang + vom tron)
    float r = e.w / 2, tp = e.y - e.h / 2 + r, bt = e.y + e.h / 2;
    thickLine(e.x - r, bt, e.x - r, tp, 4.5f, SSD1306_WHITE); thickLine(e.x + r, bt, e.x + r, tp, 4.5f, SSD1306_WHITE);
    ellipseArc(e.x, tp, r, r, 0, PI, 2 * PI, 4.5f); return;
  }
  if (e.k > 3.5f) { heartShape(e.x, e.y, e.w); return; }                                // mat trai tim (chu nha)
  if (e.k > 2.5f) { ellipseArc(e.x, e.y, e.w / 2, e.h / 2, 0, 0, 2 * PI, 3); return; }   // mat tron rong
  float w = max(2.0f, e.w), h = max(2.0f, e.h), a = w / 2, b = h / 2;
  float cbY = e.y + h * .95f - h * .55f * e.bc, ctY = e.y - h * .95f + h * .55f * e.tc, crx = w * .62f, cry = h * .62f;
  for (int row = (int)ceilf(e.y - b - .5f); row <= (int)floorf(e.y + b - .5f); row++) {
    float yy = (row + .5f - e.y) / b; yy = fmaxf(-1.0f, fminf(1.0f, yy));
    float hw = a * powf(1 - powf(fabsf(yy), 3.4f), 1 / 3.4f);
    if (yy > 0) hw *= 1 - .07f * yy;                    // day hoi thon hon dinh, giong anh mau
    float segL[3], segR[3]; int n = 1; segL[0] = e.x - hw; segR[0] = e.x + hw;
    for (int k = 0; k < 2; k++) {                       // khoet hinh elip (mat cuoi / mat ngu)
      float amt = k ? e.tc : e.bc, cy = k ? ctY : cbY;
      if (amt < .01f) continue;
      float q = (row + .5f - cy) / cry; if (fabsf(q) >= 1) continue;
      float cw = crx * sqrtf(1 - q * q), cl = e.x - cw, cr = e.x + cw;
      int m = 0; float nL[3], nR[3];
      for (int i = 0; i < n; i++) {
        if (cr <= segL[i] || cl >= segR[i]) { nL[m] = segL[i]; nR[m++] = segR[i]; continue; }
        if (cl > segL[i]) { nL[m] = segL[i]; nR[m++] = cl - 1; }
        if (cr < segR[i] && m < 3) { nL[m] = cr + 1; nR[m++] = segR[i]; }
      }
      n = m; for (int i = 0; i < n; i++) { segL[i] = nL[i]; segR[i] = nR[i]; }
    }
    for (int i = 0; i < n; i++) span(segL[i], segR[i], row, SSD1306_WHITE);
  }
  float top = e.y - h / 2, inX = e.x + side * w / 2, outX = e.x - side * w / 2;   // vat goc tren (chau may / cup mat)
  if (e.ti > .01f) oled.fillTriangle(R_(inX + side * 2), R_(top - 2), R_(inX + side * 2), R_(top + e.ti * h), R_(outX - side * 2), R_(top - 2), SSD1306_BLACK);
  if (e.to > .01f) oled.fillTriangle(R_(outX - side * 2), R_(top - 2), R_(outX - side * 2), R_(top + e.to * h), R_(inX + side * 2), R_(top - 2), SSD1306_BLACK);
}

// ---------- mieng ----------
void drawMouth(const Mouth &m) {
  if (m.k < .5f) {                                      // net cong: cuoi / meu / gach
    float px = 0, py = 0;
    for (int i = 0; i <= 10; i++) {
      float t = i / 10.0f, u = 1 - t;
      float x = u * u * (m.x - m.w) + 2 * u * t * m.x + t * t * (m.x + m.w);
      float y = u * u * (m.y - m.c * .5f) + 2 * u * t * (m.y + m.c * 1.5f) + t * t * (m.y - m.c * .5f);
      if (i) thickLine(px, py, x, y, 3.6f, SSD1306_WHITE);
      px = x; py = y;
    }
  } else if (m.k < 1.5f) {                              // mieng ha (hinh chu D)
    float top = m.y - m.h * .4f, hh = m.h;
    for (int row = R_(top); row <= R_(top + hh); row++) {
      float q = (row - top) / hh; if (q < 0 || q > 1) continue;
      float hw = m.w * sqrtf(max(0.0f, 1 - q * q));
      span(m.x - hw, m.x + hw, row, SSD1306_WHITE);
    }
  } else if (m.k < 2.5f) {                              // mieng tron o
    ellipseArc(m.x, m.y, m.rx, m.ry, 0, 0, 2 * PI, 3);
  } else if (m.k > 3.5f && m.k < 4.5f) {                // cuoi ha to: vien chu U co luoi ben trong
    float top = m.y - m.h * .4f;
    thickLine(m.x - m.w, top, m.x + m.w, top, 3.5f, SSD1306_WHITE);
    ellipseArc(m.x, top, m.w, m.h, 0, 0, PI, 3.5f);
    float ty = top + m.h - 1.5f, trx = m.w * .5f, tr = m.h * .32f;   // luoi: nua elip dac
    for (int row = R_(ty - tr); row <= R_(ty); row++) { float q = (row - ty) / tr; if (fabsf(q) <= 1) { float hw = trx * sqrtf(1 - q * q); span(m.x - hw, m.x + hw, row, SSD1306_WHITE); } }
  } else if (m.k > 4.5f) {                              // dau ^ khi gian
    thickLine(m.x - m.w, m.y + m.h / 2, m.x, m.y - m.h / 2, 3, SSD1306_WHITE);
    thickLine(m.x, m.y - m.h / 2, m.x + m.w, m.y + m.h / 2, 3, SSD1306_WHITE);
  } else {                                              // rang cua
    for (int i = 0; i < 4; i++) {
      float x0 = m.x - m.w + i * m.w / 2, x1 = x0 + m.w / 2;
      thickLine(x0, m.y + (i % 2 ? m.a : -m.a), x1, m.y + (i % 2 ? -m.a : m.a), 3, SSD1306_WHITE);
    }
  }
}

// ---------- phu kien nho ----------
void sparkle(int x, int y, float f) { const int rr[4] = {1, 3, 2, 0}; int r = rr[((int)f) & 3]; if (r) { oled.drawFastHLine(x - r, y, 2 * r + 1, SSD1306_WHITE); oled.drawFastVLine(x, y - r, 2 * r + 1, SSD1306_WHITE); } }
void cardIcon(int x, int y, bool cross) {
  oled.drawRect(x, y, 20, 14, SSD1306_WHITE); oled.fillRect(x + 3, y + 4, 5, 4, SSD1306_WHITE);
  oled.drawFastHLine(x + 10, y + 4, 7, SSD1306_WHITE); oled.drawFastHLine(x + 10, y + 7, 5, SSD1306_WHITE);
  if (cross) { thickLine(x - 1, y - 1, x + 21, y + 15, 2, SSD1306_WHITE); thickLine(x + 21, y - 1, x - 1, y + 15, 2, SSD1306_WHITE); }
}
void crown(int x, int y, int w, int h) {
  oled.fillRect(x - w / 2, y + h * 2 / 3, w, h / 3 + 1, SSD1306_WHITE);
  oled.fillTriangle(x - w / 2, y + h, x - w / 2, y + 1, x - w / 4, y + h, SSD1306_WHITE);
  oled.fillTriangle(x - w / 4, y + h, x, y, x + w / 4, y + h, SSD1306_WHITE);
  oled.fillTriangle(x + w / 4, y + h, x + w / 2, y + 1, x + w / 2, y + h, SSD1306_WHITE);
}
void phoneIcon(int x, int y) { oled.drawRect(x, y, 12, 19, SSD1306_WHITE); oled.fillRect(x + 3, y + 3, 6, 10, SSD1306_WHITE); oled.drawFastHLine(x + 4, y + 15, 4, SSD1306_WHITE); }
void lockIcon(int x, int y, bool open) {
  oled.fillCircle(x + (open ? 4 : 0), y + 7, 5, SSD1306_WHITE); oled.fillCircle(x + (open ? 4 : 0), y + 7, 3, SSD1306_BLACK);
  oled.fillRect(x - 7, y + 7, 14, 11, SSD1306_WHITE);
}
void vein(float x, float y, float s) {                  // dau gian (gan noi)
  for (int q = 0; q < 4; q++) { float a = q & 1 ? 1 : -1, b = q & 2 ? 1 : -1;
    thickLine(x + a * s * 1.7f, y + b * s * .45f, x + a * s * .45f, y + b * s * .45f, 2, SSD1306_WHITE);
    thickLine(x + a * s * .45f, y + b * s * .45f, x + a * s * .45f, y + b * s * 1.7f, 2, SSD1306_WHITE); }
}
void tearDrop(int x, int y) { oled.fillCircle(x, y + 1, 2, SSD1306_WHITE); oled.fillTriangle(x - 2, y, x + 2, y, x, y - 4, SSD1306_WHITE); }

// ---------- chon bieu cam theo trang thai cua ----------
int pickFace(unsigned long now) {
  if (state == D_ALARM || fireOpen) return F_SHOCKED;
  if (flashFace && now < flashUntil) return flashFace == 1 ? F_EXCITED : F_SAD;
  switch (state) {
    case D_READY:  return F_ATTN;
    case D_GREET:  return greetText == "Mo tu xa" ? F_REMOTE : F_HAPPY;
    case D_OWNER:  return F_OWNER;
    case D_GRANT:  return F_GUARD;
    case D_DENIED: return F_ANGRY;
    case D_LOCKED: return F_LOCKED;
    default: break;
  }
  if (distanceCm <= CURIOUS_CM) { lastAct = now; return F_CURIOUS; }     // co nguoi dang toi (5..15 cm)
  return F_IDLE;                                         // khong ai: van vui ve (khong con buon ngu)
}

// Cuoi to theo mau: ma day mat len -> hip thanh vach cong -> bat thanh > < va mieng ha cuoi ha ha -> hip lai -> mat thuong
void laughFace(long e, long endAt, Blob &L, Blob &R, Mouth &M, float &gy) {
  if (e < 160) { L.bc = R.bc = .55f; L.h = R.h = 28; L.y = R.y = 33; M.w = 9; M.c = 2; }
  else if (e < 340) { L.w = R.w = 30; L.h = R.h = 10; L.tc = R.tc = 1; L.y = R.y = 34; M.w = 7; M.c = 2.5f; }
  else if (e < endAt) {
    L.k = R.k = 1; L.x = 33; R.x = 95; L.y = R.y = 24; L.w = R.w = 28; L.s = R.s = 11;
    M.k = 4; M.w = 12; M.h = 15 + sinf(e / 70.0f) * 2; M.y = 40; gy = -fabsf(sinf(e / 170.0f)) * 2;
  }
  else if (e < endAt + 250) { L.w = R.w = 30; L.h = R.h = 10; L.tc = R.tc = 1; L.y = R.y = 34; M.w = 8; M.c = 2; }
  else { M.w = 11; M.c = 5.5f; }
}
// CHU NHA: mat det quet the -> giat minh nhan ra -> mat trai tim dap thinh thich, vuong mien roi xuong -> nhay mat
void ownerFace(long e, Blob &L, Blob &R, Mouth &M, float &gx) {
  if (e < 800) { L.w = R.w = 28; L.h = R.h = 6; L.y = R.y = 32; gx = sinf(e / 130.0f) * 6; M.w = 6; M.c = 0; M.y = 47; }
  else if (e < 1000) { L.w = R.w = 25; L.h = R.h = 38; L.y = R.y = 29; M.k = 2; M.rx = 4; M.ry = 5; M.y = 47; }
  else if (e < 2700) { long b = (e - 1000) % 900; bool beat = b < 130 || (b > 240 && b < 370);
    L.k = R.k = 4; L.w = R.w = 23 + (beat ? 5 : 0); L.y = R.y = 28; M.k = 1; M.w = 7; M.h = 8; M.y = 47; }
  else { L.k = R.k = 5; L.w = R.w = 24; L.h = R.h = 22; L.y = R.y = 27; L.bc = R.bc = L.tc = R.tc = 0; M.k = 1; M.w = 10; M.h = 11; M.y = 43; }
}
// THEM THE: the roi xuong, robot ha mieng "an" the, nhai nhop nhep, roi cuoi hip mat kem +1 lap lanh
void eatFace(long e, Blob &L, Blob &R, Mouth &M, float &gy) {
  if (e < 500) { float p = e / 500.0f; L.h = R.h = 35; L.y = R.y = 28; gy = -1; M.k = 2; M.rx = 3 + p * 2; M.ry = 4 + p * 3; M.y = 46; }
  else if (e < 800) { L.h = R.h = 36; L.y = R.y = 28; M.k = 2; M.rx = 6; M.ry = 7; M.y = 46; }
  else if (e < 1300) { bool c = ((e - 800) / 120) % 2; L.k = R.k = 5; L.w = R.w = 24; L.h = R.h = 20; L.y = R.y = 28;
    M.w = c ? 5 : 8; M.c = c ? 3 : 1; M.y = c ? 47 : 45; gy = c ? 1 : -1; }
  else { L.k = R.k = 5; L.w = R.w = 24; L.h = R.h = 22; L.y = R.y = 27; L.bc = R.bc = L.tc = R.tc = 0; M.k = 1; M.w = 11; M.h = 12; M.y = 43; gy = e < 1500 ? -3 : 0; }
}
void cardScaled(float cx, float cy, float sc) {         // the thu nho duoc (tam cx, cy)
  if (sc < .08f) return; int w = R_(20 * sc), h = R_(14 * sc);
  oled.drawRect(R_(cx - w / 2.0f), R_(cy - h / 2.0f), w, h, SSD1306_WHITE);
  if (sc > .5f) oled.fillRect(R_(cx - w / 2.0f + 3 * sc), R_(cy - h / 2.0f + 4 * sc), R_(5 * sc), R_(4 * sc), SSD1306_WHITE);
}
// Gian theo mau: mat nghieng meu -> mat rong + long may xeo + dau gan noi -> phong to rung lac -> nham "-_-"
int angryPhase = 0;
void angryFace(long e, Blob &L, Blob &R, Mouth &M, float &gx, float &gy, float &z) {
  if (e < 250) { angryPhase = 0; L.ti = R.ti = .3f; L.h = R.h = 30; L.y = R.y = 32; M.w = 7; M.c = -3; M.y = 47; return; }
  if (e < 1800) {
    L.k = R.k = 2; L.w = R.w = 22; L.h = R.h = 9; L.y = R.y = 30; M.k = 5; M.w = 4; M.h = 4; M.y = 45; gx = sinf(e / 40.0f);
    angryPhase = 1;
    if (e >= 1100) { angryPhase = 2; z = 1.8f; gx = sinf(e / 25.0f) * 3; gy = cosf(e / 31.0f) * 2; }
    return;
  }
  angryPhase = 3; L.w = R.w = 24; L.h = R.h = 3; L.y = R.y = 33; M.w = 6; M.c = 0; M.y = 47;
}

// ---------- hinh muc tieu cua tung bieu cam (theo thoi gian e ke tu luc bat dau) ----------
float faceTargets(int f, unsigned long e, unsigned long now, Blob &L, Blob &R, Mouth &M, float &gx, float &gy) {
  L = eyeT(LX); R = eyeT(EYE_RX); M = mouthT(); gx = 0; gy = sinf(now / 900.0f) * 1.2f;
  float z = 1;
  switch (f) {
    case F_IDLE: {
      if (now > idleNext) {
        float r = rnd(); idleSub = r < .35f ? 1 : r < .6f ? 3 : r < .9f ? 4 : 0;   // liec / nhun / cuoi nham mat (bo kieu luoi)
        lookDir = random(2) ? 1 : -1; idleUntil = now + 1500 + random(1300); idleNext = idleUntil + 2200 + random(2800);
      }
      int s = now < idleUntil ? idleSub : 0;
      if (s == 1) { gx = lookDir * 9; (lookDir < 0 ? L : R).h = 35; (lookDir < 0 ? R : L).h = 31; M.x += lookDir * 3; }   // liec
      else if (s == 2) { L.w = R.w = 27; L.h = R.h = 11; L.y = R.y = 34; M.w = 6; M.c = -1.6f; M.y = 47; }                   // luoi
      else if (s == 3) { bool q = (now / 260) % 2; L.w = R.w = q ? 25 : 20; L.h = R.h = q ? 29 : 36; L.y = R.y = q ? 33 : 30; } // nhun
      else if (s == 4) { L.k = R.k = 5; L.w = R.w = 24; L.h = R.h = 22; L.y = R.y = 27; L.bc = R.bc = L.tc = R.tc = 0; M.k = 1; M.w = 10; M.h = 11; M.y = 43; }   // cuoi nham mat ∩ ∩
      break; }
    case F_CURIOUS: L.w = 24; L.h = 36; L.y = 29; R.w = 20; R.h = 25; R.y = 34; M.x = 66; M.w = 10; M.c = 5; gx = -3; gy = -fabsf(sinf(now / 300.0f)) * 1.5f; break;   // to mo ma vui
    case F_ATTN:
      if (e < 700) { L.w = R.w = 25; L.h = R.h = 38; L.y = R.y = 29; M.k = 1; M.w = 8; M.h = 8; M.y = 44; }   // mat to, cuoi ha nhe
      else if (e < 1900) { L.w = R.w = 24; L.h = R.h = 28; L.y = R.y = 36; M.w = 6; M.c = 0; M.y = 49; }
      else { M.w = 11; M.c = 5; }
      break;
    case F_HAPPY: laughFace(e, 2000, L, R, M, gy); break;
    case F_REMOTE: L.k = R.k = 5; L.w = R.w = 24; L.h = R.h = 22; L.y = R.y = 27; L.bc = R.bc = L.tc = R.tc = 0; M.k = 1; M.w = 10; M.h = 11; M.y = 43; break;
    case F_OWNER:
      ownerFace(e, L, R, M, gx); break;
    case F_GUARD: L.ti = R.ti = L.to = R.to = .2f; L.h = R.h = 30; L.y = R.y = 32; M.w = 7; M.c = 0; M.y = 48; break;
    case F_EXCITED: eatFace(e, L, R, M, gy); break;
    case F_SAD: L.to = R.to = .42f; L.h = R.h = 29; L.y = R.y = 34; M.w = 8; M.c = -3.5f; M.y = 50; gy = 1.5f; break;
    case F_ANGRY: angryFace(e, L, R, M, gx, gy, z); break;
    case F_LOCKED: L.w = R.w = 26; L.h = R.h = 4; L.y = R.y = 33; M.w = 5; M.c = 0; M.y = 48; break;
    case F_SHOCKED: L.k = R.k = 3; L.w = R.w = 20; L.h = R.h = 28; L.y = R.y = 28; M.k = 2; M.rx = 6; M.ry = 7; M.y = 46; gx = sinf(now / 30.0f) * 2.5f; break;
    case F_WAKE: L.w = R.w = 25; L.h = R.h = 38; L.y = R.y = 29; M.k = 2; M.rx = 4; M.ry = 5; M.y = 47; break;
    case F_SLEEPY: L.w = R.w = 27; L.h = R.h = 11; L.y = R.y = 35; M.w = 5; M.c = -1.2f; M.y = 48; gy = sinf(now / 1400.0f) * 1.5f; break;
    case F_ASLEEP: L.tc = R.tc = 1; L.w = R.w = 24; L.h = R.h = 22; L.y = R.y = 38; M.w = 4; M.c = 0; M.y = 50; gy = sinf(now / 1400.0f) * 2; break;
  }
  return z;
}

// ---------- lo xo + doi kieu hinh ----------
const float SPRING_K = 320, SPRING_D = 2 * 17.888f * .42f;      // he so tat dan thap -> nay nhu thach
void partStep(Part &P, const float *tg, int n, float dt) {
  if (!P.init) { for (int i = 0; i < n; i++) { P.p[i] = tg[i]; P.v[i] = 0; } P.sq = 1; P.init = true; P.sw = false; return; }
  if (P.sw || fabsf(P.p[0] - tg[0]) > .5f) {              // kieu hinh khac: thu nho -> doi -> bat ra (co nay qua da)
    if (!P.sw) { P.sw = true; P.ph = 0; }
    for (int i = 0; i < n; i++) P.nx[i] = tg[i];
    P.ph += dt;
    if (P.ph < 100) P.sq = 1 - P.ph / 100;
    else {
      for (int i = 0; i < n; i++) { P.p[i] = P.nx[i]; P.v[i] = 0; }
      float q = min(1.0f, (P.ph - 100) / 200), s = 2.2f; q -= 1; P.sq = q * q * ((s + 1) * q + s) + 1;
      if (P.ph >= 300) { P.sw = false; P.sq = 1; }
    }
    return;
  }
  float s = min(dt, 40.0f) / 1000.0f;
  for (int i = 1; i < n; i++) { P.v[i] += ((tg[i] - P.p[i]) * SPRING_K - P.v[i] * SPRING_D) * s; P.p[i] += P.v[i] * s; }
}
Blob scaledBlob(const Part &P) { Blob b; memcpy(&b, P.p, sizeof(Blob)); b.w *= P.sq; b.h *= P.sq; b.s *= P.sq; return b; }
Mouth scaledMouth(const Part &P) { Mouth m; memcpy(&m, P.p, sizeof(Mouth)); m.w *= P.sq; m.c *= P.sq; m.h *= P.sq; m.rx *= P.sq; m.ry *= P.sq; m.a *= P.sq; return m; }

// ---------- bieu cam anh (faces.h) ----------
// So thu tu khung goc -> khung trong faces.h (-1 neu khong co)
int faceSlot(int n) {
  if (n >= 151 && n <= 271) return n - 151;
  if (n >= 490 && n <= 513) return 121 + n - 490;
  return -1;
}
// Bieu cam nao dung anh, khung nao (24 khung/giay theo thoi gian e)
int bmpFrame(int f, unsigned long e) {
  int k = e / 42;
  if (f == F_LOCKED) return k < 25 ? 151 + k : 176 + (k - 25) % 69;          // mat khep -> ngu, bong bong mui phong xep (lap)
  if (f == F_WAKE)   return min(244 + k, 271);                                 // bong bong vo -> mo mat, chop chop
  if (f == F_OWNER && e < 2700) {                                              // chop mat -> mat trai tim dap
    if (k < 24) return 490 + k;
    int p = (k - 24) % 18; return p < 9 ? 503 + p : 521 - p;
  }
  return -1;
}
uint8_t faceBuf[1024];
int faceBufN = -1;
void decodeFace(int n) {                                // giai nen RLE 1 khung vao faceBuf
  if (n == faceBufN) return;
  int i = faceSlot(n); if (i < 0) return;
  uint32_t a, b;
  memcpy(&a, FACE_BLOB + 2 + 2 * FACE_N + 4 * i, 4); memcpy(&b, FACE_BLOB + 2 + 2 * FACE_N + 4 * (i + 1), 4);
  const uint8_t *d = FACE_BLOB + FACE_HDR + a, *end = FACE_BLOB + FACE_HDR + b;
  int o = 0;
  while (d < end && o < 1024) {
    uint8_t c = *d++;
    if (c & 0x80) { int r = (c & 0x7F) + 1; uint8_t v = *d++; while (r-- && o < 1024) faceBuf[o++] = v; }
    else { int r = c + 1; while (r-- && o < 1024 && d < end) faceBuf[o++] = *d++; }
  }
  faceBufN = n;
}
String lastBmpKey = "";
void drawBmpFace(int n, unsigned long e, unsigned long now) {
  String txt = "";
  if (face == F_LOCKED) { long left = ((long)LOCK_MS - (long)(now - stateT)) / 1000 + 1; txt = "Khoa the " + String(max(left, 0L)) + "s"; }
  else if (face == F_OWNER && e > 1300) txt = "CHU NHA";
  String key = String(n) + "|" + txt;
  if (key == lastBmpKey && !oledInv) return;            // khung y het lan truoc: khong gui lai OLED (do ton I2C)
  lastBmpKey = key;
  decodeFace(n);
  oled.clearDisplay();
  oled.drawBitmap(0, 0, faceBuf, 128, 64, SSD1306_WHITE);
  if (face == F_LOCKED) lockIcon(116, 1, false);        // o khoa nho o goc, khong che mat
  if (txt.length()) { int w = txt.length() * 6 + 3; oled.fillRect((128 - w) / 2, 54, w, 10, SSD1306_BLACK); ctext(txt, 56); }
  if (oledInv) { oled.invertDisplay(false); oledInv = false; }
  oled.display();
}

// ---------- ve 1 khung hinh ----------
void drawRobot() {
  unsigned long now = millis();
  float dt = lastFrame ? (float)min(100UL, now - lastFrame) : 42; lastFrame = now;

  int f = pickFace(now);
  if (lastFace == F_LOCKED && f != F_LOCKED) { wakeUntil = now + WAKE_MS; lastFace = f; }   // het khoa: thuc day (nho la da thuc, khong lap lai)
  if (now < wakeUntil && (f == F_IDLE || f == F_CURIOUS || f == F_ATTN)) f = F_WAKE;      // quet the / bao dong: phan ung ngay
  if (flashFace && flashUntil != seenFlash && now < flashUntil) { seenFlash = flashUntil; faceT = now; }       // them/thu hoi lien tiep: chay lai
  if (f != face) {
    bool keep = (face == F_WAKE && (f == F_ATTN || f == F_CURIOUS));
    face = f; if (!keep) faceT = now;
  }
  if (f != F_WAKE) lastFace = f;
  unsigned long e = now - faceT;

  int bf = bmpFrame(face, e);
  if (bf >= 0) { drawBmpFace(bf, e, now); return; }     // bieu cam anh (khoa/ngu, thuc day, mat trai tim)

  Blob L, R; Mouth M; float gx, gy;
  float zt = faceTargets(face, e, now, L, R, M, gx, gy);

  // chop mat kieu thach: det xuong thanh vach rong roi nay cao len (lo xo tu tao do nay)
  bool canBlink = L.k < .5f && L.h > 20 && L.tc < .1f && (L.bc < .1f || R.bc < .1f);
  if (canBlink && now > blinkNext) { blinkAt = now; blinkNext = now + (rnd() < .2f ? 420 : 2400 + random(2600)); }
  if (face == F_GUARD && (long)grantUntil - (long)now < 5000 && blinkNext > now + 700) blinkNext = now + 500 + random(300);
  if (canBlink && now - blinkAt < 120) {                  // chi chop mat dang mo (mat nhay giu nguyen)
    if (L.bc < .1f) { L.w *= 1.3f; L.h = 4; L.y += 3; }
    if (R.bc < .1f) { R.w *= 1.3f; R.h = 4; R.y += 3; }
  }

  partStep(pL, (float*)&L, NB, dt); partStep(pR, (float*)&R, NB, dt); partStep(pM, (float*)&M, NM, dt);
  float s = min(dt, 40.0f) / 1000.0f;
  gVX += ((gx - gX) * SPRING_K - gVX * SPRING_D) * s; gX += gVX * s;
  gVY += ((gy - gY) * SPRING_K - gVY * SPRING_D) * s; gY += gVY * s;
  gVZ += ((zt - gZ) * SPRING_K - gVZ * SPRING_D) * s; gZ += gVZ * s;

  lastBmpKey = "";
  oled.clearDisplay();
  Blob bl = scaledBlob(pL), br = scaledBlob(pR); Mouth mm = scaledMouth(pM);
  float ox = R_(gX), oy = R_(gY);
  bl.x += ox; bl.y += oy; br.x += ox; br.y += oy; mm.x += ox; mm.y += oy;
  if (fabsf(gZ - 1) > .01f) {                             // phong to quanh tam man hinh (64, 30)
    Blob *bs[2] = {&bl, &br};
    for (int i = 0; i < 2; i++) { Blob &b = *bs[i]; b.x = 64 + (b.x - 64) * gZ; b.y = 30 + (b.y - 30) * gZ; b.w *= gZ; b.h *= gZ; b.s *= gZ; }
    mm.x = 64 + (mm.x - 64) * gZ; mm.y = 30 + (mm.y - 30) * gZ; mm.w *= gZ; mm.c *= gZ; mm.h *= gZ; mm.rx *= gZ; mm.ry *= gZ; mm.a *= gZ;
  }
  lineMul = gZ;
  if (pL.sq > .02f) drawBlob(bl, 1);
  if (pR.sq > .02f) drawBlob(br, -1);
  if (pM.sq > .02f) drawMouth(mm);
  lineMul = 1;

  // ---- phu kien + chu theo tung bieu cam (chu chi o dong duoi y=55 hoac dong tren y=2) ----
  String txt = "";
  bool inv = false;
  switch (face) {
    case F_IDLE: case F_CURIOUS: case F_SLEEPY: {
      if (face == F_CURIOUS) { sparkle(16, 10, now / 110.0f); sparkle(112, 12, now / 110.0f + 2); }
      struct tm t;
      if (face == F_IDLE && getLocalTime(&t, 0)) { char b[6]; strftime(b, sizeof(b), "%H:%M", &t); txt = b; }
      break; }
    case F_ASLEEP: {
      float p = (now % 2400) / 2400.0f, q = ((now + 1200) % 2400) / 2400.0f;
      ptext("z", R_(100 + p * 12), R_(22 - p * 16)); ptext("Z", R_(96 + q * 16), R_(26 - q * 20));
      break; }
    case F_ATTN:
      if (e < 700) { sparkle(18, 10, e / 90.0f); sparkle(110, 10, e / 90.0f + 2); }
      else if (e >= 1900) { if ((now / 500) % 2) cardIcon(54, 3, false); txt = "Moi quet the"; }
      break;
    case F_HAPPY: if (e >= 340) txt = greetText; break;
    case F_REMOTE: phoneIcon(58, R_(4 + sinf(now / 200.0f))); if (e > 500) txt = "Mo tu xa"; break;
    case F_OWNER:
      if (e < 800) oled.drawFastVLine(R_(12 + e / 800.0f * 104), 14, 30, SSD1306_WHITE);              // vach quet the
      else if (e < 1000) { sparkle(14, 10, e / 60.0f); sparkle(114, 10, e / 60.0f + 2); }
      if (e >= 1000) crown(64, R_(min(1.0f, -14 + (e - 1000) / 16.0f)), 18, 10);
      if (e >= 1000 && e < 2700) { float p = ((e - 1000) % 1400) / 1400.0f; heartShape(12, 40 - p * 30, 4 + p * 2); heartShape(116, 46 - p * 30, 4 + p * 2); }
      if (e > 1300 && e < 2700) txt = "CHU NHA"; else if (e >= 2700) txt = "Chao chu nha!";
      break;
    case F_GUARD: {
      long left = max(0L, (long)grantUntil - (long)now);
      bool fast = left < 5000, on = fast ? (now / 150) % 2 : (now / 600) % 3 != 0;
      if (on) cardIcon(54, 17, false);
      ctext("THEM THE " + String((left + 999) / 1000) + "s", 2);
      oled.drawRect(14, 58, 100, 5, SSD1306_WHITE);
      oled.fillRect(16, 60, R_(96.0f * left / GRANT_MS), 1 + 1, SSD1306_WHITE);
      break; }
    case F_EXCITED: {
      if (e < 500) cardScaled(64, -10 + e / 500.0f * 26, 1);                       // the roi xuong
      else if (e < 800) { float q = (e - 500) / 300.0f; cardScaled(64, 16 + q * 28, 1 - q); }   // bi "an" vao mieng
      else if (e >= 1300) {
        sparkle(14, 10, e / 80.0f); sparkle(114, 12, e / 80.0f + 1.5f); sparkle(30, 52, e / 80.0f + 3); sparkle(98, 52, e / 80.0f + .7f);
        float k = min(1.0f, (e - 1300) / 200.0f); if (k > .3f) ctext("+1", R_(10 - k * 6));
      }
      txt = flashText; break; }
    case F_SAD: {
      cardIcon(54, R_(-14 + min(16.0f, e / 50.0f)), true);
      if (e > 600) tearDrop(R_(LX - 6 + gX), R_(50 + fmodf(e / 16.0f, 12)));
      txt = flashText; break; }
    case F_ANGRY:
      if (angryPhase == 1) vein(64, 8, 3.4f + fabsf(sinf(e / 90.0f)) * .8f);
      if (angryPhase == 2) { vein(64, 7, 3.4f + sinf(e / 60.0f)); break; }   // dang phong to: khong hien chu
      txt = failCount ? "Sai the " + String(failCount) + "/" + String(MAX_FAILS) : String("He thong dang khoa"); break;
    case F_LOCKED: {
      lockIcon(116, 1, false);
      long left = ((long)LOCK_MS - (long)(now - stateT)) / 1000 + 1;
      txt = "Khoa the " + String(max(left, 0L)) + "s"; break; }
    case F_SHOCKED: if ((now / 180) % 2) ctext("!", 4); txt = fireOpen ? "THOAT HIEM" : "BAO DONG"; inv = (now / 260) % 2; break;
    default: break;
  }
  if (txt.length()) ctext(txt, 56);

  if (inv != oledInv) { oled.invertDisplay(inv); oledInv = inv; }   // bao dong: nhay sang toi
  oled.display();
}

// =====================================================================
//                       MAY TRANG THAI
// =====================================================================
void updateState(bool presence) {
  unsigned long now = millis();
  switch (state) {
    case D_IDLE:
      if (presence) { setState(D_READY); lastPresenceT = now; bzStart(PAT_HELLO, 3); Serial.println("Co nguoi lai gan"); }
      break;
    case D_READY:
      if (presence) lastPresenceT = now;
      if (now - lastPresenceT > READY_TIMEOUT_MS) setState(D_IDLE);
      break;
    case D_GREET:
      if (now - stateT >= GREET_MS) setState(D_IDLE);
      break;
    case D_OWNER:
      if (now - stateT >= OWNER_HELLO_MS) { setState(D_GRANT); grantUntil = now + GRANT_MS; }
      break;
    case D_GRANT:
      if (now >= grantUntil) { bzStart(PAT_EXIT, 3); setState(D_IDLE); Serial.println("-> HET CHE DO CAP QUYEN"); }
      break;
    case D_DENIED:
      if (now - stateT >= DENIED_MS) setState(D_IDLE);
      break;
    case D_LOCKED:
      if (now - stateT >= LOCK_MS) { failCount = 0; setState(D_IDLE); Serial.println("-> HET KHOA TAM"); }
      break;
    case D_ALARM:
      if (now - stateT >= ALARM_MS) { stopAlarm("tự tắt sau 30 giây"); break; }
      digitalWrite(PIN_BUZZER, (now / 200) % 2);
      break;
  }
}

// ----- Serial: lenh 1 ky tu (l, x, t) va cau noi USB cho trang cerberus_usb.html -----
// Trang gui:  ">7 /api/act?do=open"   ESP32 tra loi:  "@@7 {json}"
String serBuf;

String urlDecode(const String &s) {
  String o;
  for (unsigned i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '+') o += ' ';
    else if (c == '%' && i + 2 < s.length()) { o += (char)strtol(s.substring(i + 1, i + 3).c_str(), nullptr, 16); i += 2; }
    else o += c;
  }
  return o;
}
bool qHas(const String &qs, const char* key) {
  String k = String(key) + "=";
  return qs.startsWith(k) || qs.indexOf("&" + k) >= 0;
}
String qArg(const String &qs, const char* key) {
  String k = String(key) + "=";
  int p = qs.startsWith(k) ? 0 : qs.indexOf("&" + k);
  if (p < 0) return "";
  if (p > 0) p++;
  p += k.length();
  int e = qs.indexOf('&', p);
  return urlDecode(e < 0 ? qs.substring(p) : qs.substring(p, e));
}
String reply(bool ok, const String &msg);
void usbRequest(const String &line) {
  int sp = line.indexOf(' ');
  if (sp < 2) return;
  String tag = line.substring(1, sp), url = line.substring(sp + 1);
  int q = url.indexOf('?');
  String path = q < 0 ? url : url.substring(0, q), qs = q < 0 ? String("") : url.substring(q + 1);
  String out;
  if (path == "/api/poll")      out = apiPoll(qArg(qs, "since").toInt(), qHas(qs, "cv") ? qArg(qs, "cv").toInt() : -1);
  else if (path == "/api/act")  out = apiAct(qArg(qs, "do"));
  else if (path == "/api/card") out = apiCard(qArg(qs, "do"), qArg(qs, "uid"), qHas(qs, "i") ? qArg(qs, "i").toInt() : -1, qHas(qs, "n") ? qArg(qs, "n") : qArg(qs, "name"));
  else if (path == "/api/home") out = apiHome(qArg(qs, "do"), qArg(qs, "v"));
  else                          out = reply(false, "Lệnh không hợp lệ");
  Serial.print("@@" + tag + " " + out + "\n");          // gui 1 lan cho ca dong, khong bi chen chu khac vao giua
}
void serialCmd(char c) {
  if (c == 'l') {
    Serial.print("The chu nha: "); Serial.println(OWNER_UID);
    Serial.print("The thanh vien ("); Serial.print(memCount); Serial.println("):");
    for (int i = 0; i < memCount; i++) Serial.println("  " + memName[i] + "  " + memUid[i]);
  } else if (c == 'x') {
    memCount = 0; saveMembers(); Serial.println("Da xoa het the thanh vien");
  } else if (c == 't') {
    triggerAlarm();
  }
}
void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (serBuf.length() && serBuf[0] == '>') usbRequest(serBuf);
      serBuf = "";
    } else if (serBuf.length() == 0 && (c == 'l' || c == 'x' || c == 't')) {
      serialCmd(c);                                       // lenh go tay trong Serial Monitor
    } else if (serBuf.length() < 400) {
      serBuf += c;
    }
  }
}

// (khai bao truoc de phan lien ket Arduino dung)
String jsonStr(const String &s);
String reply(bool ok, const String &msg);

// =====================================================================
//                 LIEN KET VOI ARDUINO TRONG NHA (UART2 9600)
// =====================================================================
struct HomeSt {
  bool ok = false; unsigned long at = 0;
  int mode = 0, armed = 0, hum = 0, light = 0, rain = 1023, flame = 1023, flags = 0, lightMode = 0, hallMode = 0, rainCount = 0, fireCount = 0, page = 0;
  float temp = 0; bool dhtOk = false;
} HS;
uint8_t lcdRaw[32], lcdG[8]; bool lcdBl = true, lcdOk = false;
uint8_t hist[24]; int histHour = -1;                   // nhiet do moi gio (255 = chua co)
String ardBuf;
bool lastDoorSent = false; unsigned long doorSentAt = 0, timeSentAt = 0; bool timeSent = false;

void ardSend(const String &s) { ARD.print(s); ARD.print('\n'); }
bool homeLinked() { return HS.ok && millis() - HS.at < 6000; }
int hexv(char c) { return c >= 'a' ? c - 'a' + 10 : c >= 'A' ? c - 'A' + 10 : c - '0'; }
const char* MODE_VN[] = {"Ở NHÀ", "VẮNG NHÀ", "ĐI NGỦ", "CHÀO MỪNG"};

void onFire(bool on, int m) {
  if (on) {
    logEv(K_HFIRE, "", "Phát hiện lửa trong nhà");
    if (m != 1) {                                        // co nguoi trong nha: mo cua thoat hiem, bo qua moi khoa
      if (state == D_ALARM) { digitalWrite(PIN_BUZZER, LOW); setState(D_IDLE); }
      fireOpen = true; fireCloseAt = 0;
      servoGo(SERVO_OPEN); doorCloseAt = 0;
      logEv(K_FIREDOOR, "", "Có cháy · cửa tự mở để thoát hiểm");
      notify(String("🔥 CHÁY TRONG NHÀ!\nMình đã mở cửa để mọi người thoát ra ngoài. Hãy rời khỏi nhà và gọi 114.\n🕐 ") + nowStr());
    } else {
      notify(String("🔥 Phát hiện LỬA trong nhà lúc vắng nhà!\nCửa vẫn giữ đóng để tránh kẻ gian. Hãy về kiểm tra hoặc gọi 114.\n🕐 ") + nowStr());
    }
  } else {
    logEv(K_HFIRE, "", "Hết lửa");
    if (fireOpen) { fireCloseAt = millis() + 60000; logEv(K_FIREDOOR, "", "Hết cháy · cửa tự đóng sau 1 phút"); }
    notify(String("✅ Cảm biến lửa đã hết thấy lửa.") + (fireOpen ? " Cửa sẽ tự đóng sau 1 phút." : "") + "\n🕐 " + nowStr());
  }
}

void onStatus(const String &l) {                        // ST,mode,armed,temp,hum,light,rain,flame,flags,lightMode,hallMode,rainCount,fireCount,page
  int v[13]; String f[13]; int n = 0, p = 3;
  while (n < 13) { int c = l.indexOf(',', p); f[n++] = c < 0 ? l.substring(p) : l.substring(p, c); if (c < 0) break; p = c + 1; }
  if (n < 13) return;
  for (int i = 0; i < 13; i++) v[i] = f[i].toInt();
  int oldMode = HS.mode, oldFlags = HS.flags, oldLight = HS.lightMode; bool first = !HS.ok;
  HS.mode = v[0]; HS.armed = v[1]; HS.dhtOk = f[2] != "-"; HS.temp = f[2].toFloat(); HS.hum = v[3];
  HS.light = v[4]; HS.rain = v[5]; HS.flame = v[6]; HS.flags = v[7]; HS.lightMode = v[8]; HS.hallMode = v[9];
  HS.rainCount = v[10]; HS.fireCount = v[11]; HS.page = v[12];
  HS.ok = true; HS.at = millis();
  if (first) { logEv(K_HSYS, "", "Đã nối với Arduino trong nhà"); timeSent = false; return; }
  if (HS.mode != oldMode) logEv(K_HMODE, "", String("Chế độ ") + MODE_VN[constrain(HS.mode, 0, 3)]);
  if ((HS.flags & 2) && !(oldFlags & 2)) { logEv(K_HRAIN, "", "Phát hiện mưa · nhắc thu đồ"); notify("🌧️ Trời đang mưa, nhớ thu đồ và đóng cửa sổ nhé."); }
  if (!(HS.flags & 2) && (oldFlags & 2)) logEv(K_HRAIN, "", "Hết mưa");
  if ((HS.flags & 32) && !(oldFlags & 32)) logEv(K_HHOT, "", String("Nóng ") + (int)HS.temp + "°C");
  if (HS.lightMode != oldLight) logEv(K_HLIGHT, "", String("Đèn phòng khách: ") + (HS.lightMode == 0 ? "tự động" : HS.lightMode == 1 ? "bật" : "tắt"));
}
void onLcd(const String &l) {                           // LCD,<64 hex>,<16 hex>,<0/1>
  if (l.length() < 4 + 64 + 1 + 16 + 2) return;
  for (int i = 0; i < 32; i++) lcdRaw[i] = hexv(l[4 + i * 2]) * 16 + hexv(l[5 + i * 2]);
  for (int i = 0; i < 8; i++) lcdG[i] = hexv(l[69 + i * 2]) * 16 + hexv(l[70 + i * 2]);
  lcdBl = l[86] == '1'; lcdOk = true;
}
void ardLine(const String &l) {
  if (l.startsWith("ST,")) onStatus(l);
  else if (l.startsWith("LCD,")) onLcd(l);
  else if (l.startsWith("FIRE,1")) onFire(true, l.length() > 7 ? l.substring(7).toInt() : 0);
  else if (l.startsWith("FIRE,0")) onFire(false, 0);
  else if (l == "FLAME,1") {
    logEv(K_HFIRE, "", "Cảm biến lửa thấy lửa · cảnh báo, còn lửa đủ 3 giây sẽ báo cháy");
    notify(String("⚠️ Cảm biến lửa trong nhà vừa thấy lửa.\nNếu còn thấy lửa liên tục đủ 3 giây, hệ thống sẽ báo CHÁY và tự mở cửa thoát hiểm.\n🕐 ") + nowStr());
  }
  else if (l == "FLAME,0") logEv(K_HFIRE, "", "Hết cảnh báo lửa");
  else if (l == "NOISE") {
    logEv(K_HINTRUDE, "", "Có tiếng động lạ trong nhà lúc vắng nhà");
    notify(String("🚨 Có tiếng động lạ trong nhà khi bạn đang vắng nhà!\nNhà vẫn đang canh gác. Quẹt thẻ ở cửa khi bạn về.\n🕐 ") + nowStr());
  }
  else if (l == "REQ,OPEN") {                           // giu phim 2 trong nha
    if (state == D_ALARM) Serial.println("[ARD] dang bao dong, khong mo cua");
    else if (!doorIsOpen()) { logEv(K_REMOTE, "", "Bàn phím trong nhà"); grantDoor("", "ban phim trong nha"); }
  }
}
unsigned long nearSince = 0, lastBell = 0, lastGuide = 0;
void ardUpdate() {
  while (ARD.available()) {
    char c = ARD.read();
    if (c == '\r') continue;
    if (c == '\n') { if (ardBuf.length()) ardLine(ardBuf); ardBuf = ""; }
    else if (ardBuf.length() < 120) ardBuf += c;
  }
  unsigned long now = millis();
  bool open = doorIsOpen();
  if (open != lastDoorSent || now - doorSentAt > 10000) { lastDoorSent = open; doorSentAt = now; ardSend(open ? "DOOR,OPEN" : "DOOR,CLOSE"); }
  // gui gio Internet cho DS1307: khi moi noi va 6 tieng mot lan
  struct tm tt;
  if (homeLinked() && (!timeSent || now - timeSentAt > 6UL * 3600000UL) && getLocalTime(&tt, 0) && tt.tm_year > 120) {
    char b[40]; snprintf(b, sizeof(b), "TIME,%d,%d,%d,%d,%d,%d", tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
    ardSend(b); timeSent = true; timeSentAt = now;
  }
  // dang mo cua thoat hiem: tit moi 2 giay de nguoi trong nha tim duong ra cua
  if (fireOpen && !fireCloseAt && !bzPat && (now / 2000) != (lastGuide / 2000)) { lastGuide = now; bzStart(PAT_GUIDE, 1); }
  if (fireOpen && fireCloseAt && now >= fireCloseAt) { fireOpen = false; fireCloseAt = 0; closeDoorNow(); logEv(K_FIREDOOR, "", "Cửa thoát hiểm đã đóng"); }
  struct tm t;                                          // luu nhiet do theo gio cho bieu do
  if (homeLinked() && HS.dhtOk && getLocalTime(&t, 0) && t.tm_hour != histHour) { histHour = t.tm_hour; hist[t.tm_hour] = constrain((int)(HS.temp + 0.5), 0, 99); }
}
String homeJson() {
  int fl = HS.flags; bool dark = fl & 1, rain = fl & 2, fire = fl & 4, ringOn = fl & 8, hall = fl & 16, hot = fl & 32, wake = fl & 64;
  unsigned long now = millis();
  int rr = 0, rg = 0, rb = 0, lit = 12, fx = 0;
  if (fire) { rr = 255; rg = 70; fx = 6; }
  else if (wake) { rr = 255; rg = 140; rb = 60; fx = 1; }
  else if (HS.mode == 1 && !HS.armed) { rr = 255; rg = 90; fx = 2; }
  else if (HS.mode == 1) { fx = 7; }
  else if (ringOn && HS.mode == 2) { rr = 60; rb = 90; }
  else if (ringOn) { rr = 255; rg = 140; rb = 40; }
  String j; j.reserve(1500);
  char tb[12]; dtostrf(HS.temp, 1, 1, tb);
  j += "{\"mode\":"; j += HS.mode; j += ",\"armed\":"; j += HS.armed ? "true" : "false";
  j += ",\"temp\":"; j += (int)(HS.temp + 0.5); j += ",\"tempF\":"; j += tb; j += ",\"hum\":"; j += HS.hum;
  j += ",\"dhtOk\":"; j += HS.dhtOk ? "true" : "false";
  j += ",\"light\":"; j += HS.light; j += ",\"rain\":"; j += HS.rain; j += ",\"flame\":"; j += HS.flame;
  j += ",\"raining\":"; j += rain ? "true" : "false"; j += ",\"fire\":"; j += fire ? "true" : "false";
  j += ",\"dark\":"; j += dark ? "true" : "false"; j += ",\"sound\":false,\"hall\":"; j += hall ? "true" : "false";
  j += ",\"ringOn\":"; j += ringOn ? "true" : "false"; j += ",\"lightMode\":"; j += HS.lightMode; j += ",\"hallMode\":"; j += HS.hallMode;
  j += ",\"ring\":[";
  for (int i = 0; i < 12; i++) { if (i) j += ','; bool on = i < lit && !(fx == 7 && i != (int)(now / 500) % 12); int r = fx == 7 ? (on ? 200 : 0) : rr, g = fx == 7 ? 0 : rg, b = fx == 7 ? 0 : rb;
    if (fx == 6) g = (i + now / 90) % 3 ? 70 : 20;
    j += '['; j += on ? r : 0; j += ','; j += on ? g : 0; j += ','; j += on ? b : 0; j += ']'; }
  j += "],\"ringFx\":"; j += fx; j += ",\"clap\":0,\"rainCount\":"; j += HS.rainCount; j += ",\"fireCount\":"; j += HS.fireCount;
  int lo = 99, hi = -1; j += ",\"hist24\":[";
  for (int i = 0; i < 24; i++) { if (i) j += ','; j += hist[i]; if (hist[i] != 255) { lo = min(lo, (int)hist[i]); hi = max(hi, (int)hist[i]); } }
  j += "],\"hist12\":[";
  int hn = histHour < 0 ? 0 : histHour;
  for (int i = 0; i < 12; i++) { if (i) j += ','; j += hist[(hn + 13 + i) % 24]; }
  j += "],\"dayMin\":"; j += hi < 0 ? (int)HS.temp : lo; j += ",\"dayMax\":"; j += hi < 0 ? (int)HS.temp : hi;
  bool blink = (now / 400) % 2;
  j += ",\"leds\":{\"r\":"; j += (fire || ((rain || hot) && HS.mode != 1 && HS.mode != 2)) && blink ? 1 : 0;
  j += ",\"g\":"; j += (HS.mode == 0 || HS.mode == 3) ? 1 : 0; j += ",\"y\":"; j += hall ? 1 : 0; j += '}';
  j += ",\"intrude\":"; j += (fl & 512) ? "true" : "false"; j += ",\"flameWarn\":"; j += (fl & 256) ? "true" : "false";
  j += ",\"key\":0,\"bz\":false,\"doorLight\":false,\"wake\":"; j += wake ? "true" : "false";
  j += ",\"whoName\":\"\",\"time\":"; j += jsonStr(nowStr().substring(0, 8));
  const char* PG[] = {"Màn chính", "Cửa", "Cảm biến", "Chế độ", "Báo thức", "Báo thức", "Báo thức"};
  j += ",\"pageName\":"; j += jsonStr(fire ? "Báo cháy" : HS.mode == 2 ? "Đang ngủ" : HS.mode == 1 ? "Vắng nhà" : HS.mode == 3 ? "Chào mừng" : (fl & 1024) ? "Chuông cửa" : (fl & 256) ? "Cảnh báo lửa" : PG[constrain(HS.page, 0, 6)]);
  if (lcdOk) {
    j += ",\"lcd\":{\"rows\":[";
    for (int r = 0; r < 2; r++) { if (r) j += ','; j += '['; for (int c = 0; c < 16; c++) { if (c) j += ','; j += lcdRaw[r * 16 + c]; } j += ']'; }
    j += "],\"g\":["; for (int i = 0; i < 8; i++) { if (i) j += ','; j += lcdG[i]; } j += "],\"bl\":"; j += lcdBl ? "true" : "false"; j += '}';
  }
  j += '}';
  return j;
}
// /api/home?do=mode&v=home|away|sleep | do=light&v=0..2 | do=hall&v=0..2
String apiHome(const String &d, const String &v) {
  if (!homeLinked()) return reply(false, "Chưa nối được với Arduino trong nhà");
  if (d == "mode") {
    int m = v == "away" ? 1 : v == "sleep" ? 2 : 0;
    ardSend("CMD,MODE," + String(m));
    return reply(true, m == 1 ? "Nhà sẽ canh gác sau 10 giây" : m == 2 ? "Chúc ngủ ngon" : "Chào mừng về nhà");
  }
  if (d == "light" || d == "hall") {
    int x = constrain(v.toInt(), 0, 2);
    ardSend(String(d == "light" ? "CMD,LIGHT," : "CMD,HALL,") + x);
    return reply(true, String(d == "light" ? "Đèn phòng khách: " : "Đèn hành lang: ") + (x == 0 ? "Tự động" : x == 1 ? "Bật" : "Tắt"));
  }
  return reply(false, "Lệnh không hợp lệ");
}

// =====================================================================
//                       TRANG WEB + API (chay trong loop, nhan 1)
// =====================================================================
WebServer server(80);
bool webStarted = false;

bool webAuth() {
  if (server.authenticate(WEB_USER, WEB_PASS)) return true;
  server.requestAuthentication(BASIC_AUTH, "Cerberus", "Can dang nhap");
  return false;
}

String jsonStr(const String &s) { return "\"" + jsonEscape(s.c_str()) + "\""; }

const char* stateKey() {
  if (fireOpen) return "FIRE";
  if (state != D_ALARM && flashFace && millis() < flashUntil) return flashFace == 1 ? "GRANT_OK" : "GRANT_DEL";
  switch (state) {
    case D_IDLE:   return "IDLE";
    case D_READY:  return "READY";
    case D_GREET:  return "GRANTED";
    case D_OWNER:  return "OWNER";
    case D_GRANT:  return "GRANT";
    case D_DENIED: return "DENIED";
    case D_LOCKED: return "LOCKED";
    case D_ALARM:  return "ALARM";
  }
  return "IDLE";
}

long msLeft() {                                          // thoi gian con lai cua trang thai hien tai
  long now = (long)millis();
  if (state == D_GRANT)  return max(0L, (long)grantUntil - now);
  if (state == D_LOCKED) return max(0L, (long)(stateT + LOCK_MS) - now);
  if (state == D_ALARM)  return max(0L, (long)(stateT + ALARM_MS) - now);
  return 0;
}

// /api/poll?since=<id su kien cuoi da co>&cv=<phien ban danh sach the da co>
String apiPoll(uint32_t since, long cv) {
  unsigned long now = millis();
  String j;
  j.reserve(4096);
  j += "{\"st\":\""; j += stateKey(); j += "\"";
  j += ",\"greet\":"; j += jsonStr(flashFace && now < flashUntil ? flashText : greetText);
  j += ",\"fails\":"; j += failCount;
  j += ",\"maxf\":"; j += MAX_FAILS;
  j += ",\"left\":"; j += msLeft();
  j += ",\"door\":"; j += doorIsOpen() ? "true" : "false";
  j += ",\"servo\":"; j += servoPos;
  j += ",\"dist\":"; j += (distanceCm > 400 ? -1 : (int)distanceCm);
  j += ",\"near\":"; j += distanceCm < PRESENCE_CM ? "true" : "false";
  j += ",\"vib\":"; j += digitalRead(PIN_VIB);
  j += ",\"sys\":"; j += sysLock ? "true" : "false";
  j += ",\"rssi\":"; j += USE_WIFI ? WiFi.RSSI() : 0;
  j += ",\"wifi\":"; j += (USE_WIFI && WiFi.status() == WL_CONNECTED) ? "true" : "false";
  j += ",\"ip\":\""; j += USE_WIFI ? WiFi.localIP().toString() : String("0.0.0.0"); j += "\"";
  j += ",\"tg\":"; j += tgOk ? "true" : "false";
  j += ",\"up\":"; j += now;
  j += ",\"heap\":"; j += ESP.getFreeHeap();
  j += ",\"rc\":"; j += rcVersion;
  j += ",\"oled\":"; j += oledOk ? "true" : "false";
  j += ",\"time\":"; j += jsonStr(nowStr());
  j += ",\"link\":"; j += homeLinked() ? "true" : "false";
  if (homeLinked()) { j += ",\"home\":"; j += homeJson(); }
  j += ",\"grantSec\":"; j += GRANT_MS / 1000;
  j += ",\"lockSec\":"; j += LOCK_MS / 1000;
  j += ",\"ev\":[";
  uint32_t first = evNext > (uint32_t)LOG_N ? evNext - LOG_N : 1;
  if (since + 1 > first) first = since + 1;
  for (uint32_t id = first; id < evNext; id++) {
    Ev &e = evLog[id % LOG_N];
    if (id != first) j += ',';
    j += "{\"id\":"; j += e.id;
    j += ",\"age\":"; j += now - e.ms;
    j += ",\"k\":\""; j += KIND_STR[e.kind]; j += "\"";
    j += ",\"u\":"; j += jsonStr(e.uid);
    j += ",\"w\":"; j += jsonStr(e.who); j += '}';
  }
  j += "],\"cv\":"; j += cardsVer;
  if (cv != cardsVer) {                                  // chi gui danh sach the khi co thay doi
    j += ",\"owner\":{\"u\":\""; j += OWNER_UID; j += "\",\"n\":"; j += jsonStr(OWNER_NAME); j += "}";
    j += ",\"maxm\":"; j += MAX_MEMBERS;
    j += ",\"cards\":[";
    for (int i = 0; i < memCount; i++) {
      if (i) j += ',';
      j += "{\"i\":"; j += i; j += ",\"u\":"; j += jsonStr(memUid[i]); j += ",\"n\":"; j += jsonStr(memName[i]); j += '}';
    }
    j += ']';
  }
  j += '}';
  return j;
}
void hPoll() {
  if (!webAuth()) return;
  String j = apiPoll(server.arg("since").toInt(), server.hasArg("cv") ? server.arg("cv").toInt() : -1);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json; charset=utf-8", j);
}

bool apiOk = true;
String reply(bool ok, const String &msg) {
  apiOk = ok;
  return String("{\"ok\":") + (ok ? "true" : "false") + ",\"msg\":" + jsonStr(msg) + "}";
}

// /api/act?do=open|silence|grant|grant_stop|lock|unlock
String apiAct(const String &d) {
  if (d == "open") {
    if (state == D_ALARM) return reply(false, "Đang báo động, hãy tắt báo động trước");
    if (doorIsOpen())     return reply(false, "Cửa đang mở rồi");
    logEv(K_REMOTE, "", "Trang web");
    grantDoor("", "trang web");
    return reply(true, "Đã mở cửa, tự đóng sau 5 giây");
  }
  if (d == "silence") {
    if (state != D_ALARM) return reply(false, "Hiện không có báo động");
    stopAlarm("trang web");
    return reply(true, "Đã tắt báo động");
  }
  if (d == "grant") {
    if (state == D_ALARM) return reply(false, "Đang báo động, hãy tắt báo động trước");
    setState(D_GRANT); grantUntil = millis() + GRANT_MS;
    bzStart(PAT_HELLO, 3);
    return reply(true, String("Đã bật chế độ cấp quyền ") + (GRANT_MS / 1000) + " giây");
  }
  if (d == "grant_stop") {
    if (state != D_GRANT) return reply(false, "Chế độ cấp quyền đang tắt");
    bzStart(PAT_EXIT, 3); setState(D_IDLE);
    return reply(true, "Đã thoát chế độ cấp quyền");
  }
  if (d == "lock" || d == "unlock") {
    sysLock = (d == "lock"); saveSysLock();
    logEv(K_SYS, "", sysLock ? "Khóa hệ thống từ trang web" : "Mở khóa hệ thống từ trang web");
    notify(sysLock ? "🔒 Hệ thống đã được KHÓA từ trang web. Thẻ thành viên tạm thời không mở được cửa."
                   : "🔓 Hệ thống đã mở khóa từ trang web. Thẻ thành viên hoạt động lại bình thường.");
    return reply(true, sysLock ? "Đã khóa hệ thống: thẻ thành viên không mở được cửa" : "Đã mở khóa hệ thống");
  }
  return reply(false, "Lệnh không hợp lệ");
}
void hAct() {
  if (!webAuth()) return;
  String j = apiAct(server.arg("do"));
  server.send(apiOk ? 200 : 409, "application/json; charset=utf-8", j);
}

String cleanName(String s) {                             // ten chi dung chu khong dau (man OLED khong co dau)
  String o;
  for (unsigned i = 0; i < s.length() && o.length() < 16; i++) {
    char c = s[i];
    if (c >= 0x20 && c <= 0x7E && c != '"' && c != '\\') o += c;
  }
  o.trim();
  return o;
}
bool validUid(const String &u) {
  if (u.length() < 8 || u.length() > 23) return false;
  for (unsigned i = 0; i < u.length(); i++) {
    char c = u[i];
    bool hex = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
    if ((i % 3 == 2) ? c != ':' : !hex) return false;
  }
  return true;
}

// /api/card?do=add&uid=..&name=..  |  do=rename&i=..&name=..  |  do=del&i=..
String apiCard(const String &d, String uid, int i, const String &rawName) {
  String nm = cleanName(rawName);
  if (d == "add") {
    uid.toUpperCase();
    if (!validUid(uid))          return reply(false, "Mã thẻ không hợp lệ");
    if (isOwner(uid))            return reply(false, "Đây là thẻ chủ nhà");
    if (findMember(uid) >= 0)    return reply(false, "Thẻ này đã là thành viên");
    if (memCount >= MAX_MEMBERS) return reply(false, "Danh sách đã đầy 10 thẻ");
    if (nm.length() == 0) nm = nextMemberName();
    memUid[memCount] = uid; memName[memCount] = nm; memCount++; saveMembers();
    logEv(K_GRANT, uid, nm + " · thêm từ trang web");
    notify(String("➕ Đã thêm ") + nm + " (" + uid + ") từ trang web.");
    return reply(true, "Đã thêm " + nm);
  }
  if (i < 0 || i >= memCount) return reply(false, "Không tìm thấy thẻ, hãy tải lại trang");
  if (d == "rename") {
    if (nm.length() == 0) return reply(false, "Tên không được để trống");
    memName[i] = nm; saveMembers();
    return reply(true, "Đã đổi tên thành " + nm);
  }
  if (d == "del") {
    String old = memName[i], uid = memUid[i];
    for (int k = i; k < memCount - 1; k++) { memUid[k] = memUid[k + 1]; memName[k] = memName[k + 1]; }
    memCount--; saveMembers();
    logEv(K_REVOKE, uid, old + " · xóa từ trang web");
    notify(String("➖ Đã xóa thẻ của ") + old + " (" + uid + ") từ trang web.");
    return reply(true, "Đã xóa thẻ của " + old);
  }
  return reply(false, "Lệnh không hợp lệ");
}
void hCard() {
  if (!webAuth()) return;
  String j = apiCard(server.arg("do"), server.arg("uid"), server.hasArg("i") ? server.arg("i").toInt() : -1, server.hasArg("n") ? server.arg("n") : server.arg("name"));
  server.send(apiOk ? 200 : 409, "application/json; charset=utf-8", j);
}

void hHome() {
  if (!webAuth()) return;
  String j = apiHome(server.arg("do"), server.arg("v"));
  server.send(apiOk ? 200 : 409, "application/json; charset=utf-8", j);
}
void hIndex() {                                          // trang web HTML thuan (web_page.h, ~175 KB)
  if (!webAuth()) return;
  server.sendHeader("Cache-Control", "no-cache");
  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML, sizeof(INDEX_HTML) - 1);
}

void hFaces() {                                         // bieu cam anh cho o OLED tren trang web (giong man that)
  if (!webAuth()) return;
  server.sendHeader("Cache-Control", "max-age=86400");
  server.send_P(200, "application/octet-stream", (const char*)FACE_BLOB, FACE_BLOB_LEN);
}

void webUpdate() {
  if (!webStarted) {
    if (WiFi.status() != WL_CONNECTED) return;          // cho WiFi ket noi xong (o nhan 0) roi moi mo web
    server.on("/", HTTP_GET, hIndex);
    server.on("/api/poll", HTTP_GET, hPoll);
    server.on("/api/act", HTTP_ANY, hAct);
    server.on("/api/card", HTTP_ANY, hCard);
    server.on("/api/home", HTTP_ANY, hHome);
    server.on("/api/faces", HTTP_GET, hFaces);
    server.onNotFound([]() { server.send(404, "text/plain", "Khong tim thay"); });
    server.begin();
    if (MDNS.begin(MDNS_NAME)) MDNS.addService("http", "tcp", 80);
    webStarted = true;
    Serial.print("[WEB] Mo trinh duyet: http://"); Serial.print(WiFi.localIP());
    Serial.print("  hoac  http://"); Serial.print(MDNS_NAME); Serial.println(".local");
  }
  server.handleClient();
}

// ===== setup / loop =====
void setup() {
  Serial.begin(115200);
  ARD.begin(9600, SERIAL_8N1, PIN_ARD_RX, PIN_ARD_TX);    // RX = G26, TX = G25
  memset(hist, 255, sizeof(hist));
  randomSeed(esp_random());
  pinMode(PIN_BUZZER, OUTPUT); digitalWrite(PIN_BUZZER, LOW);
  pinMode(PIN_TRIG, OUTPUT);   digitalWrite(PIN_TRIG, LOW);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_VIB, INPUT);

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(400000);
  byte addr = 0;
  for (byte a = 0x3C; a <= 0x3D; a++) { Wire.beginTransmission(a); if (Wire.endTransmission() == 0) addr = a; }
  oledOk = addr && oled.begin(SSD1306_SWITCHCAPVCC, addr);
  Serial.println(oledOk ? "OLED san sang" : "KHONG THAY OLED - kiem tra day");

  doorServo.setPeriodHertz(50);
  servoGo(SERVO_CLOSED);
  servoWrite(SERVO_CLOSED);
  servoIdleT = millis();                      // giu servo vai tram ms de ve dung vi tri dong

  loadMembers();

  delay(300);
  SPI.begin(18, 19, 23, PIN_RC522_SS);
  rfid.PCD_Init();
  delay(50);
  rcVersion = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("Phien ban RC522: 0x"); Serial.println(rcVersion, HEX);
  Serial.print("So the thanh vien: "); Serial.println(memCount);
  vibIgnoreUntil = millis() + 2000;

  msgQ = xQueueCreate(8, sizeof(Msg));
  if (USE_WIFI) xTaskCreatePinnedToCore(tgTask, "tg", 10240, NULL, 1, NULL, 0);
  else Serial.println("WiFi dang TAT (USE_WIFI = false). Dong Serial Monitor, mo cerberus_usb.html bang Chrome/Edge de dieu khien qua USB.");
  notify("🛡️ Cerberus đã trực tuyến. Đầu đọc thẻ, màn hình, cảm biến người và cảm biến rung đều ổn, cửa đang được canh.\nGõ /help để xem mình làm được gì.");
  Serial.println("San sang. (Serial: 'l' = liet ke the, 'x' = xoa the thanh vien, 't' = thu bao dong)");
}

void loop() {
  static unsigned long lastDraw = 0;
  updateDistance();
  bzUpdate();
  servoUpdate();
  updateVibration();
  handleSerial();
  ardUpdate();
  handleRemote();
  if (USE_WIFI) webUpdate();

  String uid;
  if (readCard(uid)) onCard(uid);
  if (distanceCm < PRESENCE_CM) lastAct = millis();
  updateState(distanceCm < PRESENCE_CM);

  if (oledOk && millis() - lastDraw >= FRAME_MS) { lastDraw = millis(); drawRobot(); }   // 24 khung hinh/giay
}
