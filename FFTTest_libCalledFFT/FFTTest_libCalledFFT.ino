#include <fft.h>

// Factors to change

const int num_readings = 100;

const uint16_t samples = 256;

const double signal1Freq = 120;
const double signal1Amp = 500;

const double signal2Freq = 1722;
const double signal2Amp = 300;

const double signal3Freq = 2583;
const double signal3Amp = 300;

const double signal4Freq = 5167;
const double signal4Amp = 200;

const double signal5Freq = 10335;
const double signal5Amp = 200;

// End of Factors to change

const int samplingFrequency = 44100;

const double signal1Cycles = (((samples-1) * signal1Freq) / samplingFrequency);
const double signal2Cycles = (((samples-1) * signal2Freq) / samplingFrequency);
const double signal3Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal4Cycles = (((samples-1) * signal4Freq) / samplingFrequency);
const double signal5Cycles = (((samples-1) * signal5Freq) / samplingFrequency);

float *vReal; //[samples];
float *vRealOut; //[samples];
// int vImag[samples];
fft_config_t *fft_obj;

uint64_t total = 0;
int num_times = 0;
double twoPi = 6.283185307;

void dummyFFT(int real, int imag) {}

double average(uint32_t *avg_list, int len) {
  long long total = 0;
  for (int i = 0; i < len; i++) {
    total += avg_list[i];
  }
  return (double)total / (double)len;
}

class sampler{
   public:
      int prevTime = 0;
      int numSamples = 0;
      int pin;
      int sampling_ticks = 362;
      sampler(int in_pin) {
         pin = in_pin;
         pinMode(in_pin, INPUT);
      }
      int tick(uint16_t time) {
         if (time - prevTime >= sampling_ticks) {
            // vReal[numSamples] = sin_samples[numSamples % 4];
            // Serial.println(vReal[numSamples]);
            // vImag[numSamples] = 0;
            numSamples += 1;
            prevTime = TCNT1;
            if (numSamples >= samples) {
              uint32_t startTime = micros();
              fft_execute(fft_obj);
              uint32_t endTime = micros();
              numSamples = 0;
               /* fft = arduinoFFT(vReal, vImag, samples, samplingFrequency);
               fft.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
               fft.Compute(FFT_FORWARD);
               uint16_t endTime = TCNT1;
               uint16_t eta = endTime - startTime;
               startTime = TCNT1;
               fft.ComplexToMagnitude();
               numSamples = 0;
               uint16_t endTime2 = TCNT1;
               uint16_t eta2 = endTime2 - startTime; */
               /*Serial.print("compute: ");
               Serial.print(eta);
               Serial.print(" magnitude: ");
               Serial.println(eta2); */
               return 1;
            }
         }
         return 0;
      }
};

sampler mainSampler(A1);

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   while(!Serial);
   Serial.println("ready");
   Serial.println("Setup");

   TCCR1A = 0;
   TCCR1B = 1;

   fft_obj = fft_init(samples, FFT_REAL, FFT_FORWARD, vReal, vRealOut);

   Serial.println("setup complete");
}

void loop() {
  for (int i = 0; i < samples; i++) {
    vReal[i] = float(signal1Amp * (sin(i * (twoPi * signal1Cycles) / samples)) + signal2Amp * (sin(i * (twoPi * signal2Cycles) / samples)) + signal3Amp * (sin(i * (twoPi * signal3Cycles) / samples)) + signal4Amp * (sin(i * (twoPi * signal4Cycles) / samples)) + signal5Amp * (sin(i * (twoPi * signal5Cycles) / samples)));
    Serial.println(vReal[i]);
  }

  uint32_t startTime = micros();
  fft_execute(fft_obj);
  uint32_t endTime = micros();
  uint32_t ETA = endTime - startTime;
  // uncomment when testing for accuracy
  for (int i = 0; i < samples / 2; i++) {
    Serial.println(vRealOut[i]);
  }
  while(1);

  Serial.println(ETA);
  total += ETA;
  num_times++;
  if (num_times == num_readings) {
    double avg = (double)total / num_readings;
    Serial.println();
    Serial.println("Average:");
    Serial.println(avg);
    while(1);
  }
}