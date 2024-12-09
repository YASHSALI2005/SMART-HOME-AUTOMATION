 
        <!DOCTYPE html>
        <html lang="en">
        <head>
        <!-- data:image/png;base64, -->
        <!-- <meta charset="UTF-8" /> -->
        <!-- <meta name="viewport" content="width=device-width, initial-scale=1.0" /> -->
        <!-- <link rel="stylesheet" href="style.css" /> -->
        <title>Intellihub</title>
        <style>
        * {
          margin: 0;
          background-color: #d9d9d9;
        }

        .top {
          height: 12vh;
          width: 100vw;
          background-color: #a9f0ff;
          position: fixed;
          top: 0;
          display: flex;
          align-items: center;
          justify-content: space-between;
        }

        .bottom {
          height: 8vh;
          width: 100vw;
          background-color: #a9f0ff;
          position: fixed;
          bottom: 0;
        }

        .mainBody {
          background-color: rgba(148, 148, 148, 0.2);
          position: relative;
          margin-top: 8vh;
          height: 70vh;
          width: 70vw;
          margin: 15vh 15vw;
        }

        img {
          height: 150px;
          width: 150px;
          background-color: transparent;
        }

        .logo {
          width: 79px;
          height: 86px;
        }

        .logowrapper {
          margin-left: 20px;
          height: 86px;
          width: 79px;
          border-radius: 10px;
          display: inline-block;
        }

        .topcontent {
          display: inline;
          font-family: Calibri;
          background-color: transparent;
          font-size: 24px;
          margin-right: 20px;
        }

        .abs {
          position: absolute;
        }

        .fanon,
        .fanoff {
          margin-left: 33vw;
          margin-top: 25vh;
        }

        .lighton,
        .lightoff {
          bottom: 0;
          left: 0;
        }

        .door {
          right: 15px;
          opacity: 0.5;
        }

        .acon,
        .acoff {
        left: -40px;
        rotate: -90deg;
        width: 200px;
        margin-top: 200px;
        }

        .bed {
        bottom: -40px;
        left: 5vw;
        rotate: -90deg;
        width: 300px;
        height: 300px;
        opacity: 0.3;
        }

        .lighton,
        .acon,
        .fanon {
        display: none;
        }

        .lightoff {
        display: inline;
        margin-left: 33vw;
        }

        .lighton {
        margin-left: 33vw;
        }
        .wrapper{
          position: absolute;
        }
        #pi_input{
        rotate:-90deg ;
        margin-left: 41vw;
        margin-top: 25vh; 
        position: absolute;
        
        }
        .value{
        position: absolute;
        margin-left: 47vw;
        margin-top: 25vh;
        font-size:135%;
        background-color: transparent;
        }
        </style>
        </head>
        <body>
        <div class="top">
        <div class="logowrapper"><img class="logo" src="logo.png" alt="" /></div>
        <h3 class="topcontent">Welcome Neeel</h3>
        </div>

        <div class="mainBody">
        <img
        class="fanoff abs"
        id="fanOff"
        onclick="toggleApp('fanOn','fanOff')"
        src="fanOFF.png"
        />
       <div class="wrapper">
        <input id="pi_input" type="range" min="0" max="4" value="0" step="1" onchange="setSliderValue(this.value)">
        <p class="value">Value: <output id="value"></output></p>
        </div>
      
        <img
        class="fanon absn"
        id="fanOn"
        onclick="toggleApp('fanOn','fanOff')"
        style="display:none"
        src="fanON .png"
        />

        <img
        class="lightoff abs"
        id="lightOff"
        onclick="toggleApp('lightOn','lightOff')"
        src="lightOff.png"
        />

        <img
        class="lighton abs"
        id="lightOn"
        onclick="toggleApp('lightOn','lightOff')"
        style="display:none"
        src="lightOn.png"
        />

        <img
        class="acoff abs"
        id="acOff"
        onclick="toggleApp('acOn','acOff')"
        src="ACoff.png"
        />

        <img
        class="acon abs"
        id="acOn"
        onclick="toggleApp('acOn','acOff')"
        style="display:none"
        src="ACon.png"
        />
      
        <img class="door abs" src="door.png" />

        <img src="bed.png" alt="" class="bed abs" />
        </div>

        <div class="bottom"></div>

        <script>

        var lightCount = 0;
        var fanCount = 0;
        var acCount = 0;
        var fanValue=0;


            var socket = new WebSocket('ws://192.168.229.19:8000');
            socket.onmessage = function(event) {
            console.log(event.data);


            //code to toggle the images
            const data = event.data.split(":");
            
            const Data = data[0] || "";
            const equ = data[1] || "" ;
            const up1= data[1] || "";
            const up2= data[2] || "";
            const up3= data[3] || "";
            const up4= data[4] || "";

            if(equ == "light" && lightCount != Data){
                lightCount=Data;
                console.log(lightCount);
                toggleImage('lightOn','lightOff');
            }
            else if(equ=="fan" && fanCount != Data){
                fanCount=Data;
                toggleImage('fanOn','fanOff');

                document.getElementById("pi_input").value = fanCount;
                fanValue = fanCount;
                setvalue();
            }
            else if(equ=="ac" && acCount != Data){
                acCount=Data;
                toggleImage('acOn','acOff');
            
              }
              else if(equ=="fanvar" && fanValue != Data)
              {
                document.getElementById("pi_input").value = Data;
                fanValue = Data;
                console.log("count" + fanValue);
                if((fanCount==0 && 1<=fanValue)||(fanCount==1 && fanValue==0)){
                  toggleApp('fanOn','fanOff');
                }
                setvalue();
              }
        
        
              else if(Data == "update"){
              if(lightCount!=up1){
                lightCount=up1;
                toggleImage('lightOn','lightOff');
              }
              else if(fanCount!=up2){
                fanCount=up2;
                toggleImage('fanOn','fanOff');
              }
              else if(acCount!=up3){
                acCount=up3;
                toggleImage('acOn','acOff');
            }
            else if (fanValue !=up4){
              setSliderValue(up4);
            }
          }
        
      } ;     
      


      function toggleImage(t1,t2){
        var onImg = document.getElementById(t1);
        var offImg = document.getElementById(t2);
        
        onImg.style.display = (onImg.style.display === "none") ? "inline" : "none";
        offImg.style.display = (onImg.style.display === "none") ? "inline" : "none";
        

      }

      
     
      function toggleApp(t1, t2) {
        var onImg = document.getElementById(t1);
        var offImg = document.getElementById(t2);
        
        onImg.style.display = (onImg.style.display === "none") ? "inline" : "none";
        offImg.style.display = (onImg.style.display === "none") ? "inline" : "none";

        // Increment counts
        if (t1 === 'fanOn' || t1 === 'fanOff') {
          fanCount = (fanCount == 1) ? 0 : 1;
          if((fanCount==1 && fanValue==0)||(fanCount==0 && fanValue>0)){
            document.getElementById("pi_input").value = fanCount;
                fanValue = fanCount;
                sendCountsToArduino("fanvar");
          }
          sendCountsToArduino("fan");
        } else if (t1 === 'lightOn' || t1 === 'lightOff') {
          lightCount = (lightCount == 1) ? 0 : 1;
          sendCountsToArduino("light");
        }else if (t1 === 'acOn' || t1 === 'acOff') {
          acCount = (acCount == 1) ? 0 : 1;
          sendCountsToArduino("ac");
        }
        setvalue();
      }


      function setvalue(){
        const value = document.querySelector("#value");
        const input = document.querySelector("#pi_input");
        value.textContent = input.value;
        input.addEventListener("input", (event) => {
         value.textContent = event.target.value;
            });
      }

        function setSliderValue(value) {
        fanValue = parseInt(value);
        socket.send(fanValue+":fanvar:esp:localhost");
        if((fanCount==1 && fanValue==0)||(fanCount==0 && fanValue>0)){
                document.getElementById("pi_input").value = fanValue;
                toggleApp('fanOn','fanOff');
              }
          }

      const value = document.querySelector("#value");
        const input = document.querySelector("#pi_input");
        value.textContent = input.value;
        input.addEventListener("input", (event) => {
         value.textContent = event.target.value;
            });

      function sendCountsToArduino(equ) {
        if(equ=="light"){
        socket.send(lightCount + ":lightCount:esp:localhost")
        }
        else if(equ=="fan"){
          socket.send(fanCount + ":fanCount:esp:localhost")
        }
        else if(equ=="ac"){
          socket.send(acCount + ":acCount:esp:localhost")
        }
        else if(equ=="fanvar"){
          socket.send(fanValue+":fanvar:esp:localhost")
        }
      }

      
    </script>
  </body>
</html>


<?php
?>