<template>
  <div class="master-container" id="app">
    <div class="stage-container">
      <div class="background-container">
        <div
          class="bg"
          id="roller-bg"
          :style="`background-image: url(${require('@/assets/test-background.jpg')})`"
        ></div>
      </div>
      <div class="character-container">
        <div class="body"></div>
        <div class="feet" id="character-feet"></div>
      </div>
    </div>
    <div class="info-container">
      <button @click="testBtn">Add 1</button>
    </div>
  </div>
</template>

<script>
import { io } from "socket.io-client";

const URL = "http://localhost:3000";
const socket = io(URL);
socket.on("connection", (socket) => {
  console.log(socket.handshake.auth); // prints { token: "abcd" }
});

export default {
  name: "Stage",
  data() {
    return {
      rollerBgEl: null,
      characterFeetEl: null,

      isStarted: false,
      currentSpeed: 0,
      refDistance: 1000,
    };
  },
  mounted() {
    this.rollerBgEl = document.getElementById("roller-bg");
    socket.on("ping", (payload) => {
      console.log(payload);
    });
    // this.characterFeetEl = document.getElementById("character-feet");
  },
  methods: {
    testBtn() {
      let _bgClassList = this.rollerBgEl.classList;
      // let _feetClassList = this.characterFeetEl.classList;
      if (Object.values(_bgClassList).includes("walking-bg")) {
        this.rollerBgEl.classList.remove("walking-bg");
        this.rollerBgEl.classList.add("jogging-bg");

        this.characterFeetEl.classList.remove("walking-feet");
        this.characterFeet.classList.add("jogging-feet");
      } else if (Object.values(_bgClassList).includes("jogging-bg")) {
        this.rollerBgEl.classList.remove("jogging-bg");
        this.rollerBgEl.classList.add("running-bg");

        this.characterFeetEl.classList.remove("jogging-feet");
        this.characterFeet.classList.add("running-feet");
      } else if (Object.values(_bgClassList).includes("running-bg")) {
        this.rollerBgEl.classList.remove("running-bg");
        this.characterFeetEl.classList.remove("running-feet");
      } else {
        this.rollerBgEl.classList.add("walking-bg");
        this.characterFeet.classList.add("walking-feet");
      }
      // classList.remove('horizTranslate');
      // let _computedStyleBg = window.getComputedStyle(this.rollerBgEl);
      console.log(this.characterFeetEl);
    },
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.master-container {
  width: 100vw;
  height: 100vh;
}

.stage-container {
  width: 100%;
  height: 80%;
  background-color: antiquewhite;
  display: flex;
  justify-content: center;
  align-items: flex-end;
  position: relative;
}

.background-container {
  background-color: palegreen;
  position: absolute;
  width: 100%;
  height: 100%;
  overflow: hidden;
}
#roller-bg {
  position: absolute;
  width: 100%;
  height: 100%;
  background-repeat: repeat-x;
  background-position: left bottom;
  object-fit: contain;
}

.rolling {
  animation: bg-rolling 2s infinite linear;
}

.walking-bg {
  animation: bg-rolling 8s infinite linear;
}
.walking-feet {
  animation: rotation 8s infinite linear;
}

.jogging-bg {
  animation: bg-rolling 4s infinite linear;
}
.jogging-feet {
  animation: rotation 8s infinite linear;
}

.running-bg {
  animation: bg-rolling 2s infinite linear;
}
.running-feet {
  animation: rotation 2s infinite linear;
}

@keyframes bg-rolling {
  from {
    background-position: left bottom;
  }
  to {
    background-position: right bottom;
  }
}

.character-container {
  max-width: 30vw;
  width: 50%;
  height: 500px;
  display: flex;
  justify-content: center;
  position: relative;
}

.character-container > .body {
  width: 100%;
  height: 300px;
  background-color: red;
}

#character-feet {
  width: 200px;
  height: 200px;
  background-color: blue;
  position: absolute;
  top: 250px;
}

@keyframes rotation {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(359deg);
  }
}

.info-container {
  width: 100%;
  height: 20%;
  background-color: gray;
}
</style>
