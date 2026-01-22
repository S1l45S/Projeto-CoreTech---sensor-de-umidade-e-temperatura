const char index_html[] PROGMEM = R"(
<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>MONITORAMENTO SENSOR LOGGER</title>
  <style>
    :root {
      --primary: #6c5ce7;
      --accent: #a29bfe;
      --bg: #0f0c29;
      --glass: rgba(255, 255, 255, 0.05);
      --card-border: rgba(255, 255, 255, 0.1);
      --temp-color: #00d2ff;
    }

    body {
      font-family: 'Segoe UI', sans-serif;
      background: radial-gradient(circle at top right, #1e1b4b, #0f0c29);
      color: #e0e0e0;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      margin: 0;
    }

    .dashboard {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
      gap: 15px;
      width: 95%;
      max-width: 480px;
      padding: 25px;
      background: var(--glass);
      backdrop-filter: blur(25px);
      border-radius: 30px;
      border: 1px solid var(--card-border);
      box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.6);
    }

    .header-full { grid-column: span 2; text-align: center; margin-bottom: 10px; }
    .header-full h1 { margin: 0; font-weight: 300; letter-spacing: 4px; color: var(--accent); font-size: 1.2rem; }

    .card {
      background: rgba(0, 0, 0, 0.25);
      border: 1px solid var(--card-border);
      border-radius: 20px;
      padding: 15px;
      display: flex;
      flex-direction: column;
      align-items: center;
      position: relative;
    }

    .card-label { font-size: 0.65rem; text-transform: uppercase; letter-spacing: 1.2px; margin-bottom: 12px; opacity: 0.7; color: var(--accent); }

    .thermometer-track {
      width: 12px;
      height: 100px;
      background: #1a1a2e;
      border-radius: 10px;
      position: relative;
      border: 1px solid rgba(255,255,255,0.1);
    }
    .thermometer-fill {
      position: absolute;
      bottom: 0;
      width: 100%;
      background: linear-gradient(to top, #4834d4, var(--temp-color));
      border-radius: 10px;
      transition: height 0.8s ease;
    }
    .thermometer-bulb {
      width: 22px; height: 22px; background: var(--temp-color);
      border-radius: 50%; position: absolute; bottom: -10px; left: -6px;
      border: 2px solid #0f0c29; box-shadow: 0 0 10px var(--temp-color);
    }

    .water-circle {
      width: 80px; height: 80px; border-radius: 50%;
      border: 3px solid rgba(255,255,255,0.1);
      position: relative; overflow: hidden; background: #1a1a2e;
    }
    .water-wave {
      position: absolute; bottom: 0; left: 0; width: 100%;
      background: linear-gradient(to top, #6c5ce7, #a29bfe);
      transition: height 1s ease;
    }

    .big-value { font-size: 1.5rem; font-weight: 600; }
    .unit { font-size: 0.8rem; color: var(--accent); margin-left: 2px; }

    .btn-download {
      grid-column: span 2;
      background: linear-gradient(45deg, var(--primary), #4834d4);
      color: white; padding: 14px; border-radius: 15px;
      text-decoration: none; text-align: center;
      font-weight: 600; font-size: 0.8rem; transition: 0.3s;
    }
    .btn-download:hover { transform: translateY(-2px); filter: brightness(1.1); }
  </style>
</head>
<body>

  <div class="dashboard">
    <div class="header-full">
      <h1>Monitoramento UT</h1>
    </div>

    <div class="card">
      <span class="card-label">Temperatura</span>
      <div style="display: flex; align-items: center; gap: 15px;">
        <div class="thermometer-track">
          <div class="thermometer-fill" id="t-fill"></div>
          <div class="thermometer-bulb" id="t-bulb"></div>
        </div>
        <div class="big-value"><span id="t-val">--</span><span class="unit">°C</span></div>
      </div>
    </div>

    <div class="card">
      <span class="card-label">Umidade Solo</span>
      <div class="water-circle">
        <div class="water-wave" id="u-wave"></div>
        <div style="position: absolute; top:50%; left:50%; transform:translate(-50%,-50%); z-index:2; font-weight:700;">
          <span id="u-val">--</span>%
        </div>
      </div>
    </div>
    <a href="/dados_" class="btn-download">EXPORTAR RELATÓRIO CSV</a>
  </div>

  <script>
    function update() {
      fetch("/variaveis")
        .then(r => r.json())
        .then(d => {
          document.getElementById("t-val").innerText = d.t;
          document.getElementById("u-val").innerText = d.u;
          let tPercent = ((d.t + 40) / 120) * 100;
          document.getElementById("t-fill").style.height = Math.min(Math.max(tPercent, 0), 100) + "%";

          let color = "#00d2ff";
          if(d.t > 18) color = "#a29bfe";
          if(d.t > 38) color = "#ff7675";
          document.documentElement.style.setProperty('--temp-color', color);

          document.getElementById("u-wave").style.height = d.u + "%";
        })
        .catch(e => console.error("Erro na atualização"));
    }
    setInterval(update, 2000);
    update();
  </script>
</body>
</html>
)";