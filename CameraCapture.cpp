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
  camera = NULL;
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
  if (camera)
    cvReleaseCapture(&camera);
}

bool CameraCapture::initCamera()
{
  camera = cvCreateCameraCapture(COptions::Instance().getCameraDeviceNumber());

  if (!camera)
  {
    fprintf(stderr, "Couldn't access the camera!.\n");
    return false;
  }

  cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, 320);
  cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, 240);

  IplImage *frame = cvQueryFrame(camera);

  if (frame)
  {
    int w = frame->width;
    int h = frame->height;
    fprintf(stdout, "Got the camera at %dx%d resolution. \n", w, h);
  }

  usleep(1000);
  return true;
}

IplImage *CameraCapture::getCameraFrame()
{
  IplImage *frame = cvQueryFrame(camera);

  if (!frame)
    return NULL;

  return frame;
}

CvRect CameraCapture::detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade   *cascade)
{
  CvSize minFeatureSize = cvSize(80, 60);

  //int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;
  int flags = CV_HAAR_DO_CANNY_PRUNING;

  float search_scale_factor = 1.2f;

  IplImage *detectImg;
  IplImage *greyImg = 0;
  CvMemStorage *storage;

  CvRect rc;
  double t;
  CvSeq *rects;
  CvSize size;
  int ms, nFaces;

  storage = cvCreateMemStorage(0);
  cvClearMemStorage(storage);

  detectImg = (IplImage *)inputImg;

  if (inputImg->nChannels > 1)
  {
    size = cvSize(inputImg->width, inputImg->height);
    greyImg = cvCreateImage(size, IPL_DEPTH_8U, 1);
    cvCvtColor(inputImg, greyImg, CV_BGR2GRAY);
    detectImg = greyImg;
  }

  t = (double)cvGetTickCount();
  rects = cvHaarDetectObjects(detectImg, cascade, storage, search_scale_factor, 2, flags, minFeatureSize);

  t = (double)cvGetTickCount() - t;
  ms = cvRound(t / ((double)cvGetTickFrequency() * 1000.0));
  nFaces = rects->total;
  fprintf(stdout, "Face Detection took %d ms and found %d objects\n", ms, nFaces);

  if (nFaces > 0)
  {
    rc = *(CvRect *)cvGetSeqElem(rects, 0);
  }
  else
  {
    rc = cvRect(-1, -1, -1, -1);
  }

  if (greyImg)
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

int CameraCapture::loadFaceImgArray(const char *filename)
{
  FILE *imgListFile = NULL;
  char imgFilename[512];
  int iFace, nFaces = 0;

  /* Open the input file */
  if ((imgListFile = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "File '%s' failed to open.\n", filename);
    return -1;
  }

  /* count the number of faces */
  while (fgets(imgFilename, 512, imgListFile))
    nFaces++;
  rewind(imgListFile);

  /* allocate the face-image array and person number matrix */
  faceImgArr = (IplImage **)cvAlloc(nFaces * sizeof(IplImage *));
  personNumTruthMat = cvCreateMat(1, nFaces, CV_32SC1);

  /* store the face images in an array */
  for (iFace = 0; iFace < nFaces; iFace++)
  {
    /* read person number and name of image file */
    fscanf(imgListFile, "%d %s", personNumTruthMat->data.i + iFace,
           imgFilename);

    /* load the face image */
    faceImgArr[iFace] = cvLoadImage(imgFilename, CV_LOAD_IMAGE_GRAYSCALE);
    cvEqualizeHist(faceImgArr[iFace], faceImgArr[iFace]);
  }

  fclose(imgListFile);

  return nFaces;
}

void CameraCapture::doPCA()
{
  int i;
  CvTermCriteria calcLimit;
  CvSize faceImgSize;

  /* set the number of eigenvalues to use */
  nEigens = nTrainFaces - 1;

  /* allocate the eigenvector images */
  faceImgSize.width = faceImgArr[0]->width;
  faceImgSize.height = faceImgArr[0]->height;
  eigenVectArr = (IplImage**)cvAlloc(sizeof(IplImage*) * nEigens);
  for (i = 0; i < nEigens; i++)
    eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

  /* allocate the eigenvalue array */
  eigenValMat = cvCreateMat(1, nEigens, CV_32FC1);

  /* allocate the averaged image */
  pAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

  /* set the PCA termination criterion */
  calcLimit = cvTermCriteria(CV_TERMCRIT_ITER, nEigens, 1);

  /* compute average image, eigenvalues, and eigenvectors */
  cvCalcEigenObjects(nTrainFaces, faceImgArr, eigenVectArr,
                     CV_EIGOBJ_NO_CALLBACK, 0, 0, &calcLimit, pAvgTrainImg,
                     eigenValMat->data.fl);
}

bool CameraCapture::testCamera()
{
  std::string face_cascade_file = COptions::Instance().getFaceCascadeFile();

  /* Attempt to initialize the camera */
  if (!initCamera())
    return false;

  cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);
  cvNamedWindow("preProcessed", CV_WINDOW_AUTOSIZE);
  CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade *)cvLoad(face_cascade_file.c_str());

  while (cvWaitKey(10) != 27)
  {
    IplImage *frame;
    if ((frame = getCameraFrame()) == NULL)
    {
      fprintf(stderr, "Couldn't grab a camera frame!\n");
      return false;
    }

    CvRect aFace = detectFaceInImage(frame, cascade);
    cvRectangle(frame, cvPoint(aFace.x, aFace.y), cvPoint(aFace.x + aFace.width, aFace.y + aFace.height), CV_RGB(255, 0, 0), 1, 8, 0);


    if(aFace.x !=-1)
    {
	IplImage * cropped = cvCreateImage(cvSize(aFace.width, aFace.height),frame->depth,frame->nChannels);
	cvSetImageROI(frame,aFace);
	cvCopy(frame,cropped);
	cvResetImageROI(frame);

	IplImage *preprocessed = preProcessImage(cropped,145,145);
	cvShowImage("preProcessed", preprocessed);
	//delete preprocessed;
    }

    
    cvShowImage("mywindow", frame);
    //delete frame;
  }

  cvDestroyWindow("mywindow");
  cvDestroyWindow("preProcessed");
  return true;
}

bool CameraCapture::train()
{
  int i;

  /* load training data */
  std::string training_data_image_array_file =
    COptions::Instance().getTrainingDataImageArrayFile();
  nTrainFaces = loadFaceImgArray(training_data_image_array_file.c_str());
  if (nTrainFaces < 2)
  {
    fprintf(stderr, "Need 2 or more training faces\n"
                    "Input file contains only %d\n", nTrainFaces);
    return false;
  }

  /* do PCA on the training faces */
  doPCA();

  /* project the training images onto the PCA subspace */
  projectedTrainFaceMat = cvCreateMat(nTrainFaces, nEigens, CV_32FC1);
  for (i = 0; i < nTrainFaces; i++)
  {
    cvEigenDecomposite(faceImgArr[i], nEigens, eigenVectArr, 0, 0,
                       pAvgTrainImg,
                       projectedTrainFaceMat->data.fl + i * nEigens);
  }

  /* store the recognition data as an xml file */
  storeTrainingData();
  
  return true;
}

IplImage * CameraCapture::preProcessImage(IplImage *inputImg, int pWidth,int pHeight)
{
	IplImage * imageGrey;
	
	if(inputImg->nChannels == 3)
	{
		imageGrey = cvCreateImage(cvGetSize(inputImg), IPL_DEPTH_8U,1);
		cvCvtColor(inputImg,imageGrey,CV_BGR2GRAY);
	}
	else
	{
		imageGrey = inputImg;
	}
	IplImage *imageProcessed;
	imageProcessed = cvCreateImage(cvSize(pWidth, pHeight), IPL_DEPTH_8U,1);
	
	cvResize(imageGrey,imageProcessed,CV_INTER_LINEAR);
	
	cvEqualizeHist(imageProcessed,imageProcessed);

	if(imageGrey)
	{
		cvReleaseImage(&imageGrey);
	}
	
	return imageProcessed;	
}
bool CameraCapture::captureImages(int pNumImages)
{
   char option =' ';
   std::string face_cascade_file = COptions::Instance().getFaceCascadeFile();
   CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade *)cvLoad(face_cascade_file.c_str());
   string filename ="";
   string filedir="./images/";
   if(!initCamera())
   {
      fprintf(stderr,"Failed to init the camera");
      return true;
   } 
   
   cvNamedWindow("procImage", CV_WINDOW_AUTOSIZE);
   cvNamedWindow("preProcessed", CV_WINDOW_AUTOSIZE);

   int fileCount =0;
   while (cvWaitKey(10) != 27 && fileCount<pNumImages)
   {
   IplImage *img = getCameraFrame();
   cvShowImage("procImage", img);

   CvRect aFace = detectFaceInImage(img, cascade);
   cvRectangle(img, cvPoint(aFace.x, aFace.y), cvPoint(aFace.x + aFace.width, aFace.y + aFace.height), CV_RGB(255, 0, 0), 1, 8, 0);


    if(aFace.x !=-1)
    {
	IplImage * cropped = cvCreateImage(cvSize(aFace.width, aFace.height),img->depth,img->nChannels);
	cvSetImageROI(img,aFace);
	cvCopy(img,cropped);
	cvResetImageROI(img);

	IplImage *preprocessed = preProcessImage(cropped,145,145);
	cvShowImage("preProcessed", preprocessed);
	option = cvWaitKey(10);
        printf("Key pressed: %i \n",option);

	//delete preprocessed;
        if(option =='y' || option =='Y')
        {
           cin.clear();
           fileCount++;
           printf("Saving!\n");
           
           getline(cin,filename);
           string filepath;
           filepath = filedir+filename;
           printf("Saving file: %s \n",filepath.c_str());
	   cvSaveImage(filepath.c_str(),preprocessed);
        }
    }

   }
   
   return true;
}
