/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#include "options.hpp"
#include "program.hpp"

#include <cstdio>

int main(int argc, const char **argv)
{
  CProgram program;

  if (!COptions::Instance().ParseOptions(argc, argv))
    return EXIT_FAILURE;

  switch (COptions::Instance().getOperation())
  {
  case COptions::DEFAULT:
    program.testCamera();
  case COptions::TRAINING:
    fprintf(stderr, "Training operation not yet implemented.\n");
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
