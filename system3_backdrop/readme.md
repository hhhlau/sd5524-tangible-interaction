# System 3

## Descpription

## Controls (subcribing)
1. Control the background roller : `{device_id}/turn`

    #### payload 
    - motorId: Integer ( 0 - 8 )
    - speed: Integer (0 - 1023 )
    - isClockwise: BOOL

    #### example
    ```JSON
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
    
    _To turn off the light, send a black color (0,0,0) as payload_

    #### example
    ```JSON
    {
      "red":255, 
      "green":0, 
      "blue": 0
    }
    ```

## Status (publishing)
1. status: `{device_id}/status`

    #### example body
    ```JSON
    {
      
     }
    ```
