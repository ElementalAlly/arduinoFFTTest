#include "arduinoFFT.h"

// Factors to change

const int num_readings = 100;

const uint16_t samples = 512;

const double signal1Freq = 500;
const double signal1Amp = 500;

const double signal2Freq = 1000;
const double signal2Amp = 300;

const double signal3Freq = 2000;
const double signal3Amp = 300;

const double signal4Freq = 5167;
const double signal4Amp = 0;

const double signal5Freq = 10335;
const double signal5Amp = 0;

// End of Factors to change

const int samplingFrequency = 44100;

const double signal1Cycles = (((samples-1) * signal1Freq) / samplingFrequency);
const double signal2Cycles = (((samples-1) * signal2Freq) / samplingFrequency);
const double signal3Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal4Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal5Cycles = (((samples-1) * signal3Freq) / samplingFrequency);

double vReal[samples];
double vImag[samples];

arduinoFFT fft = arduinoFFT(vReal, vImag, samples, samplingFrequency);

// uint32_t ETA;
uint32_t times[num_readings];
uint64_t total = 0;
int num_times = 0;

void dummyFFT(int real, int imag) {}

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   while(!Serial);
   Serial.println("ready");
   Serial.println("Setup");

   TCCR1A = 0;
   TCCR1B = 1;

   fft.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);

   Serial.println("setup complete");
}

void loop() {
  for (int i = 0; i < samples; i++) {
    vReal[i] = double(signal1Amp * (sin(i * (twoPi * signal1Cycles) / samples)) + signal2Amp * (sin(i * (twoPi * signal2Cycles) / samples)) + signal3Amp * (sin(i * (twoPi * signal3Cycles) / samples)) + signal4Amp * (sin(i * (twoPi * signal4Cycles) / samples)) + signal5Amp * (sin(i * (twoPi * signal5Cycles) / samples)));
    vImag[i] = 0;
  }

  uint32_t startTime = micros();
  fft.Compute(FFT_FORWARD);
  uint32_t endTime = micros();
  uint32_t ETA = endTime - startTime;
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
