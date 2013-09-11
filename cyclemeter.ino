#define DEBOUNCE_TIME 15000 // microseconds
#define WHEEL_SENSOR_PIN 0 // digital pin 2
#define LED_PIN 13
#define SAMPLE_PERIOD 2000 // milliseconds
#define ALPHA 0.2  // for low-pass filter

volatile unsigned long last_rotate_isr_time;

float rotation_avg = 0.0;
volatile unsigned int rotation_count = 0;
volatile int ledVal = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(WHEEL_SENSOR_PIN, rotateInterrupt, RISING);
}

void loop() {
	delay(SAMPLE_PERIOD);

	// smooth input
	rotation_avg = lowPass( rotation_count, rotation_avg );

	Serial.println(rotation_avg);
	rotation_count = 0;
}

void rotateInterrupt() {
	unsigned long now = micros();
	if((now - last_rotate_isr_time) < DEBOUNCE_TIME) return; // debounce

	rotation_count ++;
	last_rotate_isr_time = now;

	// toggle the Arduino LED
	ledVal = (ledVal == LOW ? HIGH : LOW);
	digitalWrite(LED_PIN, ledVal);
}

/**
 * Give the output a smoothing effect
 * http://en.wikipedia.org/wiki/Low-pass_filter#Algorithmic_implementation
 */
float lowPass( unsigned int newVal, float oldVal) {
	return oldVal + ALPHA * (newVal - oldVal);
}

