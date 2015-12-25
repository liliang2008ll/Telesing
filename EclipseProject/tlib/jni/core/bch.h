


#ifndef BCH
#define BCH
#ifdef __cplusplus
extern "C" {
#endif

int bch_init();
int bch_test_func();

void bch_decoder(unsigned long EncodedMessage_v,unsigned char *out);
int  bch_encode(unsigned char a, unsigned char b,unsigned long *EncodedMessage_v);
int  Mistake(int bch) ;


#ifdef __cplusplus
}
#endif
#endif


