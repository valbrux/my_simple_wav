#include "wav.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define WAV_HEADER_LEN	12
#define WAV_FMT_LEN	24
#define SAMPLE_RATE 400
#define DURATION    20
#define BITS_PER_SAMPLE	16
#define NUM_CHANNELS	2
#define FILE_NAME	"sound.wav"
#define NUM_SAMPLES (DURATION*SAMPLE_RATE)


void make_wav_header(struct riff_header_chunk *,struct data_chunk *);
void make_wav_fmt(struct fmt_chunk *);
void make_wav_data(struct data_chunk *,struct fmt_chunk *);

void make_wav_fmt(struct fmt_chunk *fc){
	memcpy(&fc->schunk1_id,"fmt ",4);
	//16 PCM
	fc->schunk1_size = 16;
	//1 = PCM
	fc->audio_format        = (unsigned short) 1;
	fc->num_channels 	= (unsigned short) NUM_CHANNELS;
	fc->sample_rate  	= (unsigned int) SAMPLE_RATE;
	fc->bits_per_sample 	= (unsigned short) BITS_PER_SAMPLE;
	fc->byte_rate		= (unsigned int) SAMPLE_RATE * fc->num_channels * (fc->bits_per_sample/8); 	  
	fc->block_align	  = (unsigned short) fc->num_channels * (fc->bits_per_sample/8);
}

void make_wav_data(struct data_chunk *dc,struct fmt_chunk *fc){
	short *bp,val;
	int i;
	srand(time(NULL));
	memcpy(&dc->schunk2_id,"data",4);
	dc->schunk2_size	= (unsigned int)NUM_SAMPLES * fc->num_channels * (fc->bits_per_sample/8);
	dc->data = (byte *)malloc(dc->schunk2_size);
	bp = (short *)dc->data;
	for(i = 0;i<dc->schunk2_size;i+=2){
		val = rand();
		*bp = val;
		printf("%d \n",val);
		bp++;
	}
}

void make_wav_header(struct riff_header_chunk *rhc,struct data_chunk *dc){
	int i;
	memcpy(&rhc->chunk_id,"RIFF",4);
	memcpy(&rhc->format,"WAVE",4);
	rhc->chunk_size = (unsigned int) 36 + dc->schunk2_size;
}


void write_file(struct riff_header_chunk *rhc,struct fmt_chunk *fc,struct data_chunk *dc){
	FILE *f;
	if((f = fopen(FILE_NAME,"wb") )== NULL){
		perror("Error opening file ");
	}
	fwrite(rhc,sizeof(struct riff_header_chunk),1,f);
	fwrite(fc,sizeof(struct fmt_chunk),1,f);
	fwrite(&dc->schunk2_id,sizeof(unsigned int),1,f);
	fwrite(&dc->schunk2_size,sizeof(unsigned int),1,f);
	fwrite(dc->data,1,dc->schunk2_size,f);
	fclose(f);
}

int main(int argc,char *argv[]){
	char *p;
	int i;
	struct riff_header_chunk *rhc;
	struct fmt_chunk *fc;
	struct data_chunk *dc;
	rhc = (struct riff_header_chunk *)malloc(sizeof(struct riff_header_chunk));
	fc = (struct fmt_chunk *)malloc(sizeof(struct fmt_chunk));
	dc = (struct data_chunk *)malloc(sizeof(struct data_chunk));
	make_wav_fmt(fc);
	make_wav_data(dc,fc);
	make_wav_header(rhc,dc);
	p = (char *)rhc;
	printf("\nHEADER \n");
	for(i = 0;i<12;i++){
		printf("%02hhx ",*p);
		p++;
	}
	p = (char *)fc;
	printf("\nFMT \n");
	for(i = 0;i<24;i++){
		printf("%02hhx ",*p);
		p++;
	}
	p = (char *)dc;
	printf("\nDATA \n");
	for(i = 0;i<8;i++){
		printf("%02hhx ",*p);
		p++;
	}
	p = (char *)dc->data;
	for(i=0;i<dc->schunk2_size;i++){
		printf("%02hhx ",*p);
		p++;
	}
	printf("\nEND\n");
	printf("\n Writing on file ... \n");
	write_file(rhc,fc,dc);
	printf("\n Written ... \n ");
	free(dc->data);
	free(rhc);
	free(fc);
	free(dc);
	return 0;
}



