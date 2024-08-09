#include <M5Unified.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("マイク眞木");

uint8_t led_level = 128;
uint8_t beep_volume = 128;
int beep_freq = 8000;    // Hz
int beep_duration = 3000; // ms
bool current_charging_state = false;
bool past_charging_state = false;
uint8_t current_battery = 80;
uint8_t past_battery = 60;
uint8_t battery_warning = 40;

void discord_mic_mute()
{
  M5.Power.setLed(led_level);
  bleKeyboard.press(KEY_LEFT_SHIFT);
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press('m');
  delay(100);
  bleKeyboard.releaseAll();
  M5.Power.setLed(0);
}

void discord_all_mute()
{
  M5.Power.setLed(led_level);
  bleKeyboard.press(KEY_LEFT_SHIFT);
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press('d');
  delay(100);
  bleKeyboard.releaseAll();
  M5.Power.setLed(0);
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  bleKeyboard.begin();
  M5.Speaker.setVolume(beep_volume);
}

void loop()
{
  M5.update();
  M5.Power.setLed(0);
  past_charging_state = current_charging_state;
  past_battery = current_battery;
  current_battery = M5.Power.getBatteryLevel();
  current_charging_state = M5.Power.isCharging();
  bleKeyboard.setBatteryLevel(current_battery);

  M5.Display.setTextFont(7);

  if (current_charging_state || current_battery == 100)
  {
    M5.Display.setTextColor(TFT_GREEN);
  }
  else
  {
    M5.Display.setTextColor(TFT_WHITE);
  }
  if (current_battery < battery_warning)
  {
    M5.Display.setTextColor(TFT_RED);
    M5.Speaker.tone(beep_freq, beep_duration);
  }
  if ((past_battery != current_battery) || (past_charging_state != current_charging_state) || (current_battery < battery_warning))
  {
    M5.Display.clear();
  }
  M5.Display.drawCenterString(String(current_battery), int(M5.Display.width() / 2), int(M5.Display.height() / 2)-20);
  if (bleKeyboard.isConnected())
  {
    if (M5.BtnA.wasPressed())
    {
      discord_mic_mute();
    }
    if (M5.BtnB.wasPressed())
    {
      discord_all_mute();
    }
  }
  else
  {
    M5.Power.setLed(led_level);
  }
}
