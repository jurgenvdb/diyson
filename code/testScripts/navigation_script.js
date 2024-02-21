var main_controls = document.getElementById("main_controls");
        var alarm_clock = document.getElementById("alarm_clock");
        var settings = document.getElementById("settings");

        function switchVisible(element) {
            // console.log("switchVisible")
            // console.log(element.href.split("#")[1]);
            if (element.href.split("#")[1] == main_controls.id) {
                // console.log("main_controls");
                main_controls.style.display = "block";
                alarm_clock.style.display = "none";
                settings.style.display = "none";
            } else if (element.href.split("#")[1] == alarm_clock.id) {
                // console.log("alarm_clock");
                main_controls.style.display = "none";
                alarm_clock.style.display = "block";
                settings.style.display = "none";
            } else if (element.href.split("#")[1] == settings.id) {
                // console.log("settings");
                main_controls.style.display = "none";
                alarm_clock.style.display = "none";
                settings.style.display = "block";
            }
        }