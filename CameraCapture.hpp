/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#ifndef CAMERA_CAPTURE_HPP
#define CAMERA_CAPTURE_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>

class CameraCapture
{
public:
  CameraCapture();
  ~CameraCapture();
  bool initCamera();
  IplImage *getCameraFrame();
  CvRect detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade   *cascade);
  void storeTrainingData();
  int loadFaceImgArray(const char *filename);
  void doPCA();

  bool testCamera();
  bool train();

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
};
#endif
