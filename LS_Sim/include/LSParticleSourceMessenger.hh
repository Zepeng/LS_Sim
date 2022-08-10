#ifndef LSParticleSourceMessenger_h
#define LSParticleSourceMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class LSParticleSource;

class G4ParticleTable;
class G4UIcommand;
class G4UImessenger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;


class LSParticleSourceMessenger : public G4UImessenger {

    public:
        LSParticleSourceMessenger(LSParticleSource* fPtclGun);
        ~LSParticleSourceMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        G4ParticleTable* particleTable;
        LSParticleSource* fParticleGun;

    private:
        G4UIdirectory               *gunDirectory;

        G4UIcmdWithAnInteger        *numCmd;

        G4UIcmdWithAString          *particle1Cmd;
        G4UIcmdWithAString          *momType1Cmd;
        G4UIcmdWithADoubleAndUnit   *energy1Cmd;
        G4UIcmdWith3VectorAndUnit   *mom1Cmd;
        G4UIcmdWith3VectorAndUnit   *pos1Cmd;


        G4UIcmdWithAString          *particle2Cmd;
        G4UIcmdWithAString          *momType2Cmd;
        G4UIcmdWithADoubleAndUnit   *energy2Cmd;
        G4UIcmdWith3VectorAndUnit   *mom2Cmd;
        G4UIcmdWith3VectorAndUnit   *pos2Cmd;

};

#endif





