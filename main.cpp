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
  CameraCapture camcapture;

  if (!COptions::Instance().ParseOptions(argc, argv))
    return EXIT_FAILURE;

  switch (COptions::Instance().getOperation())
  {
  case COptions::DEFAULT:
    if (!camcapture.testCamera())
    {
      fprintf(stderr, "Camera test failed.\n");
      return EXIT_FAILURE;
    }
    break;
  case COptions::TRAINING:
    if (!camcapture.train())
    {
      fprintf(stderr, "Training operation failed.\n");
      return EXIT_FAILURE;
    }
    break;
  case COptions::VERIFICATION:
    fprintf(stderr, "Verification operation not yet implemented.\n");
    break;
  default:
    fprintf(stderr, "Unknown operation specified.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
