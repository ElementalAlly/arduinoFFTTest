// Factors to change

const int num_readings = 100;

const uint16_t samples = 512;

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

float vReal[samples];

uint64_t total = 0;
int num_times = 0;
double twoPi = 6.283185307;

uint8_t bit_reverse(const uint8_t nbits, uint8_t val) {
  switch(nbits) {
    case 8:
      val = bit_reverse(4, (val&0xf0)>>4) | (bit_reverse(4, val&0x0f)<<4);
      break;
    case 7:
      val = bit_reverse(3, (val&0x70)>>4) | (val&0x08) | (bit_reverse(3, val&0x07)<<4);
      break;
    case 6:
      val = bit_reverse(3, (val&0x38)>>3) | (bit_reverse(3, val&0x07)<<3);
      break;
    case 5:
      val = bit_reverse(2, (val&0x18)>>3) | (val&0x4) | (bit_reverse(2, val&0x3)<<3);
      break;
    case 4:
      val = bit_reverse(2, (val&0xc)>>2) | (bit_reverse(2, val&0x3)<<2);
      break;
    case 3:
      val = ((val&0x4)>>2) | (val&0x2) | ((val&0x1)<<2);
      break;
    case 2:
      val = ((val&0x2)>>1) | ((val&0x1)<<1);
      break;
    default:
      break;
  }
  return val;
}

void ExactFFT(float x[], const int size) {
  if (size == 1)
    return;
  /* indices */
  uint16_t i,j,k,n_1,array_num_bits;
  uint16_t n_2 = 1;
  /* temporary buffers that should be used right away. */
  float tmp,a,b,c,d,k1,k2,k3;
  /* Will store angles, and recursion values for cosine calculation */
  float cj, sj;

  uint16_t half_size = size >> 1;

  /* How many bits we need to store the positions in half the array.
     This switch could be replaced by a const variable to gain a bit
     of space if we were always computing on the same size of arrays.
  */
 
  switch(size) {
    case 2:
      array_num_bits = 0;
      break;
    case 4:
      array_num_bits = 1;
      break;
    case 8:
      array_num_bits = 2;
      break;
    case 16:
      array_num_bits = 3;
      break;
    case 32:
      array_num_bits = 4;
      break;
    case 64:
      array_num_bits = 5;
      break;
    case 128:
      array_num_bits = 6;
      break;
    case 256:
      array_num_bits = 7;
      break;
    case 512:
      array_num_bits = 8;
      break;
    default:
      array_num_bits = 0;
      break;
  }

  /* Reverse-bit ordering */
  for(i=0; i<half_size; ++i) {
    j = bit_reverse(array_num_bits, i);
        
    if(i<j) {
      /* Swapping real part */
      tmp = x[i<<1];
      x[i<<1] = x[j<<1];
      x[j<<1] = tmp;
      /* Swapping imaginary part */
      tmp = x[(i<<1)+1];
      x[(i<<1)+1] = x[(j<<1)+1];
      x[(j<<1)+1] = tmp;
    }
  }

  /* Actual FFT */
  for(i=0; i<array_num_bits; ++i){
    /* n_1 gives the size of the sub-arrays */
    n_1 = n_2; // n_1 = 2^i
    /* n_2 gives the number of steps required to go from one group of sub-arrays to another */
    n_2 = n_2<<1; // n_2 = 2^(i+1)

    /* j will be the index in Xe and Xo */
    for(j=0;j<n_1;j++) {
      /* We combine the jth elements of each group of sub-arrays */
      cj = cos(-2*PI*j/n_2);
      sj = sin(-2*PI*j/n_2);
      for(k=j; k<half_size; k+=n_2) {
        /* Now we calculate the next step of the fft process, i.e.
           X[j] = Xᵉ[j] + exp(-2im*pi*j/n₂) * Xᵒ[j]
           X[j+n₂/2] = Xᵉ[j] - exp(-2im*pi*j/n₂) * Xᵒ[j]
        */
        a = x[(k+n_1)<<1];
        b = x[((k+n_1)<<1)+1];
        c = x[k<<1];
        d = x[(k<<1)+1];
        k1 = cj * (a+b);
        k2 = a * (sj - cj);
        k3 = b * (cj + sj);
        tmp = k1 - k3;
        x[k<<1]           = c + tmp; 
        x[(k+n_1)<<1]     = c - tmp; 
        tmp = k1 + k2;
        x[(k<<1)+1]       = d + tmp; 
        x[((k+n_1)<<1)+1] = d - tmp; 
      }
    }
  }
  // Serial.println(" ");

  /* Building the final FT from its entangled version */
  /* Special case n=0 */
  x[0] = x[0] + x[1];
  x[1] = 0;
  for(j=1; j<=(half_size>>1); ++j) {
    cj = cos(-PI*j/half_size);
    sj = sin(-PI*j/half_size);
    
    a = x[j<<1] + x[(half_size-j)<<1];
    b = x[(j<<1)+1] - x[((half_size-j)<<1)+1];
    c = -x[(j<<1)+1] - x[((half_size-j)<<1)+1];
    d = x[j<<1] - x[(half_size-j)<<1];
    k1 = cj * (c+d);
    k2 = c*(sj-cj);
    k3 = d*(cj+sj);

    tmp = k1 - k3;
    x[j<<1]                 = ( a - tmp ) * 0.5;
    x[(half_size-j)<<1]     = ( a + tmp) * 0.5; 
    tmp = k1 + k2;
    x[(j<<1)+1]             = ( b - tmp) * 0.5;
    x[((half_size-j)<<1)+1] = (-b - tmp) * 0.5;
  }
}

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   while(!Serial);
   Serial.println("ready");
   Serial.println("Setup");

   TCCR1A = 0;
   TCCR1B = 1;

   Serial.println("setup complete");
}

void loop() {
  for (int i = 0; i < samples; i++) {
    vReal[i] = float(signal1Amp * (sin(i * (twoPi * signal1Cycles) / samples)) + signal2Amp * (sin(i * (twoPi * signal2Cycles) / samples)) + signal3Amp * (sin(i * (twoPi * signal3Cycles) / samples)) + signal4Amp * (sin(i * (twoPi * signal4Cycles) / samples)) + signal5Amp * (sin(i * (twoPi * signal5Cycles) / samples)));
  }

  uint32_t startTime = micros();
  ExactFFT(vReal, samples);
  uint32_t endTime = micros();
  uint32_t ETA = endTime - startTime;
  // uncomment when testing for accuracy
  /* for (int i = 0; i < samples / 2; i++) {
    Serial.println(abs(vReal[i]));
  }
  while(1); */

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
