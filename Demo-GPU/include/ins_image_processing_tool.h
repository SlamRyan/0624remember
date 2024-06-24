#pragma once
#ifdef _WIN32
#ifdef INSIMAGEPROCESSING_EXPORTS
#define INSTOOL_API __declspec(dllexport)
#else
#define INSTOOL_API __declspec(dllimport)
#endif
#else
#define INSTOOL_API
#endif

#include <opencv2/core.hpp>

#define IN
#define OUT

enum INS_PMD_ENABLED_FLAG
{
	PMD_ALL_ENABLED = -1,
	PMD_NONE_ENABLED = 0,
	PMD_NORMAL_ENABLED = 1,
	PMD_SPECULAR_ENABLED = 2,
	PMD_DIFFUSE_ENABLED = 4,
	PMD_GLOSSRATIO_ENABLED = 8,
	PMD_SHAPE_ENABLED = 16,
	PMD_SHAPEABS_ENABLED = 32
};

struct InsPMDParams
{
	int   flag = PMD_ALL_ENABLED;
	float normal_k = 1.0;
	float normal_b = 0.0;
	float specular_k = 1.0;
	float specular_b = 0.0;
	float diffuse_k = 1.0;
	float diffuse_b = 0.0;
	float glossratio_k = 1.0;
	float glossratio_b = 0.0;
	float shape_k = 1.0;
	float shape_b = 0.0;
	float shapeabs_k = 1.0;
	float shapeabs_b = 0.0;
};


extern "C" INSTOOL_API void InsPMDLumitraxToolCreateInstance(void *& model);

extern "C" INSTOOL_API void InsPMDLumitraxToolDestroyInstance(void *model);

extern "C" INSTOOL_API void InsPMDLumitraxToolSetImage(void *model, cv::Mat image);

extern "C" INSTOOL_API void InsPMDLumitraxToolClearImage(void *model);

extern "C" INSTOOL_API void InsPMDLumitrxToolRun(void *model, InsPMDParams params, cv::Mat &normal, cv::Mat &specular, cv::Mat &diffuse, cv::Mat &glossratio, cv::Mat &shape, cv::Mat &shapeabs);
