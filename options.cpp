/* Andres Mejia, Christopher O'Connell
 * CS667
 * Term Project
 * 12/05/2012
 */

#include "options.hpp"

#include <cstdio>

COptions::COptions()
{
  /* Construction of this class sets various defaults */
  face_cascade_file = "haarcascade_frontalface_alt.xml";
}

COptions& COptions::Instance()
{
  static COptions instance;
  return instance;
}

std::string COptions::getFaceCascadeFile()
{
  return face_cascade_file;
}

static const char usage[] =
  "Usage: camCapture [OPTIONS]\n"
  "Authors: Andres Mejia, Christopher O'Connell\n"
  "Description: Program implementing term project for CS667\n"
  "\n"
  "  --help, -h                           print this help message\n"
  "  --face-cascade-file [FILE]           set FILE as face cascade file\n"
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
    else
    {
      fprintf(stderr, "%s", usage);
      return false;
    }
  }

  return true;
}
