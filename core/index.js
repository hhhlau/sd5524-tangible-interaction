var mqtt = require('mqtt')
let moment = require('moment')
let resources = require("./resources.json")
let utilsHandler = require('./utils')
let sysHandler = require('./systemHandler')

var client  = mqtt.connect(`mqtt://${resources.MQTT_BROKER_ENDPOINT}:${resources.MQTT_BROKER_PORT}`, {clientId: resources.CORE_CLIENT_ID})
 
client.on('connect', function () {
  client.subscribe(resources.GENERAL_STATUS_TOPIC, function (err) {
    if (!err) {
      console.log(`Subcribed to ${resources.GENERAL_STATUS_TOPIC}`)
    }

  })

  client.subscribe(resources.DEVICE_LEVEL_STATUS_TOPIC, function(err) {
    if (!err) {
      console.log(`Subcribed to ${resources.DEVICE_LEVEL_STATUS_TOPIC}`)
    }
  })
})
 
client.on('message', function (topic, message) {
  // message is Buffer
  console.log("--------------------------------")
  console.log(topic)
  let _msg = JSON.parse(message.toString())
  // _msg['testp'] = JSON.parse(_msg.test)
  _msg['arrivedAt'] = moment().format()
  console.table(_msg)
  console.log("--------------------------------\n\n")
  // client.end()

  switch (topic) {
    case 'sd5524/2/device/sys2_1/status':
      sysHandler.stepParser(_msg, client)
  }
})