#define mdns_name String("DIYson")
#define default_ssid String("DIYson lamp project")
#define connection_timeout 30000
#define scan_interval 5000
#define configuration_server_port 80
#define main_server_port 80
#define connection_timeout 30000
#define ssid_length 255
#define pass_length 255
#define ssid_length_addr 0
#define pass_length_addr 250
#define ssid_addr 4
#define pass_addr 254
#define uint_size 4294967295

#define minimum_temperature 2700
#define maximum_temperature 6500
#define minimum_brightness 10
#define maximum_brightness 100
#define warm_white_pin 0
#define warm_white_freq 5000
#define warm_white_channel 0
#define warm_white_resolution 8
#define warm_white_enable_pin 4
#define warm_white_treshold 100
#define cold_white_pin 1
#define cold_white_freq 5000
#define cold_white_channel 1
#define cold_white_resolution 8
#define cold_white_enable_pin 6
#define cold_white_treshold 100

#define touch_address 0x5A
#define power_pad 0
#define brightness_pads {1,2,3,4,5}
#define temperature_pads {6,7,8,9,10}
#define touch_slider_length 5

#define main_loop_delay 50


struct LampState {
    bool powered;
    int brightness;
    int temperature;
};

struct TouchState {
    uint16_t current_touched;
    uint16_t last_touched;
};



   /*
  Modified on Jan 20, 2021
  Modified by MehranMaleki from Arduino Examples
  Home
*/

// #include <Wire.h>
// #include "Adafruit_MPR121.h"

// #ifndef _BV
// #define _BV(bit) (1 << (bit))
// #endif

// // You can have up to 4 on one i2c bus but one is enough for testing!
// Adafruit_MPR121 cap = Adafruit_MPR121();

// // Keeps track of the last pins touched
// // so, we know when buttons are 'released'
// uint16_t lasttouched = 0;
// uint16_t currtouched = 0;

// void setup () {
//   Serial.begin(9600);

//   while (!Serial) { // needed to keep leonardo/micro from starting too fast!
//     delay(10);
//   }

//   Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

//   // Default address is 0x5A, if tied to 3.3V its 0x5B
//   // If tied to SDA its 0x5C and if SCL then 0x5D
//   if (!cap.begin(0x5A)) {
//     Serial.println("MPR121 not found, check wiring?");
//     while (1);
//   }
//   Serial.println("MPR121 found!");
// }

// void loop() {
//   // Get the currently touched pads
//   currtouched = cap.touched();

//   for (uint8_t i = 0; i < 12; i++) {
//     // it if *is* touched and *wasnt* touched before, alert!
//     if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
//       Serial.print(i); Serial.print("  touched");
//     }
//     // if it *was* touched and now *isnt*, alert!
//     if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
//       Serial.println("         released");
//     }
//   }

//   // reset our state
//   lasttouched = currtouched;

//   // comment out this line for detailed data from the sensor!
//   return;

//   // debugging info, what
//   Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
//   Serial.print("Filt: ");
//   for (uint8_t i = 0; i < 12; i++) {
//     Serial.print(cap.filteredData(i)); Serial.print("\t");
//   }
//   Serial.println();
//   Serial.print("Base: ");
//   for (uint8_t i = 0; i < 12; i++) {
//     Serial.print(cap.baselineData(i)); Serial.print("\t");
//   }
//   Serial.println();

//   // put a delay so it isn't overwhelming
//   delay(1000);
// }
