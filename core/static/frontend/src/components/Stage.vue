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
        <div id="character-body"></div>
        <div class="feet" id="character-feet"></div>
      </div>
    </div>
    <div class="info-container">
      <button @click="toggleUiSpeed">Add 1</button>
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
      characterBodyEl: null,

      isStarted: false,
      currentSpeed: 0,
      refDistance: 1000,
    };
  },
  mounted() {
    this.rollerBgEl = document.getElementById("roller-bg");
    this.characterFeetEl = document.getElementById("character-feet");
    this.characterBodyEl = document.getElementById("character-body");
    socket.on("ping", (payload) => {
      console.log(payload);
    });
  },
  watch: {
    currentSpeed() {
      switch (this.currentSpeed) {
        case 0:
          this.characterBodyEl.style.transform = "rotate(0)";
          this.rollerBgEl.classList.remove("running-bg");
          this.characterFeetEl.classList.remove("running-feet");
          break;
        case 1:
          this.characterBodyEl.style.transform = "rotate(5deg)";
          this.rollerBgEl.classList.add("walking-bg");
          this.characterFeetEl.classList.add("walking-feet");
          break;

        case 2:
          this.characterBodyEl.style.transform = "rotate(10deg)";
          this.rollerBgEl.classList.remove("walking-bg");
          this.rollerBgEl.classList.add("jogging-bg");

          this.characterFeetEl.classList.remove("walking-feet");
          this.characterFeetEl.classList.add("jogging-feet");
          break;

        case 3:
          this.characterBodyEl.style.transform = "rotate(30deg)";
          this.rollerBgEl.classList.remove("jogging-bg");
          this.rollerBgEl.classList.add("running-bg");

          this.characterFeetEl.classList.remove("jogging-feet");
          this.characterFeetEl.classList.add("running-feet");
          break;
      }
    },
  },
  methods: {
    toggleUiSpeed() {
      if (this.currentSpeed < 3) {
        this.currentSpeed++;
      } else {
        this.currentSpeed = 0;
      }
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

#character-body {
  width: 100%;
  height: 300px;
  background-color: red;
  transform-origin: bottom center;
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
