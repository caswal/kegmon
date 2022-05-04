# KegScale

Based on the ideas of this project; https://brewkegscale.wordpress.com/ and https://github.com/crazynightgriffin/BrewKegScale

# This project is a work in progress

Currently this is a beta version that implements the hardware design linked above using an HX711 and OLED 0.96" display.

it's not really well tested but the features so far;

- Wifi setup is done via SSID:kegscale and password:password , enter up to 2 SSID that will be used to connect
- Web based configuration once connected to wifi. Configuration includes, tare and automatic calculation of scaling factor.
- Can show units of weight and remaning pints (keg weight and pint weight is set in configuration)
- Showing weight and pints on display with a 1 second update frequency

# Todo

* Allow user to select weight unit to show (does not really matter for the functionallity)
* Allow control of more than one scale (and perhaps more screens / or bigger screens) from one esp (need one pin for data per scale)
* Check if we loose wifi and do automatic reconnect.

# Later
* Do a ESP32 port (might be needed to attach more units)
* Implement screen saver for OLED
* Add integration to brewspy
* Write API tests
* Create documentation
* Implement async webserver so it's more responsive to fast requests
