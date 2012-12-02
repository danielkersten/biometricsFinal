/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#include "CameraCapture.hpp"
#include "options.hpp"

#include <cstdio>
#include <unistd.h>

CameraCapture::CameraCapture()
{
  /* Initially set all class variables to 0 or NULL */
  nTrainFaces = 0;
  nEigens = 0;
  faceImgArr = NULL;
  personNumTruthMat = NULL;
  pAvgTrainImg = NULL;
  eigenVectArr = NULL;
  eigenValMat = NULL;
  projectedTrainFaceMat = NULL;
}

CameraCapture::~CameraCapture()
{
	cvReleaseCapture(&camera);

}

bool CameraCapture::initCamera()
{
  camera = cvCreateCameraCapture(0);
  if(!camera)
  {
    fprintf(stderr, "Couldn't access the camera!.\n");
    return false;
  }
  cvSetCaptureProperty(camera,CV_CAP_PROP_FRAME_WIDTH, 320);
  cvSetCaptureProperty(camera,CV_CAP_PROP_FRAME_HEIGHT, 240);

  IplImage *frame = cvQueryFrame(camera);
  if(frame)
  {
    int w = frame->width;
    int h = frame->height;
    fprintf(stdout, "Got the camera at %dx%d resolution. \n",w,h);
  }
  usleep(1000);
  return true;
}

IplImage * CameraCapture::getCameraFrame()
{
	IplImage * frame = cvQueryFrame(camera);
	if(!frame)
	{
		fprintf(stderr, "Couldn't grab a camera frame! \n");
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
	int ms,nFaces;

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
	fprintf(stdout, "Face Detection took %d ms and found %d objects\n",ms,nFaces);

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

void CameraCapture::storeTrainingData()
{
  CvFileStorage *fileStorage;
  int i;
  std::string training_data_save_file;

  /* create a file-storage interface */
  training_data_save_file = COptions::Instance().getTrainingDataSaveFile();
  fileStorage = cvOpenFileStorage(training_data_save_file.c_str(), 0,
                                  CV_STORAGE_WRITE);

  /* store all the data */
  cvWriteInt(fileStorage, "nEigens", nEigens);
  cvWriteInt(fileStorage, "nTrainFaces", nTrainFaces);
  cvWrite(fileStorage, "trainPersonNumMat", personNumTruthMat);
  cvWrite(fileStorage, "eigenValMat", eigenValMat, cvAttrList(0, 0));
  cvWrite(fileStorage, "projectedTrainFaceMat", projectedTrainFaceMat,
          cvAttrList(0, 0));
  cvWrite(fileStorage, "avgTrainImg", pAvgTrainImg, cvAttrList(0, 0));
  for (i = 0; i < nEigens; i++)
  {
    char varname[200];
    sprintf(varname, "eigenVect_%d", i);
    cvWrite(fileStorage, varname, eigenVectArr[i], cvAttrList(0, 0));
  }

  /* release the file-storage interface */
  cvReleaseFileStorage(&fileStorage);
}
