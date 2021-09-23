#ifndef __Process_h__
#define __Process_h__

#include "Params.h"

using namespace EGammaESStudy;

namespace EGammaESStudy {

float crossSectionPicoBarns = 0;
float mcFilterEfficiency = 0;
int mcNumberEvents = 0;

CollisionSystem collisionSystem = CollisionSystem::pp15; // default is pp15
DataType dataType = DataType::Collisions; // default is collisions
SystFlag systFlag = SystFlag::Nominal; // default is no systematics

}

#endif
