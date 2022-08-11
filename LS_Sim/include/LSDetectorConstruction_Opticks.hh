#pragma once

class G4CXOpticks ;
class G4VPhysicalVolume ;
//class G4VSensitiveDetector ;

struct LSDetectorConstruction_Opticks
{
    static G4CXOpticks* Setup(const G4VPhysicalVolume* world, int opticksMode );
};

