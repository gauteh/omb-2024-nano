#include "board_control.h"
#include "gnss_manager.h"

TwoWire ArtemisWire(i2c_port_number);
TwoWire GnssWire(3);

void blink_LED_n_times(unsigned int number_of_blinks, float frequency_hz){
  wdt.restart();  // we choose to restart at the start and end, but not in loop as a way to make sure not so much blinking that we freeze.

  unsigned long delay_half_blink = static_cast<unsigned long>(1.0 / frequency_hz * 1000.0 / 2.0);
  pinMode(LED, OUTPUT);
  delay(10);

  for (unsigned int i=0; i<number_of_blinks; i++){
    digitalWrite(LED, HIGH);
    delay(delay_half_blink);
    digitalWrite(LED, LOW);
    delay(delay_half_blink);
  }

  wdt.restart();
}

void setup_pins(void){
  pinMode(LED, OUTPUT); // Make the LED pin an output
  /* pinMode(IMUPwr, g_AM_HAL_GPIO_OUTPUT_12); // 12 mA */
  pinMode(IMUPwr, g_AM_HAL_GPIO_OUTPUT_12); // 12 mA

  pinMode(iridiumSleep, g_AM_HAL_GPIO_OUTPUT_12); // Configure the Iridium Power Pin (connected to the ADM4210 ON pin)
  digitalWrite(iridiumSleep, LOW); // Disable Iridium Power (HIGH = enable; LOW = disable)

  Serial.println("waiting11");
  for (int i = 0; i < 2; i++) {
    Serial.println("waiting1..");
    delay(1000);
  }
  // Configure GNSS enable pin
  turn_gnss_on();
  /* pinMode(gnssEN, g_AM_HAL_GPIO_OUTPUT_12); // Configure the pin which enables power for the ZOE-M8Q GNSS */
  /* digitalWrite(gnssEN, LOW); // LOW = on */
  gnss_manager.setup();
  turn_gnss_off(); // Disable power for the GNSS
  /* pinMode(geofencePin, INPUT); // Configure the geofence pin as an input */

  for (int i = 0; i < 2; i++) {
    Serial.println("waiting..");
    delay(1000);
  }

  turn_iridium_off();
  pinMode(iridiumRI, INPUT); // Configure the Iridium Ring Indicator as an input
  pinMode(iridiumNA, INPUT); // Configure the Iridium Network Available as an input
  /* pinMode(superCapPGOOD, INPUT); // Configure the super capacitor charger PGOOD input */

  pinMode(busVoltageMonEN, OUTPUT); // Make the Bus Voltage Monitor Enable an output
  digitalWrite(busVoltageMonEN, LOW); // Set it low to disable the measurement to save power
  analogReadResolution(14); //Set resolution to 14 bit
}

void turn_on(void){
    wake_up();
}

void turn_gnss_on(void){
  Serial.println(F("turn gnss on"));
  turn_iridium_off();
  delay(100);
  // https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library/issues/228
  pinMode(gnssEN, OUTPUT); // Configure the pin which enables power for the ZOE-M8Q GNSS
  digitalWrite(gnssEN, LOW);
  delay(1000);
}

void turn_gnss_off(void){
  Serial.println(F("turn gnss off"));
  pinMode(gnssEN, OUTPUT);
  digitalWrite(gnssEN, HIGH);
  delay(100);
}

void turn_iridium_on(void){
  Serial.println(F("turn iridium on"));
  turn_gnss_off();
  delay(100);
  /* pinMode(superCapChgEN, OUTPUT); // Configure the super capacitor charger enable pin (connected to LTC3225 !SHDN) */
  /* digitalWrite(superCapChgEN, HIGH); // Enable the super capacitor charger */
  pinMode(iridiumSleep, g_AM_HAL_GPIO_OUTPUT_12); // Configure the Iridium Power Pin (connected to the ADM4210 ON pin)
  digitalWrite(iridiumSleep, HIGH); // Enable Iridium Power
  delay(1000);
  Serial.println("Waiting for capacitors to charge..");
  delay(10*1000);
  Serial.println("done.");
}

void turn_iridium_off(void){
  Serial.println(F("turn iridium off"));
  pinMode(iridiumSleep, g_AM_HAL_GPIO_OUTPUT_12); // Configure the Iridium Power Pin (connected to the ADM4210 ON pin)
  digitalWrite(iridiumSleep, LOW); // Disable Iridium Power (HIGH = enable; LOW = disable)
  /* pinMode(superCapChgEN, OUTPUT); // Configure the super capacitor charger enable pin (connected to LTC3225 !SHDN) */
  /* digitalWrite(superCapChgEN, LOW); // Disable the super capacitor charger (HIGH = enable; LOW = disable) */
  /* pinMode(iridiumSleep, OUTPUT); // Iridium 9603N On/Off (Sleep) pin */
  /* digitalWrite(iridiumSleep, LOW); // Put the Iridium 9603N to sleep (HIGH = on; LOW = off/sleep) */
}

# ifndef DISABLE_ALL_THERMISTOR
void turn_thermistors_on(void){
 // turn on power to the OneWire sensors
  pinMode(THERMISTORS_POWER_PIN, OUTPUT);
  digitalWrite(THERMISTORS_POWER_PIN, HIGH);
  delay(500);
}

void turn_thermistors_off(void){
  pinMode(THERMISTORS_ONE_WIRE_PIN, INPUT);
  pinMode(THERMISTORS_POWER_PIN, INPUT);
  delay(100);
}
# endif
