let  resources = require("./resources.json")
let utilsHandler = require('./utils')

const arrAverage = arr => arr.reduce((sume, el) => sume + el, 0) / arr.length;

let speedObj = {
    sys2_1: [],
    sys2_2:[]
}

speedHandler = function(id, val) {
    if (id !== 'sys2_2'){
    speedObj[id].push(Math.round((val + Number.EPSILON) * 100) / 100)
        if (speedObj[id].length > 5){
            speedObj[id].shift()
        }

        speedObj[`${id}AverageSpeed`] = Math.round((arrAverage(speedObj[id]) + Number.EPSILON) * 100) / 100


        console.table(speedObj)}
}


module.exports.stepParser = function (msg,id, client){
    // console.log(msg)
    console.log(msg['msg'])
    if (msg['msg'] === 'stopped'){
        console.log('STOPPED')
        // User stopped running
        speedHandler(id, 0)
        let _targetSys = 'sys4_1'
        // console.log(`-----> Resetting ${_targetSys}'s Servo to 0`)
        // utilsHandler.setServoAngle(client, _targetSys,0, 0)

        // Should ask Sys 3 to stop rotating the background

        // Should ask Sys 1 to stop rotating feet and stand up
        // utilsHandler.setServoAngle(client, 'sys1_1', 0, 0)


    }else if (msg['msg'] === 'pressed') {
        // User is moving
        // let _angle = utilsHandler.getRandomIntInclusive(30, 180)
        // speedObj[id].push(resources.REF_AVERAGE_STEP_DISTANCE/(msg['delta']/1000))
        let _targetSys = 'sys4_1'
        // console.log(`-----> Turning ${_targetSys}'s Servo to ${_angle}`)
        // utilsHandler.setServoAngle(client, _targetSys, _angle)
        // Should ask Sys 1 to start rotating feet and incline
        let _dSec = (msg.delta - resources.ARDUINO_DELAY_BUFFER)/1000
        let _currentSpeed = resources.REF_AVERAGE_STEP_DISTANCE/ _dSec
        speedHandler(id, _currentSpeed)
        if (_currentSpeed < (resources.REF_AVERAGE_WALKING_SPEED)){
            // tier 1: slow moving

        }else if (resources.REF_AVERAGE_WALKING_SPEED< _currentSpeed < resources.REF_AVERAGE_JOGGING_SPEED){
            // tier 2: jogging

        }else if (_currentSpeed > (resources.REF_AVERAGE_JOGGING_SPEED)){
            // tier 3: Running

        }
    }
}

module.exports.setCharacterMode = function (client,mode){
    switch (mode){
        case "STANDING":
            utilsHandler.setBgRollerSpeed(client, "sys3_1", 0, true)
            utilsHandler.setServoAngle(client, "sys1_1", 0,60)
            utilsHandler.setCharacterRunSpeed(client, "sys1_1", 0, true)
            break 
        case "WALKING":
            utilsHandler.setBgRollerSpeed(client, "sys3_1", 450, false)
            utilsHandler.setServoAngle(client, "sys1_1", 0,50)
            utilsHandler.setCharacterRunSpeed(client, "sys1_1", 130, true)
            break
        case "JOGGING":
            utilsHandler.setBgRollerSpeed(client, "sys3_1", 550, false)
            utilsHandler.setServoAngle(client, "sys1_1", 0,40)
            utilsHandler.setCharacterRunSpeed(client, "sys1_1", 180, true)
            break
        case "RUNNING": 
            utilsHandler.setBgRollerSpeed(client, "sys3_1", 700, false)
            utilsHandler.setServoAngle(client, "sys1_1", 0,20)
            utilsHandler.setCharacterRunSpeed(client, "sys1_1", 300, true)
        break
    }
}

module.exports.setStageScene = function (client,stage) {
    switch(stage) {
        case 0:
            utilsHandler.setBackgroundLedColor(client, "sys3_1", "#000", 100)
            utilsHandler.setServoAngle(client, "sys4_1", 0, 0) //monster
            utilsHandler.setServoAngle(client, "sys4_1", 1, 180) // House
            break
        case 1:
            utilsHandler.setBackgroundLedColor(client, "sys3_1", "#845EC2", 100)
            utilsHandler.setServoAngle(client, "sys4_1", 0, 180) //monster
            break
        case 2:
            utilsHandler.setBackgroundLedColor(client, "sys3_1", "#51C2D5", 100)
            break
        case 3:
            utilsHandler.setBackgroundLedColor(client, "sys3_1", "#D31E00", 100)
            break
        case 4:
            utilsHandler.setBackgroundLedColor(client, "sys3_1", "#FF7D09", 100)
            break
        case 5:
            utilsHandler.setBackgroundLedColor(client, "sys3_1", "#FFFFFF", 100)
            utilsHandler.setServoAngle(client, "sys4_1", 0, 180) //monster
            utilsHandler.setServoAngle(client, "sys4_1", 1, 0) // House
            break
    }
}

module.exports.offEverything = function (client){
    utilsHandler.setBackgroundLedColor(client, "sys3_1", "#000", 100)
    utilsHandler.setServoAngle(client, "sys4_1", 0, 180) //monster
    utilsHandler.setServoAngle(client, "sys4_1", 1, 0) // House

    utilsHandler.setBgRollerSpeed(client, "sys3_1", 0, true)
    utilsHandler.setServoAngle(client, "sys1_1", 0,60)
    utilsHandler.setCharacterRunSpeed(client, "sys1_1", 0, true)
}