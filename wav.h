/**
 *	
 *  WAV FILE STRUCTURE
 *
 *  @Author:REmaxer	
 *  @Version:1.0
 *
 */


/* define byte type */
typedef unsigned char byte;

/*
 *	General RIFF header
 */
struct riff_header_chunk{
	unsigned int chunk_id;		/* "RIFF" in ASCII (big-endian)*/
	unsigned int chunk_size;	/* Size of entire file minus chunk_id and chunk_size */
	unsigned int format;		/* File format (Wav Files = "WAVE" in ASCII ) (big-endian)*/
};	

/*
 *	Fmt header
 */
struct fmt_chunk{
	unsigned int schunk1_id;	/* "fmt " in ASCII (big-endian)*/
	unsigned int schunk1_size;	/* Size of the rest of subchunk fmt (16 for PCM ) */
	unsigned short audio_format;	/* Audio format ( 1 for PCM) */
	unsigned short num_channels;	/* Number of channels */
	unsigned int sample_rate;	/* Samples per seconds */
	unsigned int byte_rate;		/* Bytes per seconds ( sample_rate * block_align )*/
	unsigned short block_align;	/* Bytes per sample  ( including all channels ) */	
	unsigned short bits_per_sample; /* Bits per sample (excluding channels ) */
};

/*
 *	Data header
 */

struct data_chunk{
	unsigned int schunk2_id;	/* "DATA" in ASCII (big-endian)*/
	unsigned int schunk2_size;	/* Bytes in data ( num_samples * block_align )*/
	byte *data;			/* Generic data pointer */
};
	


