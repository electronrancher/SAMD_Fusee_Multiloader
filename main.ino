// Multiloader firmware for SAMD Loader - user can now select from 3 payloads
// 1 - hekate_ctcaer_3.2
// 2 - ReiNX chainloader
// 3 - SXOS Launcher
//
// On powerup, the board will blink the blue activity LED 1, 2, or 3 times to indicate active payload
//
// To change payload - hold the change button (D1), the device will cycle through 1, 2, then 3 blinks to
// indicate payload selection.  Let go of change button once your desired payload is indicated
//
// To store payload, momentarily press the store button (D0). The activity LED will light for 1 second to indicate flashing
// and upon the next reset, your desired payload will be automatically selected.
//
// The rest is the same, device will blink 1-3 times while searching for Tegra, then give a short-long "Ta-daa" blink
// as the payload is sent and activated.  At that point, you're done!  Unplug and switch the loader off.


#include <FlashAsEEPROM.h>
#include <FlashStorage.h>
#include <Usb.h>
#include <Arduino.h>
#include "fuseegelee.h"

// Select bin file payload here
// Must contain bin file as fuseeBin array with correct FUSEE_BIN_SIZE
#include "hekate_ctcaer_3.2.h"
#include "ReiNX.h"
#include "SXLauncher.h"

// Pin definition
// Onboard activity LED (blue) - 13
// Change payload button - 1
// Store payload config - 0
#define led 13
#define change 1
#define store 0


FlashStorage(payload_cfg, int);
int payload;     // Get payload config from flash
int gotTegra=0;

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  pinMode(change, INPUT_PULLUP);
  pinMode(store, INPUT_PULLUP);
  payload = payload_cfg.read();     // Get payload config from flash
  payload%=3;                     // Snap it to 0-2 range
  gotTegra=0;
  // Try to bring up USB
  if (usbInit() == -1){
      digitalWrite(led, HIGH);   // LED on full time means usb Init failed
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /* Enable deepsleep */
      __DSB(); /* Ensure effect of last store takes effect */
      __WFI(); /* Enter sleep mode */
  }
  
  // OK, USB is up.  Start searching for a Tegra.
  // Search for the Tegra.  Show 1-3 blinks, then pause and repeat
  gotTegra=searchTegraDevice();
  while (!gotTegra)
  {
    if (digitalRead(change)==LOW) ++payload%=3;  //If the change button is pressed, advance the payload
    if (digitalRead(store)==LOW) {
      payload_cfg.write(payload);  //If the store button is pressed, flash payload cfg
      digitalWrite(led,HIGH);
      delay(1200); //20ms on
      digitalWrite(led,LOW);
      delay(200);
    }
    for(int i=0; i<=payload; i++){
      digitalWrite(led,HIGH);
      delay(25); 
      digitalWrite(led,LOW);
      delay(175);
    }
    gotTegra=searchTegraDevice();
    delay(400); //180ms off
    
  }
  // Found a tegra, let's party
  // There is a delay here when Tegra wakes, the fast blinking stops for a moment
  // Next, you will see short/long LED blink (ta daa!) on the LED as the payload is sent and launched
  // When launching SXlauncher, keep holding + for SXOS menu if desired 
  digitalWrite(led, HIGH);                  // Short blink when starting the party (ta)
  delay(20);
  setupTegraDevice();                       // Do USB setup for fusee gelee 
  digitalWrite(led, LOW);                   // LED Off when sending 
  // Send the selected Payload
  if (payload==2)
    sendPayload(sxBin, SX_BIN_SIZE);   
  else if (payload==1)
    sendPayload(reinxBin, REINX_BIN_SIZE);  
  else 
    sendPayload(hekateBin, HEKATE_BIN_SIZE);
  digitalWrite(led, HIGH);                  // On for a medium flash when launching (daa!)
  launchPayload();                          // Launch bin file

  // Turn off LED and go into deep sleep
  digitalWrite(led,LOW);
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /* Enable deepsleep */
  __DSB(); /* Ensure effect of last store takes effect */
  __WFI(); /* Enter sleep mode */
}

void loop()
{
}
