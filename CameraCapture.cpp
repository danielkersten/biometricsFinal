#include "CameraCapture.h"

CameraCapture::CameraCapture()
{
	camera = cvCreateCameraCapture(0);

}
CameraCapture::~CameraCapture()
{
	cvReleaseCapture(&camera);

}

