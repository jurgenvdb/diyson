def web_page(value:int=50):
  html = """
<html>

<head>
    <title>DIYson</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Helvetica;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
        }

        h1 {
            color: #0F3376;
            padding: 2vh;
        }

        p {
            font-size: 1.5rem;
        }

        .slider {
            -webkit-appearance: none;
            width: 100%;
            height: 15px;
            border-radius: 5px;
            background: #d3d3d3;
            outline: none;
            opacity: 0.7;
            -webkit-transition: .2s;
            transition: opacity .2s;
        }

        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            border-radius: 50%;
            background: #04AA6D;
            cursor: pointer;
        }

        .slider::-moz-range-thumb {
            width: 25px;
            height: 25px;
            border-radius: 50%;
            background: #04AA6D;
            cursor: pointer;
        }
    </style>
</head>

<body>
    <h1>Diyson brightness slider</h1>
    <div class="slidecontainer">
        <input type="range" min="0" max="100" value=""" + str(value) + """ class="slider" id="brightness">
        <p>Brightness: <span id="brightnessText"></span></p>
        <input type="number" min="0" max="100" id="brightnessInput">
    </div>
</body>
<script>
    var slider = document.getElementById("brightness");
    var output = document.getElementById("brightnessText");
    var input = document.getElementById("brightnessInput");
    output.innerHTML = slider.value; // Display the default slider value
    input.value = slider.value;
    // Update the current slider value (each time you drag the slider handle)
    slider.oninput = function () {
        output.innerHTML = this.value;
        input.value = this.value;
    }
    input.oninput = function () {
        output.innerHTML = this.value;
        slider.value = this.value;
    }

    slider.onchange = function () {
        document.location.href = "/?brightness=" + this.value;
    }
    input.onchange = function () {
        document.location.href = "/?brightness=" + this.value;
    }
</script>

</html>
  """
  return html

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

while True:
  conn, addr = s.accept()
  print('Got a connection from %s' % str(addr))
  request = conn.recv(1024)
  request = str(request)
  print('Content = %s' % request)
  brightness = request.find('GET /?brightness')
  if brightness != - 1:
      end = request.find('HTTP/1.1')
      value = int(request[19:end-1])
      print(f'value: "{request[19:end-1]}"')
      led.duty(min(floor(value / 100 * 1024),1023))
      response = web_page(value = value)
      conn.send('HTTP/1.1 200 OK\n')
      conn.send('Content-Type: text/html\n')
      conn.send('Connection: close\n\n')
      conn.sendall(response)
      conn.close()
  else:
      print("value field not found")
      response = web_page(value = 50)
      conn.send('HTTP/1.1 200 OK\n')
      conn.send('Content-Type: text/html\n')
      conn.send('Connection: close\n\n')
      conn.sendall(response)
      conn.close()

