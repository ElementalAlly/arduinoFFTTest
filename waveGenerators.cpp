// 1 wave generator
const double signal1Freq = 250;
const double signal1Cycles = (((samples-1) * signal1Freq) / samplingFrequency);
const double signal1Amp = 500;

signal1Amp * (sin(numSamples * (twoPi * signal1Cycles) / samples))

// 3 wave generator
const double signal1Freq = 500;
const double signal1Cycles = (((samples-1) * signal1Freq) / samplingFrequency);
const double signal1Amp = 500;
const double signal2Freq = 1000;
const double signal2Cycles = (((samples-1) * signal2Freq) / samplingFrequency);
const double signal2Amp = 300;
const double signal3Freq = 2000;
const double signal3Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal3Amp = 300;

signal1Amp * (sin(numSamples * (twoPi * signal1Cycles) / samples)) + signal2Amp * (sin(numSamples * (twoPi * signal2Cycles) / samples)) + signal3Amp * (sin(numSamples * (twoPi * signal3Cycles) / samples))

// 5 wave generator
const double signal1Freq = 120;
const double signal1Cycles = (((samples-1) * signal1Freq) / samplingFrequency);
const double signal1Amp = 500;
const double signal2Freq = 1722;
const double signal2Cycles = (((samples-1) * signal2Freq) / samplingFrequency);
const double signal2Amp = 300;
const double signal3Freq = 2583;
const double signal3Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal3Amp = 300;
const double signal4Freq = 5167;
const double signal4Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal4Amp = 200;
const double signal5Freq = 10335;
const double signal5Cycles = (((samples-1) * signal3Freq) / samplingFrequency);
const double signal5Amp = 200;

signal1Amp * (sin(numSamples * (twoPi * signal1Cycles) / samples)) + signal2Amp * (sin(numSamples * (twoPi * signal2Cycles) / samples)) + signal3Amp * (sin(numSamples * (twoPi * signal3Cycles) / samples)) + signal4Amp * (sin(numSamples * (twoPi * signal4Cycles) / samples)) + signal5Amp * (sin(numSamples * (twoPi * signal5Cycles) / samples))
