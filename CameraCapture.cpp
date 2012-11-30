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

CvRect CameraCapture::detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade  * cascade)
{

	CvSize minFeatureSize = cvSize(20,20);

	int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;
	
	float search_scale_factor=1.1f;
	
	IplImage *detectImg;
	IplImage *greyImg = 0;
	CvMemStorage * storage;

	CvRect rc;
	double t;
	CvSeq* rects;
	CvSize size;
	int i, ms,nFaces;

	storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);

	detectImg = (IplImage*)inputImg;

	if(inputImg->nChannels >1)
	{
		size = cvSize(inputImg->width, inputImg->height);
		greyImg = cvCreateImage(size,IPL_DEPTH_8U,1);
		cvCvtColor(inputImg, greyImg, CV_BGR2GRAY);
		detectImg = greyImg;
	}

	t = (double)cvGetTickCount();
	rects = cvHaarDetectObjects(detectImg, cascade,storage,search_scale_factor,3,flags,minFeatureSize);

	t = (double)cvGetTickCount()-t;
	ms = cvRound(t/((double)cvGetTickFrequency()*1000.0) );
	nFaces = rects->total;
	printf("Face Detection took %d ms and found %d objects\n",ms,nFaces);

	if(nFaces >0)
	{
		rc = *(CvRect *)cvGetSeqElem(rects,0);
	}
	else
	{
		rc =cvRect(-1,-1,-1,-1);
	}

	if(greyImg)
	{
		cvReleaseImage(&greyImg);
	}
	cvReleaseMemStorage(&storage);

	return rc;
}
