#ifndef __LocalUtilities_h__
#define __LocalUtilities_h__

#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TProfile.h>

#include <vector>

typedef TGraphAsymmErrors TGAE;
typedef std::pair <float, float> QnVector;
typedef std::pair <float, float> FCalEt;

namespace EGammaESStudy {

// run 2 HI data sets
enum class CollisionSystem { pp15, PbPb15, pPb16s5TeV, pPb16, Pbp16, XeXe17, pp17, PbPb18 };
static const std::vector <CollisionSystem> AllCollisionSystem = { CollisionSystem::pp15, CollisionSystem::PbPb15, CollisionSystem::pPb16s5TeV, CollisionSystem::pPb16, CollisionSystem::Pbp16, CollisionSystem::XeXe17, CollisionSystem::pp17, CollisionSystem::PbPb18 };

// data types used in HI
enum class DataType { Collisions, MCSignal, MCDataOverlay, MCHijing, MCHijingOverlay };
static const std::vector <DataType> AllDataType = { DataType::Collisions, DataType::MCSignal, DataType:: MCDataOverlay, DataType::MCHijing, DataType::MCHijingOverlay };

// types of systematic variations
enum class SystFlag {
  Nominal
};
static const std::vector <SystFlag> AllSystFlag = {
  SystFlag::Nominal
};

enum class JetRadius { R0p2, R0p3, R0p4, R0p6, R0p8, R1p0, Invalid };


TString ToTString (const CollisionSystem& collSys);
TString ToTString (const DataType& dType);
TString ToTString (const SystFlag& sFlag);
float   GetRadius (const JetRadius& r);


bool SetCollisionSystem (TString ts);
bool SetDataType (TString ts);
bool SetSystFlag (TString ts);


bool IsIons (const CollisionSystem& collSys);
bool IsPbPb (const CollisionSystem& collSys);
bool IsPbPb18 (const CollisionSystem& collSys);
bool IsPbPb15 (const CollisionSystem& collSys);
bool IsXeXe (const CollisionSystem& collSys);
bool IspPb (const CollisionSystem& collSys);
bool IspPb16 (const CollisionSystem& collSys);
bool Ispp (const CollisionSystem& collSys);
bool Ispp15 (const CollisionSystem& collSys);
bool Ispp17 (const CollisionSystem& collSys);
bool IsPeriodA (const CollisionSystem& collSys);
bool Is5TeV (const CollisionSystem& collSys);
bool Is8TeV (const CollisionSystem& collSys);
bool Is2018 (const CollisionSystem& collSys);
bool Is2017 (const CollisionSystem& collSys);
bool Is2016 (const CollisionSystem& collSys);
bool Is2015 (const CollisionSystem& collSys);

bool IsCollisions (const DataType& dType);
bool IsOverlay (const DataType& dType);
bool IsDataOverlay (const DataType& dType);
bool IsHijing (const DataType& dType);


bool IsIons ();
bool IsPbPb ();
bool IsPbPb18 ();
bool IsPbPb15 ();
bool IsXeXe ();
bool IspPb ();
bool IspPb16 ();
bool Ispp ();
bool Ispp15 ();
bool Ispp17 ();
bool IsPeriodA ();
bool Is5TeV ();
bool Is8TeV ();
bool Is2018 ();
bool Is2017 ();
bool Is2016 ();
bool Is2015 ();

bool IsCollisions ();
bool IsOverlay ();
bool IsDataOverlay ();
bool IsHijing ();


/**
 * Returns the CoM boost relevant for asymmetric collision systems (i.e. p+Pb). 0 for everything else.
 */
double GetBoost (int rn);


/**
 * Establishes path variables appropriately.
 */
void SetupDirectories (const TString& dataSubDir, const bool addSubDir = true);


/**
 * Looks up MC sample cross section, filter efficiency, and number of events.
 */
bool GetMCWeights (const TString& fname);


/**
 * Returns the minimum anti-kT R=0.4 truth jet pT for this JZXR04 MC sample.
 */
float GetJZXR04MinPt (const TString& fname);


/**
 * Returns the maximum anti-kT R=0.4 truth jet pT for this JZXR04 MC sample.
 */
float GetJZXR04MaxPt (const TString& fname);


/**
 * Returns a copy of the histogram detailing the Zdc cuts.
 */
TH1D* GetZdcCuts ();


/**
 * Returns an abbreviated, unique identifier for a given dataset.
 */
TString GetIdentifier (const int dataSet, const char* directory, const char* inFileName);


/**
 * Returns the proper jet trigger luminosity for this data set in nb^-1
 */
double GetJetLuminosity ();


/**
 * Returns true if this truth jet passes selection criteria.
 */
bool MeetsTruthJetAcceptanceCuts (int iTJ, const JetRadius& radius);


/**
 * Returns true if this jet passes selection criteria.
 */
bool MeetsJetAcceptanceCuts (int iJ, const JetRadius& radius, const int nJESVar = -1);


/**
 * Returns the matched truth jet within DR < 1 to this HI jet.
 * Returns -1 if no truth jet is matched within this DR range, or the radius is invalid.
 */
int GetAktTruthJetMatch (const int iJ, const JetRadius& radius, const int nJESVar = -1);


/**
 * Determines the correct truth jet count for this radius.
 * Returns -1 if radius was not recognized.
 */
int GetAktTruthJetN (const JetRadius& radius);


/**
 * Returns the appropriate truth jet pT for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetPt (const int iJ, const JetRadius& radius);


/**
 * Returns the appropriate truth jet eta for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetEta (const int iJ, const JetRadius& radius);


/**
 * Returns the appropriate truth jet phi for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetPhi (const int iJ, const JetRadius& radius);


/**
 * Returns the appropriate truth jet energy for the given jet radius.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthJetEn (const int iJ, const JetRadius& radius);


/**
 * Returns the minimum delta R to any other truth jet with a pT above some threshold.
 * Will cause an error due to NaN returns if the radius is not supported.
 */
float GetAktTruthJetIso (const int iJ, const JetRadius& radius);


/**
 * Returns the minimum truth jet pT for consideration in isolation calculation.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthIsoMinPtCut (const JetRadius& radius);


/**
 * Returns the minimum DR for considering a truth jet isolated.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthIsoMinDR (const JetRadius& radius);


/**
 * Returns the minimum pT for reco. jets in the truth-matching procedure.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthMatchMinRecoPt (const JetRadius& radius);


/**
 * Returns the matched HI jet within DR < 1 to this truth jet.
 * Returns -1 if no HI jet is matched within this DR range, or the radius is invalid.
 */
int GetAktHIJetMatch (const int iTJ, const JetRadius& radius, const int nJESVar = -1);


/**
 * Determines the correct HI jet count for this radius.
 * Returns -1 if radius was not recognized.
 */
int GetAktHIJetN (const JetRadius& radius);


/**
 * Determines the optimal jet pT to return (EtaJES or Cross-calibrated).
 * Jets in data must be cross-calibrated and jets in MC must not be, but jets in MC + data overlay should be cross-calibrated if they are not truth-matched.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetPt (const int iJ, const JetRadius& radius, const int nJESVar = -1, const short scale = -1);


/**
 * Determines the optimal jet eta to return (EtaJES or Cross-calibrated).
 * The cross-calibration does nothing to jet eta so this function is trivial.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetEta (const int iJ, const JetRadius& radius, const int nJESVar = -1);


/**
 * Determines the optimal jet phi to return.
 * The EtaJES and cross-calibration do nothing to jet phi so this function is trivial.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetPhi (const int iJ, const JetRadius& radius, const int nJESVar = -1);


/**
 * Determines the optimal jet energy to return (EtaJES or Cross-calibrated).
 * Jets in data must be cross-calibrated and jets in MC must not be, but jets in MC + data overlay should be cross-calibrated if they are not truth-matched.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetEn (const int iJ, const JetRadius& radius, const int nJESVar = -1, const short scale = -1);


/**
 * Returns the maximum delta R for a truth-reco. jet match.
 * Returns NaN if radius was not recognized.
 */
float GetAktTruthMatchMaxDR (const JetRadius& radius);


/**
 * Returns the minimum reconstructed jet pT for consideration in isolation calculation.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIIsoMinPtCut (const JetRadius& radius);


/**
 * Returns the minimum DR for considering a reco. jet isolated.
 * Returns NaN if radius was not recognized.
 */
float GetAktHIIsoMinDR (const JetRadius& radius);


/**
 * Returns the minimum delta R to any other reconstructed jet with a pT above some threshold.
 * Will cause an error due to NaN returns if the radius is not supported.
 */
float GetAktHIJetIso (const int iJ, const JetRadius& radius, const int nJESVar = -1);


/**
 * Determines the optimal jet timing to return (depends on jet radius).
 * Returns NaN if radius was not recognized.
 */
float GetAktHIJetTiming (const int iJ, const JetRadius& radius);

} // end namespace

#endif
