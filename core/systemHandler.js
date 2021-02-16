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