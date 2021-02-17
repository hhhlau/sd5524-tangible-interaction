let resources = require("./resources.json");
let moment = require("moment");
let utilsHandler = require("./utils");

const arrAverage = (arr) => arr.reduce((sume, el) => sume + el, 0) / arr.length;

let speedObj = {
  sys2_1: [],
  sys2_2: [],
};

let dataStore = {
  unit: null,
  lastRefStamp: null,
  currentRecords: [],
  displayRecords: [],
};

let referenceValues = {
  lowerTier: 2,
  midTier: 5,
  upperTier: 8,
  weighting: {
    second: 1,
    minute: 1,
    hour: 60,
    "30s": 0.5,
    "30m": 30,
    "12h": 720,
  },
};

let _lastRefValue = 0;

let displayMode = "displayRecords";

module.exports.initDataStore = function (unit) {
  // minute, hour, 30s,
  console.log(unit);
  if (["minute", "hour", "second", "30s", "30m", "12h"].includes(unit)) {
    dataStore.unit = unit;
    dataStore.currentRecords = [];
    dataStore.displayRecords = [];
    switch (unit) {
      case "minute":
        dataStore.lastRefStamp = moment().minute();
        break;
      case "hour":
        dataStore.lastRefStamp = moment().hour();
        break;
      case "second":
        dataStore.lastRefStamp = moment().second();
        break;
      case "30s":
        dataStore.lastRefStamp = moment().second();
        break;
      case "30m":
        dataStore.lastRefStamp = moment().minute();
        break;
      case "12h":
        dataStore.lastRefStamp = moment().hour();
        break;
    }
    console.log(`The data store is initialized with data internal as ${unit}`);
  } else {
    console.log("Incorrect unit specified");
  }
};

const recordHandler = function (val, client) {
  let cStamp = null;
  switch (dataStore.unit) {
    case "minute":
      cStamp = moment().minute();
      if (cStamp != dataStore.lastRefStamp) {
        dataStore.lastRefStamp = cStamp;
        dataStore.displayRecords = dataStore.currentRecords;
        dataStore.currentRecords = [];
      }

      break;
    case "hour":
      cStamp = moment().hour();
      if (cStamp != dataStore.lastRefStamp) {
        dataStore.lastRefStamp = cStamp;
        dataStore.displayRecords = dataStore.currentRecords;
        dataStore.currentRecords = [];
      }

      break;
    case "second":
      cStamp = moment().second();
      if (cStamp != dataStore.lastRefStamp) {
        dataStore.lastRefStamp = cStamp;
        dataStore.displayRecords = dataStore.currentRecords;
        dataStore.currentRecords = [];
      }

      break;
    case "30s":
      cStamp = moment().second();
      if (cStamp != dataStore.lastRefStamp) {
        dataStore.lastRefStamp = cStamp;
        dataStore.displayRecords = dataStore.currentRecords;
        dataStore.currentRecords = [];
      }

      break;
    case "30m":
      cStamp = moment().minute();
      if (cStamp != dataStore.lastRefStamp) {
        dataStore.lastRefStamp = cStamp;
        dataStore.displayRecords = dataStore.currentRecords;
        dataStore.currentRecords = [];
      }

      break;
    case "12h":
      cStamp = moment().hour();
      if (cStamp != dataStore.lastRefStamp) {
        dataStore.lastRefStamp = cStamp;
        dataStore.displayRecords = dataStore.currentRecords;
        dataStore.currentRecords = [];
      }

      break;
  }
  if (val) {
    dataStore.currentRecords.push(val);

    console.table(dataStore);
  }
  stageRenderer(client);
};

module.exports.displayHandler = function (socketIOClient, mqttClient) {
  recordHandler(null, mqttClient);
  if (["displayRecords", "currentRecords"].includes(displayMode)) {
    socketIOClient.emit("display", {
      mode: displayMode,
      unit: dataStore.unit,
      recordCount: dataStore[displayMode].length,
      rawData: dataStore[displayMode],
    });
  } else {
    console.log("-------> ! Incorrect display mode");
  }
};

module.exports.setDisplayMode = function (mode) {
  if (["displayRecords", "currentRecords"].includes(mode)) {
    displayMode = mode;
  } else {
    console.log("-------> ! Incorrect display mode is received");
  }
};

module.exports.stepParser = function (msg, id, client) {
  // console.log(msg)
  if (msg["msg"] === "stopped") {
    console.log("STOPPED");
  } else if (msg["msg"] === "pressed") {
    recordHandler(moment().unix(), client);
  }
};

module.exports.setCharacterMode = function (client, mode) {
  switch (mode) {
    case "STANDING": // no people
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 0, true);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 60);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 0, true);
      break;
    case "WALKING": // some people
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 550, false);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 50);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 130, true);
      break;
    case "JOGGING": // lots of people
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 650, false);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 40);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 200, true);
      break;
    case "RUNNING":
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 700, false);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 20);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 300, true);
      break;
  }
};

module.exports.setStageScene = function (client, stage) {
  switch (stage) {
    case 0:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#000", 100);
      utilsHandler.setServoAngle(client, "sys4_1", 0, 0); //monster
      utilsHandler.setServoAngle(client, "sys4_1", 1, 180); // House
      break;
    case 1: // no people
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#845EC2", 100);
      utilsHandler.setServoAngle(client, "sys4_1", 0, 180); //monster
      break;
    case 2:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#51C2D5", 100);
      break;
    case 3:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#D31E00", 100);
      break;
    case 4:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#FF7D09", 100);
      break;
    case 5:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#FFFFFF", 100);
      utilsHandler.setServoAngle(client, "sys4_1", 0, 180); //monster
      utilsHandler.setServoAngle(client, "sys4_1", 1, 0); // House
      break;
  }
};

module.exports.offEverything = function (client) {
  utilsHandler.setBackgroundLedColor(client, "sys3_1", "#000", 100);
  utilsHandler.setServoAngle(client, "sys4_1", 0, 180); //monster
  utilsHandler.setServoAngle(client, "sys4_1", 1, 0); // House

  utilsHandler.setBgRollerSpeed(client, "sys3_1", 0, true);
  utilsHandler.setServoAngle(client, "sys1_1", 0, 60);
  utilsHandler.setCharacterRunSpeed(client, "sys1_1", 0, true);
};

const setCharacterMode = function (client, mode) {
  switch (mode) {
    case "STANDING": // no people
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 0, true);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 60);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 0, true);
      break;
    case "WALKING": // some people
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 550, false);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 50);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 130, true);
      break;
    case "JOGGING": // lots of people
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 750, false);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 40);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 200, true);
      break;
    case "RUNNING":
      utilsHandler.setBgRollerSpeed(client, "sys3_1", 700, false);
      utilsHandler.setServoAngle(client, "sys1_1", 0, 20);
      utilsHandler.setCharacterRunSpeed(client, "sys1_1", 300, true);
      break;
  }
};

const setStageScene = function (client, stage) {
  switch (stage) {
    case 0:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#000", 100);
      utilsHandler.setServoAngle(client, "sys4_1", 0, 0); //monster
      utilsHandler.setServoAngle(client, "sys4_1", 1, 180); // House
      break;
    case 1: // no people
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#845EC2", 100);
      utilsHandler.setServoAngle(client, "sys4_1", 0, 180); //monster
      break;
    case 2:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#51C2D5", 100);
      break;
    case 3:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#D31E00", 100);
      break;
    case 4:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#FF7D09", 100);
      break;
    case 5:
      utilsHandler.setBackgroundLedColor(client, "sys3_1", "#FFFFFF", 100);
      utilsHandler.setServoAngle(client, "sys4_1", 0, 180); //monster
      utilsHandler.setServoAngle(client, "sys4_1", 1, 0); // House
      break;
  }
};

const stageRenderer = function (client) {
  let _renderValue = dataStore[displayMode].length;
  if (_renderValue !== _lastRefValue) {
    if (_renderValue == 0) {
      // no people
      console.log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> STANDING");
      setCharacterMode(client, "STANDING");
      setStageScene(client, 0);
    } else if (
      _renderValue <=
      referenceValues.lowerTier * referenceValues.weighting[dataStore.unit]
    ) {
      // few people
      console.log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> STANDING");
      setStageScene(client, 1);
      setCharacterMode(client, "STANDING");
    } else if (
      _renderValue >
        referenceValues.lowerTier * referenceValues.weighting[dataStore.unit] &&
      _renderValue <
        referenceValues.upperTier * referenceValues.weighting[dataStore.unit]
    ) {
      // some people
      console.log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> WALKING");
      setStageScene(client, 3);
      setCharacterMode(client, "WALKING");
    } else if (
      _renderValue >=
      referenceValues.upperTier * referenceValues.weighting[dataStore.unit]
    ) {
      // lot of people
      console.log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> JOGGING");
      setStageScene(client, 5);
      setCharacterMode(client, "JOGGING");
    }
    _lastRefValue = _renderValue
  }
};
