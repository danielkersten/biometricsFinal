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
private:
  /* Global options are set/retrieved via the COptions singleton class */
  COptions();
  COptions(COptions const&);
  void operator=(COptions const&);
  std::string face_cascade_file;
};

#endif /*OPTIONS_HPP*/
