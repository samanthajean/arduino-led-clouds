//============
//Webpage Code
//============
String webpageCode = R"***(
<!DOCTYPE html>
<head>
  <title> Sunshine House Cloud Control </title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.0/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KyZXEAg3QhqLMpG8r+8fhAXLRk2vvoC2f3B09zVXn8CA5QIVfZOJ3BCsw2P0p/We" crossorigin="anonymous">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.0/dist/js/bootstrap.bundle.min.js" integrity="sha384-U1DAWAznBHeqEIlVSCgzq+c9gqGAJn5c/t99JyeKa9xxaYpSvHU5awsuZVVFIhvj" crossorigin="anonymous"></script>
</head>
<html>
<!----------------------------CSS---------------------------->
<style>
  
</style>
<!----------------------------HTML--------------------------->
<body>
  <div class="container">
    <div class="row">
    <div class="col">
    <h1><span>Cloud Control to Major Tom</span></h1>
    </div>
    </div>
    <div class="row">
    <div class="col-8">
      <button type="button" class="btn btn-primary btn-lg" onclick="sendData(1)">LED ON</button>
      <button type="button" class="btn btn-outline-secondary btn-lg" onclick="sendData(0)">LED OFF</button>
      <p>LED State is : <span id="LEDState"> </span></p>
      <div class="d-grid gap-2 d-md-block">
        <button type="button" class="btn btn-outline-primary btn-sm" onclick="triggerPattern(0)" data-bs-toggle="button" autocomplete="off">Solid</button>
        <button type="button" class="btn btn-outline-success btn-sm" onclick="triggerPattern(1)" data-bs-toggle="button" autocomplete="off">Rainbow Cycle</button>
        <button type="button" class="btn btn-outline-warning btn-sm" onclick="triggerPattern(3)" data-bs-toggle="button" autocomplete="off">Lightning</button>
        <button type="button" class="btn btn-outline-info btn-sm" onclick="triggerPattern(4)" data-bs-toggle="button" autocomplete="off">Breath Sync</button>
      </div>
    </div>
     <div class="col-4">
      <label for="ColorInput1" class="form-label">Color 1</label>
      <input type="color" class="form-control form-control-color" id="ColorInput1" value="#ffffff" title="Choose your color" oninput="setColor(1)">

      <p id="color1"></p>

      <label for="ColorInput2" class="form-label">Color 2</label>
      <input type="color" class="form-control form-control-color" id="ColorInput2" value="#b58fdc" title="Choose your color" oninput="setColor(2)">

      <p id="color2"></p>
      
      <label for="ColorInput3" class="form-label">Color 3</label>
      <input type="color" class="form-control form-control-color" id="ColorInput3" value="#3b0372" title="Choose your color" oninput="setColor(3)">
      <p id="color3"> </p>
    </div>
    </div>
  </div>
<!-------------------------JavaScript------------------------->
  <script>

  (function(){
    var picker = document.getElementById('ColorInput1'),
            tog = false;


  picker.addEventListener('focus', function(){
    if(tog === true){
        console.log('closed (focus)');
    }else{
        console.log('open (focus)');
    }
  });

  picker.addEventListener('blur', function(){
    tog = !tog;
    if(tog === true){
      tog = false;
        console.log('closed (blur)');
    }else{
        console.log('open (blur)');
    }
  });

}());

  setInterval(function()
    {
      getColor();
      //console.log(getLEDval());
    }, 2000);


    setInterval(function()
    {
      getLEDval();
      //console.log(getLEDval());
    }, 2000);

  function toPaddedHexString(num, len) {
    str = num.toString(16);
    return "0".repeat(len - str.length) + str;
  }

    function sendData(led) {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "setLED?LEDstate="+led, true);
      xhttp.send();
     }

     function getLEDval() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("LEDState").innerHTML =
        this.responseText;
        }
      };
      xhttp.open("GET", "readLED", true);
      xhttp.send();
     }


     function getColor() {
      var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          //console.log(this.responseText);
          const cloudParams = parseQuery(this.responseText);
          console.log(cloudParams);

//          var color1Hex = cloudParams.color1;
//          var color2Hex = cloudParams.color2;
//          var color3Hex = cloudParams.color3;
          
          document.getElementById("color1").innerHTML =
        "#" + toPaddedHexString(cloudParams.color1, 6);
          document.getElementById("color2").innerHTML =
        "#" + toPaddedHexString(cloudParams.color2, 6);
          document.getElementById("color3").innerHTML =
        "#" + toPaddedHexString(cloudParams.color3, 6);

        document.getElementById("ColorInput1").value =
        "#" + toPaddedHexString(cloudParams.color1, 6);
          document.getElementById("ColorInput2").value =
        "#" + toPaddedHexString(cloudParams.color2, 6);
          document.getElementById("ColorInput3").value =
        "#" + toPaddedHexString(cloudParams.color3, 6);
        }
      };
      xhttp.open("GET", "readColor", true);
      xhttp.send();
      }
      
     function setColor(n) {
      var hexcolor = document.getElementById("ColorInput"+n).value;
      console.log(hexcolor);
      console.log(n);
      hexcolor = hexcolor.replace(/[^0-9A-F]/gi, '');
      
      var xhttp = new XMLHttpRequest();
      
      switch(n) {
        case 1:
           xhttp.onreadystatechange = function() {
             if (this.readyState == 4 && this.status == 200) {
              console.log(this.responseText);
                document.getElementById("ColorInput1").value = this.responseText;
             }
           }
           break;
        case 2:
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              console.log(this.responseText);
                document.getElementById("ColorInput2").value = this.responseText;
            }
          }
           break;
         case 3:
           xhttp.onreadystatechange = function() {
             if (this.readyState == 4 && this.status == 200) {
              console.log(this.responseText);
                document.getElementById("ColorInput3").value = this.responseText;
             }
           }
           break;
         default:
           break;
      };
      
      xhttp.open("GET", "setColor?Color="+n+"&Hex="+hexcolor, true);
      xhttp.send();
     }

     function triggerPattern(pattern) {
       var xhttp = new XMLHttpRequest();
       xhttp.open("GET", "triggerPattern?Pattern="+pattern, true);
       xhttp.send();
     }

     function parseQuery(queryString) {
        var query = {};
        var pairs = (queryString[0] === '?' ? queryString.substr(1) : queryString).split('&');
        for (var i = 0; i < pairs.length; i++) {
            var pair = pairs[i].split('=');
            query[decodeURIComponent(pair[0])] = decodeURIComponent(pair[1] || '');
        }
    return query;
}
     
  </script>
</body>
</html>
)***";
