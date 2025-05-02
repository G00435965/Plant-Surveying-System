String homePagePart1 = F(R"=====(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
  <title>Self Watering Plant WebServer</title>
  <style>
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: Arial, sans-serif;
    }

    body {
      background-color: #f9f9f9;
      color: #333;
      display: flex;
      flex-direction: column;
      align-items: center;
      min-height: 100vh;
      margin: 0;
    }

    .flex-header {
      background-color: #4CAF50;
      color: white;
      width: 100%;
      padding: 20px 0;
      text-align: center;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    }

    .main-content {
      flex: 1;
      width: 100%;
      max-width: 1000px;
      display: flex;
      flex-direction: column;
      align-items: center;
      margin: 0 auto;
    }

    .interactive-section {
      display: flex;
      justify-content: center;
      flex-wrap: wrap;
      gap: 20px;
      margin: 30px 0;
    }

    .info-box {
      background-color: #e8f5e9;
      border: 2px solid #c8e6c9;
      border-radius: 10px;
      width: 90%;
      max-width: 600px;
      padding: 20px;
      cursor: pointer;
      transition: background-color 0.3s ease;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);
      display: flex;
      flex-direction: column;
      align-items: center;
      text-align: center;
    }

    .info-box:hover {
      background-color: #dcedc8;
    }

    .box-title {
      font-size: 20px;
      font-weight: bold;
      color: #2e7d32;
      margin-bottom: 10px;
    }

    .box-content {
      font-size: 16px;
      text-align: left;
      display: none;
    }

    .info-box.active .box-content {
      display: block;
    }

    /* Ensure iframes are fully responsive */
    iframe {
      width: 100%;
      max-width: 100%; /* Ensure it doesn't exceed the container width */
      height: 500px;
      border: none;
    }

    /* Add a max-width constraint to avoid overflowing content */
    .live-readings-container {
      max-width: 100%;
      overflow: hidden; /* Prevents horizontal scroll */
      padding: 10px;
    }

    .image-gallery {
      display: flex;
      flex-wrap: wrap;
      gap: 20px;
      justify-content: center;
      margin: 20px 0;
    }

    .image-box {
      width: 300px;
      text-align: center;
    }

    .image-box img {
      width: 100%;
      border-radius: 10px;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);
    }

    .info-card {
      background-color: #f0f4c3;
      border: 1px solid #dce775;
      border-radius: 10px;
      padding: 20px;
      margin: 10px;
      width: 90%;
      max-width: 600px;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);
    }

    .flex-footer {
      background-color: #4CAF50;
      color: white;
      text-align: center;
      padding: 10px 0;
      width: 100%;
    }
  </style>
  <script>
    function toggleBox(box) {
      box.classList.toggle('active');
    }

    function showSection(section) {
      document.querySelectorAll('section').forEach(function(s) {
        s.style.display = 'none';
      });
      document.getElementById(section).style.display = 'block';
    }

    function fetchData() {
      fetch('/getTemperature')
        .then(response => response.text())
        .then(data => {
          console.log("Temperature:", data);
          document.getElementById("tempValue").innerText = data + " °C";
        })
        .catch(error => console.error('Error fetching data:', error));
    }

    setInterval(fetchData, 5000);
    window.onload = function() {
      showSection('homepage');
      fetchData();
    }
  </script>
</head>
<body>

<header class="flex-header">
  <h1>Self Watering Plant</h1>
</header>

<main class="main-content">

  <!-- Homepage (index) Section -->
  <section id="homepage">
    <div class="interactive-section">
      <div class="info-box" onclick="showSection('project-info')">
        <div class="box-title">Project Info</div>
        <div class="box-content">
          <p>This self-watering plant system uses sensors to monitor moisture, temperature, and humidity. Data is visualized via ThingSpeak.</p>
        </div>
      </div>

      <div class="info-box" onclick="showSection('live-readings')">
        <div class="box-title">Live Readings</div>
        <div class="box-content">
          <p>View live temperature, humidity, and soil moisture charts on the next page.</p>
        </div>
      </div>
    </div>

    <section class="overview-section">
      <div class="info-box">
        <div class="box-title">System Overview</div>
        <div class="box-content" style="display: block;">
          <p>
            The Self-Watering Plant system uses moisture, temperature, and humidity sensors connected to an ESP32. When the soil becomes too dry, the system automatically pumps water via a peristaltic pump to rehydrate the plant. The live sensor data is sent to ThingSpeak, where users can monitor readings such as temperature, humidity and moisture through live updating charts.
          </p>
        </div>
      </div>
    </section>

    <section id="project-images">
      <h2>Project Photos</h2>
      <div class="image-gallery">
        <div class="image-box">
          <img src="images/IOT poster.jpg" alt="Self Watering Plant - Side View" />
          <p>Poster showing the system layout and components</p>
        </div>
        <div class="image-box">
          <img src="images/realproject.jpg" alt="Self Watering Plant - Top View" />
          <p>Top view of the physical device setup</p>
        </div>
      </div>
    </section>
  </section>

  <!-- Live Readings Section -->
  <section id="live-readings" style="display: none;">
    <h2>Live Sensor Readings</h2>
    <div class="live-readings-container">
      <h3>Temperature</h3>
      <iframe src="https://thingspeak.com/channels/2773493/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperature&type=line&xaxis=Time&yaxis=C&yaxismax=50&yaxismin=0"></iframe>
    </div>
    <div class="live-readings-container">
      <h3>Humidity</h3>
      <iframe src="https://thingspeak.com/channels/2773493/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Humidity&type=line&xaxis=Time&yaxis=%25&yaxismax=90&yaxismin=20"></iframe>
    </div>
    <div class="live-readings-container">
      <h3>Moisture</h3>
      <iframe src="https://thingspeak.com/channels/2773493/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&max=4605&min=0&results=60&timescale=10&title=Moisture&type=line&xaxis=Time&yaxis=Level&yaxismax=4605&yaxismin=0"></iframe>
    </div>
  </section>

  <!-- Project Info Section -->
  <section id="project-info" style="display: none;">
    <h2>Project Info</h2>
    <div class="flex-container">
      <div class="info-card">
        <h3>Components Used</h3>
        <p>
          - ESP32<br>
          - Capacitive Soil Moisture Sensor<br>
          - DHT11 Temperature & Humidity Sensor<br>
          - Peristaltic Water Pump<br>
          - ULN2003A<br>
          - Jumper Wires<br>
          - ThingSpeak for data display
        </p>
      </div>

      <div class="info-card">
        <h3>How It Works</h3>
        <p>
          The DHT11 sensor measures the temperature and humidity of the environment, allowing the user to pick an appropriate environment to place their plant. The capacitive moisture sensor reads the moisture levels of the soil, and if it detects dryness, it triggers the peristaltic pump to rehydrate the plant.
        </p>
      </div>

      <div class="info-card">
        <h3>Future Improvements</h3>
        <p>
          - Add water level detection in the tank<br>
          - Add a camera for improved surveillance<br>
          - Include more accurate sensors<br>
          - Real-time notifications
        </p>
      </div>

      <div class="info-card">
        <h3>Meeting United Nations’ SDGs</h3>
        <p>
          <strong>Goal 3: Ensure healthy lives and promote well-being for all at all ages.</strong><br><br>
          Plants are excellent for mental health and well-being, especially in homes full of technology. Indoor plants provide a natural connection, reduce anxiety, and boost mood and productivity.
        </p>
      </div>

      <div class="info-card">
        <h3>My Motivation</h3>
        <p>
          My objective is to help people who may not have the time to tend to their plants, ensuring they can keep plants healthy without constant attention.
        </p>
      </div>
    </div>
  </section>

</main>

<footer class="flex-footer">
  <p>&copy; 2025 Self Watering Plant System</p>
</footer>

</body>
</html>)=====");

