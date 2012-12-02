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

static const char usage[] =
  "Usage: camCapture [OPTIONS]\n"
  "Authors: Andres Mejia, Christopher O'Connell\n"
  "Description: Program implementing term project for CS667\n"
  "\n"
  "  --help, -h                           print this help message\n"
  "  --face-cascade-file [FILE]           set FILE as face cascade file\n"
  "  --training-data-save-file [FILE]     set FILE as training data save file\n"
  "  --camera-device-number [NUMBER]      set the camera device number to use\n"
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
    else
    {
      fprintf(stderr, "%s", usage);
      return false;
    }
  }

  return true;
}
