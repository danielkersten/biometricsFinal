#include<stdio.h>
#include "CameraCapture.h"



int main()
{
	CameraCapture * aCapture = new CameraCapture();

	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);

	CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade *)cvLoad("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml");	

	while(cvWaitKey(10) != 27)
	{
		IplImage *frame = aCapture->getCameraFrame();

		CvRect aFace = aCapture->detectFaceInImage(frame,cascade);
		cvRectangle(frame,cvPoint(aFace.x, aFace.y), cvPoint(aFace.x+aFace.width, aFace.y+aFace.height), CV_RGB(255,0,0),1,8,0);

		

		cvShowImage("mywindow",frame);
	    		
	}

	delete aCapture;
	cvDestroyWindow("mywindow");
	
	return 0;
}
