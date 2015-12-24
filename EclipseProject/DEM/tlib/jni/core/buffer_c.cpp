

//#include <android/log.h>
#include <stdio.h>
#include <string.h>
#define LOG_TAG "xx"
#define LOGW(a )  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)


#include "buffer_c.h"


	buffer_c::buffer_c(int size)
	{
	        if (size < 1) { 
	            size = 1;
	        }
	        data_length=size;
	        data = new short[size + 1];//�����[���鳤��-1]��Ԫ�أ��������ֿջ�������
	        wPoint = 0;
	        rPoint = 0;
	}
	
	buffer_c::~buffer_c(void)
	{
		delete []data;
	}
    /**
     * �򻺳���д������
     *
     * @param wData д�������
     * @return �Ƿ�ɹ�д��
     */
	bool buffer_c::write(short *wData,int len) 
	{
		int wLen = len;//wData.length;
		if (wLen > 0)
		{
			if (wLen <= emptySize()) //�пռ����д
			{
				int old_wPoint=wPoint;
				wPoint = (wPoint + wLen) % data_length;//�ƶ�дָ��
		
				if (old_wPoint < rPoint) 
				{
					memcpy(&data[old_wPoint],wData,wLen*sizeof(short));
				} 
				else 
				{
					int emptySizeEnd = data_length - old_wPoint;//�ײ��ж��ٿհ׵Ŀռ�
					int writeSizeEnd = emptySizeEnd < wLen ? emptySizeEnd : wLen;//ʵ��д��ײ������ݵĳ���
					memcpy(&data[old_wPoint],wData,writeSizeEnd*sizeof(short));
					int writeSizeTop = wLen - writeSizeEnd;//ʵ��д��ͷ�������ݵĳ���
					if (writeSizeTop > 0) 
					{
						//�ײ���������ʣ��
						memcpy(data,&wData[writeSizeEnd],writeSizeTop*sizeof(short));	
					}
				}
				return true;
			}
		}
		return false;//�ռ�����
	}

    /**
     * �ӻ�������ȡ����
     *
     * @param rData ��ȡ������
     * @return �Ƿ�ɹ���ȡ
     */
    bool buffer_c::read(short *rData,int size) 
	{
        if (preview(rData,size, 0)) {
            doSkip(size);
            return true;
        } else {
            return false;
        }
    } 

    /**
     * �ӻ�����Ԥ������
     * Ԥ������*���ƶ�*��ָ��
     *
     * @param pData  Ԥ��������
     * @param offset �������ٸ����ݿ�ʼԤ��
     * @return �Ƿ�ɹ�Ԥ��
     */
    bool buffer_c::preview(short *pData, int pData_length,int offset) 
    	{
        int pLen = pData_length;
        if (pLen > 0 && offset >= 0) 
        	{
            if (pLen + offset <= fillSize()) 
            	{
                int pPoint = (rPoint + offset) % data_length;//����Ԥ��ָ��
                if (wPoint > pPoint) 
                {
                    //System.arraycopy(data, pPoint, pData, 0, pLen);
                    memcpy(pData,&data[pPoint],pLen*sizeof(short));
                } 
                else 
                {
                    int fillSizeEnd = data_length - pPoint;//�ײ��ж������Ŀռ�
                    int previewSizeEnd = fillSizeEnd < pLen ? fillSizeEnd : pLen;//ʵ��Ԥ����ײ������ݵĳ���
                    //System.arraycopy(data, pPoint, pData, 0, previewSizeEnd);
                    memcpy(pData,&data[pPoint],previewSizeEnd*sizeof(short));
                    int previewSizeTop = pLen - previewSizeEnd;//ʵ��Ԥ��ͷ�������ݵĳ���
                    if (previewSizeTop > 0) 
                    {
                        //�ײ�Ԥ���껹����
                        //System.arraycopy(data, 0, pData, previewSizeEnd, previewSizeTop);
                        memcpy(&pData[previewSizeEnd],data,previewSizeTop*sizeof(short));
                    }
                }
                return true;
            }
        }
        return false;
    }
    
    
    /**
     * �ӻ�������������
     * ����*�ƶ�*��ָ��
     *
     * @param count �������ݵĸ���
     * @return �Ƿ�ɹ�����
     */
    bool buffer_c::skip(int count) 
    	{
        if (0 <= count && count <= fillSize()) 
        {
            doSkip(count);
            return true;
        } else {
            return false;
        }
    }

   void buffer_c::doSkip(int count) 
   {
        rPoint = (rPoint + count) % data_length;
    }





    /**
     * ��ȡ�������հ������С
     *
     * @return �հ������С
     */
	int buffer_c::emptySize() 
	{
        return size() - fillSize();
    }
    
        /**
     * ��ȡ��������������С
     *
     * @return ��������С
     */
    int buffer_c::fillSize() 
    	{
        if (wPoint < rPoint) {
            return wPoint - rPoint + data_length;
        } else {
            return wPoint - rPoint;
        }
    }
    
    
       /**
     * ��ȡ��ǰ�������Ĵ�С
     *
     * @return ���λ������Ĵ�С
     */
    int buffer_c::size()
    	 {
        return data_length - 1;
    }
    
    
        /**
     * ��������û��д������ȡ������ȡ��ǰ����������������
     *
     * @return ���λ��������е����ݣ�����һ�����ݷ��أ��Է�ֹ�޸ģ�
     */
    int buffer_c::getData(short *data_v,int *data_size_v) 
    {
    		memcpy(data_v,data,data_length*sizeof(short));
    		memcpy(data_size_v,&data_length,sizeof(int));
    
		return 0;		
    }
    
    
    /**
     * ��ȡ��ǰдָ��
     *
     * @return дָ��
     */
    int buffer_c::getwPoint() 
	{
        return wPoint;
    }

    /**
     * ��ȡ��ǰ��ָ��
     *
     * @return ��ָ��
     */
    int buffer_c::getrPoint() 
	{
        return rPoint;
    }
    
    
/******************************************************************************
���ƣ�setBufferSize
���ܣ����û�������С
������
���أ�
��ע��

******************************************************************************/
void buffer_c::setBufferSize(int size)
{
	if(data!=NULL)
	{
		delete [] data;
	}
	if (size < 1) 
	{
		size = 1;
	}
	data_length=size;
	data = new short[size + 1];//�����[���鳤��-1]��Ԫ�أ��������ֿջ�������
	wPoint = 0;
	rPoint = 0;
}


/******************************************************************************
���ƣ�clearBuffer
���ܣ���ջ�����
������
���أ�
��ע��

******************************************************************************/
void buffer_c::clearBuffer()
{
	if(data!=NULL)
	{
		memset(data,0,data_length*sizeof(short));
	}
	wPoint = 0;
	rPoint = 0;
}
