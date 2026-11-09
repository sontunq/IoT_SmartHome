# IoT Smart Home – Cerberus

Mô hình nhà thông minh gồm 2 vi điều khiển nói chuyện với nhau qua UART:

| Thư mục | Bo mạch | Vai trò |
|---|---|---|
| [`cerberus_web/`](cerberus_web/) | **ESP32** | "Cerberus" – cửa ra vào: quẹt thẻ RFID, servo mở cửa, mặt robot trên OLED, chống phá cửa, Telegram bot, trang web điều khiển |
| [`nha_thong_minh/`](nha_thong_minh/) | **Arduino Uno/Nano** | Trong nhà: LCD đồng hồ, nhiệt độ/độ ẩm, đèn tự động, báo mưa, báo cháy, chống đột nhập, báo thức bình minh |

## Tính năng

### ESP32 – Cerberus (cửa ra vào)
- **Mở cửa bằng thẻ RFID (RC522)**: thẻ chủ nhà + danh sách thẻ thành viên lưu trong flash (`Preferences`). Quẹt sai 3 lần → khoá 30 giây.
- **Cấp / thu hồi thẻ** bằng thẻ chủ nhà hoặc qua trang web.
- **Servo** mở cửa từ từ, tự đóng sau 5 giây; cảm biến siêu âm phát hiện có người đứng trước cửa.
- **Mặt robot trên OLED SSD1306** với nhiều biểu cảm (ngủ, chào, giận, mắt trái tim…).
- **Cảm biến rung**: gõ cửa 1–4 lần → báo "có người gõ cửa"; đập liên tục → báo động.
- **Telegram bot**: gửi cảnh báo và nhận lệnh
  - `/trang_thai` – báo cáo nhanh tình hình cửa
  - `/danh_sach_the` – xem các thẻ được vào nhà
  - `/mo_cua` – mở cửa từ xa (cần `/xacnhan`)
  - `/tat_bao_dong` – tắt còi báo động
- **Trang web** tại `http://cerberus.local` (có đăng nhập): xem trạng thái, mở cửa, quản lý thẻ, điều khiển các thiết bị bên Arduino.

### Arduino – Nhà thông minh
- LCD 16x2 I2C: màn chính, đồng hồ số lớn, menu bằng bàn phím 2x3.
- Đồng hồ thời gian thực DS1307, cảm biến DHT11.
- Đèn LED vòng NeoPixel 12 bóng: tự bật khi trời tối, báo thức kiểu bình minh.
- Chế độ: **Ở nhà / Vắng nhà / Ngủ / Chào mừng**.
- Báo mưa, báo nóng, **báo cháy** (cảm biến lửa: cảnh báo sau 1 giây, còi hú + mở cửa thoát hiểm sau 3 giây).
- Chống đột nhập khi vắng nhà (cảm biến âm thanh, cảm biến Hall ở cửa).
- Cài đặt được lưu vào EEPROM.

## Phần cứng & sơ đồ chân

### ESP32
| Linh kiện | Chân |
|---|---|
| RC522 (SPI) | SS = G5, RST = G27, SCK = G18, MISO = G19, MOSI = G23 |
| Servo | G13 |
| Còi buzzer | G17 |
| Siêu âm HC-SR04 | TRIG = G16, ECHO = G34 |
| Cảm biến rung | G39 |
| OLED SSD1306 (I2C) | SDA = G21, SCL = G22 |
| UART sang Arduino | RX = G26 (qua chia áp từ Arduino D1), TX = G25 (→ Arduino D0) |

### Arduino
| Linh kiện | Chân |
|---|---|
| DHT11 | D2 |
| Vòng LED NeoPixel | D3 |
| Bàn phím 2x3 | Hàng D4, D5 – Cột D6, D7, A3 |
| LED đỏ / xanh | D8 / D9 |
| Cảm biến Hall (cửa) | D10 |
| Còi buzzer | D11 |
| Cảm biến âm thanh | D12 |
| Quang trở / mưa / lửa | A0 / A1 / A2 |
| LCD 16x2 + DS1307 (I2C) | SDA = A4, SCL = A5 (LCD địa chỉ `0x27`) |
| UART sang ESP32 | D0 (RX), D1 (TX) – 9600 baud |

> ⚠️ Arduino dùng chung D0/D1 với cổng USB: **rút dây nối ESP32 ra khi nạp code cho Arduino**. Tín hiệu 5V từ Arduino D1 phải qua mạch chia áp trước khi vào ESP32.

## Cài đặt

### 1. Thư viện (Arduino IDE → Library Manager)
- **ESP32**: board package *esp32* (Espressif), `MFRC522`, `ESP32Servo`, `Adafruit GFX Library`, `Adafruit SSD1306`
- **Arduino**: `LiquidCrystal I2C`, `RTClib`, `DHT sensor library`, `Adafruit NeoPixel`

### 2. Cấu hình thông tin bí mật (ESP32)
```bash
cd cerberus_web
cp secrets.example.h secrets.h
```
Mở `secrets.h` và điền: tên/mật khẩu WiFi (2.4 GHz), token Telegram bot (lấy từ [@BotFather](https://t.me/BotFather)), chat ID của bạn, tài khoản đăng nhập web.
File `secrets.h` đã nằm trong `.gitignore` nên sẽ không bị đưa lên GitHub.

Các thông số khác (UID thẻ chủ nhà, góc servo, thời gian…) sửa trực tiếp ở đầu `cerberus_web.ino`.

### 3. Nạp code
1. Mở `nha_thong_minh/nha_thong_minh.ino`, chọn board Arduino Uno/Nano → Upload.
2. Mở `cerberus_web/cerberus_web.ino`, chọn board ESP32 Dev Module → Upload.
3. Mở Serial Monitor (115200) của ESP32 để xem địa chỉ IP, hoặc truy cập `http://cerberus.local`.

## Sửa giao diện web
- `cerberus_web/index.html` – bản HTML gốc, mở trực tiếp bằng trình duyệt để chỉnh.
- `cerberus_web/web_page.h` – cùng nội dung, nhúng vào firmware (`INDEX_HTML`). Sửa xong `index.html` thì dán lại vào giữa `R"rawliteral(` … `)rawliteral"`.
- `cerberus_web/faces.h` – dữ liệu khung hình biểu cảm OLED (nén RLE, tự động tạo, không cần sửa tay).

## API web (ESP32)
| Endpoint | Mô tả |
|---|---|
| `GET /` | Trang điều khiển |
| `GET /api/poll` | Trạng thái hiện tại + sự kiện mới |
| `/api/act?do=...` | Hành động với cửa (mở, tắt báo động…) |
| `/api/card?do=...` | Quản lý thẻ RFID |
| `/api/home?do=...&v=...` | Điều khiển thiết bị bên Arduino |
| `GET /api/faces` | Dữ liệu biểu cảm OLED cho trang web |

Tất cả đều yêu cầu đăng nhập HTTP Basic Auth (`WEB_USER` / `WEB_PASS`).
