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
 @param（input） srcImg : 8张图片
 @param（input） gain_normal: normal图增益
 @param（input） gain_specular: 镜面反射图增益
 @param（input） gain_diffuse: 漫反射图增益
 @param（input） abs_mode: shape图是否为绝对值模式 0 - 绝对值； 1 - 非绝对值
 @param（input） gain_shape: Shape图增益
 @param（output） dstImg: 返回合成后的图片，目前共返回5张：
					dstImg[0] - normal图
					dstImg[1] - specular图
					dstImg[2] - diffuse图
					dstImg[3] - glossratio图
					dstImg[4] - shape图
*/
extern "C" INSTOOL_API void CreateLumitrxImage(void *model, InsImage *srcImg, InsImage *dstImg);

extern "C" INSTOOL_API void ImageExtarction(unsigned char *srcDta, int inWidth, int inHeight, int inStride, int num, InsImage *dstData);

extern "C" INSTOOL_API void DestroyImageArray(InsImage *dstData, int num);