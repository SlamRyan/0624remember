#include "ImageProcessor.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "ins_image_processing.h"
#include "ins_global.h"
#include "insglobalvar.h"

ImageProcessor::ImageProcessor(QObject* parent) : QObject(parent)
{
    shouldStopProcessing = false;
}


void ImageProcessor::processImages(unsigned char* pbuffer, int cameraWidth, int cameraHeight, std::string appDirCv, bool freeCollect, int picNumCount)
{
    qDebug() << "Entered processImages";
    qDebug() << "[ImageProcessor] Processing images...";
    int cameraFrameSize = cameraHeight * cameraWidth;
    if (shouldStopProcessing)
    {
        return;
    }

    if (freeCollect)
    {
        cv::Mat img = cv::Mat(cameraHeight, cameraWidth, CV_8UC1, pbuffer);
        cv::imwrite(appDirCv + "/image/Normal.bmp", img);
    }
    else
    {
        if (picNumCount == 9)
        {
            void* toolPtr;
            uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize * sizeof(uint8_t));//0925
            memcpy(l_img_buffer, pbuffer, cameraFrameSize);//处理l_img_buffer //0925

        //===========================================================================0823+
        //cv::Mat src = l_img_buffer;

        //cv::Mat src(InsGlobalVar::lines,InsGlobalVar::m_Width, CV_8UC1, l_img_buffer);

        //===================================图像处理==============================================

            InsImage srcImage[9];//122089
            ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth, 9, srcImage);//1211
            CreateLumitraxToolPtr(toolPtr);

            SetLumitraxToolParam(toolPtr, 1, 1, 1, false, 1, 0);//1.0绝对值模式//toolPtr, normalGain, specualrGain, diffuseGain, false, shapeGain, 0

            InsImage dstImage[5];

            CreateLumitrxImage(toolPtr, srcImage, dstImage);

            // 显示及保存图片
             //0925

            cv::Mat X1 = cv::Mat(srcImage[0].height, srcImage[0].width, CV_8UC1, srcImage[0].data, srcImage[0].stride);
            cv::Mat X2 = cv::Mat(srcImage[1].height, srcImage[1].width, CV_8UC1, srcImage[1].data, srcImage[1].stride);
            cv::Mat X3 = cv::Mat(srcImage[2].height, srcImage[2].width, CV_8UC1, srcImage[2].data, srcImage[2].stride);
            cv::Mat X4 = cv::Mat(srcImage[3].height, srcImage[3].width, CV_8UC1, srcImage[3].data, srcImage[3].stride);
            cv::Mat Y1 = cv::Mat(srcImage[4].height, srcImage[4].width, CV_8UC1, srcImage[4].data, srcImage[4].stride);
            cv::Mat Y2 = cv::Mat(srcImage[5].height, srcImage[5].width, CV_8UC1, srcImage[5].data, srcImage[5].stride);
            cv::Mat Y3 = cv::Mat(srcImage[6].height, srcImage[6].width, CV_8UC1, srcImage[6].data, srcImage[6].stride);
            cv::Mat Y4 = cv::Mat(srcImage[7].height, srcImage[7].width, CV_8UC1, srcImage[7].data, srcImage[7].stride);
            cv::Mat IR = cv::Mat(srcImage[8].height, srcImage[8].width, CV_8UC1, srcImage[8].data, srcImage[8].stride);

            cv::Mat Normal = cv::Mat(dstImage[0].height, dstImage[0].width, CV_8UC1, dstImage[0].data, dstImage[0].stride);
            cv::Mat Specular = cv::Mat(dstImage[1].height, dstImage[1].width, CV_8UC1, dstImage[1].data, dstImage[1].stride);
            cv::Mat Diffuse = cv::Mat(dstImage[2].height, dstImage[2].width, CV_8UC1, dstImage[2].data, dstImage[2].stride);
            cv::Mat Glossratio = cv::Mat(dstImage[3].height, dstImage[3].width, CV_8UC1, dstImage[3].data, dstImage[3].stride);
            cv::Mat Shape = cv::Mat(dstImage[4].height, dstImage[4].width, CV_8UC1, dstImage[4].data, dstImage[4].stride);//aaa

            cv::imwrite(appDirCv + "/image/X1.bmp", X1);//0919 need change//1018changeaabbb
            cv::imwrite(appDirCv + "/image/X2.bmp", X2);
            cv::imwrite(appDirCv + "/image/X3.bmp", X3);
            cv::imwrite(appDirCv + "/image/X4.bmp", X4);
            cv::imwrite(appDirCv + "/image/Y1.bmp", Y1);
            cv::imwrite(appDirCv + "/image/Y2.bmp", Y2);
            cv::imwrite(appDirCv + "/image/Y3.bmp", Y3);
            cv::imwrite(appDirCv + "/image/Y4.bmp", Y4);
            cv::imwrite(appDirCv + "/image/Normal.bmp", Normal);
            cv::imwrite(appDirCv + "/image/Specular.bmp", Specular);
            cv::imwrite(appDirCv + "/image/Diffuse.bmp", Diffuse);
            cv::imwrite(appDirCv + "/image/Shape.bmp", Shape);
            cv::imwrite(appDirCv + "/image/Glossratio.bmp", Glossratio);//1018改路径
            cv::imwrite(appDirCv + "/image/IR.bmp", IR);


            //===========================================================================0823end


            free(l_img_buffer);//释放
            l_img_buffer = NULL;
            InsGlobalVar::pixelView_update_flag = false;
            //qDebug()<<"CCC";
            DestroyImageArray(srcImage, 9);
            DestroyImageArray(dstImage, 5);
            DestroyLumitraxToolPtr(toolPtr);

        }

        else if (picNumCount == 8)
        {
            void* toolPtr;


            qDebug() << "BBB";

            qDebug() << "[pbuffer:]" << pbuffer;
            qDebug() << *pbuffer << endl;

            uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize * sizeof(uint8_t));//0925


            memcpy(l_img_buffer, pbuffer, cameraFrameSize);//处理l_img_buffer //0925

            qDebug() << "[l_img_buffer:]" << l_img_buffer;
            qDebug() << *l_img_buffer << endl;



            //===================================图像处理==============================================

            InsImage srcImage[8];//122089

            ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth, 8, srcImage);//1211

            CreateLumitraxToolPtr(toolPtr);

            SetLumitraxToolParam(toolPtr, 1, 1, 1, false, 1, 0);//1.0绝对值模式//toolPtr, normalGain, specualrGain, diffuseGain, false, shapeGain, 0

            InsImage dstImage[5];

            CreateLumitrxImage(toolPtr, srcImage, dstImage);

            qDebug() << "[ImageExtarction:]";

            // 显示及保存图片
             //0925

            cv::Mat X1 = cv::Mat(srcImage[0].height, srcImage[0].width, CV_8UC1, srcImage[0].data, srcImage[0].stride);
            cv::Mat X2 = cv::Mat(srcImage[1].height, srcImage[1].width, CV_8UC1, srcImage[1].data, srcImage[1].stride);
            cv::Mat X3 = cv::Mat(srcImage[2].height, srcImage[2].width, CV_8UC1, srcImage[2].data, srcImage[2].stride);
            cv::Mat X4 = cv::Mat(srcImage[3].height, srcImage[3].width, CV_8UC1, srcImage[3].data, srcImage[3].stride);
            cv::Mat Y1 = cv::Mat(srcImage[4].height, srcImage[4].width, CV_8UC1, srcImage[4].data, srcImage[4].stride);
            cv::Mat Y2 = cv::Mat(srcImage[5].height, srcImage[5].width, CV_8UC1, srcImage[5].data, srcImage[5].stride);
            cv::Mat Y3 = cv::Mat(srcImage[6].height, srcImage[6].width, CV_8UC1, srcImage[6].data, srcImage[6].stride);
            cv::Mat Y4 = cv::Mat(srcImage[7].height, srcImage[7].width, CV_8UC1, srcImage[7].data, srcImage[7].stride);
            //cv::Mat IR = cv::Mat(srcImage[8].height, srcImage[8].width, CV_8UC1 , srcImage[8].data, srcImage[8].stride);



            cv::Mat Normal = cv::Mat(dstImage[0].height, dstImage[0].width, CV_8UC1, dstImage[0].data, dstImage[0].stride);
            cv::Mat Specular = cv::Mat(dstImage[1].height, dstImage[1].width, CV_8UC1, dstImage[1].data, dstImage[1].stride);
            cv::Mat Diffuse = cv::Mat(dstImage[2].height, dstImage[2].width, CV_8UC1, dstImage[2].data, dstImage[2].stride);
            cv::Mat Glossratio = cv::Mat(dstImage[3].height, dstImage[3].width, CV_8UC1, dstImage[3].data, dstImage[3].stride);
            cv::Mat Shape = cv::Mat(dstImage[4].height, dstImage[4].width, CV_8UC1, dstImage[4].data, dstImage[4].stride);//aaa

            cv::imwrite(appDirCv + "/image/X1.bmp", X1);//0919 need change//1018changeaabbb
            cv::imwrite(appDirCv + "/image/X2.bmp", X2);
            cv::imwrite(appDirCv + "/image/X3.bmp", X3);
            cv::imwrite(appDirCv + "/image/X4.bmp", X4);
            cv::imwrite(appDirCv + "/image/Y1.bmp", Y1);
            cv::imwrite(appDirCv + "/image/Y2.bmp", Y2);
            cv::imwrite(appDirCv + "/image/Y3.bmp", Y3);
            cv::imwrite(appDirCv + "/image/Y4.bmp", Y4);
            cv::imwrite(appDirCv + "/image/Normal.bmp", Normal);
            cv::imwrite(appDirCv + "/image/Specular.bmp", Specular);
            cv::imwrite(appDirCv + "/image/Diffuse.bmp", Diffuse);
            cv::imwrite(appDirCv + "/image/Shape.bmp", Shape);
            cv::imwrite(appDirCv + "/image/Glossratio.bmp", Glossratio);//1018改路径
            //cv::imwrite(appDirCv + "/image/IR.bmp", IR);


            //===========================================================================0823end



            free(l_img_buffer);//释放

            l_img_buffer = NULL;
            InsGlobalVar::pixelView_update_flag = false;
            //qDebug()<<"CCC";
            DestroyImageArray(srcImage, 8);
            DestroyImageArray(dstImage, 5);
            DestroyLumitraxToolPtr(toolPtr);

        }
    }

    emit imagesProcessed();
    frameId++;
}
void ImageProcessor::setStopProcessing(bool stop)
{
    shouldStopProcessing = stop;
}