/* This example shows how to get single-shot range
 measurements from the VL53L0X. The sensor can optionally be
 configured with different ranging profiles, as described in
 the VL53L0X API user manual, to get better performance for
 a certain application. This code is based on the four
 "SingleRanging" examples in the VL53L0X API.

 The range readings are in units of mm. */
#include <pic16f886.h>
//#include <Wire.h>
#include <VL53L0X.h>

#define millis()  (msTimer)
int32 msTimer=0;


#INT_TIMER1
void timer1_isr() {
   set_timer1(65286); // keep period at 1 ms (at 8 MHz)
   msTimer++;
}


void pic_setup() {
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
   set_timer1(65286); // keep period at 1 ms (at 8 MHz)
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);

}



// VL53L0X sensor;


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
#define HIGH_ACCURACY


void setup()
{
  // Serial.begin(9600);
  // Wire.begin();

   output_high(PIN_B5);
   delay_ms(1000);
   output_low(PIN_B5);
   printf("Starting\r\n");
   
   pic_setup();

  init();
  setTimeout(200);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  setVcselPulsePeriod(VcselPeriodPreRange, 18);
  setVcselPulsePeriod(VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  setMeasurementTimingBudget(200000);
#endif
}

void loop()
{
  printf("%lu\r\n", readRangeSingleMillimeters());
  if (timeoutOccurred()) { printf(" TIMEOUT\r\n"); }

}

void main() {
   setup();
   while(1) {
      loop();
     // delay_ms(200);
   }

}


#include <VL53L0X.c>
