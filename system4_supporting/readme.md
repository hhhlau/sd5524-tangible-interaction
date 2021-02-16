# System 4

## Descpription

## Controls (subcribing)
1. Turn the servo to an angle value : `{device_id}/turn`

    #### payload 
    - servoId: Integer ( 0 - 8 )
        
        current pins mapping 0: D8, 1: D7
    - angle: Integer (0 - 180 )

    #### example payload
    ```JSON
    // setting servo 0 (pin: D8) to 90 degrees
    {
      "servoId": 0,
      "angle": 90 
     }
    ```

## Status (publishing)
1. status: `{device_id}/status`

    #### example body
    ```JSON
    {
      
     }
    ```
