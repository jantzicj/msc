#include "mscSteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "TFile.h"
#include "TTree.h"



mscSteppingAction::mscSteppingAction(G4int *evN)		
{
  //eventID pointer from the mscEventAction.cc file
  evNr=evN;

  /*Create root file and initialize what I want to put in it*/


  fout=new TFile("o_mscSteppingAction.root","RECREATE");
  htst=new TH1D("htst","Test histo",100,-10,10);
  htst->FillRandom("gaus");

}


mscSteppingAction::~mscSteppingAction()
{
  /*Write out root file*/
  fout->cd();
  htst->Write();
  fout->Close();
}


void mscSteppingAction::UserSteppingAction(const G4Step* theStep)
{


  G4Track*              theTrack     = theStep->GetTrack();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  G4StepPoint*          thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint*          thePostPoint = theStep->GetPostStepPoint();
  //G4VPhysicalVolume*    thePostPV    = thePostPoint->GetPhysicalVolume();
  G4String              particleName = theTrack->GetDefinition()->GetParticleName();
  
  //get material
  G4Material* theMaterial = theTrack->GetMaterial();



  if(theMaterial){
    if(theMaterial->GetName().compare("detectorMat")==0){
      G4cout<<" In  detector " << *evNr<<" "
	    <<theTrack->GetTrackID()<<" "<< theTrack->GetParentID()<< " "
	    <<thePrePoint->GetPosition().getX()<<" "
	    <<thePrePoint->GetMomentum().getX()<<" "
	    <<thePostPoint->GetPosition().getY()<<" "
	    <<thePostPoint->GetMomentum().getZ()<<" "
	    <<thePrePoint->GetTotalEnergy()<<" "
	    <<particleType->GetPDGEncoding()<<G4endl;    
    }
    if(theMaterial->GetName().compare("PBA")==0){
      G4cout<<" In  radiator " << *evNr<<" "
	    <<theTrack->GetTrackID()<<" "<< theTrack->GetParentID()<< " "
	    <<thePrePoint->GetPosition().getX()<<" "
	    <<thePrePoint->GetMomentum().getX()<<" "
	    <<thePostPoint->GetPosition().getY()<<" "
	    <<thePostPoint->GetMomentum().getZ()<<" "
	    <<thePrePoint->GetTotalEnergy()<<" "
	    <<particleType->GetPDGEncoding()<<G4endl;   
    }
  }

  
  /*fill tree*/ 

  TTree *data = new TTree("mscData", "DATA");

  Int_t pre_pos_x = thePrePoint->GetPosition().getX();
  Int_t post_pos_x = thePostPoint->GetPosition().getX();

  data->Branch("pre_pos_x", &pre_pos_x, 64000);
  data->Branch("post_pos_x", &post_pos_x, 64000);

  data->SetBranchAddress("pre_pos_x", &pre_pos_x);
  data->SetBranchAddress("post_pos_x", &post_pos_x);

  GetEntry();

  for (Int_t i = 0; i<; i++)
  {
  data->Fill();
  }

  data->Write();
  fout->Close();

}




