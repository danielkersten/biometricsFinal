#include "CameraCapture.h"

CameraCapture::CameraCapture()
{
	camera = cvCreateCameraCapture(0);

	if(!camera)
	{

		printf("Couldn't access the camera!.\n");
		exit(1);
	}
	cvSetCaptureProperty(camera,CV_CAP_PROP_FRAME_WIDTH, 320);
	cvSetCaptureProperty(camera,CV_CAP_PROP_FRAME_HEIGHT, 240);

	IplImage *frame = cvQueryFrame(camera);
	if(frame)
	{
		int w = frame->width;
		int h = frame->height;
		printf("Got the camera at %dx%d resolution. \n",w,h);
	}
	usleep(1000);

}
CameraCapture::~CameraCapture()
{
	cvReleaseCapture(&camera);

}
IplImage * CameraCapture::getCameraFrame()
{
	IplImage * frame = cvQueryFrame(camera);
	if(!frame)
	{
		printf("Couldn't grab a camera frame! \n");
		exit(1);
	}
	return frame;
}

