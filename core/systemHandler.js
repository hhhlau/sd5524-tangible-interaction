let  resources = require("./resources.json")
let utilsHandler = require('./utils')



module.exports.stepParser = function (msg, client){
    // console.log(msg)
    console.log(msg['msg'])
    if (msg['msg'] === 'stopped'){
        console.log('STOPPED')
        // User stopped running

        let _targetSys = 'sys4_1'
        console.log(`-----> Resetting ${_targetSys}'s Servo to 0`)
        utilsHandler.setServoAngle(client, _targetSys, 0)

        // Should ask Sys 3 to stop rotating the background

        // Should ask Sys 1 to stop rotating feet and stand up


    }else if (msg['msg'] === 'pressed') {
        // User is moving
        let _angle = utilsHandler.getRandomIntInclusive(30, 180)
        let _targetSys = 'sys4_1'
        console.log(`-----> Turning ${_targetSys}'s Servo to ${_angle}`)
        utilsHandler.setServoAngle(client, _targetSys, _angle)
        // Should ask Sys 1 to start rotating feet and incline
        let _currentSpeed = resources.REF_AVERAGE_STEP_DISTANCE/ msg.delta

        if (_currentSpeed < (resources.REF_AVERAGE_WALKING_SPEED)){
            // tier 1: slow moving

        }else if (resources.REF_AVERAGE_WALKING_SPEED< _currentSpeed < resources.REF_AVERAGE_JOGGING_SPEED){
            // tier 2: jogging

        }else if (_currentSpeed > (resources.REF_AVERAGE_JOGGING_SPEED)){
            // tier 3: Running

        }
    }
}