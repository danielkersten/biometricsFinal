#ifndef CAMERA_CAPTURE_HPP
#define CAMERA_CAPTURE_HPP

#include "cv.h"
#include "highgui.h"

#include<stdio.h>

class CameraCapture
{
	public:
		CameraCapture();
		~CameraCapture();

	
	private:
		CvCapture *camera;



};
#endif
