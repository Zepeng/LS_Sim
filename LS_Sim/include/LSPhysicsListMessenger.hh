#ifndef LSPhysicsListMessenger_h
#define LSPhysicsListMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
class LSPhysicsList;

class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class LSPhysicsListMessenger : public G4UImessenger {

    public:
        LSPhysicsListMessenger(LSPhysicsList* phylist);
        ~LSPhysicsListMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);


    private:
        LSPhysicsList* LSPhyList;

        G4UIdirectory       *phyDirectory;
        G4UIcmdWithABool    *opticalCmd;
        G4UIcmdWithADouble  *yieldCmd;
};

#endif


