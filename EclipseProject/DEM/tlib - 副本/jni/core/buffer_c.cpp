

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
	        data = new short[size + 1];//最多存放[数组长度-1]个元素，便于区分空环与满环
	        wPoint = 0;
	        rPoint = 0;
	}
	
	buffer_c::~buffer_c(void)
	{
		delete []data;
	}
    /**
     * 向缓冲区写入数据
     *
     * @param wData 写入的数据
     * @return 是否成功写入
     */
	bool buffer_c::write(short *wData,int len) 
	{
		int wLen = len;//wData.length;
		if (wLen > 0)
		{
			if (wLen <= emptySize()) //有空间可以写
			{
				int old_wPoint=wPoint;
				wPoint = (wPoint + wLen) % data_length;//移动写指针
		
				if (old_wPoint < rPoint) 
				{
					memcpy(&data[old_wPoint],wData,wLen*sizeof(short));
				} 
				else 
				{
					int emptySizeEnd = data_length - old_wPoint;//底部有多少空白的空间
					int writeSizeEnd = emptySizeEnd < wLen ? emptySizeEnd : wLen;//实际写入底部的数据的长度
					memcpy(&data[old_wPoint],wData,writeSizeEnd*sizeof(short));
					int writeSizeTop = wLen - writeSizeEnd;//实际写入头部的数据的长度
					if (writeSizeTop > 0) 
					{
						//底部填满还有剩余
						memcpy(data,&wData[writeSizeEnd],writeSizeTop*sizeof(short));	
					}
				}
				return true;
			}
		}
		return false;//空间满了
	}

    /**
     * 从缓冲区读取数据
     *
     * @param rData 读取的数据
     * @return 是否成功读取
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
     * 从缓冲区预览数据
     * 预览数据*不移动*读指针
     *
     * @param pData  预览的数据
     * @param offset 跳过多少个数据开始预览
     * @return 是否成功预览
     */
    bool buffer_c::preview(short *pData, int pData_length,int offset) 
    	{
        int pLen = pData_length;
        if (pLen > 0 && offset >= 0) 
        	{
            if (pLen + offset <= fillSize()) 
            	{
                int pPoint = (rPoint + offset) % data_length;//计算预览指针
                if (wPoint > pPoint) 
                {
                    //System.arraycopy(data, pPoint, pData, 0, pLen);
                    memcpy(pData,&data[pPoint],pLen*sizeof(short));
                } 
                else 
                {
                    int fillSizeEnd = data_length - pPoint;//底部有多少填充的空间
                    int previewSizeEnd = fillSizeEnd < pLen ? fillSizeEnd : pLen;//实际预览入底部的数据的长度
                    //System.arraycopy(data, pPoint, pData, 0, previewSizeEnd);
                    memcpy(pData,&data[pPoint],previewSizeEnd*sizeof(short));
                    int previewSizeTop = pLen - previewSizeEnd;//实际预览头部的数据的长度
                    if (previewSizeTop > 0) 
                    {
                        //底部预览完还不够
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
     * 从缓冲区跳过数据
     * 跳过*移动*读指针
     *
     * @param count 跳过数据的个数
     * @return 是否成功跳过
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
     * 获取缓冲区空白区域大小
     *
     * @return 空白区域大小
     */
	int buffer_c::emptySize() 
	{
        return size() - fillSize();
    }
    
        /**
     * 获取缓冲区填充区域大小
     *
     * @return 填充区域大小
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
     * 获取当前缓冲区的大小
     *
     * @return 环形缓冲区的大小
     */
    int buffer_c::size()
    	 {
        return data_length - 1;
    }
    
    
        /**
     * 不区分有没有写入过或读取过，获取当前缓冲区的所有数据
     *
     * @return 环形缓冲区所有的数据（复制一份数据返回，以防止修改）
     */
    int buffer_c::getData(short *data_v,int *data_size_v) 
    {
    		memcpy(data_v,data,data_length*sizeof(short));
    		memcpy(data_size_v,&data_length,sizeof(int));
    
		return 0;		
    }
    
    
    /**
     * 获取当前写指针
     *
     * @return 写指针
     */
    int buffer_c::getwPoint() 
	{
        return wPoint;
    }

    /**
     * 获取当前读指针
     *
     * @return 读指针
     */
    int buffer_c::getrPoint() 
	{
        return rPoint;
    }
    
    
/******************************************************************************
名称：setBufferSize
功能：重置缓冲区大小
参数：
返回：
备注：

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
	data = new short[size + 1];//最多存放[数组长度-1]个元素，便于区分空环与满环
	wPoint = 0;
	rPoint = 0;
}


/******************************************************************************
名称：clearBuffer
功能：清空缓冲区
参数：
返回：
备注：

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
