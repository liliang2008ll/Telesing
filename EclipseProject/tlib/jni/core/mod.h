#ifndef __MOD
#define __MOD

//sample number
#define A_SYMBOL_N  4
#define B_SYMBOL_N  4
#define S_SYMBOL_N  2

//frame parts
#define SYMBOL_HEAD_LENGTH       128
#define SYMBOL_NECK_LENGTH       128
#define SYMBOL_BODY_LENGTH       2048
#define SYMBOL_TAIL_LENGTH       128

//frame data length
#define SYMBOL_FRAME_LENGHT      (SYMBOL_HEAD_LENGTH + SYMBOL_NECK_LENGTH + \
								  SYMBOL_BODY_LENGTH + SYMBOL_TAIL_LENGTH)//2432
//waves length
#define WAVE_HEAD_LENGTH   1
#define WAVE_BODY_LENGTH   62

#define WAVE_FRAME_LENGTH  (WAVE_HEAD_LENGTH + WAVE_BODY_LENGTH)   //63
#define WAVE_SIZE          (WAVE_FRAME_LENGTH * SYMBOL_FRAME_LENGHT)//63*2432



class WAVE{

private:
		double *s_td,*a_td,*b_td;
		double *last_frame;
		double *wave;
		double *gern_head_frame(char td_code);
		double *gern_data_frame(char td_code);


public:

		double *create_wava(int frame_data);

		WAVE();

		~WAVE();

};
short* create_short_wave(int data);


#endif

