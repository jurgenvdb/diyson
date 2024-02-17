void setup_drivers() {
    ledcSetup(warm_white_channel, warm_white_freq, warm_white_resolution);
    ledcAttachPin(warm_white_pin, warm_white_channel);
    pinMode(warm_white_enable_pin, OUTPUT);
    ledcSetup(cold_white_channel, cold_white_freq, cold_white_resolution);
    ledcAttachPin(cold_white_pin, cold_white_channel);
    pinMode(cold_white_enable_pin, OUTPUT);
}

void set_driver(LampState state){
    int warm_white_duty = round(map(state.temperature, minimum_temperature, maximum_temperature, 255, 0) * state.brightness / maximum_brightness);
    int cold_white_duty = round(map(state.temperature, minimum_temperature, maximum_temperature, 0, 255) * state.brightness / maximum_brightness);
    digitalWrite(warm_white_enable_pin, (state.powered && state.temperature <= minimum_temperature + warm_white_treshold));
    ledcWrite(warm_white_channel, warm_white_duty);
    digitalWrite(cold_white_enable_pin, (state.powered && state.temperature >= maximum_temperature - cold_white_treshold));
    ledcWrite(cold_white_channel, cold_white_duty);
}