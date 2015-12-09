
#ifndef DEM
#define DEM
#ifdef __cplusplus
extern "C" {
#endif

int max(int x,int y,short buf_p[],int buf_size);

void start();
void stop();
int getStatus();
int getBufferSize() ; 
void setBufferSize(int size);
void clearBuffer();
int wirte(short data[],int data_size); 
double fillRate();
short* getRegSignal(int &len_v);
//int dem_init(JavaVM * gJavaVM_v,jobject  gJavaObj_v,jclass  gJavaClass_v,
    int dem_init(int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size);
extern double ST;
extern int thread_1_stause;
#ifdef __cplusplus
}
#endif
#endif

