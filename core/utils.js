 let  resources = require("./resources.json")
 const hexRgb = require('hex-rgb');

module.exports.setServoAngle = function (mqttClient, deviceId, servoId, angle){
  let toTopic = `${resources.DEVICE_COMMAND_TOPIC_HEAD}/${deviceId}/turn`
    console.log(`----${toTopic}----`)
    mqttClient.publish(toTopic, JSON.stringify({servoId, angle}))
}

module.exports.setCharacterRunSpeed = function(mqttClient,deviceId, speed, isClockwise) {
  let toTopic = `${resources.DEVICE_COMMAND_TOPIC_HEAD}/${deviceId}/run`
  console.log(`----${toTopic}----`)
  mqttClient.publish(toTopic, JSON.stringify({speed, isClockwise}))
}

module.exports.setBackgroundLedColor= function(mqttClient, deviceId, hexCode, brightness) {
  let toTopic = `${resources.DEVICE_COMMAND_TOPIC_HEAD}/${deviceId}/pixel/set`
  console.log(`----${toTopic}----`)
  let _rgb = hexRgb(hexCode)
  delete _rgb['alpha']
  console.log(`Converting ${hexCode} to ${JSON.stringify(_rgb)}`)
  mqttClient.publish(toTopic, JSON.stringify({..._rgb, brightness}))
}

module.exports.setBgRollerSpeed= function(mqttClient,deviceId, speed, isClockwise) {
  let toTopic = `${resources.DEVICE_COMMAND_TOPIC_HEAD}/${deviceId}/turn`
  console.log(`----${toTopic}----`)
  mqttClient.publish(toTopic, JSON.stringify({speed, isClockwise}))
}

module.exports.getRandomIntInclusive = function (min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1) + min); //The maximum is inclusive and the minimum is inclusive
  }

//   module.exports {setServoAngle, getRandomIntInclusive}