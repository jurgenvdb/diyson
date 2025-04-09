const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>Slider Control</title>
</head>

<body>
    <h2>Cool white slider</h2>
    <input type='range' id='cwSlider' min='0' max='255' value='0'><br>
    <p>Cool white value: <span id='cwSliderValue'></span></p>
    <h2>Warm white slider</h2>
    <input type='range' id='wwSlider' min='0' max='255' value='0'><br>
    <p>Warm white value: <span id='wwSliderValue'></span></p>
    <script>
        var wwSlider = document.getElementById('wwSlider');
        var wwSliderOutput = document.getElementById('wwSliderValue');
        var cwSlider = document.getElementById('cwSlider');
        var cwSliderOutput = document.getElementById('cwSliderValue');
        wwSliderOutput.innerHTML = wwSlider.value;
        cwSliderOutput.innerHTML = cwSlider.value;
        function postValues() {
            wwSliderOutput.innerHTML = wwSlider.value;
            cwSliderOutput.innerHTML = cwSlider.value;
            fetch('/update_values', {
                method: 'POST',
                header: {
                    'Content-type': 'application/json',
                },
                body: JSON.stringify({
                    wwValue: wwSlider.value,
                    cwValue: cwSlider.value
                }),
            }).then(response => {
                if (!response.ok) {
                    console.error('Error sending data');
                }
            }).catch(error => {
                console.error('Fetch error:', error);
            });
        }
        wwSlider.oninput = postValues;
        cwSlider.oninput = postValues;

        function getValues() {
            fetch('/get_values')
                .then(response => response.json())
                .then(data => {
                    wwSlider.value = data.wwValue;
                    cwSlider.value = data.cwValue;
                    wwSliderOutput.innerHTML = data.wwValue;
                    cwSliderOutput.innerHTML = data.cwValue;
                })
                .catch(error => {
                    console.error('Error fetching values:', error);
                });
        }
        getValues();
    </script>
</body>

</html>
)rawliteral";