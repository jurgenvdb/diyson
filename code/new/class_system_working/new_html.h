const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Slider input</title>
    <style>
        body {
            background-color: #334;
            color: #fff;
            font-family: Arial, sans-serif;
            font-size: 24px;
        }

        div.sliderContainer {
            width: 50%;
            display: flex;
            flex-direction: column;
            float: left;
        }

        input.brightnessSlider {
            writing-mode: vertical-rl;
            direction: rtl;
            appearance: none;
            margin: 10px 5%;
            padding: 0;
            background-color: rgba(255, 255, 255, .2);
            cursor: row-resize;
            overflow: hidden;
            /* height: 300px; */

            &::-webkit-slider-thumb {
                -webkit-appearance: none;
                appearance: none;
                height: 0;
            }
        }

        #warmWhite::-webkit-slider-thumb {
            box-shadow: 0 300px 0 300px rgba(255, 179, 0, 0.2);
        }

        #coolWhite::-webkit-slider-thumb {
            box-shadow: 0 300px 0 300px rgba(0, 255, 255, 0.2);
        }

        p.sliderInfo {
            background-color: rgba(#fff, 1);
            margin: 0;
            padding-top: 5px;
            text-align: center;
            display: flex;
            flex-direction: column;
            float: left;
        }
    </style>
</head>

<body>
    <div class="sliderContainer">
        <p class="sliderInfo">Warm white</p>
        <input type="range" min="0" max="1000" class="brightnessSlider" id="warmWhite">
        <p class="sliderInfo" id="warmWhiteValue">0</p>
    </div>
    <div class="sliderContainer">
        <p class="sliderInfo">Cool white</p>
        <input type="range" min="0" max="1000" class="brightnessSlider" id="coolWhite">
        <p class="sliderInfo" id="coolWhiteValue">0</p>
    </div>
    <script>
        var warmWhiteSlider = document.getElementById("warmWhite");
        var coolWhiteSlider = document.getElementById("coolWhite");
        var warmWhiteValue = document.getElementById("warmWhiteValue");
        var coolWhiteValue = document.getElementById("coolWhiteValue");


        function adaptSliderHeight() {
            var sliderInfo = document.querySelectorAll(".sliderInfo");
            var totalHeight = 0;
            sliderInfo.forEach(slider => {
                totalHeight += slider.offsetHeight;
            });
            var rangeSliders = document.querySelectorAll(".brightnessSlider");
            rangeSliders.forEach(slider => {
                if (window.innerHeight - totalHeight < 100) {
                    slider.style.height = "100px";
                }
                else {
                    slider.style.height = window.innerHeight - totalHeight + "px";
                }
            });
        }

        function updateValues() {
            warmWhiteValue.innerHTML = warmWhiteSlider.value;
            coolWhiteValue.innerHTML = coolWhiteSlider.value;

            fetch('/update_values', {
                method: 'POST',
                header: {
                    'Content-type': 'application/json',
                },
                body: JSON.stringify({
                    wwValue: Math.round((warmWhiteSlider.value / 1000) * 255),
                    cwValue: Math.round((coolWhiteSlider.value / 1000) * 255)
                }),
            }).then(response => {
                if (!response.ok) {
                    console.error('Error sending data');
                }
            }).catch(error => {
                console.error('Fetch error:', error);
            });
        }

        function fetchValues() {
            fetch('/get_values')
                .then(response => response.json())
                .then(data => {
                    // Set the slider values based on the fetched data
                    // map the values from 0-255 to 0-1000
                    warmWhiteSlider.value = Math.round((data.wwValue / 255) * 1000);
                    coolWhiteSlider.value = Math.round((data.cwValue / 255) * 1000);
                    warmWhiteValue.innerHTML = warmWhiteSlider.value;
                    coolWhiteValue.innerHTML = coolWhiteSlider.value;
                })
                .catch(error => {
                    console.error('Error fetching values:', error);
                });
        }

        fetchValues();

        adaptSliderHeight();
        window.addEventListener("resize", adaptSliderHeight);

        updateValues();
        warmWhiteSlider.addEventListener("input", updateValues);
        coolWhiteSlider.addEventListener("input", updateValues);
    </script>
</body>

</html>
)rawliteral";