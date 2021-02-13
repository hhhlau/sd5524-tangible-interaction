# System 4

## Descpription

## Controls (subcribing)
1. Turn the character to an angle value : `{device_id}/turn`

    #### payload 
    - servoId: Integer ( 0 - 8 )
    - angle: Integer (0 - 180 )

    #### example
    ```JSON
    {
      "servoId": 0,
      "angle": 90 
     }
    ```
2. To make the character run : `{device_id}/run`
   
    #### payload
      - speed: Integer ( 0 - 1023, default= 550 ) 
      - isClockwise: BOOL (default= true)

    #### example payload
    ```JSON
    // To set the motor 0 with speed 500 in clockwise
    {
      "motorId": 0,
      "speed": 500,
      "isClockwise": true
    }
    ```

