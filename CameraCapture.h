#ifndef CAMERA_CAPTURE_HPP
#define CAMERA_CAPTURE_HPP

#include <opencv2/opencv.hpp>

#include<stdio.h>

class CameraCapture
{
	public:
		CameraCapture();
		~CameraCapture();
		IplImage *getCameraFrame();
		CvRect detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade  * cascade);

	
	private:
		CvCapture *camera;



};
#endif
