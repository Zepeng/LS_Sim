
#include "g4root.hh"

#include "LSAnalysisManager.hh"

LSAnalysisManager* LSAnalysisManager::instance = 0;

LSAnalysisManager::LSAnalysisManager()
    : outputFileName("toyMC")
{
    G4AnalysisManager::Instance();

    analysisMessenger = new LSAnalysisMessenger(this);

}

LSAnalysisManager::~LSAnalysisManager()
{
    delete instance;
    instance = 0;

    delete G4AnalysisManager::Instance();
}

void LSAnalysisManager::book()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    //Open an output file
    man->OpenFile(outputFileName);
    man->SetVerboseLevel(1);
    man->SetFirstHistoId(1);
    man->SetFirstNtupleId(1);

    G4cout << "Open output file: " << outputFileName << G4endl;
    man->CreateNtuple("photon", "Hits Info on SD");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleIColumn("totPE");
    man->CreateNtupleIColumn("cerPE");
    man->CreateNtupleIColumn("sctPE");
    man->FinishNtuple();
    G4cout << "Created ntuple for photon counting" << G4endl;
}

void LSAnalysisManager::finish()
{
    G4cout << "Going to save ntuples" << G4endl;
    // Save histograms
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}

LSAnalysisManager* LSAnalysisManager::getInstance()
{
    if (instance==0) { instance = new LSAnalysisManager(); }
    return instance;
}


void LSAnalysisManager::analyseEventID( G4int evtid )
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn( 0, evtid );
}


void LSAnalysisManager::analyseTotNPE(G4int number)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn( 1, number );
}

void LSAnalysisManager::analyseCerNPE(G4int number)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn( 2, number );
}

void LSAnalysisManager::analyseSctNPE(G4int number)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn( 3, number );
}


void LSAnalysisManager::analyseAddNtupleRow()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->AddNtupleRow();
}

