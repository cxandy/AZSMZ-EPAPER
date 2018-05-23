## Download and prepare the Arduino IDE: ##

`1.1`  Download Arduino IDE from https://www.arduino.cc/en/Main/Software

`1.2`  Install the Arduino IDE

`2.`  Open the Arduino IDE and click File, Preferences   
`2.1`   In the "Additional Boards Manager URL field:  Paste: http://arduino.esp8266.com/stable/package_esp8266com_index.json   
`2.2`   Click OK  
`2.3`   Click Tools -> Boards -> Board Manager   
`2.4`   Scroll to near the bottom, and find "esp8266 by ESP8266 Community) and click on the row   
`2.5`   On the "Select Version" dropdown, select version 2.3.0 and click Install   
`2.6`   Wait for the ESP8266 support to be installed   

## Download the code ##

`3.`   Download this project:    
https://github.com/cxandy/espaper-weatherstation/         
`3.1`   Extract it to your sketchbook or other location     

## Initial Configuration ##

`4.`   Open the Arduino IDE and open espaper-weatherstation.ino    
`4.1` edit setting. h file 
    
      if module is AZSMZ EPaper 2.9":
             //#define AZSMZ_EPAPER_MINI 21
      
      if module is AZSMZ EPaper 2.9" Mini:    
             #define AZSMZ_EPAPER_MINI 21  

## Upload the code ##
`5.` Configure your Board:    
  NodeMCU 1.0(ESP-12E Module)    
  Make sure you have the clock speed set to 160Mhz    
  Upload speed set to 921600 (try others speed if upload errors. such as 115200)    
`5.1` Upload the sketch:  Click the Upload button in Arduino IDE (Or press Ctrl+U)
