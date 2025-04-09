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

    // map the slider values from 0-1000 to 0-255
    var warmWhiteBrightness = Math.round((warmWhiteSlider.value / 1000) * 255);
    var coolWhiteBrightness = Math.round((coolWhiteSlider.value / 1000) * 255);

    fetch('/update_values', {
        method: 'POST',
        header: {
            'Content-type': 'application/json',
        },
        body: JSON.stringify({
            wwValue: warmWhiteBrightness,
            cwValue: coolWhiteBrightness
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



adaptSliderHeight();
window.addEventListener("resize", adaptSliderHeight);

updateValues();
warmWhiteSlider.addEventListener("input", updateValues);
coolWhiteSlider.addEventListener("input", updateValues);

fetchValues();




// function postValues() {
//     wwSliderOutput.innerHTML = wwSlider.value;
//     cwSliderOutput.innerHTML = cwSlider.value;
//     fetch('/update_values', {
//         method: 'POST',
//         header: {
//             'Content-type': 'application/json',
//         },
//         body: JSON.stringify({
//             wwValue: wwSlider.value,
//             cwValue: cwSlider.value
//         }),
//     }).then(response => {
//         if (!response.ok) {
//             console.error('Error sending data');
//         }
//     }).catch(error => {
//         console.error('Fetch error:', error);
//     });
// }
// wwSlider.oninput = postValues;
// cwSlider.oninput = postValues;

// function getValues() {
//     fetch('/get_values')
//         .then(response => response.json())
//         .then(data => {
//             wwSlider.value = data.wwValue;
//             cwSlider.value = data.cwValue;
//             wwSliderOutput.innerHTML = data.wwValue;
//             cwSliderOutput.innerHTML = data.cwValue;
//         })
//         .catch(error => {
//             console.error('Error fetching values:', error);
//         });
// }