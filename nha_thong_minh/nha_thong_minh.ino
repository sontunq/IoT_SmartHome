#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// ===== Chân =====
const int DHT_PIN = 2, RING_PIN = 3;
const int ROWS[2] = {4, 5};
const int COLS[3] = {6, 7, A3};
const int RED_PIN = 8, GREEN_PIN = 9, HALL_PIN = 10;
const int BUZZER_PIN = 11;
const int SOUND_PIN = 12, LIGHT_PIN = A0, RAIN_PIN = A1, FLAME_PIN = A2;

// ===== Cài đặt (sửa theo số đo của bạn) =====
const int  THRESHOLD = 400, MARGIN = 60;       // ánh sáng: trên 460 là tối, dưới 340 là sáng
const int  RAIN_THRESHOLD = 500;               // mưa: dưới 500 là ướt
const int  SOUND_ACTIVE = HIGH;
const int  SLEEP_HOUR = 23, WAKE_HOUR = 6;
const int  HOT_TEMP = 35;
const int  FLAME_ON = 400, FLAME_OFF = 520;     // lửa: dưới 400 là thấy lửa, trên 520 là hết
const unsigned long FLAME_HOLD = 1000;         // thấy lửa liên tục 1 giây -> CẢNH BÁO LỬA (còi kêu ngắt quãng)
const unsigned long FLAME_FORCE = 3000;        // thấy lửa liên tục 3 giây -> CHÁY: còi hú, mở cửa thoát hiểm (DHT11 ở ngoài mái nên không dùng nhiệt độ)
const unsigned long FIRE_CLEAR = 5000, FIRE_MUTE = 30000;
const unsigned long HALL_TIME = 5000, DOOR_LIGHT = 30000;
const unsigned long RAIN_ON_DELAY = 2000, RAIN_OFF_DELAY = 5000;
const unsigned long AUTO_HOME = 20000;         // 20 giây không bấm: về màn chính
const unsigned long MAIN_SHOW = 10000, BIG_SHOW = 5000;   // màn chính 10 giây -> đồng hồ lớn 5 giây -> màn chính
const unsigned long HOLD_MS = 1000;
const unsigned long ARM_DELAY = 10000;
const unsigned long NOISE_IGNORE = 5000;       // bỏ qua tiếng động 5 giây đầu khi vừa canh gác
const int  NOISE_HITS = 3;                     // 3 tiếng động trong 5 giây mới tính là đột nhập
const unsigned long WELCOME_TIME = 12000;
const unsigned long ALARM_SOUND = 10000;       // tiếng báo thức kéo dài 10 giây
const unsigned long WAKE_MAX = 60000, SUNRISE_TIME = 30000;

enum { M_HOME = 0, M_AWAY = 1, M_SLEEP = 2, M_WELCOME = 3 };
enum { P_HOME, P_DOOR, P_SENS, P_MODE, P_ALARM };
struct Mean { uint8_t ic; const char *name, *word; bool bad; };   // khai báo ở đầu file cho Arduino IDE

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
DHT dht(DHT_PIN, DHT11);
Adafruit_NeoPixel ring(12, RING_PIN, NEO_GRB + NEO_KHZ800);

// ===== Ký tự tự vẽ =====
enum {
  G_EYE, G_BLINK, G_MOUTHL, G_MOUTHR, G_SUN, G_SUN2, G_THERM, G_DROP, G_MOON, G_MOON2,
  G_DOOR, G_DOORB, G_LOCK, G_UNLOCK, G_HOUSE, G_BULB, G_BULBON, G_WALK, G_PERSON, G_PERSON2,
  G_CLOUD, G_RAIN1, G_RAIN2, G_FIRE1, G_FIRE2, G_FIRE3, G_CHECK, G_WARN, G_CROSS, G_HEART,
  G_BELL, G_BELL2, G_TICK1, G_TICK2, G_TICK3,
  G_BAR1, G_BAR2, G_BAR3, G_BAR4, G_BAR5,
  G_BIG0, G_BIG1, G_BIG2, G_BIG3, G_BIG4, G_BIG5, G_BIG6, G_BIG7, G_COUNT
};
const byte GLY[G_COUNT][8] PROGMEM = {
  {0,14,31,31,31,14,0,0}, {0,0,0,0,31,0,0,0}, {0,0,0,16,8,7,0,0}, {0,0,0,1,2,28,0,0},
  {0,21,14,27,14,21,0,0}, {4,0,14,31,14,0,4,0}, {4,10,10,10,14,31,31,14}, {4,4,10,10,17,17,17,14},
  {12,6,3,3,3,6,12,0}, {0,12,6,3,3,3,6,12},
  {31,17,17,19,17,17,31,0}, {31,19,19,19,19,19,31,0}, {14,17,17,31,27,27,31,0}, {14,17,1,31,27,27,31,0},
  {4,14,31,17,21,17,31,0}, {14,17,17,17,10,14,14,4}, {14,31,31,31,14,14,14,4}, {0,4,2,31,2,4,0,0},
  {14,14,4,14,21,4,10,17}, {14,14,4,14,21,4,10,10},
  {0,0,14,31,31,14,0,0}, {14,31,31,0,8,2,8,2}, {14,31,31,0,2,8,2,8},
  {4,12,14,30,31,31,31,14}, {8,12,6,15,31,31,31,14}, {2,6,14,15,31,31,31,14},
  {0,1,3,22,28,8,0,0}, {4,4,4,4,4,0,4,0}, {0,17,10,4,10,17,0,0}, {0,10,31,31,14,4,0,0},
  {4,14,14,14,31,0,4,0}, {4,14,14,31,31,0,4,0}, {0,0,0,0,16,8,0,0}, {0,0,0,16,24,8,0,0}, {0,0,1,18,28,8,0,0},
  {0,16,16,16,16,16,16,0}, {0,24,24,24,24,24,24,0}, {0,28,28,28,28,28,28,0}, {0,30,30,30,30,30,30,0}, {0,31,31,31,31,31,31,0},
  {7,15,31,31,31,31,31,31}, {31,31,31,0,0,0,0,0}, {28,30,31,31,31,31,31,31}, {31,31,31,31,31,31,15,7},
  {0,0,0,0,0,31,31,31}, {31,31,31,31,31,31,30,28}, {31,31,31,0,0,0,31,31}, {31,0,0,0,0,31,31,31}
};
const byte DIGITS[10][6] PROGMEM = {
  {0,1,2,3,4,5}, {1,2,32,4,255,4}, {6,6,2,3,4,4}, {6,6,2,4,4,5}, {3,4,255,32,32,255},
  {255,6,6,4,4,5}, {0,6,6,3,4,5}, {1,1,2,32,32,255}, {0,6,2,3,4,5}, {0,6,2,32,32,255}
};

// ===== Trạng thái =====
int mode = M_HOME;
int lightMode = 0, hallMode = 0;               // 0 TỰ ĐỘNG, 1 BẬT, 2 TẮT
bool dark = false, ringOn = false;
int lightVal = 0, rainVal = 1023, flameVal = 1023;
float temp = 0, hum = 0;
bool dhtOk = false, raining = false, hotFlag = false;
unsigned long rainPending = 0;
int rainCount = 0, alertKind = 0;              // 1 mưa, 2 nóng
unsigned long alertUntil = 0;
bool fireActive = false, flameWarn = false, fireMuted = false;
unsigned long flameSince = 0, clearSince = 0, fireMuteAt = 0, fireEndUntil = 0;
int fireCount = 0, awayFire = 0;
bool hallSensorOn = false;
unsigned long hallOffAt = 0, lastHit = 0, winStart = 0, doorLightUntil = 0;
int hits = 0;
bool lastSound = !SOUND_ACTIVE;
unsigned long armAt = 0, welcomeStart = 0, intrudeUntil = 0;
bool armed = false;
uint32_t awayStartUnix = 0, awayDur = 0;
int awayRain = 0;
bool wakeAlarm = false;
unsigned long wakeStart = 0, wakeShowAt = 0;
long lastSched = -1;
unsigned long lastSensor = 0, lastLcd = 0, lastBlink = 0;
bool blinkState = false;
bool doorOpen = false, linkOk = false;
unsigned long linkAt = 0, bellAt = 0;
char lastName[9] = "";
uint8_t lastH = 255, lastM = 0;
extern int page;

// ===== Bộ đệm LCD =====
char L[2][16];
uint8_t shown[2][16];
uint8_t cgWant[8], cgHave[8];
bool blOn = true, lcdChanged = true;
unsigned long lcdSentAt = 0;
void clr() { memset(L, ' ', 32); }
void put(uint8_t r, int c, const char* s) { while (*s) { if (c >= 0 && c < 16) L[r][c] = *s; c++; s++; } }
void putP(uint8_t r, int c, PGM_P s) { char ch; while ((ch = pgm_read_byte(s++))) { if (c >= 0 && c < 16) L[r][c] = ch; c++; } }
void putRP(uint8_t r, PGM_P s, int end = 16) { putP(r, end - (int)strlen_P(s), s); }
void putR(uint8_t r, const char* s, int end = 16) { put(r, end - (int)strlen(s), s); }
void putCP(uint8_t r, PGM_P s) { putP(r, (16 - (int)strlen_P(s)) / 2, s); }
void putC(uint8_t r, const char* s) { put(r, (16 - (int)strlen(s)) / 2, s); }
void ic(uint8_t r, int c, uint8_t code) { if (c >= 0 && c < 16) L[r][c] = (char)code; }
void cg(uint8_t slot, uint8_t g) { cgWant[slot] = g; }
void put2(uint8_t r, int c, int v) { char b[3] = {(char)('0' + v / 10), (char)('0' + v % 10), 0}; put(r, c, b); }
void putNum(uint8_t r, int c, int v) { char b[7]; itoa(v, b, 10); put(r, c, b); }
void fmtTemp(char* b) { int v = (int)(temp * 10 + 0.5); itoa(v / 10, b, 10); int n = strlen(b); b[n] = '.'; b[n + 1] = '0' + v % 10; b[n + 2] = 0; }
void bar(uint8_t r, int c0, int cells, int pct, uint8_t slot0) {
  int cols = (long)constrain(pct, 0, 100) * cells * 5 / 100;
  for (int i = 0; i < cells; i++) { int v = constrain(cols - i * 5, 0, 5); L[r][c0 + i] = v == 0 ? ' ' : (char)(slot0 + v - 1); }
}
void barSlots(uint8_t s0) { for (int i = 0; i < 5; i++) cg(s0 + i, G_BAR1 + i); }
void setBacklight(bool on) { if (on != blOn) { blOn = on; lcdChanged = true; if (on) lcd.backlight(); else lcd.noBacklight(); } }
void flush() {
  byte g[8];
  for (int s = 0; s < 8; s++)
    if (cgWant[s] != cgHave[s] && cgWant[s] < G_COUNT) { memcpy_P(g, GLY[cgWant[s]], 8); lcd.createChar(s, g); cgHave[s] = cgWant[s]; lcdChanged = true; }
  for (int r = 0; r < 2; r++) {
    int c = 0;
    while (c < 16) {
      if ((uint8_t)L[r][c] == shown[r][c]) { c++; continue; }
      lcd.setCursor(c, r);
      while (c < 16 && (uint8_t)L[r][c] != shown[r][c]) { lcd.write((uint8_t)L[r][c]); shown[r][c] = L[r][c]; c++; lcdChanged = true; }
    }
  }
}

// =====================================================================
//   ÂM THANH: còi chủ động chỉ có 1 cao độ, nên mỗi sự kiện có một NHỊP riêng để nghe là biết
//   Mỗi mảng: thời gian kêu, nghỉ, kêu, nghỉ... (đơn vị 10 ms), 0 = hết
// =====================================================================
const uint8_t S_CLICK[]  PROGMEM = {2, 0};                         // tách: nhấn phím
const uint8_t S_DONE[]   PROGMEM = {4, 5, 4, 0};                   // tít tít: giữ phím xong
const uint8_t S_CANCEL[] PROGMEM = {18, 0};                        // tèèè: hủy
const uint8_t S_ON[]     PROGMEM = {3, 6, 10, 0};                  // tí-tíiii: bật đèn (ngắn -> dài)
const uint8_t S_OFF[]    PROGMEM = {10, 6, 3, 0};                  // tíiii-tí: tắt đèn (dài -> ngắn)
const uint8_t S_BELL[]   PROGMEM = {30, 18, 60, 60, 30, 18, 60, 0};// kính... coong, kính... coong: chuông cửa
const uint8_t S_ENTRY[]  PROGMEM = {8, 6, 8, 6, 35, 0};            // ta-ta-taaa: có người về
const uint8_t S_WELCOME[]PROGMEM = {8, 6, 8, 6, 8, 6, 45, 0};      // ta-ta-ta-taaa: chào mừng về nhà
const uint8_t S_RAIN[]   PROGMEM = {4, 14, 4, 14, 4, 0};           // tách tách tách: mưa (như giọt nước)
const uint8_t S_DRY[]    PROGMEM = {12, 0};                        // tít: hết mưa
const uint8_t S_HOT[]    PROGMEM = {35, 25, 35, 0};                // tèèè tèèè: nóng
const uint8_t S_AWAY[]   PROGMEM = {10, 10, 10, 10, 10, 0};        // tít tít tít: bắt đầu đếm ngược ra ngoài
const uint8_t S_TICK[]   PROGMEM = {2, 0};                         // tích tắc mỗi giây khi đếm ngược
const uint8_t S_TICK2[]  PROGMEM = {3, 8, 3, 0};                   // 3 giây cuối: tích tích
const uint8_t S_ARMED[]  PROGMEM = {70, 0};                        // tèèèèè: đã canh gác
const uint8_t S_NOISE[]  PROGMEM = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0};   // rè rè: có kẻ lạ
const uint8_t S_SLEEP[]  PROGMEM = {30, 15, 15, 15, 6, 0};         // dài -> ngắn dần: đi ngủ
const uint8_t S_WAKEUP[] PROGMEM = {6, 15, 15, 15, 30, 0};         // ngắn -> dài dần: thức dậy
const uint8_t S_BOOT[]   PROGMEM = {5, 5, 5, 5, 20, 0};            // tí tí te: khởi động xong
const uint8_t S_HOME[]   PROGMEM = {6, 6, 6, 6, 6, 0};             // tí tí tí: về chế độ ở nhà
const uint8_t S_DENY[]   PROGMEM = {12, 8, 12, 0};                 // không làm được
const uint8_t* sPat = 0; uint8_t sIdx = 0; unsigned long sNext = 0; bool bzOn = false;
void buzzSet(bool on) { digitalWrite(BUZZER_PIN, on ? HIGH : LOW); bzOn = on; }
void buzzStop() { sPat = 0; buzzSet(false); }
void play(const uint8_t* p) {
  if (fireActive || flameWarn || wakeAlarm) return;                 // khi đang cháy / báo thức thì nhịp đó được ưu tiên
  sPat = p; sIdx = 0; buzzSet(true); sNext = millis() + pgm_read_byte(p) * 10UL;
}
void soundTask(unsigned long now) {
  if (!sPat || (long)(now - sNext) < 0) return;
  uint8_t d = pgm_read_byte(sPat + (++sIdx));
  if (!d) { sPat = 0; buzzSet(false); return; }
  buzzSet(sIdx % 2 == 0); sNext = now + d * 10UL;
}
// Cháy thật: còi hú dồn dập (4 tiếng nhanh + 1 tiếng dài, lặp lại) – kiểu còi báo cháy
void fireSound(unsigned long now) {
  if (fireMuted) { buzzSet(false); return; }
  unsigned long p = now % 1700;
  buzzSet(p < 1200 ? (p % 300) < 150 : p < 1600);
}
// Cảnh báo lửa (chưa xác nhận): 3 tiếng ngắn mỗi 3 giây
void flameSound(unsigned long now) {
  if (fireMuted) { buzzSet(false); return; }
  unsigned long p = now % 3000;
  buzzSet(p < 600 && (p % 200) < 100);
}
// Báo thức 10 giây, to dần: 0-3 s bíp... thưa; 3-7 s bíp bíp; 7-10 s bíp bíp bíp bíp dồn dập
void alarmSound(unsigned long now) {
  unsigned long e = now - wakeStart;
  if (e >= ALARM_SOUND) { buzzSet(false); return; }
  bool on;
  if (e < 3000) on = (e % 1000) < 120;
  else if (e < 7000) { unsigned long p = (e - 3000) % 800; on = p < 100 || (p >= 200 && p < 300); }
  else on = (e % 200) < 110;
  buzzSet(on);
}

// ===== Vòng NeoPixel =====
uint8_t ringCur[12][3];
void ringSetAll(uint8_t (*t)[3], int r, int g, int b) { for (int i = 0; i < 12; i++) { t[i][0] = r; t[i][1] = g; t[i][2] = b; } }
void ringChase(uint8_t (*t)[3], unsigned long now, unsigned long ms, int r, int g, int b) {
  const uint8_t lv[5] = {100, 60, 32, 14, 5};
  int head = (now / ms) % 12;
  for (int k = 0; k < 5; k++) { int i = (head - k + 12) % 12; t[i][0] = r * lv[k] / 100; t[i][1] = g * lv[k] / 100; t[i][2] = b * lv[k] / 100; }
}
void updateRing(bool force = false) {
  static unsigned long lastTick = 0;
  unsigned long now = millis();
  if (!force && now - lastTick < 25) return;
  lastTick = now;
  bool want = false; int cr = 255, cgc = 140, cb = 40;
  if (wakeAlarm) want = true;
  else if (mode == M_AWAY) want = false;
  else if (mode == M_SLEEP) { want = (lightMode != 2) && dark; cr = 15; cgc = 0; cb = 22; }
  else want = lightMode == 1 || (lightMode == 0 && dark);
  uint8_t tgt[12][3]; memset(tgt, 0, sizeof(tgt));
  int step = 6;
  if (fireActive) { for (int i = 0; i < 12; i++) { int fl = 35 + random(66); tgt[i][0] = 255 * fl / 100; tgt[i][1] = (30 + random(60)) * fl / 100; } step = 70; }
  else if (flameWarn) { unsigned long ph = now % 1000; int p = ph < 500 ? ph / 5 : (1000 - ph) / 5; ringSetAll(tgt, 255 * p / 100, 60 * p / 100, 0); step = 255; }
  else if (wakeAlarm) { float f = (float)(now - wakeStart) / SUNRISE_TIME; if (f > 1) f = 1; ringSetAll(tgt, 40 + 215 * f, 10 + 130 * f, 60 - 20 * f); step = 255; }
  else if (mode == M_AWAY && !armed) {
    long left = (long)ARM_DELAY - (long)(now - armAt); if (left < 0) left = 0;
    long lit = left * 1200L / (long)ARM_DELAY;
    for (int i = 0; i < 12; i++) { long p = constrain(lit - i * 100L, 0L, 100L); tgt[i][0] = 255 * p / 100; tgt[i][1] = 90 * p / 100; }
    step = 255;
  } else if (mode == M_AWAY && now < intrudeUntil) { if ((now / 250) % 2) ringSetAll(tgt, 200, 0, 0); step = 255; }
  else if (mode == M_AWAY) { }
  else if (now - bellAt < 15000 && bellAt && !doorOpen) { ringChase(tgt, now, 90, 120, 60, 255); step = 255; }
  else if (mode == M_WELCOME && now - welcomeStart < 3000) { ringChase(tgt, now, 80, 60, 255, 110); step = 255; }
  else if (mode == M_HOME && now < alertUntil && alertKind == 1) { ringChase(tgt, now, 70, 0, 110, 255); step = 255; }
  else if (mode == M_HOME && now < alertUntil && alertKind == 2) {
    unsigned long ph = now % 1200; int p = ph < 600 ? ph / 6 : (1200 - ph) / 6;
    ringSetAll(tgt, 255 * p / 100, 40 * p / 100, 0); step = 255;
  } else if (want) ringSetAll(tgt, cr, cgc, cb);
  bool changed = force;
  for (int i = 0; i < 12; i++) for (int c = 0; c < 3; c++) {
    int cur = ringCur[i][c], t = tgt[i][c];
    if (cur == t) continue;
    cur = (t > cur) ? min(t, cur + step) : max(t, cur - step);
    ringCur[i][c] = cur; changed = true;
  }
  if (changed) { for (int i = 0; i < 12; i++) ring.setPixelColor(i, ring.Color(ringCur[i][0], ringCur[i][1], ringCur[i][2])); ring.show(); }
  ringOn = want;
}
bool hallIsOn() {
  if (mode == M_WELCOME) return true;
  if (mode == M_AWAY) return false;
  if (millis() < doorLightUntil) return true;           // cửa vừa mở lúc tối
  if (mode == M_SLEEP) return false;
  if (hallMode == 1) return true;
  if (hallMode == 2) return false;
  return hallSensorOn;
}

// ===== Bản tin Serial =====
void logEvt(const __FlashStringHelper* s) { Serial.print(F(">> ")); Serial.println(s); }
void sendStatus() {     // cờ: 1 tối, 2 mưa, 4 cháy, 8 đèn khách, 16 hành lang, 32 nóng, 64 báo thức, 128 tắt còi, 256 cảnh báo lửa, 512 tiếng động lạ, 1024 chuông
  unsigned long now = millis();
  int f = dark | (raining << 1) | (fireActive << 2) | (ringOn << 3) | (hallIsOn() << 4) | (hotFlag << 5) | (wakeAlarm << 6) | (fireMuted << 7)
          | (flameWarn << 8) | ((now < intrudeUntil) << 9) | ((bellAt && now - bellAt < 15000) << 10);
  Serial.print(F("ST,")); Serial.print(mode); Serial.print(','); Serial.print(armed); Serial.print(',');
  char tb[8]; fmtTemp(tb); if (dhtOk) Serial.print(tb); else Serial.print('-'); Serial.print(',');
  if (dhtOk) Serial.print((int)(hum + 0.5)); else Serial.print('-');
  int v[9] = {lightVal, rainVal, flameVal, f, lightMode, hallMode, rainCount, fireCount, page};
  for (int i = 0; i < 9; i++) { Serial.print(','); Serial.print(v[i]); }
  Serial.println();
}
void sendLcd() {
  const char* hx = "0123456789ABCDEF";
  Serial.print(F("LCD,"));
  for (int r = 0; r < 2; r++) for (int c = 0; c < 16; c++) { uint8_t b = shown[r][c]; Serial.write(hx[b >> 4]); Serial.write(hx[b & 15]); }
  Serial.print(',');
  for (int i = 0; i < 8; i++) { uint8_t b = cgHave[i]; Serial.write(hx[b >> 4]); Serial.write(hx[b & 15]); }
  Serial.print(','); Serial.println(blOn);
}

// ===== Chuyển chế độ =====
void startAway() {
  logEvt(F("VANG NHA: dem nguoc 10 giay"));
  mode = M_AWAY; armed = false; armAt = millis(); intrudeUntil = 0;
  awayStartUnix = rtc.now().unixtime(); awayRain = 0; awayFire = 0; hallSensorOn = false; alertUntil = 0;
  updateRing(true); play(S_AWAY);
}
void startWelcome() {
  logEvt(F("CHAO MUNG ve nha"));
  awayDur = rtc.now().unixtime() - awayStartUnix;
  mode = M_WELCOME; armed = false; intrudeUntil = 0; welcomeStart = millis();
  play(S_WELCOME); updateRing(true);
}
void goHome() { logEvt(F("O NHA")); mode = M_HOME; armed = false; updateRing(true); play(S_HOME); }
void enterSleep() { logEvt(F("DI NGU")); mode = M_SLEEP; armed = false; hallSensorOn = false; updateRing(true); play(S_SLEEP); }
void startWake() { logEvt(F("BAO THUC")); buzzStop(); mode = M_HOME; armed = false; wakeAlarm = true; wakeStart = millis(); }
void stopWake() { logEvt(F("Tat bao thuc")); wakeAlarm = false; buzzSet(false); updateRing(true); }

// ===== Cảm biến =====
void updateRain() {
  rainVal = analogRead(RAIN_PIN);
  bool wetNow = rainVal < RAIN_THRESHOLD;
  unsigned long now = millis();
  if (wetNow == raining) { rainPending = 0; return; }
  if (rainPending == 0) { rainPending = now; return; }
  if (now - rainPending >= (wetNow ? RAIN_ON_DELAY : RAIN_OFF_DELAY)) {
    raining = wetNow; rainPending = 0;
    logEvt(raining ? F("Bat dau mua") : F("Het mua"));
    if (raining) { rainCount++; if (mode == M_AWAY) awayRain++; }
    if (mode == M_HOME && !wakeAlarm) { if (raining) { alertKind = 1; alertUntil = now + 5000; play(S_RAIN); } else play(S_DRY); }
  }
}
void checkSchedule() {
  DateTime t = rtc.now();
  long stamp = t.day() * 1440L + t.hour() * 60 + t.minute();
  if (stamp == lastSched) return;
  lastSched = stamp;
  if (fireActive || t.minute() != 0) return;
  if (t.hour() == SLEEP_HOUR && mode == M_HOME) enterSleep();
  else if (t.hour() == WAKE_HOUR && mode == M_SLEEP) startWake();
}
void readSensors() {
  lightVal = analogRead(LIGHT_PIN);
  if (!dark && lightVal > THRESHOLD + MARGIN) dark = true;
  else if (dark && lightVal < THRESHOLD - MARGIN) dark = false;
  float t = dht.readTemperature(), h = dht.readHumidity();
  static uint8_t dhtFail = 3;
  if (isnan(t) || isnan(h)) { if (dhtFail < 3) dhtFail++; } else { temp = t; hum = h; dhtFail = 0; }
  dhtOk = dhtFail < 3;
  if (dhtOk && temp >= HOT_TEMP && !hotFlag) {
    hotFlag = true;
    if (mode == M_HOME && !wakeAlarm) { alertKind = 2; alertUntil = millis() + 6000; play(S_HOT); }
  } else if (temp < HOT_TEMP - 2) hotFlag = false;
  updateRain();
  checkSchedule();
}
void handleSound(unsigned long now) {
  bool s = digitalRead(SOUND_PIN);
  bool rising = (s == SOUND_ACTIVE && lastSound != SOUND_ACTIVE);
  lastSound = s;
  if (rising && now - lastHit > 100) {
    lastHit = now;
    if (hits == 0 || now - winStart > (mode == M_AWAY ? 5000UL : 1000UL)) { hits = 1; winStart = now; } else hits++;
    if (mode == M_HOME && hallMode == 0 && hits >= 2) { hallOffAt = now + HALL_TIME; hallSensorOn = true; hits = 0; logEvt(F("Vo tay 2 lan: den hanh lang")); }
    else if (mode == M_AWAY && armed && now - armAt > ARM_DELAY + NOISE_IGNORE && hits >= NOISE_HITS) {
      hits = 0;
      if (now >= intrudeUntil) { Serial.println(F("NOISE")); logEvt(F("TIENG DONG LA trong nha")); play(S_NOISE); }
      intrudeUntil = now + 60000;
    }
  }
  if (hallSensorOn && (long)(now - hallOffAt) >= 0) hallSensorOn = false;
  digitalWrite(HALL_PIN, hallIsOn() ? HIGH : LOW);
}
void handleSecurity(unsigned long now) {
  static int lastTickS = -1;
  if (mode == M_AWAY && !armed) {
    int s = (ARM_DELAY - (now - armAt)) / 1000;                  // tích tắc mỗi giây, 3 giây cuối kêu đôi
    if (s != lastTickS && now - armAt > 600) { lastTickS = s; if (!sPat) play(s < 3 ? S_TICK2 : S_TICK); }
    if (now - armAt >= ARM_DELAY) { armed = true; logEvt(F("Bat dau canh gac")); lastSound = digitalRead(SOUND_PIN); hits = 0; buzzStop(); play(S_ARMED); }
  }
  if (mode == M_WELCOME && (long)(now - welcomeStart) >= (long)WELCOME_TIME) { mode = M_HOME; updateRing(true); }
}
// ---- Cháy: xác nhận bằng 2 dấu hiệu ----
void startFire() {
  logEvt(F("CHAY! (thay lua lien tuc 3 giay)"));
  Serial.print(F("FIRE,1,")); Serial.println(mode);         // ESP32: ở nhà / đi ngủ thì mở cửa thoát hiểm
  if (flameWarn) Serial.println(F("FLAME,0"));
  fireActive = true; flameWarn = false; fireMuted = false; fireCount++;
  if (mode == M_AWAY) awayFire++;
  wakeAlarm = false; sPat = 0; alertUntil = 0; updateRing(true);
}
void endFire(unsigned long now) {
  if (fireActive) { logEvt(F("Het chay")); Serial.println(F("FIRE,0")); fireEndUntil = now + 4000; }
  else { logEvt(F("Het canh bao lua")); Serial.println(F("FLAME,0")); }
  fireActive = false; flameWarn = false; fireMuted = false; buzzSet(false); setBacklight(true);
  updateRing(true); play(S_DRY);
}
void handleFire(unsigned long now) {
  static unsigned long lastRead = 0;
  if (now - lastRead < 100) return;
  lastRead = now;
  flameVal = analogRead(FLAME_PIN);
  bool flame = flameVal < ((fireActive || flameWarn) ? FLAME_OFF : FLAME_ON);
  if (flame) {
    clearSince = 0;
    if (!flameSince) flameSince = now;
    unsigned long held = now - flameSince;
    if (!fireActive && held >= FLAME_HOLD) {
      if (held >= FLAME_FORCE) startFire();
      else if (!flameWarn) { flameWarn = true; fireMuted = false; sPat = 0; Serial.println(F("FLAME,1")); logEvt(F("CANH BAO LUA")); updateRing(true); }
    }
  } else {
    flameSince = 0;
    if (fireActive || flameWarn) { if (!clearSince) clearSince = now; if (now - clearSince >= (fireActive ? FIRE_CLEAR : 3000)) { clearSince = 0; endFire(now); } }
  }
  if (fireMuted && now - fireMuteAt > FIRE_MUTE) fireMuted = false;
}

// ===== Số đo kèm trạng thái (Bảng 3.11) =====
Mean meaning(uint8_t i) {
  Mean m; m.bad = false;
  int tp = (int)(temp + 0.5), h = (int)(hum + 0.5);
  switch (i) {
    case 0: m.ic = G_THERM; m.name = PSTR("NHIET DO");
      m.word = !dhtOk ? PSTR("LOI DHT") : tp >= 35 ? PSTR("RAT NONG") : tp > 30 ? PSTR("HOI NONG") : tp < 20 ? PSTR("LANH") : PSTR("DE CHIU");
      m.bad = !dhtOk || tp >= 35; break;
    case 1: m.ic = G_DROP; m.name = PSTR("DO AM"); m.word = h > 75 ? PSTR("AM UOT") : h < 40 ? PSTR("KHO") : PSTR("DE CHIU"); break;
    case 2: m.ic = dark ? G_MOON : G_SUN; m.name = PSTR("ANH SANG"); m.word = dark ? PSTR("TROI TOI") : PSTR("TROI SANG"); break;
    case 3: m.ic = raining ? G_RAIN1 : G_CLOUD; m.name = PSTR("MUA"); m.word = raining ? PSTR("DANG MUA") : PSTR("KHO RAO"); m.bad = raining; break;
    case 4: m.ic = G_FIRE1; m.name = PSTR("LUA"); m.word = fireActive ? PSTR("CHAY!") : flameWarn ? PSTR("THAY LUA") : PSTR("AN TOAN"); m.bad = fireActive || flameWarn; break;
    default: m.ic = G_HOUSE; m.name = PSTR("CUA"); m.word = doorOpen ? PSTR("DANG MO") : PSTR("DA KHOA"); m.bad = !linkOk;
  }
  return m;
}
uint8_t animIc(uint8_t g, unsigned long now) {
  bool f = (now / 400) % 2;
  if (g == G_FIRE1) return G_FIRE1 + (now / 150) % 3;
  if (g == G_RAIN1) return f ? G_RAIN2 : G_RAIN1;
  if (g == G_MOON) return f ? G_MOON2 : G_MOON;
  if (g == G_SUN) return f ? G_SUN2 : G_SUN;
  if (g == G_BELL) return (now / 200) % 2 ? G_BELL2 : G_BELL;
  if (g == G_PERSON) return f ? G_PERSON2 : G_PERSON;
  return g;
}

// ===== Bàn phím =====
int page = P_HOME;
unsigned long pageAt = 0, lastAct = 0;
int holdK = 0; unsigned long holdAt = 0;
int doneK = 0, pendK = 0; unsigned long doneAt = 0, cancelAt = 0;
bool swallow = false;
PGM_P toastA = 0; char toastB[17] = ""; uint8_t toastIc = 255; unsigned long toastUntil = 0;
const unsigned long SENS_MS = 2000;
const uint8_t HOLD_IC[7] = {0, 0, G_DOOR, 0, G_BULB, G_WALK, G_MOON};

void goPage(int p) { page = p; pageAt = millis(); }
void toast(PGM_P a, PGM_P b, uint8_t icon, unsigned long ms) { toastA = a; strncpy_P(toastB, b, 16); toastB[16] = 0; toastIc = icon; toastUntil = millis() + ms; }
bool bellOn(unsigned long now) { return bellAt && (long)(now - bellAt) < 15000 && !doorOpen; }
bool canHold(int k) {
  if (mode == M_WELCOME || mode == M_AWAY) return false;
  if (mode == M_SLEEP) return k == 6;
  return k == 2 || k == 4 || k == 5 || k == 6;
}
void doHold(int k) {
  unsigned long t = millis();
  if (k == 2) { Serial.println(F("REQ,OPEN")); bellAt = 0; goPage(P_DOOR); toast(PSTR("DA GUI LENH"), PSTR("Mo cua..."), G_DOOR, 1600); }
  else if (k == 4) { lightMode = 0; EEPROM.update(0, lightMode); updateRing(true); toast(PSTR("DEN P.KHACH"), PSTR("TU DONG"), G_BULB, 1600); }
  else if (k == 5) { startAway(); goPage(P_HOME); }
  else if (k == 6) {
    if (mode == M_SLEEP) { startWake(); goPage(P_HOME); updateRing(true); }          // thức dậy: báo thức 10 giây + đèn bình minh
    else { enterSleep(); goPage(P_HOME); }
  }
}
void tap(int k) {
  unsigned long t = millis();
  if (mode == M_SLEEP) { toast(PSTR("Dang ngu..."), PSTR("Giu 6 de day"), G_MOON, 1800); return; }
  if (mode == M_AWAY && !armed) { toast(PSTR("Dang ra ngoai"), PSTR(""), G_WALK, 1200); return; }
  if (mode == M_AWAY && (k == 2 || k == 4 || k == 6)) { toast(PSTR("Dang vang nha"), PSTR("Quet the o cua"), G_LOCK, 1800); play(S_DENY); return; }
  if (mode == M_WELCOME) return;
  if (k != 4) play(S_CLICK);
  if (k == 1) { toastUntil = 0; bellAt = 0; goPage(P_HOME); }
  else if (k == 2) goPage(P_DOOR);
  else if (k == 3) { if (page == P_SENS) { int n = ((t - pageAt) / SENS_MS) % 7; pageAt = t - ((n + 1) % 7) * SENS_MS; } else goPage(P_SENS); }
  else if (k == 4) {                                         // bật / tắt đèn ngay
    bool on = ringOn; lightMode = on ? 2 : 1; EEPROM.update(0, lightMode); updateRing(true);
    play(on ? S_OFF : S_ON); toast(PSTR("DEN P.KHACH"), on ? PSTR("TAT") : PSTR("BAT"), on ? G_BULB : G_BULBON, 1500);
  }
  else if (k == 5) goPage(P_MODE);
  else if (k == 6) { if (page == P_ALARM) { startWake(); goPage(P_HOME); } else goPage(P_ALARM); }   // nhấn 6 lần 2: thử báo thức
}
void onPress(int k, unsigned long now) {
  lastAct = now; swallow = false;
  if (fireActive || flameWarn) { fireMuted = true; fireMuteAt = now; buzzSet(false); swallow = true; return; }
  if (wakeAlarm) { stopWake(); wakeShowAt = now; swallow = true; return; }
  if (canHold(k)) { holdK = k; holdAt = now; } else holdK = 0;
}
void onRelease(int k, unsigned long now) {
  lastAct = now;
  if (swallow) { swallow = false; return; }
  if (holdK == k) { unsigned long held = now - holdAt; holdK = 0; if (held > 350) { cancelAt = now; play(S_CANCEL); return; } }
  else if (doneK == k && now - doneAt < 1500) return;
  tap(k);
}
int readKey() {
  int key = 0;
  for (int c = 0; c < 3; c++) {
    digitalWrite(COLS[c], LOW); delayMicroseconds(50);
    for (int r = 0; r < 2; r++) if (digitalRead(ROWS[r]) == LOW) key = r * 3 + c + 1;
    digitalWrite(COLS[c], HIGH);
  }
  return key;
}

// ===== Các màn hình =====
void dow(uint8_t r, int c, uint8_t w) { if (w == 0) putP(r, c, PSTR("CN")); else { L[r][c] = 'T'; L[r][c + 1] = '1' + w; } }
void drawMain(unsigned long now, DateTime& t) {
  bool blink = now % 4100 < 150, alert = raining || hotFlag || !linkOk;
  clr();
  cg(0, blink ? G_BLINK : G_EYE); cg(1, doorOpen ? G_DOORB : G_LOCK); cg(2, G_MOUTHL); cg(3, G_MOUTHR);
  cg(4, animIc(G_RAIN1, now)); cg(5, G_THERM); cg(6, G_DROP); cg(7, G_WARN);
  ic(0, 0, 0); ic(0, 1, 0); putP(0, 3, PSTR("TUNG'S HOME"));
  if (linkOk) ic(0, 14, 1);                                      // cửa: khóa / mở
  if (raining && (now / 1000) % 2 == 0) ic(0, 15, 4);            // mưa và cảnh báo thay nhau ở ô cuối
  else if (alert && (now / 500) % 2) ic(0, 15, 7);
  ic(1, 0, 2); ic(1, 1, 3); ic(1, 3, 5);
  if (dhtOk) { char tb[8]; fmtTemp(tb); put(1, 4, tb); int c = 4 + strlen(tb); L[1][c] = (char)0xDF; L[1][c + 1] = 'C';
    ic(1, c + 3, 6); char b[5]; itoa((int)(hum + 0.5), b, 10); strcat(b, "%"); put(1, c + 4, b); }
  else putP(1, 4, PSTR("DHT11 loi"));
}
void drawBig(unsigned long now, DateTime& t) {
  clr(); for (int i = 0; i < 8; i++) cg(i, G_BIG0 + i);
  int h = t.hour(), m = t.minute(), d[4] = {h / 10, h % 10, m / 10, m % 10}, col[4] = {0, 3, 7, 10};
  for (int k = 0; k < 4; k++) for (int i = 0; i < 3; i++) { L[0][col[k] + i] = pgm_read_byte(&DIGITS[d[k]][i]); L[1][col[k] + i] = pgm_read_byte(&DIGITS[d[k]][3 + i]); }
  char dot = (t.second() % 2) ? ' ' : (char)0xA5; L[0][6] = dot; L[1][6] = dot;
  if ((((now - pageAt) % (MAIN_SHOW + BIG_SHOW)) - MAIN_SHOW) >= BIG_SHOW / 2) { putP(0, 14, PSTR("Th")); put2(1, 14, t.month()); }   // nửa sau: tháng
  else { dow(0, 14, t.dayOfTheWeek()); put2(1, 14, t.day()); }                                                               // nửa đầu: thứ, ngày
}
void drawSleep(DateTime& t) {
  clr(); cg(0, G_MOON); cg(1, G_SUN);
  ic(0, 0, 0); putP(0, 2, PSTR("Ngu ngon")); put2(0, 11, t.hour()); L[0][13] = t.second() % 2 ? ' ' : ':'; put2(0, 14, t.minute());
  ic(1, 0, 1); putP(1, 2, PSTR("Bao thuc")); put2(1, 11, WAKE_HOUR); putP(1, 13, PSTR(":00"));
}
void drawLeaving(unsigned long now) {
  long left = ((long)ARM_DELAY - (long)(now - armAt)); if (left < 0) left = 0;
  clr(); barSlots(0); cg(5, left < 600 ? G_LOCK : ((now / 250) % 2 ? G_UNLOCK : G_LOCK)); cg(6, G_WALK);
  ic(0, 0, 5); putP(0, 2, PSTR("Sap vang nha")); if ((now / 400) % 2) ic(0, 15, 6);
  putP(1, 0, PSTR("Ra ngoai ")); int s = (left + 999) / 1000; putNum(1, 9, s); L[1][s >= 10 ? 11 : 10] = 's';
  bar(1, 12, 4, 100 - left / 100, 0);
}
void drawAway(unsigned long now, DateTime& t) {
  clr(); cg(0, G_LOCK); cg(1, G_WARN);
  ic(0, 0, 0); putP(0, 2, PSTR("VANG NHA"));
  uint32_t mn = (t.unixtime() - awayStartUnix) / 60; char b[8]; itoa(mn / 60, b, 10); strcat(b, "h"); int c = 16 - strlen(b) - 3; put(0, c, b); put2(0, c + strlen(b), mn % 60); L[0][15] = 'm';
  if (now < intrudeUntil && (now / 1500) % 2) { ic(1, 0, 1); putP(1, 2, PSTR("Tieng dong la!")); } else putCP(1, PSTR("Dang canh gac"));
}
void drawWelcome(unsigned long now) {
  unsigned long e = now - welcomeStart;
  clr(); cg(0, G_HEART); cg(1, G_HOUSE); cg(2, animIc(G_PERSON, now));
  if (e < 6000) { ic(0, 0, 0); putP(0, 2, PSTR("Chao mung ve!")); ic(1, 0, 2); put(1, 2, lastName[0] ? lastName : "Chu nha"); }
  else { int h = awayDur / 3600, m = (awayDur % 3600) / 60; ic(0, 0, 1); putP(0, 2, PSTR("Vang ")); putNum(0, 7, h); int c = h >= 10 ? 9 : 8; L[0][c] = 'h'; put2(0, c + 1, m); L[0][c + 3] = 'm';
    putP(1, 0, PSTR("Mua ")); putNum(1, 4, awayRain); putP(1, 7, PSTR("Lua ")); putNum(1, 11, awayFire); putP(1, 13, PSTR("lan")); }
}
void drawFire(unsigned long now) {
  clr(); cg(0, G_FIRE1); cg(1, G_FIRE2); cg(2, G_FIRE3); uint8_t f = (now / 150) % 3;
  if (fireActive) {
    ic(0, 0, f); putP(0, 2, PSTR("!!! CHAY !!!")); ic(0, 15, f);
    if (fireMuted) { putP(1, 1, PSTR("Tat coi ")); putNum(1, 9, (FIRE_MUTE - (now - fireMuteAt)) / 1000 + 1); putP(1, 11, PSTR("s")); }
    else if (mode == M_AWAY) putP(1, 1, PSTR("Da bao Telegram")); else putP(1, 1, PSTR("Cua da mo, ra!"));
  } else {
    ic(0, 0, f); putP(0, 2, PSTR("CANH BAO LUA"));
    if (fireMuted) { putP(1, 0, PSTR("Tat coi ")); putNum(1, 8, (FIRE_MUTE - (now - fireMuteAt)) / 1000 + 1); putP(1, 10, PSTR("s")); }
    else putP(1, 0, (now / 2000) % 2 ? PSTR("Cua van dong") : PSTR("Kiem tra bep!"));
  }
}
void drawAlert(unsigned long now) {
  clr(); bool on = (now / 400) % 2 == 0;
  if (alertKind == 1) { cg(0, animIc(G_RAIN1, now)); if (on) putCP(0, PSTR("TROI MUA!")); putCP(1, PSTR("Thu do vao nha")); }
  else { cg(0, G_THERM); if (on) putCP(0, PSTR("NONG QUA!")); putP(1, 2, PSTR("Nhiet do ")); putNum(1, 11, (int)temp); L[1][13] = (char)0xDF; L[1][14] = 'C'; }
  ic(0, 0, 0); ic(0, 15, 0);
}
void drawWake(unsigned long now) {
  clr(); cg(0, G_SUN); cg(1, G_SUN2); cg(2, G_HEART);
  ic(0, 0, (now / 300) % 2); putP(0, 1, PSTR("Chao buoi sang"));
  if (wakeAlarm && (now / 1500) % 2) putP(1, 0, PSTR("Bam phim de tat")); else { putP(1, 2, PSTR("Ngay moi vui!")); ic(1, 15, 2); }
}
void drawBell(unsigned long now) {
  clr(); cg(0, G_BELL); cg(1, G_BELL2); cg(2, animIc(G_PERSON, now));
  ic(0, 0, (now / 200) % 2); putP(0, 1, PSTR("Co nguoi o cua")); ic(0, 15, 2);
  putP(1, 0, PSTR("Giu 2 de mo cua"));
}
void drawDoor(unsigned long now) {
  clr(); cg(0, doorOpen ? G_DOORB : G_DOOR); cg(1, G_LOCK); cg(2, G_UNLOCK); cg(3, G_PERSON); cg(4, G_WARN);
  ic(0, 0, 0); putP(0, 2, PSTR("CUA"));
  if (!linkOk) { ic(0, 15, 4); putP(1, 0, PSTR("Mat noi ESP32")); return; }
  putRP(0, doorOpen ? PSTR("Dang mo") : PSTR("Da khoa"), 14); ic(0, 15, doorOpen ? 2 : 1);
  if (lastH == 255) putP(1, 0, PSTR("Chua ai vao"));
  else { ic(1, 0, 3); put(1, 2, lastName); put2(1, 11, lastH); L[1][13] = ':'; put2(1, 14, lastM); }
}
void drawSens(unsigned long now) {
  unsigned long e = now - pageAt; int n = (e / SENS_MS) % 7; clr();
  if (n == 6) {
    int bad = 0; for (int k = 0; k < 6; k++) if (meaning(k).bad) bad++;
    cg(0, G_CHECK); cg(1, G_WARN);
    if (bad) { ic(0, 0, 1); char b[16]; itoa(bad, b, 10); strcat_P(b, PSTR(" dieu can xem")); put(0, 2, b); putCP(1, PSTR("Bam 3 xem lai")); }
    else { putCP(0, PSTR("Moi thu deu on")); ic(1, 7, 0); ic(1, 8, 0); }
    return;
  }
  Mean m = meaning(n); unsigned long ph = e % SENS_MS; int tf = ((long)ph - 500) / 90;
  cg(0, animIc(m.ic, now)); cg(1, tf < 0 ? G_TICK1 : tf >= 3 ? G_CHECK : G_TICK1 + tf); cg(2, G_WARN);
  ic(0, 0, 0); putP(0, 2, m.name); L[0][13] = '1' + n; L[0][14] = '/'; L[0][15] = '6';
  char v[8];
  if (n == 0) { if (dhtOk) { fmtTemp(v); put(1, 0, v); int c = strlen(v); L[1][c] = (char)0xDF; L[1][c + 1] = 'C'; } else putP(1, 0, PSTR("--")); }
  else if (n == 1) { itoa((int)(hum + 0.5), v, 10); strcat(v, "%"); put(1, 0, v); }
  else if (n == 2) putNum(1, 0, lightVal); else if (n == 3) putNum(1, 0, rainVal); else if (n == 4) putNum(1, 0, flameVal);
  else putP(1, 0, linkOk ? (doorOpen ? PSTR("Mo") : PSTR("Dong")) : PSTR("?"));
  if (ph > 250 && !(m.bad && (now / 300) % 2)) putRP(1, m.word, 15);
  if (ph > 500) ic(1, 15, m.bad ? 2 : 1);
}
void drawMode() {
  clr(); cg(0, G_HOUSE); ic(0, 0, 0); putP(0, 2, PSTR("CHE DO")); putRP(0, PSTR("O NHA"));
  putP(1, 0, PSTR("Giu 5:")); putRP(1, PSTR("Ra ngoai"));
}
void drawAlarmPage(unsigned long now) {
  clr(); cg(0, (now / 600) % 2 ? G_MOON : G_MOON2);
  ic(0, 0, 0); putP(0, 2, PSTR("BAO THUC")); put2(0, 11, WAKE_HOUR); putP(0, 13, PSTR(":00"));
  if ((now / 2500) % 2) putP(1, 0, PSTR("Nhan 6: nghe thu")); else { putP(1, 0, PSTR("Giu 6:")); putRP(1, PSTR("Di ngu")); }
}
void drawHold(unsigned long now) {
  int p = min(100L, (long)(now - holdAt) * 100 / HOLD_MS);
  clr(); barSlots(0); cg(5, HOLD_IC[holdK]);
  if ((now / 150) % 2) ic(0, 0, 5);
  putP(0, 2, PSTR("Giu: "));
  putP(0, 7, holdK == 2 ? PSTR("MO CUA") : holdK == 4 ? PSTR("TU DONG") : holdK == 5 ? PSTR("RA NGOAI") : mode == M_SLEEP ? PSTR("THUC DAY") : PSTR("DI NGU"));
  bar(1, 0, 16, p, 0);
}
void drawDone(unsigned long now) { int f = (now - doneAt) / 90; clr(); cg(0, f >= 3 ? G_CHECK : G_TICK1 + f); ic(0, 7, 0); ic(0, 8, 0); putCP(1, PSTR("Xong!")); }
void drawCancel(unsigned long now) { clr(); cg(0, G_CROSS); if ((now - cancelAt) / 150 % 2 == 0 || now - cancelAt > 600) ic(0, 7, 0); putCP(1, PSTR("Da huy")); }
void drawToast(unsigned long now) {
  clr(); if (toastIc != 255) { cg(0, animIc(toastIc, now)); ic(0, 0, 0); ic(0, 15, 0); }
  putCP(0, toastA); putC(1, toastB);
}
void drawLcd(unsigned long now) {
  DateTime t = rtc.now();
  if (holdK && now - holdAt >= HOLD_MS) { doneK = pendK = holdK; holdK = 0; doneAt = now; play(S_DONE); }
  if (pendK && now - doneAt >= 700) { int k = pendK; pendK = 0; doHold(k); }
  if (page != P_HOME && !holdK && now - lastAct > AUTO_HOME) goPage(P_HOME);
  setBacklight(!fireActive || fireMuted || (now / 300) % 2 == 0);
  if (fireActive || flameWarn) drawFire(now);
  else if (now < fireEndUntil) { clr(); putCP(0, PSTR("Da het lua")); putP(1, 2, PSTR("Lan thu: ")); putNum(1, 11, fireCount); }
  else if (holdK && now - holdAt > 180) drawHold(now);
  else if (doneK && now - doneAt < 800) drawDone(now);
  else if (now - cancelAt < 1100) drawCancel(now);
  else if (wakeAlarm || (wakeShowAt && now - wakeShowAt < 2600)) drawWake(now);
  else if (mode == M_WELCOME) drawWelcome(now);
  else if (bellOn(now) && mode != M_AWAY) drawBell(now);
  else if (now < toastUntil) drawToast(now);
  else if (mode == M_HOME && now < alertUntil) drawAlert(now);
  else if (mode == M_SLEEP && page != P_ALARM) drawSleep(t);
  else if (mode == M_AWAY && !armed) drawLeaving(now);
  else if (mode == M_AWAY && page != P_SENS) drawAway(now, t);
  else if (page == P_HOME) { if ((now - pageAt) % (MAIN_SHOW + BIG_SHOW) >= MAIN_SHOW) drawBig(now, t); else drawMain(now, t); }
  else if (page == P_DOOR) drawDoor(now);
  else if (page == P_SENS) drawSens(now);
  else if (page == P_MODE) drawMode();
  else drawAlarmPage(now);
  flush();
}

// ===== Nhận bản tin từ ESP32 =====
char rx[40]; uint8_t rxn = 0;
int numAt(const char* s, int idx) { while (idx--) { s = strchr(s, ','); if (!s) return 0; s++; } return atoi(s); }
void handleLine() {
  unsigned long now = millis();
  linkOk = true; linkAt = now;
  if (!strcmp_P(rx, PSTR("DOOR,OPEN"))) {
    if (!doorOpen && dark && mode != M_AWAY) doorLightUntil = now + DOOR_LIGHT;   // cửa mở lúc tối: bật đèn hành lang 30 giây
    doorOpen = true; bellAt = 0;
  }
  else if (!strcmp_P(rx, PSTR("DOOR,CLOSE"))) doorOpen = false;
  else if (!strcmp_P(rx, PSTR("BELL"))) { if (mode != M_AWAY && !bellOn(now)) { bellAt = now; play(S_BELL); logEvt(F("Chuong cua")); } }
  else if (!strncmp_P(rx, PSTR("ENTRY,"), 6)) {
    strncpy(lastName, rx + 6, 8); lastName[8] = 0; DateTime t = rtc.now(); lastH = t.hour(); lastM = t.minute();
    if (mode == M_AWAY) startWelcome();                     // chỉ quẹt thẻ mới tắt canh gác
    else { play(S_ENTRY); char b[17]; strcpy(b, lastName); strcat_P(b, PSTR(" vua ve")); toastA = PSTR("CO NGUOI VE"); strncpy(toastB, b, 16); toastIc = G_PERSON; toastUntil = now + 4000; }
  }
  else if (!strncmp_P(rx, PSTR("TIME,"), 5)) {
    DateTime n(numAt(rx, 1), numAt(rx, 2), numAt(rx, 3), numAt(rx, 4), numAt(rx, 5), numAt(rx, 6));
    if (n.year() >= 2024) { long d = (long)(n.unixtime() - rtc.now().unixtime()); if (d > 2 || d < -2) { rtc.adjust(n); logEvt(F("Da chinh gio theo Internet")); } }
  }
  else if (!strncmp_P(rx, PSTR("CMD,MODE,"), 9)) {
    int m = rx[9] - '0';
    if (m == M_HOME) { if (mode == M_AWAY) startWelcome(); else if (mode != M_HOME) goHome(); }
    else if (m == M_AWAY && mode != M_AWAY) startAway();
    else if (m == M_SLEEP && mode != M_SLEEP) enterSleep();
  }
  else if (!strncmp_P(rx, PSTR("CMD,LIGHT,"), 10)) { int v = rx[10] - '0'; if (v >= 0 && v < 3) { lightMode = v; EEPROM.update(0, lightMode); updateRing(true); } }
  else if (!strncmp_P(rx, PSTR("CMD,HALL,"), 9)) { int v = rx[9] - '0'; if (v >= 0 && v < 3) { hallMode = v; hallSensorOn = false; EEPROM.update(1, hallMode); } }
}
void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') { if (rxn) { rx[rxn] = 0; handleLine(); rxn = 0; } }
    else if (rxn < sizeof(rx) - 1) rx[rxn++] = c;
  }
  if (linkOk && millis() - linkAt > 25000) linkOk = false;     // ESP32 gửi DOOR mỗi 10 giây
}

void splash() {
  lcd.clear();
  const char* title = "SMART HOME";
  for (int i = 0; i < 10; i++) { lcd.setCursor(3 + i, 0); lcd.print(title[i]); delay(80); }
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) { lcd.write((uint8_t)255); delay(60); }
  delay(300);
  lcd.clear();
  memset(shown, ' ', sizeof(shown)); memset(cgHave, 0xFF, 8);
}

void setup() {
  Serial.begin(9600);
  for (int r = 0; r < 2; r++) pinMode(ROWS[r], INPUT_PULLUP);
  for (int c = 0; c < 3; c++) { pinMode(COLS[c], OUTPUT); digitalWrite(COLS[c], HIGH); }
  pinMode(RED_PIN, OUTPUT); pinMode(GREEN_PIN, OUTPUT); pinMode(HALL_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, LOW);
  pinMode(SOUND_PIN, INPUT);
  lcd.init(); lcd.backlight();
  if (EEPROM.read(0) < 3) lightMode = EEPROM.read(0);
  if (EEPROM.read(1) < 3) hallMode = EEPROM.read(1);
  dht.begin();
  ring.begin(); ring.setBrightness(60); ring.show();
  if (!rtc.begin()) { lcd.print(F("Khong thay RTC!")); while (true); }
  if (!rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  splash();
  readSensors();
  rainCount = 0; raining = false; alertUntil = 0;
  int h = rtc.now().hour();
  if (h >= SLEEP_HOUR || h < WAKE_HOUR) mode = M_SLEEP;
  updateRing(true); play(S_BOOT);
  pageAt = lastAct = millis();
  logEvt(F("Cerberus - cum trong nha da khoi dong"));
}

void loop() {
  unsigned long now = millis();
  static int cand = 0, stable = 0;
  static unsigned long candSince = 0;
  int raw = readKey();
  if (raw != cand) { cand = raw; candSince = now; }
  if (now - candSince >= 30 && cand != stable) {
    int prev = stable; stable = cand;
    if (prev) onRelease(prev, now);
    if (stable) onPress(stable, now);
  }
  readSerial();
  if (now - lastSensor > 1000) { lastSensor = now; readSensors(); }
  static unsigned long lastSt = 0;
  if (now - lastSt >= 2000) { lastSt = now; sendStatus(); }
  handleSound(now);
  handleSecurity(now);
  handleFire(now);
  if (fireActive) fireSound(now);
  else if (flameWarn) flameSound(now);
  else if (wakeAlarm) { alarmSound(now); if (now - wakeStart > WAKE_MAX) stopWake(); }
  else soundTask(now);
  updateRing();
  if (now - lastBlink > 700) { lastBlink = now; blinkState = !blinkState; }
  digitalWrite(GREEN_PIN, (mode == M_HOME || mode == M_WELCOME) ? HIGH : LOW);
  bool warn = (raining || hotFlag || flameWarn) && mode != M_SLEEP && mode != M_AWAY;
  digitalWrite(RED_PIN, fireActive ? (now / 150) % 2 : (warn ? blinkState : LOW));
  if (now - lastLcd >= 60) { lastLcd = now; drawLcd(millis()); }
  if (lcdChanged && now - lcdSentAt >= 600) { lcdChanged = false; lcdSentAt = now; sendLcd(); }
}
