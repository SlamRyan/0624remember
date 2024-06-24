// image_processing_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <ins_image_processing.h>
#include <opencv2/opencv.hpp>

int main()
{
	// 加载图片
    std::cout << "Hello World!\n";
	cv::Mat src= cv::imread("./image/a.png", 0);

	// 开始抽帧，图像内存在函数内部申请，需在使用后调用 DestroyImageArray进行内存释放
	InsImage srcImage[8];
	ImageExtarction(src.data, src.cols, src.rows, src.step, srcImage);

	// 合成，图像内存在函数内部申请，需在使用后调用 DestroyImageArray进行内存释放
	// 默认四张图顺序：Normal/Specular/Diffuse/Shape

    void* model;
	CreateLumitraxToolPtr(model);
	InsImage dstImage[5];
	CreateLumitrxImage(model, srcImage, dstImage);
	DestroyLumitraxToolPtr(model);

	// 显示及保存图片
	cv::Mat X1 = cv::Mat(srcImage[0].height, srcImage[0].width, CV_8UC1 , srcImage[0].data, srcImage[0].stride);
	cv::Mat X2 = cv::Mat(srcImage[1].height, srcImage[1].width, CV_8UC1 , srcImage[1].data, srcImage[1].stride);
	cv::Mat X3 = cv::Mat(srcImage[2].height, srcImage[2].width, CV_8UC1 , srcImage[2].data, srcImage[2].stride);
	cv::Mat X4 = cv::Mat(srcImage[3].height, srcImage[3].width, CV_8UC1 , srcImage[3].data, srcImage[3].stride);
	cv::Mat Y1 = cv::Mat(srcImage[4].height, srcImage[4].width, CV_8UC1 , srcImage[4].data, srcImage[4].stride);
	cv::Mat Y2 = cv::Mat(srcImage[5].height, srcImage[5].width, CV_8UC1 , srcImage[5].data, srcImage[5].stride);
	cv::Mat Y3 = cv::Mat(srcImage[6].height, srcImage[6].width, CV_8UC1 , srcImage[6].data, srcImage[6].stride);
	cv::Mat Y4 = cv::Mat(srcImage[7].height, srcImage[7].width, CV_8UC1 , srcImage[7].data, srcImage[7].stride);
	
	cv::Mat Normal = cv::Mat(dstImage[0].height, dstImage[0].width, CV_8UC1 , dstImage[0].data, dstImage[0].stride);
	cv::Mat Specular = cv::Mat(dstImage[1].height, dstImage[1].width, CV_8UC1 , dstImage[1].data, dstImage[1].stride);
	cv::Mat Diffuse = cv::Mat(dstImage[2].height, dstImage[2].width, CV_8UC1 , dstImage[2].data, dstImage[2].stride);
	cv::Mat GlossRatio = cv::Mat(dstImage[3].height, dstImage[3].width, CV_8UC1 , dstImage[3].data, dstImage[3].stride);
	cv::Mat Shape = cv::Mat(dstImage[4].height, dstImage[4].width, CV_8UC1 , dstImage[4].data, dstImage[4].stride);
	
	cv::imwrite("./image/X1.bmp", X1);
	cv::imwrite("./image/X2.bmp", X2);
	cv::imwrite("./image/X3.bmp", X3);
	cv::imwrite("./image/X4.bmp", X4);
	cv::imwrite("./image/Y1.bmp", Y1);
	cv::imwrite("./image/Y2.bmp", Y2);
	cv::imwrite("./image/Y3.bmp", Y3);
	cv::imwrite("./image/Y4.bmp", Y4);
	cv::imwrite("./image/Normal.bmp", Normal);
	cv::imwrite("./image/Specular.bmp", Specular);
	cv::imwrite("./image/Diffuse.bmp", Diffuse);
	cv::imwrite("./image/GlossRatio.bmp", GlossRatio);
	cv::imwrite("./image/Shape.bmp", Shape);

	// 释放内存
	DestroyImageArray(srcImage, 8);
	DestroyImageArray(dstImage, 5);

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
