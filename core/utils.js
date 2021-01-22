let  resources = require("./resources.json")

module.exports.setServoAngle = function (mqttClient, deviceId, angle){
    console.log(`----${resources.DEVICE_COMMAND_TOPIC_HEAD}/${deviceId}/turn----`)
    mqttClient.publish(`${resources.DEVICE_COMMAND_TOPIC_HEAD}/${deviceId}/turn`, JSON.stringify({angle}))
}

module.exports.getRandomIntInclusive = function (min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1) + min); //The maximum is inclusive and the minimum is inclusive
  }

//   module.exports {setServoAngle, getRandomIntInclusive}