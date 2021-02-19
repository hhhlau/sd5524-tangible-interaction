var mqtt = require('mqtt')
let moment = require('moment')
let resources = require("./resources.json")
let utilsHandler = require('./utils')
let sysHandler = require('./systemHandler')

const httpServer = require("http").createServer();
const io = require("socket.io")(httpServer, {
  cors: {
    origin: ["http://158.132.54.138:8080", "http://localhost:8080"],
  },
});

let counterUnit = "minute"

sysHandler.initDataStore(counterUnit)

io.on("connection", (socket) => {
  // console.log(socket)
  socket.on("toggleStageScene", (data) => {
    console.log("!!!--> toggleStageScene to :", data)
    sysHandler.setStageScene(client,data)
  })

  socket.on("toggleCharacterMode", (data) => {
    console.log("!!!--> toggleCharacterMode to :", data)
    sysHandler.setCharacterMode(client,data)
  })

  socket.on("setDisplayMode", (data)=> {
    sysHandler.setDisplayMode(data)
    console.log(`!!!--> Changed display mode: ${data}`)
  })

  socket.on("testTier", (data)=> {
    sysHandler.stageRenderer_p(client,data)
  })

  socket.on("offEverything", () => {
    console.log("!!!--> offEverything")
    sysHandler.offEverything(client)
  })
});



httpServer.listen(3000);

var client  = mqtt.connect(`mqtt://${resources.MQTT_BROKER_ENDPOINT}:${resources.MQTT_BROKER_PORT}`, {clientId: resources.CORE_CLIENT_ID})
 
client.once('connect', function () {
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
  // console.log("--------------------------------")
  // console.log(topic)
  let _msg = JSON.parse(message.toString())
  // // _msg['testp'] = JSON.parse(_msg.test)
  _msg['arrivedAt'] = moment().format()
  // // console.table(_msg)
  // console.log("--------------------------------\n\n")
  // // client.end()

  switch (topic) {
    case 'sd5524/2/device/sys2_1/status':
      sysHandler.stepParser(_msg,'sys2_1', client)
      break
      case 'sd5524/2/device/sys2_2/status':
        // sysHandler.stepParser(_msg,'sys2_2', client)
        break
  }
  sysHandler.displayHandler(io.sockets, client)
})

setInterval(()=> {
  // Regularly chacking status
  sysHandler.displayHandler(io.sockets, client)
}, 1000)