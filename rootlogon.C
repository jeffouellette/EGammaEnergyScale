#include "../atlas-hi/src/AtlasStyle.cxx"

void rootlogon()
{
  std::cout << "Found rootlogon.C in /atlasgpfs01/usatlas/workarea/jeff/EGammaESStudy" << std::endl;
  SetAtlasStyle();

  gROOT->ProcessLine (".include /usatlas/u/jeff/RootUtilities/include");
  gROOT->ProcessLine (".include /atlasgpfs01/usatlas/workarea/jeff/atlas-hi/include");
  gROOT->ProcessLine (".include include");

  gROOT->ProcessLine (".L lib/libLocalUtilities.so");

  gROOT->ProcessLine (".L /usatlas/u/jeff/RootUtilities/lib/libUtilities.so");
  gROOT->ProcessLine (".L /usatlas/u/jeff/RootUtilities/lib/libAggressiveAvocado.so");

  gROOT->ProcessLine (".L /atlasgpfs01/usatlas/workarea/jeff/atlas-hi/lib/libAtlasStyle.so");
}
