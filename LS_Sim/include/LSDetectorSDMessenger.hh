#ifndef LSDetectorSDMessenger_h
#define LSDetectorSDMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class LSDetectorSD;

class G4UIcommand;
class G4UIcmdWithADouble;

class LSDetectorSDMessenger : public G4UImessenger {

    public:
        LSDetectorSDMessenger(LSDetectorSD* sd);
        ~LSDetectorSDMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        LSDetectorSD* LSDetSD;

    private:
        G4UIdirectory           *sdDirectory;

        G4UIcmdWithADouble      *effCmd;

};

#endif
