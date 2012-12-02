/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>

class COptions
{
public:
  static COptions &Instance();
  bool ParseOptions(int argc, const char **argv);
  std::string getFaceCascadeFile();
  std::string getTrainingDataSaveFile();
  int getCameraDeviceNumber();
  int getOperation();
  int getNumImages();
  std::string getTrainingDataImageArrayFile();

  /* Different operations supported by this program */
  enum
  {
    DEFAULT,
    TRAINING,
    VERIFICATION,
    IMAGE_CAPTURE
  };
private:
  /* Global options are set/retrieved via the COptions singleton class */
  COptions();
  COptions(COptions const &);
  void operator=(COptions const &);
  std::string face_cascade_file;
  std::string training_data_save_file;
  int camera_device_number;
  int operation;
  std::string training_data_image_array_file;
  int num_images;
};

#endif /*OPTIONS_HPP*/
