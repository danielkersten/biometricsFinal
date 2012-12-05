/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#include "options.hpp"

#include <cstdio>
#include <cstdlib>

COptions::COptions()
{
  /* Construction of this class sets various defaults */
  face_cascade_file = "haarcascade_frontalface_alt.xml";
  training_data_save_file = "facedata.xml";
  camera_device_number = 0;
  operation = DEFAULT;
  training_data_image_array_file = "train.txt";
  num_images = 5;  
  user_name = -1;
  threshold = -1;
}

COptions &COptions::Instance()
{
  static COptions instance;
  return instance;
}

std::string COptions::getFaceCascadeFile()
{
  return face_cascade_file;
}

std::string COptions::getTrainingDataSaveFile()
{
  return training_data_save_file;
}

int COptions::getCameraDeviceNumber()
{
  return camera_device_number;
}

int COptions::getOperation()
{
  return operation;
}

std::string COptions::getTrainingDataImageArrayFile()
{
  return training_data_image_array_file;
}

int COptions::getNumImages()
{
   return num_images;
}

int COptions::getUserName()
{
   return user_name;
}
float COptions::getThreshold()
{
   return threshold;
}

static const char usage[] =
  "Usage: camCapture [OPTIONS]\n"
  "Authors: Andres Mejia, Christopher O'Connell\n"
  "Description: Program implementing term project for CS667\n"
  "\n"
  "  --help, -h                           print this help message\n"
  "  --face-cascade-file [FILE]           set FILE as face cascade file\n"
  "  --training-data-save-file [FILE]     set FILE as training data save file\n"
  "  --camera-device-number [NUMBER]      set the camera device number to use\n"
  "  --num_images [NUMBER]                sets the number of images to capture, only used with IMAGE_CAPTURE\n"
  "  --operation [ARG]                    set the operation to run, valid\n"
  "                                       operations are 'default', \n"
  "                                       'training', and 'verification'\n"
  "  --training-data-image-array-file [FILE] set FILE as training data image\n"
  "                                          array file\n"
  "  --user_name [NUMBER]                 Sets the username of the person in front\n"
  "  of camera \n"
  "  --threshold [NUMBER]                 The threshold to set to accept. Expect this\n"
  " to be a decimal from 1-2\n"
  ;

bool COptions::ParseOptions(int argc, const char **argv)
{
  int i;
  std::string arg;

  /* Parse command line options */
  for (i = 1; i < argc; i++)
  {
    arg = argv[i];

    if (!arg.compare("--face-cascade-file") && argv[++i])
    {
      face_cascade_file = argv[i];
    }
    else if (!arg.compare("--training-data-save-file") && argv[++i])
    {
      training_data_save_file = argv[i];
    }
    else if (!arg.compare("--camera-device-number") && argv[++i])
    {
      camera_device_number = atoi(argv[i]);
    }
    else if (!arg.compare("--num_images") && argv[++i])
    {
       num_images = atoi(argv[i]);
    }
    else if (!arg.compare("--operation") && argv[++i])
    {
      std::string str = argv[i];

      if (!str.compare("default"))
        operation = DEFAULT;
      else if (!str.compare("training"))
        operation = TRAINING;
      else if (!str.compare("verification"))
        operation = VERIFICATION;
      else if (!str.compare("image_capture"))
        operation = IMAGE_CAPTURE;
      else
      {
        fprintf(stderr, "Unknown phase '%s'.\n", str.c_str());
        return false;
      }
    }
    
    else if (!arg.compare("--training-data-image-array-file") && argv[++i])
    {
      training_data_image_array_file = argv[i];
    }
    else if(!arg.compare("--user_name") && argv[++i])
    {
        user_name = atoi(argv[i]);
    }
    else if (!arg.compare("--threshold") &&argv[++i])
    {
        threshold = atof(argv[i]);
    }
    else
    {
      fprintf(stderr, "%s", usage);
      return false;
    }
  }

  return true;
}
