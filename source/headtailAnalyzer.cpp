#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <getopt.h>

#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TF1.h"
#include "TText.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-s] [evtNum]" << std::endl;
        return 1;
    }

    int opt;
    int flag_stat = 0;

    while ((opt = getopt(argc, argv, "s")) != -1) {
        switch (opt) {
            case 's':
                flag_stat = 1;
                break;
            default:
                break;
        }
    }

    std::string evtNum = argv[argc-1];

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );

    gStyle->SetOptStat(0);
    TFile *file = new TFile("./cvvar.root", "READ");
    if (!file->IsOpen()) {
        std::cerr << "Error: rootfile not found." << std::endl;
        return 1;
    }
    TTree *tree = (TTree*)file->Get("cvvar_tree");

    //#########################################//
    // multi event analysis
    //#########################################// 

    // TODO GetEntryでまとめて読み込んだ方がずっと早いのであとで修正する
    if (flag_stat) {
        TCanvas *cvs = new TCanvas("cvs", "cvs", 400, 400);
        TH1D *hist_x = new TH1D("hist_x", "", 768, -15., 15.);
        tree->Draw("x_xz>>hist_x", Form("evtNum==%s", evtNum.c_str()), "bar");
        TH1D *hist_y = new TH1D("hist_y", "", 768, -15., 15.);
        tree->Draw("y_yz>>hist_y", Form("evtNum==%s", evtNum.c_str()), "bar");
        TF1 *fit_x = new TF1("fit_x", "pol1", -15., 15.);
        hist_x->Fit("fit_x", "R q");
        TF1 *fit_y = new TF1("fit_y", "pol1", -15., 15.);
        hist_y->Fit("fit_y", "R q");
        std::ofstream ofs;
        ofs.open("./slopeList.dat", std::ios::app);
        ofs << evtNum << "\t" << fit_x->GetParameter(1) << "\t" << fit_x->GetParError(1) << "\t" << fit_y->GetParameter(1) << "\t" << fit_y->GetParError(1) << std::endl;
        ofs.close();

        file->Close();
        delete file;

        return 0;
    }

    //#########################################//
    // single event analysis
    //#########################################// 

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 400);
    c1->Draw();
    c1->Divide(2, 1);

    // x-z
    c1->cd(1);
    TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 0.65);
    pad1->SetTopMargin(0);
    pad1->SetGrid();
    pad1->Draw();
    TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.65, 1.0, 0.9);
    pad2->Draw();
    TPad *pad5 = new TPad("pad5", "pad5", 0.0, 0.9, 1.0, 1.0);
    pad5->Draw();
    pad1->cd();
    TH2D *hist_xz = new TH2D("hist_xz", "", 768, -15., 15., 1000, -41., 0.);
    tree->Draw("z_xz:x_xz>>hist_xz", Form("evtNum==%s", evtNum.c_str()), "colz");
    hist_xz->GetXaxis()->SetTitle("x (cm)");
    hist_xz->GetYaxis()->SetTitle("z (cm)");
    pad2->cd();
    TH1D *hist_x = new TH1D("hist_x", "", 768, -15., 15.);
    hist_x->SetFillColorAlpha(kBlue, 0.1);
    tree->Draw("x_xz>>hist_x", Form("evtNum==%s", evtNum.c_str()), "bar");
    pad5->cd();
    TText *text = new TText(0.5, 0.5, Form("evtNum: %s x-z", evtNum.c_str()));
    text->SetTextAlign(22);
    text->SetTextSize(0.5);
    text->Draw();
 
    // y-z
    c1->cd(2);
    TPad *pad3 = new TPad("pad3", "pad3", 0.0, 0.0, 1.0, 0.65);
    pad3->SetTopMargin(0);
    pad3->SetGrid();
    pad3->Draw();
    TPad *pad4 = new TPad("pad4", "pad4", 0.0, 0.65, 1.0, 0.9);
    pad4->Draw();
    TPad *pad6 = new TPad("pad6", "pad6", 0.0, 0.9, 1.0, 1.0);
    pad6->Draw();
    pad3->cd();
    TH2D *hist_yz = new TH2D("hist_yz", "", 1000, -15., 15., 768, -41., 0.);
    tree->Draw("z_yz:y_yz>>hist_yz", Form("evtNum==%s", evtNum.c_str()), "colz");
    hist_yz->GetXaxis()->SetTitle("y (cm)");
    hist_yz->GetYaxis()->SetTitle("z (cm)");
    pad4->cd();
    TH1D *hist_y = new TH1D("hist_y", "", 768, -15., 15.);
    hist_y->SetFillColorAlpha(kBlue, 0.1);
    tree->Draw("y_yz>>hist_y", Form("evtNum==%s", evtNum.c_str()), "bar");
    pad6->cd();
    TText *text2 = new TText(0.5, 0.5, Form("evtNum: %s y-z", evtNum.c_str()));
    text2->SetTextAlign(22);
    text2->SetTextSize(0.5);
    text2->Draw();

    // x-z
    pad2->cd();
    TF1 *fit_x = new TF1("fit_x", "pol1", -15., 15.);
    hist_x->Fit("fit_x", "R");
    fit_x->SetLineColor(kRed);
    fit_x->Draw("same");
    TText *slope_x = new TText(-14, 0, Form("slope: %.2f +- %.2f", fit_x->GetParameter(1), fit_x->GetParError(1)));
    slope_x->SetTextSize(0.2);
    slope_x->Draw();
    // y-z
    pad4->cd();
    TF1 *fit_y = new TF1("fit_y", "pol1", -15., 15.);
    hist_y->Fit("fit_y", "R");
    fit_y->SetLineColor(kRed);
    fit_y->Draw("same");
    TText *slope_y = new TText(-14, 0, Form("slope: %.2f +- %.2f", fit_y->GetParameter(1), fit_y->GetParError(1)));
    slope_y->SetTextSize(0.2);
    slope_y->Draw();

    c1->SaveAs(Form("headtail_%s.png", evtNum.c_str()));

    file->Close();
    delete file;

    return 0;
}