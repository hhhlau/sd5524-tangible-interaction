# System 3

## Descpription
[NEO Pixel Doc](https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html#a74e917093c2eb16848c4a09427680f79)
## Controls (subcribing)
1. Control the background roller : `{device_id}/turn`

    #### payload 
    - motorId: Integer ( 0 - 8 )
    - speed: Integer (0 - 1023 )
    - isClockwise: BOOL

    #### example payload
    ```JSON
    // To set the motor 0 with speed 500 in clockwise
    {
      "motorId": 0,
      "speed": 500,
      "isClockwise": true
    }
    ```
2. Control the background LED projection : `{device_id}/pixel/set`

    #### payload 
    - red: Integer ( 0 - 255 )
    - green: Integer ( 0 - 255 )
    - blue: Integer ( 0 - 255 )
    - brightness: Integer (0 - 100, optional)
    
    _To turn off the light, send a black color (0,0,0) as payload_

    #### example payload
    ```JSON
    // To turn the strip to pure red @ 75% brightness
    {
      "red":255, 
      "green":0, 
      "blue": 0,
      "brightness": 75

    }
    ```
3. Init and reset the total number of pixels : `{device_id}/pixel/init`


    #### payload
    - numOfPixels: Integer (optional)

    #### example payload
    ```JSON
    // To change the total number of pixels to 16
    {
      "numOfPixels": 16
    }
    ```

## Status (publishing)
1. status: `{device_id}/status`

    #### example body
    ```JSON
    {
      
     }
    ```
