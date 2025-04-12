const char selectorHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Diyson WiFi selector</title>
</head>

<body>
    <form>
        <h1>Wifi Selector</h1>
        <label for="wifi">Select WiFi:</label>
        <select id="wifi" name="wifi">
            <option value="wifi1">WiFi 1</option>
            <option value="wifi2">WiFi 2</option>
            <option value="wifi3">WiFi 3</option>
        </select><br>
        <label for="password" class="password">Password:</label>
        <input type="password" id="password" name="password" placeholder="Enter Password" class="password" required>
        <button type="button" id="connectButton">Connect</button>
    </form>

</body>
<script src="selector_script.js"></script>
<script>
/*
    setInterval(() => {
        fetchWiFiSignals();
    }, 3000); // Fetch every second 
*/
    function fetchWiFiSignals() {
        fetch('/get_wifi_signals')
            .then(response => response.json())
            .then(data => {
                // Set the slider values based on the fetched data
                // map the values from 0-255 to 0-1000
                data.forEach((signal, index) => {
                    console.log(signal);
                });
            })
            .catch(error => {
                console.error('Error fetching values:', error);
            });
    }

</script>

</html>
)rawliteral";