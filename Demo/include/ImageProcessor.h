#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <opencv2/opencv.hpp>



class ImageProcessor : public QObject
{
    Q_OBJECT

public:
    explicit ImageProcessor(QObject* parent = nullptr);
    bool shouldStopProcessing;

    void setStopProcessing(bool stop);

    int frameId = 0;


public slots:
    void processImages(unsigned char* pbuffer, int cameraWidth, int cameraHeight, std::string appDirCv, bool freeCollect, int picNumCount);

signals:
    void imagesProcessed();
};
#endif // IMAGEPROCESSOR_H