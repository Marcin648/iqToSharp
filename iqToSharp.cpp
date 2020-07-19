#include <cstdio>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <unistd.h>

struct WavHeader{
  uint32_t chunkID; // == 'RIFF'
  uint32_t chunkSize;
  uint32_t format; // == 'WAVE'
  uint32_t subchunk1ID; // == 'fmt '
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  uint32_t subchunk2ID; // == 'data'
  uint32_t subchunk2Size;
};

template<class T>
struct complex{
  T re;
  T im;
};

void usage(){
  fprintf(stderr,
          "iqToSharp, an I/Q converter for SDRSharp\n\n"
          "Usage:\t -i input file\n"
          "\t[-o output file without .wav extension (default: out)]\n"
          "\t[-b buffer size (default: 2048)]\n"
          "\t[-s samplerate (default: 2048000 Hz)]\n"
          "\t[-f center frequency (default: 0)]\n"
          "\t[-z add 1 sec of silence]\n");

  exit(1);
}

int main(int argc, char **argv){

  const char *inFileName = NULL;
  const char *outFileNameArg = "out";
  uint32_t sampleRate = 2048000;
  const char *centerFreq = "0";
  bool silence = false;

  int c;
  size_t bufsize = 2048;

  while ((c = getopt(argc, argv, "i:b:o:s:f:z")) != -1) {
    switch(c){
      case 'i':
        inFileName = optarg;
        break;
      case 'o':
        outFileNameArg = optarg;
        break;
      case 's':
        sampleRate = atoi(optarg);
        break;
	  case 'b':
        bufsize = atoi(optarg);
        break;
      case 'f':
        centerFreq = optarg;
        break;
      case 'z':
          silence = true;
          break;
      default:
        usage();
        break;
    }


  }

  if(optind < argc) usage();

  char outFileName[255] = {0};
  snprintf(outFileName, 255, "%s_%sHz_IQ.wav", outFileNameArg, centerFreq);


  FILE *f;
  FILE *fout;

  f = fopen(inFileName, "rb");

  if(!f){
      puts("failed open input file");
      return 1;
  }

  fout = fopen(outFileName, "wb");

  if(!fout){
      puts("failed open output file");
      return 1;
  }

  printf("Input file: %s\n"
         "Output file: %s\n"
         "Sample rate: %d\n"
         "Frequency: %sHz\n",
         inFileName, outFileName, sampleRate, centerFreq);

  printf("Write samples...");
  fflush(stdout);
  fseek(fout, sizeof(WavHeader), SEEK_SET);

  complex<int8_t> ctx[bufsize];

  size_t dataSize = 0;
  size_t elemRead = 0;
  while((elemRead=fread(&ctx, sizeof(complex<int8_t>), bufsize, f))>0){

    //processing

    fwrite(&ctx, sizeof(complex<int8_t>), elemRead, fout);
    dataSize += sizeof(complex<int8_t>)*elemRead;
  }

  printf("DONE\n");
  fflush(stdout);

  if(silence){
    printf("Write silence...");
    fflush(stdout);

    ctx[0].re = 0; ctx[0].im = 0;

    for(uint32_t i = 0; i < sampleRate; i++){
        fwrite(&ctx, sizeof(complex<int8_t>), 1, fout);
        dataSize += sizeof(complex<int8_t>);
    }

    printf("DONE\n");
    fflush(stdout);
  }


  printf("Write wav header...");
  fflush(stdout);

  size_t fileSize = sizeof(WavHeader) + dataSize;

  WavHeader wav;

  wav.chunkID = 0x46464952; // == 'RIFF'
  wav.chunkSize = fileSize - 8;
  wav.format = 0x45564157; // == 'WAVE'
  wav.subchunk1ID = 0x20746D66; // == 'fmt '
  wav.subchunk1Size = 16;
  wav.audioFormat = 1;
  wav.numChannels = 2;
  wav.sampleRate = sampleRate;

  wav.bitsPerSample = sizeof(ctx[0].re)*8;

  wav.byteRate = wav.sampleRate *
                  wav.numChannels *
                  wav.bitsPerSample/8;

  wav.blockAlign = wav.numChannels * wav.bitsPerSample/8;
  wav.subchunk2ID = 0x61746164; // == 'data'
  wav.subchunk2Size = dataSize;

  fseek(fout, 0, SEEK_SET);
  fwrite(&wav, sizeof(wav), 1, fout);
  fclose(f);
  fclose(fout);

  printf("DONE\n");
  fflush(stdout);
  return 0;
}
