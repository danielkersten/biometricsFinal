/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#ifndef CAMERA_CAPTURE_HPP
#define CAMERA_CAPTURE_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
using namespace std;

class CameraCapture
{
public:
  CameraCapture();
  ~CameraCapture();
  bool initCamera();
  IplImage *getCameraFrame();
  CvRect detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade   *cascade);
  void storeTrainingData();
  int loadTrainingData(CvMat **pTrainPersonNumMat);
  int loadFaceImgArray(const char *filename);
  void doPCA();

  IplImage * preProcessImage(IplImage *inputImg, int pWidth=100,int pHeight=100);

  bool captureImages(int pNumImages);
  bool testCamera();
  bool train();
  void recognize(IplImage * pFaceImage);
  int findNearestNeighbor();

private:
  CvCapture *camera;
  int nTrainFaces;
  int nEigens;
  IplImage **faceImgArr;
  CvMat *personNumTruthMat;
  IplImage *pAvgTrainImg;
  IplImage **eigenVectArr;
  CvMat *eigenValMat;
  CvMat *projectedTrainFaceMat;
  CvMat *pTrainPersonNumMat;
  float * projectedTestFace;
  vector<string> mTrainImageNames;
};
#endif
