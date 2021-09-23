#ifndef __Process_cxx__
#define __Process_cxx__

#include "ZBosonReco.h"
#include "Params.h"
#include "Process.h"

#include <string>
#include <iostream>

using namespace std;
using namespace EGammaESStudy;


int main (int argc, char** argv) {

  int argn                = 1;
  const string alg        = string (argv[argn++]);
  const char* subdir      = argv[argn++];
  const int dataSet       = atoi (argv[argn++]);

  TString collSys = TString (argv[argn++]);
  if (!SetCollisionSystem (collSys)) {
    std::cout << "In Process.cxx: Invalid collision system, exiting." << std::endl;
    return 1;
  }

  TString dType = TString (argv[argn++]);
  if (!SetDataType (dType)) {
    std::cout << "In Process.cxx: Invalid data type, exiting." << std::endl;
    return 2;
  }

  TString sFlag = TString (argv[argn++]);
  if (!SetSystFlag (sFlag)) {
    std::cout << "In Process.cxx: Invalid systematic flag, exiting." << std::endl;
    return 4;
  }

  const char* inFileName            = (argc > argn && argv[argn] ? argv[argn++] : "");
  if (!GetMCWeights (inFileName))
    std::cout << "In Process.cxx: Failed to get event weights, please check." << std::endl;

  std::cout << "Info: In Process.cxx: Configuration set to";
  std::cout << "\n\talg = " << alg;
  std::cout << "\n\tsubdir = " << subdir;
  std::cout << "\n\tdataSet = " << dataSet;
  std::cout << "\n\tCollisionSystem = " << ToTString (collisionSystem);
  std::cout << "\n\tDataType = " << ToTString (dataType);
  std::cout << "\n\tSystFlag = " << ToTString (systFlag);
  std::cout << "\n\tinFileName = " << inFileName;
  if (crossSectionPicoBarns != 0.)
    std::cout << "\n\t  --> deduced crossSectionPicoBarns = " << crossSectionPicoBarns;
  if (mcFilterEfficiency != 0.)
    std::cout << "\n\t  --> deduced mcFilterEfficiency = " << mcFilterEfficiency;
  if (mcNumberEvents != 0.)
    std::cout << "\n\t  --> deduced mcNumberEvents = " << mcNumberEvents;
  std::cout << std::endl;


  bool success = false;
  if (alg == "ZBosonReco") {
    std::cout << "Info: In Process.cxx: Running ZBosonReco algorithm..." << std::endl;
    success = EGammaESStudy::ZBosonReco (subdir, dataSet, inFileName);
  }
  else {
    std::cout << "Error: In Process.cxx: Failed to recognize algorithm! Quitting." << std::endl;
    return 1;
  }

  if (success) {
    std::cout << "Info: In Process.cxx: Finished algorithm!" << std::endl;
    return 0;
  }
  else {
    std::cout << "Error: In Process.cxx: Algorithm failed!" << std::endl;
    return 1;
  }
}

#endif
