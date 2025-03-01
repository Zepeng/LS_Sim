#include <vector>
#include <string>

#include "LSDetectorConstruction_Opticks.hh"

#ifdef WITH_G4CXOPTICKS
//#include "G4PVPlacement.hh"
//#include "junoSD_PMT_v2.hh"

#include "G4CXOpticks.hh"
//#include "PMTEfficiencyTable.hh"
#include "PLOG.hh"
//#include "SSys.hh"
#endif

#ifdef WITH_G4CXOPTICKS
/**
LSExpDetectorConstruction_Opticks::Setup
------------------------------------------

1. pass geometry to Opticks, translate it to GPU and return sensor placements 
2. use the placements to pass sensor data : efficiencies, categories, identifiers
3. pass theta dependent efficiency tables for all sensor categories




                             |--------- 2230 ----------------|-- 120--|
                             20050                           17820    17700
                          / /                               /         /
                         / /                               /         /
                        / pInnerWater                     /         /
                       / /                               /         /
                      / /                  (0)          /         /
                     pTyvek                  \         pAcrylic  /
                    / /                       \       /         /
                   / /                         \     /         pTarget:LS
                  / /                           \   /         /
                 / /                             \ /         /
                / /                              (1)        /
               / /                               / \       /
              / /                               /   \     /
             / /                               /     \   /         
            / /                               /       \ /
           / /                          Wa   /  Ac    (2)             
          / /                               /         / \
         / /                               /         /   \
        / /                               /         /     \        LS    




The below examles of geospecific options control which boundary_pos is captured 
into the way buffer and saved with hits "hiy". Using::

      --way --pvname pAcrylic --boundary Water///Acrylic --waymask 3 --gdmlkludge    # (1): gives radius 17820
      --way --pvname pTarget  --boundary Acrylic///LS --waymask 3  --gdmlkludge      # (2): gives radius 17700

*way*
    enables way point recording 

*pvname*
    potential input to control which way boundary position to record

*boundary*
    potential input to control which way boundary position to record

*waymask* 
    selects whether to match on pvname and/or boundary for the recorder boundary position see OGeo::initWayControl


*gdmlkludge*
   applies kludge to exported origin.gdml creating origin_CGDMLKludge.gdml with a few fixes:

   1. define/matrix with truncated values are trimmed to even to make them parseable
   2. define/constant are converted to define/matrix 

   The "--gdmlkludge" option is needed to avoid GDML parse failures that cause around 8 opticks-t tests to fail.

**/


G4CXOpticks* LSDetectorConstruction_Opticks::Setup(const G4VPhysicalVolume* world)  // static
{
    assert(world); 

    // 1. pass geometry to Opticks, translate it to GPU and return sensor placements  

    //G4CXOpticks* g4cx = G4CXOpticks::Get() ; 
	//const char * gdml_path = 
	G4CXOpticks::SetGeometry(world);
    //g4cx->setGeometry(world); 
	G4CXOpticks* g4cx = G4CXOpticks::Get();	
	LOG(info) << "] WITH_G4OPTICKS " ; 
    return g4cx ; 
}
#else
G4CXOpticks* LSDetectorConstruction_Opticks::Setup(const G4VPhysicalVolume*)  // static
{
    return nullptr ; 
}
#endif

