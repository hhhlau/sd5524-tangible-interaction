# Core
## Requirement
nodeJS v12+

## Set up
- Create `resources.json` in the root of the core folder with below 
```JSON
{
    "MQTT_BROKER_ENDPOINT": "YOUR_BROKER_ENDPOINT",
    "MQTT_BROKER_PORT": 1883,
    "CORE_CLIENT_ID": "YOUR_MQTT_CLIENT_ID",
    "GENERAL_STATUS_TOPIC": "sd5524/2/status",
    "DEVICE_LEVEL_STATUS_TOPIC": "sd5524/2/device/+/status",
    "DEVICE_COMMAND_TOPIC_HEAD": "sd5524/2/device",

    "REF_AVERAGE_STEP_DISTANCE": 0.65,
    "REF_AVERAGE_WALKING_SPEED": 1.37,
    "REF_AVERAGE_JOGGING_SPEED": 1.79,
    "REF_AVERAGE_SPEED_UNIT": "m/s"
}

```

## Install and run
To start the core service
```bash
    yarn install
    yarn nodemon
```

To start to dev UI
```
cd static/frontend

yarn install

yarn serve
```