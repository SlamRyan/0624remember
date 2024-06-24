#pragma once

#ifdef INSIMAGEPROCESSING_EXPORTS
#define INSTOOL_API __declspec(dllexport)
#else
#define INSTOOL_API __declspec(dllimport)
#endif

typedef struct InsImage
{
	int width;
	int height;
	int stride;
	unsigned char *data;
};


extern "C" INSTOOL_API void CreateLumitraxToolPtr(void *& model);
extern "C" INSTOOL_API void DestroyLumitraxToolPtr(void *model);
extern "C" INSTOOL_API void SetLumitraxToolParam(void* model, double gain_normal, double gain_specular, double gain_diffuse,bool abs_mode, double gain_shape, int noise_reduction_size);


/*
 @param��input�� srcImg : 8��ͼƬ
 @param��input�� gain_normal: normalͼ����
 @param��input�� gain_specular: ���淴��ͼ����
 @param��input�� gain_diffuse: ������ͼ����
 @param��input�� abs_mode: shapeͼ�Ƿ�Ϊ����ֵģʽ 0 - ����ֵ�� 1 - �Ǿ���ֵ
 @param��input�� gain_shape: Shapeͼ����
 @param��output�� dstImg: ���غϳɺ��ͼƬ��Ŀǰ������5�ţ�
					dstImg[0] - normalͼ
					dstImg[1] - specularͼ
					dstImg[2] - diffuseͼ
					dstImg[3] - glossratioͼ
					dstImg[4] - shapeͼ
*/
extern "C" INSTOOL_API void CreateLumitrxImage(void *model, InsImage *srcImg, InsImage *dstImg);

extern "C" INSTOOL_API void ImageExtarction(unsigned char *srcDta, int inWidth, int inHeight, int inStride, int num, InsImage *dstData);

extern "C" INSTOOL_API void DestroyImageArray(InsImage *dstData, int num);