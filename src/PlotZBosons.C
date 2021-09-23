#include <ArrayTemplates.h>
#include <Utilities.h>
#include <MyStyle.h>
#include <MyColors.h>

#include "Params.h"
#include "TreeVariables.h"
#include "LocalUtilities.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooPlot.h"

using namespace EGammaESStudy;
using namespace RooFit;

RooRealVar mass ("m", "m", 40, 160);

RooRealVar mz ("mz", "mz", 91, 86, 96);
RooRealVar width ("width", "width", 3, 1, 10);

RooRealVar ampl ("A", "A", 0, 0, 1e6);
RooRealVar alpha ("alpha", "alpha", -4, -10, 0.); 

TF1* f_z_mass[2][2];
TF1* f_z_mass_tot[2];


TF1* DoRBWFit (TH1D* h, float* fit_mz, float* fit_width, float* fit_ampl, float* fit_alpha) {

  TString name = h->GetName ();
  name.ReplaceAll ("h_", "dh_");

  RooDataHist dh (name, "dh", mass, Import (*h));

  name = h->GetName ();
  name.ReplaceAll ("h_", "rgpdf_bwpl");

  RooGenericPdf* bwpl = new RooGenericPdf (name, "Breit-Wigner plus Power Law bkgd", "(2 * sqrt(2) * mz * width * sqrt (mz^2*(mz^2+width^2)) / (pi * sqrt(mz^2+sqrt(mz^2*(mz^2+width^2))))) / ((m^2 - mz^2)^2 + mz^2*width^2) + A * m^alpha", RooArgSet(mass, mz, width, ampl, alpha));
  bwpl->fitTo (dh, Minos (true));

  fit_mz[0] = mz.getVal ();
  fit_mz[1] = mz.getAsymErrorHi ();
  fit_mz[2] = mz.getAsymErrorLo ();

  fit_width[0] = width.getVal ();
  fit_width[1] = width.getAsymErrorHi ();
  fit_width[2] = width.getAsymErrorLo ();

  fit_ampl[0] = ampl.getVal ();
  fit_ampl[1] = ampl.getAsymErrorHi ();
  fit_ampl[2] = ampl.getAsymErrorLo ();

  fit_alpha[0] = alpha.getVal ();
  fit_alpha[1] = alpha.getAsymErrorHi ();
  fit_alpha[2] = alpha.getAsymErrorLo ();

  TF1* f =  bwpl->asTF (RooArgList (mass), RooArgList (mz, width, ampl, alpha), RooArgList (mass));

  name = h->GetName ();
  name.ReplaceAll ("h_", "f_");
  f->SetName (name);

  return f;
}


double GetRatio (double* x, double* p) {
  if ((int)p[0] < 2)
    return f_z_mass[(int)p[0]][1]->Eval (x[0]) / f_z_mass[(int)p[0]][0]->Eval (x[0]);
  else
    return f_z_mass_tot[1]->Eval (x[0]) / f_z_mass_tot[0]->Eval (x[0]);
}


void PlotZBosons () {

  const Color_t colors[] = {(Color_t) TColor::GetColor (0, 0, 153), (Color_t) TColor::GetColor (255, 102, 0)};

  TFile* inFile = nullptr;

  TH1D* h_z_mass[2][2];
  TH1D* h_z_y[2][2];
  TH1D* h_z_eta[2][2];

  TH1D* h_z_mass_tot[2];
  TH1D* h_z_y_tot[2];
  TH1D* h_z_eta_tot[2];

  TH1D* h_z_mass_ratio[2];
  TH1D* h_z_mass_tot_ratio;
  TH1D* h_z_y_ratio;
  TH1D* h_z_eta_ratio;

  //RooGenericPdf* f_z_mass[2][2];
  //RooGenericPdf* f_z_mass_tot[2];

  float* f_mz[2][2];
  float* f_width[2][2];
  float* f_ampl[2][2];
  float* f_alpha[2][2];

  float* f_mz_tot[2];
  float* f_width_tot[2];
  float* f_ampl_tot[2];
  float* f_alpha_tot[2];

  for (short iYear : {0, 1}) {
    for (short iReg : {0, 1}) {
      f_mz[iReg][iYear]     = new float[3];
      f_width[iReg][iYear]  = new float[3];
      f_ampl[iReg][iYear]   = new float[3];
      f_alpha[iReg][iYear]  = new float[3];
    }
    f_mz_tot[iYear]     = new float[3];
    f_width_tot[iYear]  = new float[3];
    f_ampl_tot[iYear]   = new float[3];
    f_alpha_tot[iYear]  = new float[3];
  }


  const int nmBins = 42;
  double* mBins = new double[nmBins+1];
  for (int i = 0; i <= nmBins; i++) {
    if (i < 4)
      mBins[i] = 40 + 5*i;
    else if (i < 34)
      mBins[i] = 60 + 2*(i-4);
    else
      mBins[i] = 120 + 5*(i-34);
  }


  {
    inFile = new TFile (Form ("%s/ZBosonReco/Nominal/data15_5TeV.root", rootPath.Data ()), "read");

    for (short iReg : {0, 1}) {
      h_z_mass[iReg][0] = (TH1D*) inFile->Get (Form ("h_z_mass_iReg%i", iReg))->Clone (Form ("h_z_mass_iReg%i_data15", iReg));
      h_z_y[iReg][0]    = (TH1D*) inFile->Get (Form ("h_z_y_iReg%i", iReg))->Clone (Form ("h_z_y_iReg%i_data15", iReg));
      h_z_eta[iReg][0]  = (TH1D*) inFile->Get (Form ("h_z_eta_iReg%i", iReg))->Clone (Form ("h_z_eta_iReg%i_data15", iReg));

      h_z_mass[iReg][0]->Rebin (2);
    }
  }

  {
    inFile = new TFile (Form ("%s/ZBosonReco/Nominal/data17_5TeV.root", rootPath.Data ()), "read");

    for (short iReg : {0, 1}) {
      h_z_mass[iReg][1] = (TH1D*) inFile->Get (Form ("h_z_mass_iReg%i", iReg))->Clone (Form ("h_z_mass_iReg%i_data17", iReg));
      h_z_y[iReg][1]    = (TH1D*) inFile->Get (Form ("h_z_y_iReg%i", iReg))->Clone (Form ("h_z_y_iReg%i_data17", iReg));
      h_z_eta[iReg][1]  = (TH1D*) inFile->Get (Form ("h_z_eta_iReg%i", iReg))->Clone (Form ("h_z_eta_iReg%i_data17", iReg));

      h_z_mass[iReg][1]->Rebin (2);
    }
  }


  h_z_mass_tot[0] = (TH1D*) h_z_mass[0][0]->Clone ("h_z_mass_tot_data15");
  h_z_mass_tot[0]->Add (h_z_mass[1][0]);
  h_z_mass_tot[1] = (TH1D*) h_z_mass[0][1]->Clone ("h_z_mass_tot_data17");
  h_z_mass_tot[1]->Add (h_z_mass[1][1]);
  h_z_y_tot[0] = (TH1D*) h_z_y[0][0]->Clone ("h_z_y_tot_data15");
  h_z_y_tot[0]->Add (h_z_y[1][0]);
  h_z_y_tot[1] = (TH1D*) h_z_y[0][1]->Clone ("h_z_y_tot_data17");
  h_z_y_tot[1]->Add (h_z_y[1][1]);
  h_z_eta_tot[0] = (TH1D*) h_z_eta[0][0]->Clone ("h_z_eta_tot_data15");
  h_z_eta_tot[0]->Add (h_z_eta[1][0]);
  h_z_eta_tot[1] = (TH1D*) h_z_eta[0][1]->Clone ("h_z_eta_tot_data17");
  h_z_eta_tot[1]->Add (h_z_eta[1][1]);


  for (short iYear : {0, 1}) {
    for (short iReg : {0, 1}) {
      f_z_mass[iReg][iYear] = DoRBWFit (h_z_mass[iReg][iYear], f_mz[iReg][iYear], f_width[iReg][iYear], f_ampl[iReg][iYear], f_alpha[iReg][iYear]);
    }
    f_z_mass_tot[iYear] = DoRBWFit (h_z_mass_tot[iYear], f_mz_tot[iYear], f_width_tot[iYear], f_ampl_tot[iYear], f_alpha_tot[iYear]);
  }


  for (short iYear : {0, 1}) {
    for (short iReg : {0, 1}) {
      RebinSomeBins (&h_z_mass[iReg][iYear], nmBins, mBins);
      h_z_mass[iReg][iYear]->Scale (1./h_z_mass[iReg][iYear]->Integral (), "width");
      h_z_y[iReg][iYear]->Scale (1./h_z_y[iReg][iYear]->Integral (), "width");
      h_z_eta[iReg][iYear]->Scale (1./h_z_eta[iReg][iYear]->Integral (), "width");
    }
    RebinSomeBins (&h_z_mass_tot[iYear], nmBins, mBins);
    h_z_mass_tot[iYear]->Scale (1./h_z_mass_tot[iYear]->Integral (), "width");
    h_z_y_tot[iYear]->Scale (1./h_z_y_tot[iYear]->Integral (), "width");
    h_z_eta_tot[iYear]->Scale (1./h_z_eta_tot[iYear]->Integral (), "width");
  }


  {
    for (short iReg : {0, 1}) {
      h_z_mass_ratio[iReg] = (TH1D*) h_z_mass[iReg][1]->Clone (Form ("h_z_mass_ratio_iReg%i", iReg));
      h_z_mass_ratio[iReg]->Divide (h_z_mass[iReg][0]);
    }
    h_z_mass_tot_ratio = (TH1D*) h_z_mass_tot[1]->Clone ("h_z_mass_tot_ratio");
    h_z_mass_tot_ratio->Divide (h_z_mass_tot[0]);
    h_z_y_ratio = (TH1D*) h_z_y_tot[1]->Clone ("h_z_y_ratio");
    h_z_y_ratio->Divide (h_z_y_tot[0]);
    h_z_eta_ratio = (TH1D*) h_z_eta_tot[1]->Clone ("h_z_eta_ratio");
    h_z_eta_ratio->Divide (h_z_eta_tot[0]);
  }


  for (short iReg : {0, 1}) {

    const char* canvasName = Form ("c_z_mass_iReg%i", iReg);
    TCanvas* c = new TCanvas (canvasName, "", 800, 1000);

    const double fuPad = 400./1000.;
    const double fdPad = 335./1000.;
    const double fcPad = 1-fuPad-fdPad;
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0, 1-fuPad, 1, 1.0);
    TPad* cPad = new TPad (Form ("%s_cPad", canvasName), "", 0, fdPad, 1, 1-fuPad);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0, 0.0, 1, fdPad);

    uPad->SetTopMargin (0.04);
    uPad->SetBottomMargin (0.03);
    cPad->SetTopMargin (0.03);
    cPad->SetBottomMargin (0.03);
    dPad->SetTopMargin (0.03);
    dPad->SetBottomMargin (0.25);

    uPad->SetLeftMargin (0.12);
    cPad->SetLeftMargin (0.12);
    dPad->SetLeftMargin (0.12);
    uPad->SetRightMargin (0.03);
    cPad->SetRightMargin (0.03);
    dPad->SetRightMargin (0.03);

    uPad->Draw ();
    cPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr;
    TGAE* g = nullptr;

    uPad->cd ();
    uPad->SetLogy ();

    float ymin = 5e-5;
    float ymax = 1e0;

    h = new TH1D ("htemp", ";m_{ee} [GeV];Normalized pairs", 1, 40, 160); //66, 116);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.028/fuPad);
    h->GetYaxis ()->SetLabelSize (0.028/fuPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fuPad);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    for (short iYear : {0, 1}) {
      myDraw (h_z_mass[iReg][iYear], colors[iYear], kFullCircle, 1.2);
      myDraw (f_z_mass[iReg][iYear], colors[iYear], 1, 2);
    }

    myText (0.20, 0.85, kBlack, "#bf{#it{ATLAS}} Internal", 0.022/fuPad);
    myText (0.20, 0.78, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.022/fuPad);
    myText (0.60, 0.85, kBlack, Form ("LHLoose, #it{p}_{T}^{e} > 16 GeV, |#it{y}_{ee}| %s 1", iReg == 0 ? "<" : ">"), 0.022/fuPad);
    myText (0.60, 0.78, kBlack, "Breit-Wigner + Power Law", 0.022/fuPad);
    myLineText2 (0.24, 0.15, colors[0], kFullCircle, Form ("2015 data, m_{Z}^{fit} = %.3f #pm %.3f", f_mz[iReg][0][0], f_mz[iReg][0][1]), 1.2, 0.018/fuPad, true);
    myLineText2 (0.24, 0.08, colors[1], kFullCircle, Form ("2017 data, m_{Z}^{fit} = %.3f #pm %.3f", f_mz[iReg][1][0], f_mz[iReg][1][1]), 1.2, 0.018/fuPad, true);


    cPad->cd ();

    ymin = 0.65;
    ymax = 1.35;

    h = new TH1D ("htemp", ";m_{ee} [GeV];Data / Fit", 1, 40, 160); //66, 116);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.028/fcPad);
    h->GetYaxis ()->SetLabelSize (0.028/fcPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fcPad);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    for (short iYear : {0, 1}) {
      TH1D* h = (TH1D*) h_z_mass[iReg][iYear]->Clone ("h");
      h->Divide (f_z_mass[iReg][iYear]);
      myDraw (h, colors[iYear], kFullCircle, 1.2);
      SaferDelete (&h);
    }


    dPad->cd ();

    ymin = 0.0;
    ymax = 2.0;

    h = new TH1D ("htemp", Form (";m_{ee} [GeV];#color[%i]{2017} / #color[%i]{2015}", colors[1], colors[0]), 1, 40, 160); //66, 116);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0.028/fdPad);
    h->GetXaxis ()->SetLabelSize (0.028/fdPad);
    h->GetXaxis ()->SetLabelOffset (2.0*h->GetXaxis ()->GetLabelOffset ());
    h->GetYaxis ()->SetTitleSize (0.028/fdPad);
    h->GetYaxis ()->SetLabelSize (0.028/fdPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fdPad);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_z_mass_ratio[iReg], kBlack, kFullCircle, 1.2);

    TF1* f_z_mass_ratio = new TF1 (Form ("f_z_mass_ratio_iReg%i", iReg), GetRatio, 40, 160, 1); //66, 116, 1);
    f_z_mass_ratio->SetParameter (0, iReg);
    myDraw (f_z_mass_ratio, kBlack, 1, 2);

    c->SaveAs (Form ("Plots/Zmass_iReg%i.pdf", iReg));
  }


  {

    const char* canvasName = "c_z_mass_tot";
    TCanvas* c = new TCanvas (canvasName, "", 800, 1000);

    const double fuPad = 400./1000.;
    const double fdPad = 335./1000.;
    const double fcPad = 1-fuPad-fdPad;
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0, 1-fuPad, 1, 1.0);
    TPad* cPad = new TPad (Form ("%s_cPad", canvasName), "", 0, fdPad, 1, 1-fuPad);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0, 0.0, 1, fdPad);

    uPad->SetTopMargin (0.04);
    uPad->SetBottomMargin (0.03);
    cPad->SetTopMargin (0.03);
    cPad->SetBottomMargin (0.03);
    dPad->SetTopMargin (0.03);
    dPad->SetBottomMargin (0.25);

    uPad->SetLeftMargin (0.12);
    cPad->SetLeftMargin (0.12);
    dPad->SetLeftMargin (0.12);
    uPad->SetRightMargin (0.03);
    cPad->SetRightMargin (0.03);
    dPad->SetRightMargin (0.03);

    uPad->Draw ();
    cPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr;
    TGAE* g = nullptr;

    uPad->cd ();
    uPad->SetLogy ();

    float ymin = 5e-5;
    float ymax = 1e0;
    h = new TH1D ("htemp", ";m_{ee} [GeV];Normalized pairs", 1, 40, 160);//66, 116);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.028/fuPad);
    h->GetYaxis ()->SetLabelSize (0.028/fuPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fuPad);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    for (short iYear : {0, 1}) {
      myDraw (h_z_mass_tot[iYear], colors[iYear], kFullCircle, 1.2);
      myDraw (f_z_mass_tot[iYear], colors[iYear], 1, 2);
    }

    myText (0.20, 0.85, kBlack, "#bf{#it{ATLAS}} Internal", 0.022/fuPad);
    myText (0.20, 0.78, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.022/fuPad);
    myText (0.60, 0.85, kBlack, "LHLoose, #it{p}_{T}^{e} > 16 GeV", 0.022/fuPad);
    myText (0.60, 0.78, kBlack, "Breit-Wigner + Power Law", 0.022/fuPad);
    myLineText2 (0.24, 0.15, colors[0], kFullCircle, Form ("2015 data, m_{Z}^{fit} = %.3f #pm %.3f", f_mz_tot[0][0], f_mz_tot[0][1]), 1.2, 0.018/fuPad, true);
    myLineText2 (0.24, 0.08, colors[1], kFullCircle, Form ("2017 data, m_{Z}^{fit} = %.3f #pm %.3f", f_mz_tot[1][0], f_mz_tot[1][1]), 1.2, 0.018/fuPad, true);


    cPad->cd ();

    ymin = 0.65;
    ymax = 1.35;

    h = new TH1D ("htemp", ";m_{ee} [GeV];Data / Fit", 1, 40, 160);//66, 116);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.028/fcPad);
    h->GetYaxis ()->SetLabelSize (0.028/fcPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fcPad);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    for (short iYear : {0, 1}) {
      TH1D* h = (TH1D*) h_z_mass_tot[iYear]->Clone ("h");
      h->Divide (f_z_mass_tot[iYear]);
      myDraw (h, colors[iYear], kFullCircle, 1.2);
      SaferDelete (&h);
    }


    dPad->cd ();

    ymin = 0.0;
    ymax = 2.0;

    h = new TH1D ("htemp", Form (";m_{ee} [GeV];#color[%i]{2017} / #color[%i]{2015}", colors[1], colors[0]), 1, 40, 160);//66, 116);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0.028/fdPad);
    h->GetXaxis ()->SetLabelSize (0.028/fdPad);
    h->GetXaxis ()->SetLabelOffset (2.0*h->GetXaxis ()->GetLabelOffset ());
    h->GetYaxis ()->SetTitleSize (0.028/fdPad);
    h->GetYaxis ()->SetLabelSize (0.028/fdPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fdPad);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_z_mass_tot_ratio, kBlack, kFullCircle, 1.2);

    TF1* f_z_mass_tot_ratio = new TF1 ("f_z_mass_tot_ratio", GetRatio, 40, 160, 1);//66, 116, 1);
    f_z_mass_tot_ratio->SetParameter (0, 2);
    myDraw (f_z_mass_tot_ratio, kBlack, 1, 2);

    c->SaveAs ("Plots/Zmass_tot.pdf");
  }


  {

    const char* canvasName = "c_z_y";
    TCanvas* c = new TCanvas (canvasName, "", 800, 1000);

    const double fPad = 600./1000.;
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0, 1-fPad, 1, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0, 0.0, 1, 1-fPad);

    uPad->SetTopMargin (0.04);
    uPad->SetBottomMargin (0.03);
    dPad->SetTopMargin (0.03);
    dPad->SetBottomMargin (0.25);

    uPad->SetLeftMargin (0.12);
    dPad->SetLeftMargin (0.12);
    uPad->SetRightMargin (0.03);
    dPad->SetRightMargin (0.03);

    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr;
    TGAE* g = nullptr;

    uPad->cd ();
    uPad->SetLogy ();

    float ymin = 5e-3;
    float ymax = 8e-1;
    h = new TH1D ("htemp", ";y_{ee};Normalized pairs", 1, -3, 3);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.028/fPad);
    h->GetYaxis ()->SetLabelSize (0.028/fPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fPad);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    for (short iYear : {0, 1}) {
      myDraw (h_z_y_tot[iYear], colors[iYear], kFullCircle, 1.2);
    }

    myText (0.62, 0.89, kBlack, "#bf{#it{ATLAS}} Internal", 0.024/fPad);
    myText (0.62, 0.84, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.024/fPad);
    myText (0.62, 0.79, kBlack, "LHLoose, #it{p}_{T}^{e} > 16 GeV", 0.024/fPad);
    myLineText2 (0.24, 0.13, colors[0], kFullCircle, "2015 data", 1.4, 0.022/fPad, true);
    myLineText2 (0.24, 0.08, colors[1], kFullCircle, "2017 data", 1.4, 0.022/fPad, true);


    dPad->cd ();

    ymin = 0.0;
    ymax = 2.0;

    h = new TH1D ("htemp", Form (";y_{ee};#color[%i]{2017} / #color[%i]{2015}", colors[1], colors[0]), 1, -3, 3);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0.028/(1-fPad));
    h->GetXaxis ()->SetLabelSize (0.028/(1-fPad));
    h->GetYaxis ()->SetTitleSize (0.028/(1-fPad));
    h->GetYaxis ()->SetLabelSize (0.028/(1-fPad));
    h->GetYaxis ()->SetTitleOffset (2.0*(1-fPad));
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_z_y_ratio, kBlack, kFullCircle, 1.2);

    c->SaveAs ("Plots/Z_rapidity.pdf");
  }



  {

    const char* canvasName = "c_z_eta";
    TCanvas* c = new TCanvas (canvasName, "", 800, 1000);

    const double fPad = 600./1000.;
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0, 1-fPad, 1, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0, 0.0, 1, 1-fPad);

    uPad->SetTopMargin (0.04);
    uPad->SetBottomMargin (0.03);
    dPad->SetTopMargin (0.03);
    dPad->SetBottomMargin (0.25);

    uPad->SetLeftMargin (0.12);
    dPad->SetLeftMargin (0.12);
    uPad->SetRightMargin (0.03);
    dPad->SetRightMargin (0.03);

    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr;
    TGAE* g = nullptr;

    uPad->cd ();
    uPad->SetLogy ();

    float ymin = 5e-3;
    float ymax = 8e-1;
    h = new TH1D ("htemp", ";#eta_{ee};Normalized pairs", 1, -3, 3);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.028/fPad);
    h->GetYaxis ()->SetLabelSize (0.028/fPad);
    h->GetYaxis ()->SetTitleOffset (2.0*fPad);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    for (short iYear : {0, 1}) {
      myDraw (h_z_eta_tot[iYear], colors[iYear], kFullCircle, 1.2);
    }

    myText (0.62, 0.89, kBlack, "#bf{#it{ATLAS}} Internal", 0.024/fPad);
    myText (0.62, 0.84, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.024/fPad);
    myText (0.62, 0.79, kBlack, "LHLoose, #it{p}_{T}^{e} > 16 GeV", 0.024/fPad);
    myLineText2 (0.24, 0.13, colors[0], kFullCircle, "2015 data", 1.4, 0.022/fPad, true);
    myLineText2 (0.24, 0.08, colors[1], kFullCircle, "2017 data", 1.4, 0.022/fPad, true);


    dPad->cd ();

    ymin = 0.0;
    ymax = 2.0;

    h = new TH1D ("htemp", Form (";#eta_{ee};#color[%i]{2017} / #color[%i]{2015}", colors[1], colors[0]), 1, -3, 3);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0.028/(1-fPad));
    h->GetXaxis ()->SetLabelSize (0.028/(1-fPad));
    h->GetYaxis ()->SetTitleSize (0.028/(1-fPad));
    h->GetYaxis ()->SetLabelSize (0.028/(1-fPad));
    h->GetYaxis ()->SetTitleOffset (2.0*(1-fPad));
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_z_eta_ratio, kBlack, kFullCircle, 1.2);

    c->SaveAs ("Plots/Z_pseudorapidity.pdf");
  }

}
