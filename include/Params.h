#ifndef __Params_h__
#define __Params_h__

#include "LocalUtilities.h"

#include <Utilities.h>

#include <TString.h>

#include <fstream>
#include <string>
#include <set>
#include <math.h>

typedef std::pair <float, float> QnVector;

namespace EGammaESStudy { 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variable declarations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const double electron_mass = 0.000511;
const double muon_mass = 0.105658;

extern TString workPath;
extern TString extWorkPath;
extern TString rootPath;
extern TString dataPath;

extern float crossSectionPicoBarns;
extern float mcFilterEfficiency;
extern int mcNumberEvents;

extern CollisionSystem collisionSystem;
extern DataType dataType; 
extern SystFlag systFlag;

const float akt2_TruthMatchMaxDR = 0.1; // Truth-matching maximum dR for R=0.2 jets
const float akt4_TruthMatchMaxDR = 0.2; // Truth-matching maximum dR for R=0.4 jets
const float akt2_hi_TruthMatchMinRecoPt = 0; // Minimum HI jet pT for reco. matching of R=0.2 truth jets
const float akt4_hi_TruthMatchMinRecoPt = 0; // Minimum HI jet pT for reco. matching of R=0.4 truth jets
const float akt2_hi_IsoMinPt = 7; // Minimum pT for isolation calculation of R=0.2 HI jets
const float akt4_hi_IsoMinPt = 7; // Minimum pT for isolation calculation of R=0.4 HI jets
const float akt2_truth_IsoMinPt = 7; // Minimum pT for isolation calculation of R=0.2 truth jets
const float akt4_truth_IsoMinPt = 7; // Minimum pT for isolation calculation of R=0.4 truth jets
const float akt2_hi_IsoMinDR = 0.0; // Minimum isolation DR for R=0.2 HI jets
const float akt4_hi_IsoMinDR = 0.0; // Minimum isolation DR for R=0.4 HI jets
const float akt2_truth_IsoMinDR = 0.5; // Minimum isolation DR for R=0.2 truth jets
const float akt4_truth_IsoMinDR = 1.0; // Minimum isolation DR for R=0.4 truth jets

} // end namespace

#endif
