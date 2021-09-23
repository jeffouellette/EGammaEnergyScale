#ifndef __LocalUtilities_cxx__
#define __LocalUtilities_cxx__

#include "LocalUtilities.h"
#include "TreeVariables.h"
#include "Params.h"
#include "Process.h"

#include <AtlasUtils.h>

#include <Utilities.h>
#include <ArrayTemplates.h>

#include <TSystemDirectory.h>
#include <TLorentzVector.h>

#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>


namespace EGammaESStudy {


TString workPath = TString (std::getenv ("EGAMMAESSTUDY_PATH"));
TString extWorkPath = TString (std::getenv ("EGAMMAESSTUDY_DATA_PATH")) + "/";
TString rootPath = extWorkPath + "rootFiles/";
TString dataPath = extWorkPath + "data/";



TString ToTString (const CollisionSystem& collSys) {
  switch (collSys) {
    case CollisionSystem::pp15:       return TString ("pp15_5TeV");
    case CollisionSystem::PbPb15:     return TString ("PbPb15_5TeV");
    case CollisionSystem::pPb16s5TeV: return TString ("pPb16_5TeV");
    case CollisionSystem::pPb16:      return TString ("pPb16_8TeV");
    case CollisionSystem::Pbp16:      return TString ("Pbp16_8TeV");
    case CollisionSystem::XeXe17:     return TString ("XeXe17_5TeV");
    case CollisionSystem::pp17:       return TString ("pp17_5TeV");
    case CollisionSystem::PbPb18:     return TString ("PbPb18_5TeV");
    default:                          return TString ("???");
  }
}
 
 
 
TString ToTString (const DataType& dType) {
  switch (dType) {
    case DataType::Collisions:      return TString ("Collisions");
    case DataType::MCSignal:        return TString ("MCSignal");
    case DataType::MCDataOverlay:   return TString ("MCDataOverlay");
    case DataType::MCHijing:        return TString ("MCHijing");
    case DataType::MCHijingOverlay: return TString ("MCHijingOverlay");
    default:                        return TString ("???");
  }
}



TString ToTString (const SystFlag& sFlag) {
  switch (sFlag) {
    case SystFlag::Nominal:                 return TString ("Nominal");
    default:                                return TString ("???");
  }
}



float GetRadius (const JetRadius& r) {
  switch (r) {
    case JetRadius::R0p2:     return 0.2;
    case JetRadius::R0p3:     return 0.3;
    case JetRadius::R0p4:     return 0.4;
    case JetRadius::R0p6:     return 0.6;
    case JetRadius::R0p8:     return 0.8;
    case JetRadius::R1p0:     return 1.0;
    case JetRadius::Invalid:  return 0.;
    default:                  return -1.;
  }
}



bool SetCollisionSystem (TString ts) {
  for (CollisionSystem cs : AllCollisionSystem) {
    if (ts == ToTString (cs)) { collisionSystem = cs; return true; }
  }
  return false;
}



bool SetDataType (TString ts) {
  for (DataType dt : AllDataType) {
    if (ts == ToTString (dt)) { dataType = dt; return true; }
  }
  return false;
}



bool SetSystFlag (TString ts) {
  for (SystFlag sf : AllSystFlag) {
    if (ts == ToTString (sf)) { systFlag = sf; return true; }
  }
  return false;
}



bool IsIons (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::PbPb15 || collSys == CollisionSystem::pPb16s5TeV || collSys == CollisionSystem::pPb16 || collSys == CollisionSystem::Pbp16 || collSys == CollisionSystem::PbPb18);
}



bool IsPbPb (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::PbPb15 || collSys == CollisionSystem::PbPb18);
}



bool IsPbPb18 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::PbPb18);
}



bool IsPbPb15 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::PbPb15);
}



bool IsXeXe (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::XeXe17);
}



bool IspPb (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pPb16s5TeV || collSys == CollisionSystem::pPb16 || collSys == CollisionSystem::Pbp16);
}



bool IspPb16 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pPb16s5TeV || collSys == CollisionSystem::pPb16 || collSys == CollisionSystem::Pbp16);
}



bool Ispp (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pp15 || collSys == CollisionSystem::pp17);
}



bool Ispp15 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pp15);
}



bool Ispp17 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pp17);
}



bool IsPeriodA (const CollisionSystem& collSys) {
  if (collSys == CollisionSystem::pPb16s5TeV || collSys == CollisionSystem::pPb16) {
    return true;
  }
  else if (collSys == CollisionSystem::Pbp16) {
    return false;
  }
  std::cout << "In LocalUtilities.cxx::IsPeriodA (const CollisionSystem): Warning: collision system is symmetric, returning true by default." << std::endl;
  return true;
}



bool Is5TeV (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pp15 || collSys == CollisionSystem::PbPb15 || collSys == CollisionSystem::pPb16s5TeV || collSys == CollisionSystem::XeXe17 || collSys == CollisionSystem::pp17 || collSys == CollisionSystem::PbPb18);
}



bool Is8TeV (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pPb16 || collSys == CollisionSystem::Pbp16);
}



bool Is2018 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::PbPb18);
}



bool Is2017 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::XeXe17 || collSys == CollisionSystem::pp17);
}



bool Is2016 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pPb16s5TeV || collSys == CollisionSystem::pPb16 || collSys == CollisionSystem::Pbp16);
}



bool Is2015 (const CollisionSystem& collSys) {
  return (collSys == CollisionSystem::pp15 || collSys == CollisionSystem::PbPb15);
}



bool IsCollisions (const DataType& dType) {
  return (dType == DataType::Collisions);
}



bool IsDataOverlay (const DataType& dType) {
  return (dType == DataType::MCDataOverlay);
}



bool IsOverlay (const DataType& dType) {
  return (dType == DataType::MCDataOverlay || dType == DataType::MCHijingOverlay);
}



bool IsHijing (const DataType& dType) {
  return (dType == DataType::MCHijing || dType == DataType::MCHijingOverlay);
}



bool IsIons () {
  return IsIons (collisionSystem);
}



bool IsPbPb () {
  return IsPbPb (collisionSystem);
}



bool IsPbPb18 () {
  return IsPbPb18 (collisionSystem);
}



bool IsPbPb15 () {
  return IsPbPb15 (collisionSystem);
}



bool IsXeXe () {
  return IsXeXe (collisionSystem);
}



bool IspPb () {
  return IspPb (collisionSystem);
}



bool IspPb16 () {
  return IspPb16 (collisionSystem);
}



bool Ispp () {
  return Ispp (collisionSystem);
}



bool Ispp15 () {
  return Ispp15 (collisionSystem);
}



bool Ispp17 () {
  return Ispp17 (collisionSystem);
}



bool IsPeriodA () {
  return IsPeriodA (collisionSystem);
}



bool Is5TeV () {
  return Is5TeV (collisionSystem);
}



bool Is8TeV () {
  return Is8TeV (collisionSystem);
}



bool Is2018 () {
  return Is2018 (collisionSystem);
}



bool Is2017 () {
  return Is2017 (collisionSystem);
}



bool Is2016 () {
  return Is2016 (collisionSystem);
}



bool Is2015 () {
  return Is2015 (collisionSystem);
}



bool IsCollisions () {
  return IsCollisions (dataType);
}



bool IsOverlay () {
  return IsOverlay (dataType);
}



bool IsDataOverlay () {
  return IsDataOverlay (dataType);
}



bool IsHijing () {
  return IsHijing (dataType);
}



/**
 * Returns the CoM boost relevant for asymmetric collision systems (i.e. p+Pb). 0 for everything else.
 */
double GetBoost (int rn) {
  double boost = 0;
  if (IspPb ()) {
    boost = (IsPeriodA () ? -0.465 : 0.465);
  }
  return boost;
}



/**
 * Establishes path variables appropriately.
 */
void SetupDirectories (const TString& dataSubDir, const bool addSubDir) {
  rootPath = extWorkPath + "rootFiles/" + dataSubDir + "/";

  if (addSubDir)
    rootPath = rootPath + ToTString (systFlag) + "/";
}



/**
 * Looks up MC sample cross section, filter efficiency, and number of events.
 */
bool GetMCWeights (const TString& fname) {
  ifstream f_wgts;
  f_wgts.open (Form ("%s/aux/MC_Weights.dat", workPath.Data ()));

  std::string line;
  while (getline (f_wgts, line)) {

    std::istringstream lineStream (line);

    std::vector <std::string> words = {};
    std::string word;

    while (lineStream >> word)
      words.push_back (word);

    assert (words.size () == 4);

    if (fname.Contains (TString (words[0]))) {
      crossSectionPicoBarns = atof (words[1].c_str ());
      mcFilterEfficiency = atof (words[2].c_str ());
      mcNumberEvents = atof (words[3].c_str ());
      return true;
    }
  }
  return false;
}



/**
 * Returns the minimum anti-kT R=0.4 truth jet pT for this JZXR04 MC sample.
 */
float GetJZXR04MinPt (const TString& fname) {
  if (!IsCollisions ()) {
    if (TString (fname).Contains ("JZ0"))      return 0;
    else if (TString (fname).Contains ("JZ1")) return 20;
    else if (TString (fname).Contains ("JZ2")) return 60;
    else if (TString (fname).Contains ("JZ3")) return 160;
    else if (TString (fname).Contains ("JZ4")) return 400;
    else if (TString (fname).Contains ("JZ5")) return 800;
  }
  return 0;
}



/**
 * Returns the maximum anti-kT R=0.4 truth jet pT for this JZXR04 MC sample.
 */
float GetJZXR04MaxPt (const TString& fname) {
  if (!IsCollisions ()) {
    if (TString (fname).Contains ("JZ0"))      return 20;
    else if (TString (fname).Contains ("JZ1")) return 60;
    else if (TString (fname).Contains ("JZ2")) return 160;
    else if (TString (fname).Contains ("JZ3")) return 400;
    else if (TString (fname).Contains ("JZ4")) return 800;
    else if (TString (fname).Contains ("JZ5")) return 1300;
  }
  return FLT_MAX;
}



/**
 * Returns a copy of the histogram detailing the Zdc cuts.
 */
TH1D* GetZdcCuts () {
  if (!IsPbPb ())
    return nullptr; // no cuts needed outside of Pb+Pb

  TFile* zdcFile = new TFile (Form ("%s/ZdcAnalysis/HIRun2PileUp_PbPb5p02_%s.root", rootPath.Data (), Is2015 () ? "2015" : "2018"), "read");
  TH1D* h_zdcCuts = (TH1D*) ((TH1D*) zdcFile->Get (Is2015 () ? "hCut" : "h_cuts")->Clone ("h_zdcCuts"));
  h_zdcCuts->SetDirectory (0);
  zdcFile->Close ();
  SaferDelete (&zdcFile);
  return h_zdcCuts;
}



/**
 * Returns an abbreviated, unique identifier for a given dataset.
 */
TString GetIdentifier (const int dataSet, const char* directory, const char* inFileName) {
  TString id;
  if (IsCollisions ()) {
    id = to_string (dataSet);
    if (TString (directory).Contains ("pc"))
      id += "_pc";
    else if (TString (directory).Contains ("cc"))
      id += "_cc";
    return id;
  }

  if (IsPbPb ())
    id = (Is2015 () ? "PbPb15" : "PbPb18");
  else if (IspPb ())
    id = (IsPeriodA () ? "pPb16" : "Pbp16");
  else if (Ispp ())
    id = (Is2015 () ? "pp15" : "pp17");

  if ((IsPbPb () || IspPb ()) && !IsCollisions () && !IsOverlay () && !IsHijing ())
    id = id + "_so";

  if (TString (inFileName).Contains ("420010"))
    id = id + "_JZ0";
  else if (TString (inFileName).Contains ("420011"))
    id = id + "_JZ1";
  else if (TString (inFileName).Contains ("420012"))
    id = id + "_JZ2";
  else if (TString (inFileName).Contains ("420013"))
    id = id + "_JZ3";
  else if (TString (inFileName).Contains ("420014"))
    id = id + "_JZ4";
  else if (TString (inFileName).Contains ("420015"))
    id = id + "_JZ5";

  if (TString (inFileName).Contains ("e4108"))
    id = id + "_SampleA";
  else if (TString (inFileName).Contains ("e6608"))
    id = id + "_SampleB";

  // FROM Z+h analysis (now deprecated)
  if (TString (inFileName).Contains ("Sherpa"))
    id = id + "_Sherpa";

  if (IsPbPb () || IspPb ()) {
    if (TString (inFileName).Contains ("pp_Zee"))
      id = id + "_pp_Zee";
    else if (TString (inFileName).Contains ("pn_Zee"))
      id = id + "_pn_Zee";
    else if (TString (inFileName).Contains ("np_Zee"))
      id = id + "_np_Zee";
    else if (TString (inFileName).Contains ("nn_Zee"))
      id = id + "_nn_Zee";
    else if (TString (inFileName).Contains ("Zee"))
      id = id + "_pp_Zee";
    else if (TString (inFileName).Contains ("pp_Zmumu"))
      id = id + "_pp_Zmumu";
    else if (TString (inFileName).Contains ("pn_Zmumu"))
      id = id + "_pn_Zmumu";
    else if (TString (inFileName).Contains ("np_Zmumu"))
      id = id + "_np_Zmumu";
    else if (TString (inFileName).Contains ("nn_Zmumu"))
      id = id + "_nn_Zmumu";
    else if (TString (inFileName).Contains ("Zmumu"))
      id = id + "_pp_Zmumu";
    else if (TString (inFileName).Contains ("Ztautau"))
      id = id + "_pp_Ztautau";
    else if (TString (inFileName).Contains ("ttbar"))
      id = id + "_pp_ttbar";
  } 
  else {
    if (TString (inFileName).Contains ("Zee"))
      id = id + "_Zee";
    else if (TString (inFileName).Contains ("Zmumu"))
      id = id + "_Zmumu";
    else if (TString (inFileName).Contains ("Ztautau"))
      id = id + "_Ztautau";
    else if (TString (inFileName).Contains ("ttbar"))
      id = id + "_ttbar";
  }
  

  if (IsHijing ()) {
    id = id + "_Hijing";

    // FROM Z+h analysis Hijing samples (now deprecated)
    if (TString (inFileName).Contains ("e4858") || TString (inFileName).Contains ("r11899") || TString (inFileName).Contains ("r11900") || TString (inFileName).Contains ("r11901") || TString (inFileName).Contains ("r11902") || TString (inFileName).Contains ("r11903"))
      id += "_SC";
    else if (TString (inFileName).Contains ("e4962") || TString (inFileName).Contains ("r11892") || TString (inFileName).Contains ("r11893") || TString (inFileName).Contains ("r11894") || TString (inFileName).Contains ("r11895") || TString (inFileName).Contains ("r11898"))
      id += "_MB";

    if (TString (inFileName).Contains ("r11892") || TString (inFileName).Contains ("r11899") || TString (inFileName).Contains ("s3531"))
      id += "_vtxz_n64";
    else if (TString (inFileName).Contains ("r11893") || TString (inFileName).Contains ("r11900") || TString (inFileName).Contains ("s3530"))
      id += "_vtxz_n24";
    else if (TString (inFileName).Contains ("r11894") || TString (inFileName).Contains ("r11901") || TString (inFileName).Contains ("s3520"))
      id += "_vtxz_0";
    else if (TString (inFileName).Contains ("r11895") || TString (inFileName).Contains ("r11902") || TString (inFileName).Contains ("s3528"))
      id += "_vtxz_p24";
    else if (TString (inFileName).Contains ("r11898") || TString (inFileName).Contains ("r11903") || TString (inFileName).Contains ("s3529"))
      id += "_vtxz_p64";
  }

  // FROM Z+h analysis Hijing samples (now deprecated)
  if (TString (inFileName).Contains ("ptmin25"))
    id = id + "_ptmin25";
  else if (TString (inFileName).Contains ("ygt175"))
    id = id + "_ygt175";

  return id;
}



/**
 * Returns true if this truth jet passes selection criteria.
 */
bool MeetsTruthJetAcceptanceCuts (int iTJ, const JetRadius& radius) {
  assert (GetAktTruthJetN (radius) > iTJ);
  if (std::fabs (GetAktTruthJetEta (iTJ, radius)) > 2.8)
    return false;
  //if (IspPb () && InDisabledHEC (GetAktTruthJetEta (iTJ, radius), GetAktTruthJetPhi (iTJ, radius), GetRadius (radius)))
  //  return false; // cut out jets in the disabled HEC
  return true;
}



/**
 * Returns true if this jet passes selection criteria.
 */
bool MeetsJetAcceptanceCuts (int iJ, const JetRadius& radius, const int nJESVar) {
  assert (GetAktHIJetN (radius) > iJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);
  if (std::fabs (GetAktHIJetEta (iJ, radius, nJESVar)) > 2.8)
    return false; // cut out forward jets
  if (IspPb () && InDisabledHEC (GetAktHIJetEta (iJ, radius, nJESVar), GetAktHIJetPhi (iJ, radius, nJESVar), GetRadius (radius)))
    return false; // cut out jets in the disabled HEC
  return true;
}



/**
 * Returns the matched truth jet within DR < pi to this HI jet.
 * Returns -1 if no truth jet is matched within this DR range, or the radius is invalid.
 */
int GetAktTruthJetMatch (const int iJ, const JetRadius& radius, const int nJESVar) {
  assert (GetAktHIJetN (radius) > iJ);
  float mindr = M_PI;
  int match = -1;
  const float jeta = GetAktHIJetEta (iJ, radius, nJESVar);
  const float jphi = GetAktHIJetPhi (iJ, radius, nJESVar);
  const int nTJ = GetAktTruthJetN (radius);
  for (int iTJ = 0; iTJ < nTJ; iTJ++) {
    const float dr = DeltaR (jeta, GetAktTruthJetEta (iTJ, radius), jphi, GetAktTruthJetPhi (iTJ, radius));
    if (dr < mindr) {
      match = iTJ;
      mindr = dr;
    }
  }
  if (mindr > GetAktTruthMatchMaxDR (radius))
    return -1;
  return match;
}



/**
 * Determines the correct truth jet count for this radius.
 * Returns -1 if radius was not recognized.
 */
int GetAktTruthJetN (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: {
    assert (akt4_truth_jet_n >= 0);
    return akt4_truth_jet_n;
  }
  case JetRadius::R0p2: {
    assert (akt2_truth_jet_n >= 0);
    return akt2_truth_jet_n;
  }
  default: return -1;
  } // end switch
}



/**
 * Returns the appropriate truth jet pT for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetPt (const int iJ, const JetRadius& radius) {
  assert (GetAktTruthJetN (radius) > iJ);
  switch (radius) {
  case JetRadius::R0p4: return akt4_truth_jet_pt[iJ];
  case JetRadius::R0p2: return akt2_truth_jet_pt[iJ];
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the appropriate truth jet eta for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetEta (const int iJ, const JetRadius& radius) {
  assert (GetAktTruthJetN (radius) > iJ);
  switch (radius) {
  case JetRadius::R0p4: return akt4_truth_jet_eta[iJ];
  case JetRadius::R0p2: return akt2_truth_jet_eta[iJ];
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the appropriate truth jet phi for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetPhi (const int iJ, const JetRadius& radius) {
  assert (GetAktTruthJetN (radius) > iJ);
  switch (radius) {
  case JetRadius::R0p4: return akt4_truth_jet_phi[iJ];
  case JetRadius::R0p2: return akt2_truth_jet_phi[iJ];
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the appropriate truth jet energy for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetEn (const int iJ, const JetRadius& radius) {
  assert (GetAktTruthJetN (radius) > iJ);
  switch (radius) {
  case JetRadius::R0p4: return akt4_truth_jet_e[iJ];
  case JetRadius::R0p2: return akt2_truth_jet_e[iJ];
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the minimum delta R to any other truth jet with a pT above some threshold.
 * Will cause an error due to NaN returns if the radius is not supported.
 */
float GetAktTruthJetIso (const int iJ, const JetRadius& radius) {
  assert (GetAktTruthJetN (radius) > iJ);

  int jn = GetAktTruthJetN (radius);
  float maxdr = FLT_MAX;
  float jeta = GetAktTruthJetEta (iJ, radius);
  float jphi = GetAktTruthJetPhi (iJ, radius);

  for (int iJp = 0; iJp < jn; iJp++) {
    if (iJp == iJ)
      continue; // skip the jet of interest
    if (GetAktTruthJetPt (iJp, radius) < GetAktTruthIsoMinPtCut (radius))
      continue; // minimum jet pT cut
    float dr = DeltaR (jeta, GetAktTruthJetEta (iJp, radius), jphi, GetAktTruthJetPhi (iJp, radius));
    maxdr = std::fmin (dr, maxdr); // set isolation
  }
  return maxdr;
}



/**
 * Returns the minimum truth jet pT for consideration in isolation calculation.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthIsoMinPtCut (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: return akt4_truth_IsoMinPt;
  case JetRadius::R0p2: return akt2_truth_IsoMinPt;
  default: return std::nan ("");
  } // end switch
}


/**
 * Returns the minimum DR for considering a truth jet isolated.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthIsoMinDR (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: return akt4_truth_IsoMinDR;
  case JetRadius::R0p2: return akt2_truth_IsoMinDR;
  default: return std::nan ("");
  } // end switch
}


/**
 * Returns the minimum pT for reco. jets in the truth-matching procedure.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthMatchMinRecoPt (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: return akt4_hi_TruthMatchMinRecoPt;
  case JetRadius::R0p2: return akt2_hi_TruthMatchMinRecoPt;
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the matched HI jet within DR < 1 to this truth jet.
 * Returns -1 if no HI jet is matched within this DR range, or the radius is invalid.
 */
int GetAktHIJetMatch (const int iTJ, const JetRadius& radius, const int nJESVar) {
  assert (GetAktTruthJetN (radius) > iTJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);
  float mindr = M_PI;
  int match = -1;
  const float tjeta = GetAktTruthJetEta (iTJ, radius);
  const float tjphi = GetAktTruthJetPhi (iTJ, radius);
  //const float minpt = GetAktTruthMatchMinRecoPt (radius);
  const int nJ = GetAktHIJetN (radius);
  for (int iJ = 0; iJ < nJ; iJ++) {
    //if (GetAktHIJetPt (iJ, radius, nJESVar) < minpt)
    //  continue; 
    const float dr = DeltaR (tjeta, GetAktHIJetEta (iJ, radius, nJESVar), tjphi, GetAktHIJetPhi (iJ, radius, nJESVar));
    if (dr < mindr) {
      match = iJ;
      mindr = dr;
    }
  }
  if (mindr > GetAktTruthMatchMaxDR (radius))
    return -1;
  return match;
}



/**
 * Determines the correct HI jet count for this radius.
 * Returns -1 if radius was not recognized.
 */
int GetAktHIJetN (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: {
    assert (akt4_hi_jet_n >= 0);
    return akt4_hi_jet_n;
  }
  case JetRadius::R0p2: {
    assert (akt2_hi_jet_n >= 0);
    return akt2_hi_jet_n;
  }
  default: return -1;
  } // end switch
}



/**
 * Determines the optimal jet pT to return (EtaJES or Cross-calibrated).
 * Jets in data must be cross-calibrated and jets in MC must not be, but jets in MC + data overlay should be cross-calibrated if they are not truth-matched.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetPt (const int iJ, const JetRadius& radius, const int nJESVar, const short scale) {
  assert (GetAktHIJetN (radius) > iJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);
  assert (scale >= -1 && scale <= 2);
  switch (radius) {
  case JetRadius::R0p4: {
    const float jesVar = 1. + (nJESVar == -1 ? 0 : akt4_hi_jet_pt_sys_JES_ALL[nJESVar][iJ]);
    if (IsCollisions () || scale == 0)                  return akt4_hi_jet_pt_xcalib[iJ] * jesVar;
    if (!IsDataOverlay () || scale == 1)                return akt4_hi_jet_pt_etajes[iJ] * jesVar;
    if (scale == 2)                                     return akt4_hi_jet_pt_precalib[iJ] * jesVar;
    if (GetAktTruthJetMatch (iJ, radius, nJESVar) >= 0) return akt4_hi_jet_pt_etajes[iJ] * jesVar;
    else                                                return akt4_hi_jet_pt_xcalib[iJ] * jesVar;
  }
  case JetRadius::R0p2: {
    const float jesVar = 1. + (nJESVar == -1 ? 0 : akt2_hi_jet_pt_sys_JES_ALL[nJESVar][iJ]);
    if (IsCollisions () || scale == 0)                  return akt2_hi_jet_pt_xcalib[iJ] * jesVar;
    if (!IsDataOverlay () || scale == 1)                return akt2_hi_jet_pt_etajes[iJ] * jesVar;
    if (scale == 2)                                     return akt2_hi_jet_pt_precalib[iJ] * jesVar;
    if (GetAktTruthJetMatch (iJ, radius, nJESVar) >= 0) return akt2_hi_jet_pt_etajes[iJ] * jesVar;
    else                                                return akt2_hi_jet_pt_xcalib[iJ] * jesVar;
  }
  default: 
    return std::nan ("");
  } // end switch
}



/**
 * Determines the optimal jet eta to return (EtaJES or Cross-calibrated).
 * The cross-calibration does nothing to jet eta so this function is trivial.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetEta (const int iJ, const JetRadius& radius, const int nJESVar) {
  assert (GetAktHIJetN (radius) > iJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);
  switch (radius) {
  case JetRadius::R0p4: return akt4_hi_jet_eta_etajes[iJ];
  case JetRadius::R0p2: return akt2_hi_jet_eta_etajes[iJ];
  default:              return std::nan ("");
  } // end switch
}



/**
 * Determines the optimal jet phi to return.
 * The EtaJES and cross-calibration do nothing to jet phi so this function is trivial.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetPhi (const int iJ, const JetRadius& radius, const int nJESVar) {
  assert (GetAktHIJetN (radius) > iJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);
  switch (radius) {
  case JetRadius::R0p4: return akt4_hi_jet_phi[iJ];
  case JetRadius::R0p2: return akt2_hi_jet_phi[iJ];
  default:              return std::nan ("");
  } // end switch
}



/**
 * Determines the optimal jet energy to return (EtaJES or Cross-calibrated).
 * Jets in data must be cross-calibrated and jets in MC must not be, but jets in MC + data overlay should be cross-calibrated if they are not truth-matched.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetEn (const int iJ, const JetRadius& radius, const int nJESVar, const short scale) {
  assert (GetAktHIJetN (radius) > iJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);
  assert (scale >= -1 && scale <= 2);
  switch (radius) {
  case JetRadius::R0p4: {
    const float jesVar = 1. + (nJESVar == -1 ? 0 : akt4_hi_jet_pt_sys_JES_ALL[nJESVar][iJ]);
    if (IsCollisions () || scale == 0)                  return akt4_hi_jet_e_xcalib[iJ] * jesVar;
    if (!IsDataOverlay () || scale == 1)                return akt4_hi_jet_e_etajes[iJ] * jesVar;
    if (scale == 2)                                     return akt4_hi_jet_e_precalib[iJ] * jesVar;
    if (GetAktTruthJetMatch (iJ, radius, nJESVar) >= 0) return akt4_hi_jet_e_etajes[iJ] * jesVar;
    else                                                return akt4_hi_jet_e_xcalib[iJ] * jesVar;
  }
  case JetRadius::R0p2: {
    const float jesVar = 1. + (nJESVar == -1 ? 0 : akt2_hi_jet_pt_sys_JES_ALL[nJESVar][iJ]);
    if (IsCollisions () || scale == 0)                  return akt2_hi_jet_e_xcalib[iJ] * jesVar;
    if (!IsDataOverlay () || scale == 1)                return akt2_hi_jet_e_etajes[iJ] * jesVar;
    if (scale == 2)                                     return akt2_hi_jet_e_precalib[iJ] * jesVar;
    if (GetAktTruthJetMatch (iJ, radius, nJESVar) >= 0) return akt2_hi_jet_e_etajes[iJ] * jesVar;
    else                                                return akt2_hi_jet_e_xcalib[iJ] * jesVar;
  }

  default:
    return std::nan ("");
  } // end switch
}



/**
 * Returns the maximum delta R for a truth-reco. jet match.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthMatchMaxDR (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: return akt4_TruthMatchMaxDR;
  case JetRadius::R0p2: return akt2_TruthMatchMaxDR;
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the minimum reconstructed jet pT for consideration in isolation calculation.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIIsoMinPtCut (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: return akt4_hi_IsoMinPt;
  case JetRadius::R0p2: return akt2_hi_IsoMinPt;
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the minimum DR for considering a reco. jet isolated.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIIsoMinDR (const JetRadius& radius) {
  switch (radius) {
  case JetRadius::R0p4: return akt4_hi_IsoMinDR;
  case JetRadius::R0p2: return akt2_hi_IsoMinDR;
  default: return std::nan ("");
  } // end switch
}



/**
 * Returns the minimum delta R to any other reconstructed jet with a pT above some threshold.
 * Will cause an error due to NaN returns if the radius is not supported.
 */
float GetAktHIJetIso (const int iJ, const JetRadius& radius, const int nJESVar) {
  assert (GetAktHIJetN (radius) > iJ);
  assert (nJESVar >= -1 && nJESVar < nJESSys);

  int jn = GetAktHIJetN (radius);
  float maxdr = FLT_MAX;
  float jeta = GetAktHIJetEta (iJ, radius, nJESVar);
  float jphi = GetAktHIJetPhi (iJ, radius, nJESVar);

  for (int iJp = 0; iJp < jn; iJp++) {
    if (iJp == iJ)
      continue; // skip the jet of interest
    if (GetAktHIJetPt (iJp, radius, nJESVar, 2) < GetAktHIIsoMinPtCut (radius))
      continue; // minimum jet pT cut
    float dr = DeltaR (jeta, GetAktHIJetEta (iJp, radius, nJESVar), jphi, GetAktHIJetPhi (iJp, radius, nJESVar));
    maxdr = std::fmin (dr, maxdr); // set isolation
  }
  return maxdr;
}



/**
 * Determines the optimal jet timing to return (depends on jet radius).
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetTiming (const int iJ, const JetRadius& radius) {
  assert (GetAktHIJetN (radius) > iJ);
  switch (radius) {
  case JetRadius::R0p4: return akt4_hi_jet_timing[iJ];
  case JetRadius::R0p2: return akt2_hi_jet_timing[iJ];
  default:              return std::nan ("");
  } // end switch
}


} // end namespace

#endif
