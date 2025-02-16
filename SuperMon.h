const ound-color: #4CAF50; color: white; border: none; border-radius: 5px; }
    .btn:hover { background-color: #45a049; }
    .tabledata { text-align: center; }
    footer { padding: 10px; background-color: #333; color: white; text-align: center; }
  </style>
</head>
<body onload="process()">
  <header>
    <div class="navbar fixed-top">
      <div class="container">
        <div class="navtitle">Sensor Monitor</div>
        <div class="navdata" id="date">mm/dd/yyyy</div>
        <div class="navheading">DATE</div><br>char PAGE_MAIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Web Page Update Demo</title>
  <style>
    body {
      background-color: #efefef;
      font-family: Arial, sans-serif;
    }
    .navbar {
      padding: 10px;
      background-color: #333;
      color: white;
    }
    .container {
      padding: 20px;
    }
    .heading {
      font-weight: bold;
    }
    .bodytext {
      margin: 10px 0;
    }
    .btn {
      padding: 10px;
      margin: 5px;
      background-color: #007bff;
      color: white;
      border: none;
      cursor: pointer;
    }
    .btn:hover {
      background-color: #0056b3;
    }
  </style>
</head>
<body>
  <header class="navbar">
    <div class="navdata" id="time">00:00:00</div>
    <div class="navdata" id="date">YYYY-MM-DD</div>
    <div class="navheading">TIME</div>
  </header>
  <main class="container" style="margin-top: 70px">
    <div class="category">Sensor Readings</div>
    <div style="border-radius: 10px !important;">
      <table style="width: 50%">
        <colgroup>
          <col span="1" style="background-color: rgb(230,230,230); width: 20%; color: #000000;">
          <col span="1" style="background-color: rgb(200,200,200); width: 15%; color: #000000;">
          <col span="1" style="background-color: rgb(180,180,180); width: 15%; color: #000000;">
        </colgroup>
        <tr>
          <th><div class="heading">Sensor</div></th>
          <th><div class="heading">Value</div></th>
        </tr>
        <tr>
          <td><div class="bodytext">Flame Sensor</div></td>
          <td><div class="tabledata" id="flameSensor">--</div></td>
        </tr>
        <tr>
          <td><div class="bodytext">Rain Sensor</div></td>
          <td><div class="tabledata" id="rainSensor">--</div></td>
        </tr>
        <tr>
          <td><div class="bodytext">PIR Sensor</div></td>
          <td><div class="tabledata" id="pirSensor">--</div></td>
        </tr>
      </table>
    </div>
    <br>
    <div class="category">Controls</div>
    <br>
    <div class="bodytext">Servo Motor (Window Control)</div>
    <button type="button" class="btn" id="servoBtn" onclick="toggleServo()">Toggle</button>
    <br>
    <div class="bodytext">Buzzer (Fire Alert)</div>
    <button type="button" class="btn" id="buzzerBtn" onclick="toggleBuzzer()">Toggle</button>
    <br>
  </main>
  <footer class="foot" id="temp">ESP8266-Arduino Project Demo</footer>
  
  <script>
    var xmlHttp = new XMLHttpRequest();

    function toggleServo() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("PUT", "/toggle_servo", true);
      xhttp.send();
    }

    function toggleBuzzer() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("PUT", "/toggle_buzzer", true);
      xhttp.send();
    }

    function updateData() {
      if (xmlHttp.readyState === 4 && xmlHttp.status === 200) {
        var jsonResponse = JSON.parse(xmlHttp.responseText);
        document.getElementById("time").innerHTML = new Date().toLocaleTimeString();
        document.getElementById("date").innerHTML = new Date().toLocaleDateString();
        document.getElementById("flameSensor").innerHTML = jsonResponse.flame;
        document.getElementById("rainSensor").innerHTML = jsonResponse.rain;
        document.getElementById("pirSensor").innerHTML = jsonResponse.intruder;
      }
    }

    function fetchData() {
      xmlHttp.open("GET", "/json", true);
      xmlHttp.onreadystatechange = updateData;
      xmlHttp.send();
      setTimeout(fetchData, 1000); // Fetch data every second
    }

    // Start fetching data when the page loads
    window.onload = fetchData;
  </script>
</body>
</html>
)=====";
