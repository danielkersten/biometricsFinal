#include<stdio.h>
#include "CameraCapture.h"



int main()
{
	CameraCapture * aCapture = new CameraCapture();

	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);

	while(cvWaitKey(10) != 27)
	{
		IplImage *frame = aCapture->getCameraFrame();

		cvShowImage("mywindow",frame);
	}

	delete aCapture;
	cvDestroyWindow("mywindow");
	
	return 0;
}
