enum trajectory {LEFT, RIGHT};

// h-bridge inputs
static int hinput_pin_1, hinput_pin_2;
static bool swinging;

// LDR sensor pin 
static int sensor_pin;

void sensor_interrupt_high_handler(void);
void sensor_interrupt_low_handler(void);

// Time-stamp for last time the arm swung measured in millis
unsigned long millis_since_last_swing;

// Setup boxer arm
void boxer_arm_init(int hinput_pin_1_arg, int hinput_pin_2_arg, int sensor_pin_arg) {
    hinput_pin_1 = hinput_pin_1_arg;
    hinput_pin_2 = hinput_pin_2_arg;
    sensor_pin = sensor_pin_arg;

    pinMode(hinput_pin_1, OUTPUT);
    pinMode(hinput_pin_2, OUTPUT);
    digitalWrite(hinput_pin_1, LOW);
    digitalWrite(hinput_pin_2, LOW);

    swinging = false;
    millis_since_last_swing = millis();
    randomSeed(analogRead(0));

    // Interrupt when sensor pin is high
    // This triggers when the arm swings past the LDR sensor
    attachInterrupt(digitalPinToInterrupt(sensor_pin), sensor_interrupt_high_handler, RISING);
    interrupts();
}

// Stop the DC motor once it's over the LDR
void sensor_interrupt_high_handler(void) {

    // Stop interrupts any further interrupts
    detachInterrupt(digitalPinToInterrupt(sensor_pin));

    // Stop DC motor
    digitalWrite(hinput_pin_1, LOW);
    digitalWrite(hinput_pin_2, LOW);

    // Save the millisecond this interrupt occured in
    millis_since_last_swing = millis();

    // Attach new interrupt for when the sensor pin is low;
    attachInterrupt(digitalPinToInterrupt(sensor_pin), sensor_interrupt_low_handler, LOW);
}

// Wait until the DC motor has passed the LDR to
// prevent any accidental interrupts
void sensor_interrupt_low_handler(void) {
    // Stop interrupts any further interrupts
    detachInterrupt(digitalPinToInterrupt(sensor_pin));

    attachInterrupt(digitalPinToInterrupt(sensor_pin), sensor_interrupt_high_handler, RISING);
}

void swing(enum trajectory dir) {
    switch(dir){
        case RIGHT:
            digitalWrite(hinput_pin_1, LOW);
            digitalWrite(hinput_pin_2, HIGH);
            break;
        case LEFT:
            digitalWrite(hinput_pin_2, LOW);
            digitalWrite(hinput_pin_1, HIGH);
            break;
    }
}

// Swings arm in random direction, either left or right
inline
void swing_random_direction(void) {
    int random_direction = random(trajectory::LEFT, trajectory::RIGHT + 1);
    swing(random_direction);
}

