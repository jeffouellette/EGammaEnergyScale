#ifndef __ZBosonReco_cxx__
#define __ZBosonReco_cxx__

#include "ZBosonReco.h"
#include "Params.h"
#include "TreeVariables.h"
#include "LocalUtilities.h"
#include "Trigger.h"

#include <Utilities.h>
#include <AtlasUtils.h>

#include <TH2D.h>
#include <TChain.h>
#include <TSystem.h>
#include <TLorentzVector.h>

#include <iostream>

using namespace std;

namespace EGammaESStudy {


bool ZBosonReco (const char* directory,
                 const int dataSet,
                 const char* inFileName) {

  std::cout << "Info: In ZBosonReco.cxx: Entered ZBosonReco routine." << std::endl;

  SetupDirectories ("ZBosonReco");


  // this identifier here corresponds to the name of the output file
  const TString identifier = GetIdentifier (dataSet, directory, inFileName);
  std::cout << "Info: In ZBosonReco.cxx: File Identifier: " << identifier << std::endl;
  std::cout << "Info: In ZBosonReco.cxx: Saving output to " << rootPath << std::endl;


  // creates a file identifier pattern that we will use to identify the directory containing the input files
  TString fileIdentifier;
  if (TString (inFileName) == "") {
    if (IsCollisions ()) {
      fileIdentifier = to_string (dataSet);
    }
    else {
      std::cout << "Error: In ZBosonReco.cxx: Cannot identify this MC file! Quitting." << std::endl;
      return false;
    }
  }
  else
    fileIdentifier = inFileName;


  // opens a TTree as a TChain from all files in a directory matching the file identifier
  TChain* tree = new TChain ("bush", "bush");
  {
    TString pattern = "*.root";
    std::cout << "DataPath = " << dataPath << std::endl;
    auto dir = gSystem->OpenDirectory (dataPath + directory);
    while (const char* f = gSystem->GetDirEntry (dir)) {
      TString file = TString (f);

      if (IsCollisions () && Ispp ()) {
        if (!file.Contains ("Main"))
          continue;
      }

      if (!file.Contains (fileIdentifier))
        continue;
      std::cout << "Adding " << dataPath + directory + "/" + file + "/*.root" << " to TChain" << std::endl;
      tree->Add (dataPath + directory + "/" + file + "/*.root");
      break;
    }
    std::cout << "Chain has " << tree->GetListOfFiles ()->GetEntries () << " files, " << tree->GetEntries () << " entries" << std::endl;
  }


  tree->SetBranchAddress ("run_number",     &run_number);
  tree->SetBranchAddress ("event_number",   &event_number);
  tree->SetBranchAddress ("lumi_block",     &lumi_block);

  tree->SetBranchAddress ("actualInteractionsPerCrossing",  &actualInteractionsPerCrossing);
  tree->SetBranchAddress ("averageInteractionsPerCrossing", &averageInteractionsPerCrossing);


  if (!IsCollisions ()) {
    tree->SetBranchAddress ("mcEventWeights", &mcEventWeights);
  }


  tree->SetBranchAddress ("nvert",     &nvert);
  tree->SetBranchAddress ("vert_x",    &vert_x);
  tree->SetBranchAddress ("vert_y",    &vert_y);
  tree->SetBranchAddress ("vert_z",    &vert_z);
  tree->SetBranchAddress ("vert_ntrk", &vert_ntrk);
  tree->SetBranchAddress ("vert_type", &vert_type);


  tree->SetBranchAddress ("fcalA_et",       &fcalA_et);
  tree->SetBranchAddress ("fcalC_et",       &fcalC_et);


  if (!IsCollisions ()) {

    tree->SetBranchAddress ("truth_electron_n",       &truth_electron_n);
    tree->SetBranchAddress ("truth_electron_pt",      &truth_electron_pt);
    tree->SetBranchAddress ("truth_electron_eta",     &truth_electron_eta);
    tree->SetBranchAddress ("truth_electron_phi",     &truth_electron_phi);
    tree->SetBranchAddress ("truth_electron_charge",  &truth_electron_charge);
    tree->SetBranchAddress ("truth_electron_barcode", &truth_electron_barcode);


    //tree->SetBranchAddress ("akt2_truth_jet_n",     &akt2_truth_jet_n);
    //tree->SetBranchAddress ("akt2_truth_jet_pt",    &akt2_truth_jet_pt);
    //tree->SetBranchAddress ("akt2_truth_jet_eta",   &akt2_truth_jet_eta);
    //tree->SetBranchAddress ("akt2_truth_jet_phi",   &akt2_truth_jet_phi);
    //tree->SetBranchAddress ("akt2_truth_jet_e",     &akt2_truth_jet_e);

    tree->SetBranchAddress ("akt4_truth_jet_n",     &akt4_truth_jet_n);
    tree->SetBranchAddress ("akt4_truth_jet_pt",    &akt4_truth_jet_pt);
    tree->SetBranchAddress ("akt4_truth_jet_eta",   &akt4_truth_jet_eta);
    tree->SetBranchAddress ("akt4_truth_jet_phi",   &akt4_truth_jet_phi);
    tree->SetBranchAddress ("akt4_truth_jet_e",     &akt4_truth_jet_e);
  }

  tree->SetBranchAddress ("electron_n",                     &electron_n);
  tree->SetBranchAddress ("electron_pt_precalib",           &electron_pt_precalib);
  tree->SetBranchAddress ("electron_pt",                    &electron_pt);
  tree->SetBranchAddress ("electron_eta",                   &electron_eta);
  tree->SetBranchAddress ("electron_phi",                   &electron_phi);
  tree->SetBranchAddress ("electron_charge",                &electron_charge);
  tree->SetBranchAddress ("electron_lhtight",               &electron_lhtight);
  tree->SetBranchAddress ("electron_lhmedium",              &electron_lhmedium);
  tree->SetBranchAddress ("electron_lhloose",               &electron_lhloose);
  tree->SetBranchAddress ("electron_lhmedium_hi",           &electron_lhmedium_hi);
  tree->SetBranchAddress ("electron_lhloose_hi",            &electron_lhloose_hi);
  if (IsCollisions ())
    tree->SetBranchAddress ("electron_matched",             &electron_matched);
  tree->SetBranchAddress ("electron_etcone20",              &electron_etcone20);
  tree->SetBranchAddress ("electron_etcone30",              &electron_etcone30);
  tree->SetBranchAddress ("electron_etcone40",              &electron_etcone40);
  tree->SetBranchAddress ("electron_topoetcone20",          &electron_topoetcone20);
  tree->SetBranchAddress ("electron_topoetcone30",          &electron_topoetcone30);
  tree->SetBranchAddress ("electron_topoetcone40",          &electron_topoetcone40);
  tree->SetBranchAddress ("electron_id_track_pt",           &electron_id_track_pt);
  tree->SetBranchAddress ("electron_id_track_eta",          &electron_id_track_eta);
  tree->SetBranchAddress ("electron_id_track_phi",          &electron_id_track_phi);
  tree->SetBranchAddress ("electron_id_track_charge",       &electron_id_track_charge);
  tree->SetBranchAddress ("electron_id_track_d0",           &electron_id_track_d0);
  tree->SetBranchAddress ("electron_id_track_d0sig",        &electron_id_track_d0sig);
  tree->SetBranchAddress ("electron_id_track_z0",           &electron_id_track_z0);
  tree->SetBranchAddress ("electron_id_track_z0sig",        &electron_id_track_z0sig);
  tree->SetBranchAddress ("electron_id_track_theta",        &electron_id_track_theta);
  tree->SetBranchAddress ("electron_id_track_vz",           &electron_id_track_vz);
  tree->SetBranchAddress ("electron_id_track_TightPrimary", &electron_id_track_TightPrimary);
  tree->SetBranchAddress ("electron_id_track_HILoose",      &electron_id_track_HILoose);
  tree->SetBranchAddress ("electron_id_track_HITight",      &electron_id_track_HITight);
  tree->SetBranchAddress ("electron_Rhad1",                 &electron_Rhad1);
  tree->SetBranchAddress ("electron_Rhad",                  &electron_Rhad);
  tree->SetBranchAddress ("electron_e277",                  &electron_e277);
  tree->SetBranchAddress ("electron_Reta",                  &electron_Reta);
  tree->SetBranchAddress ("electron_Rphi",                  &electron_Rphi);
  tree->SetBranchAddress ("electron_weta1",                 &electron_weta1);
  tree->SetBranchAddress ("electron_weta2",                 &electron_weta2);
  tree->SetBranchAddress ("electron_wtots1",                &electron_wtots1);
  tree->SetBranchAddress ("electron_f1",                    &electron_f1);
  tree->SetBranchAddress ("electron_f3",                    &electron_f3);
  tree->SetBranchAddress ("electron_fracs1",                &electron_fracs1);
  tree->SetBranchAddress ("electron_DeltaE",                &electron_DeltaE);
  tree->SetBranchAddress ("electron_Eratio",                &electron_Eratio);
  //tree->SetBranchAddress ("electron_pt_sys",                &electron_pt_sys);
  //tree->SetBranchAddress ("electron_eta_sys",               &electron_eta_sys);
  //tree->SetBranchAddress ("electron_phi_sys",               &electron_phi_sys);


  if (!Is5TeV ()) {
    std::cout << "Error: In ZBosonReco.cxx::ZBosonReco (const char*, const int, const char*): Unsupported beam collision energy, quitting." << std::endl;
    return false;
  }
  else if (Ispp17 ()) {
    electron_trig_n = electron_trig_n_pp17;
    electron_trig_name = electron_trig_name_pp17;
  }
  else if (Ispp15 ()) {
    electron_trig_n = electron_trig_n_pp15;
    electron_trig_name = electron_trig_name_pp15;
  }
  else {
    std::cout << "Error: In ZBosonReco.cxx::ZBosonReco (const char*, const int, const char*): Beam configuration not recognized, quitting." << std::endl;
    return false;
  }


  Trigger* electronTrigger = nullptr;

  if (IsCollisions ()) {
    electronTrigger = new Trigger (electron_trig_name[0]);
    tree->SetBranchAddress ((electron_trig_name[0]+"_decision").c_str (), &(electronTrigger->trigDecision));
    tree->SetBranchAddress ((electron_trig_name[0]+"_prescale").c_str (), &(electronTrigger->trigPrescale));
  }


  // Load files for output
  TFile* outFile = new TFile (Form ("%s/%s.root", rootPath.Data (), identifier.Data ()), "recreate");

  const int nMuBins = 100;
  const double* muBins = linspace (0, 20, nMuBins);

  TH1D* h_mb_instMu = new TH1D (Form ("h_mb_instMu_run%i", dataSet), ";#mu_{inst};", nMuBins, muBins);
  h_mb_instMu->Sumw2 ();
  TH1D* h_mb_avgMu = new TH1D (Form ("h_mb_avgMu_run%i", dataSet), ";#mu_{avg};", nMuBins, muBins);
  h_mb_avgMu->Sumw2 ();

  TH1D* h_z_mass[2];
  TH1D* h_z_y[2];
  TH1D* h_z_eta[2];
  for (short iReg : {0, 1}) {
    h_z_mass[iReg] = new TH1D (Form ("h_z_mass_iReg%i", iReg), ";m_{ee} [GeV];Number of pairs / GeV", 120, 40, 160);
    h_z_mass[iReg]->Sumw2 ();
    h_z_y[iReg] = new TH1D (Form ("h_z_y_iReg%i", iReg), ";y_{ee};Number of pairs / 0.1", 60, -3, 3);
    h_z_y[iReg]->Sumw2 ();
    h_z_eta[iReg] = new TH1D (Form ("h_z_eta_iReg%i", iReg), ";#eta_{ee};Number of pairs / 0.1", 50, -2.5, 2.5);
    h_z_eta[iReg]->Sumw2 ();
  }


  const int nEvts = tree->GetEntries ();

  // First loop over events
  for (int iEvt = 0; iEvt < nEvts; iEvt++) {
    if (nEvts > 0 && iEvt % (nEvts / 100) == 0)
      std::cout << "Info: In ZBosonReco.cxx: Events " << iEvt / (nEvts / 100) << "\% done...\r" << std::flush;

    tree->GetEntry (iEvt);

    // vertexing cuts, require a primary vertex with |vz| < 150mm
    {
      bool hasPrimary = false;
      //bool hasPileup = false;
      float vz = -999;
      for (int iVert = 0; iVert < nvert; iVert++) {
        if (vert_type[iVert] == 1) {
          hasPrimary = true;
          vz = vert_z[iVert];
        }
        //else if (vert_type[iVert] == 3 && (Ispp () || vert_ntrk[iVert] > 6))
        //  hasPileup = true;
      }
      if (std::fabs (vz) > 150 || !hasPrimary)
        continue;
    }


    // ALGORITHM: find all unique electron pairs in the event and calculate the invariant mass. Match this to the Z peak.
    TLorentzVector tlv_e1, tlv_e2, tlv_z;

    // first electron loop.
    for (int iE1 = 0; iE1 < electron_n; iE1++) {

      if (electron_pt[iE1] < 16)
        continue; // pT cut
      if (!InEMCal (electron_eta[iE1]))
        continue; // check for in EMCal
      if (!electron_lhloose[iE1])
        continue; // quality cut

      tlv_e1.SetPtEtaPhiM (electron_pt[iE1], electron_eta[iE1], electron_phi[iE1], electron_mass);

      // second electron loop.
      for (int iE2 = iE1+1; iE2 < electron_n; iE2++) {

        if (electron_pt[iE2] < 16)
          continue; // pT cut
        if (!InEMCal (electron_eta[iE2]))
          continue; // check for in EMCal
        if (!electron_lhloose[iE2])
          continue; // quality cut

        if (electron_charge[iE1] + electron_charge[iE2] != 0)
          continue; // check for opposite sign electrons
        if (IsCollisions () && !electron_matched[0][iE1] && !electron_matched[0][iE2])
          continue; // trigger matching requirement

        tlv_e2.SetPtEtaPhiM (electron_pt[iE2], electron_eta[iE2], electron_phi[iE2], electron_mass);

        tlv_z = tlv_e1 + tlv_e2;

        const short iReg = (std::fabs (tlv_z.Rapidity ()) < 1 ? 0 : 1);

        h_z_mass[iReg]->Fill (tlv_z.M ());
        h_z_y[iReg]->Fill (tlv_z.Rapidity ());
        h_z_eta[iReg]->Fill (tlv_z.PseudoRapidity ());

      } // end loop over iE2

    } // end loop over iE1

  } // end event loop
  std::cout << std::endl << "Info: In ZBosonReco.cxx: Finished processing events." << std::endl;


  if (IsCollisions ()) {
    SaferDelete (&electronTrigger);
  }
  SaferDelete (&tree);


  outFile->cd ();

  h_mb_instMu->Write ();
  SaferDelete (&h_mb_instMu);
  h_mb_avgMu->Write ();
  SaferDelete (&h_mb_avgMu);

  for (short iReg : {0, 1}) {

    h_z_mass[iReg]->Write ();
    SaferDelete (&h_z_mass[iReg]);
    h_z_y[iReg]->Write ();
    SaferDelete (&h_z_y[iReg]);
    h_z_eta[iReg]->Write ();
    SaferDelete (&h_z_eta[iReg]);

  }

  outFile->Close ();
  SaferDelete (&outFile);

  return true;
}

} // end namespace

#endif
