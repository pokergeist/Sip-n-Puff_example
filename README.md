# Sip-n-Puff Example

## Intro

This repos exists for the sole purpose of helping a new user jumpstart his Assistive Technology project.

## Affiliates

[ATMakers.org](http://www.ATMakers.org) and the "ATMakers - Makers and Users" Facebook group.

Please see the videos linked to in the Summary box on this Thingiverse [post](https://www.thingiverse.com/thing:3458117).

## Remarks

The Sip-n-Puff.ino [code](Sip-n-Puff/Sip-n-Puff.ino) is heavily commented. Start there.

The sensor libraries are largely interchangeable and either can be used since they're accessing the same device via I2C.

## Status

|    Date    | Status                                                       |
| :--------: | ------------------------------------------------------------ |
| 2022-11-26 | Added Neopixel code. Tested Adafruit device with both drivers. |
| 2022-11-25 | Added support for the SparkFun MicroPressure (Qwiic) sensor. |
| 2022-11-25 | It compiles. No Neopixel support yet.                        |

## Load the Program on the QT Py

* **Copy** the Sip-n-Puff directory into your Arduino directory (usually in My Documents). Open in the Arduino IDE.
* **Open** the Arduino **IDE**.
* In the IDE, "**Open**" the Sip-n-Puff **sketch folder**. That will load the .ino file in the editor.
* Run Sketch / Verify/Compile, first button, or Cntl-R to **compile** the code.
* If you're missing a library:
  * Tools/Manage Libraries ... (Cntl-Shift-I) and wait for it to pop up
  * type mprls or micropressure in the text box, third field to the right
  * the library will pop up in the list. Cursor over it and click install.
  * close the Library Manager Window. Repeat the compile step.
* **Connect** the QT Py microcontroller to computer via the USB-C cable.
* Select the **COM port** via Tools/Port. The correct one will show the device name too.
* Run Sketch/Upload, second button, or Cntl-U to compile and **upload** the program to the QT Py.
* Launch the **Console** window with Tools/Serial Monitor, the button on the far right, or Cntl-Shift-M. You have to have the Console window open for the program to run. Note: the Console (SerialMonitor) window will have to be closed and relaunched if you hit the MCU's reset button. If you just reload code it will reconnect.

## Run the Program

Connect the TQT Py MCU and the sensor. Connect the Sip and Puff tubing to the sensor tube.

Connect the MCU to the computer and launch the Serial Monitor. It will tell you to wait while it gets a baseline pressure reading.

Once that is done it will tell you it's ready for use.

Blow lightly into the tube to turn on the green LED, and the relay/device if it's wired to the MCU. The pin is set to digital pin 0 (or "A0") for now. There are prompts in the console as well.

Suck lightly on the tube. The LED will turn red for a second. The connected device will turn off.

If the device is turned on continuously for 25 seconds the safety lockout will engage, turn off the device, and the LED will blink yellow. You'll have to reset the MCU and the Console to clear the lockout.

## Proposed Components

### Pressure Sensor

Adafruit MPRLS Pressure Sensor [#3965](https://www.adafruit.com/product/3965) or SparkFun [SEN-16476](https://www.sparkfun.com/products/16476).

### Microcontroller

Adafruit QT Py SAMD21 [#4600](https://www.adafruit.com/product/4600). **SELECTED**

### Low-side Switch

5-36V 15A N-channel MOSFET on [Amzn](https://smile.amazon.com/dp/B07XJSRY6B).

### Solenoid

12V 0.3A 10mm stroke 4N [Amzn](https://www.amazon.com/dp/B07Z3HYDBV)

### Tubing

The sensor tube is 2.5mm O.D. Silicone tubing with that inner diameter works well. Use whatever the O.T.s recommend.

## Schematic

The following wiring diagram has been put forth. The microcontroller (IC) pinouts are not particularly cromulent.

![schematic](assets/schematic.png)
