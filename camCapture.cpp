/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#include "CameraCapture.hpp"
#include "options.hpp"

#include <cstdio>

int main(int argc, const char **argv)
{
  if (!COptions::Instance().ParseOptions(argc, argv))
    return EXIT_FAILURE;

  std::string face_cascade_file = COptions::Instance().getFaceCascadeFile();
  
  CameraCapture * aCapture = new CameraCapture();

	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);

	CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade *)cvLoad(face_cascade_file.c_str());

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
