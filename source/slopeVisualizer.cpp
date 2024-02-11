#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "TStyle.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"

int main(int argc, char* argv[]) {
    std::string filename = "./slopeList.dat";
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Error: file " << filename << " not found." << std::endl;
        return 1;
    }
    std::vector<double> v_slope_x, v_slope_x_err, v_slope_y, v_slope_y_err;
    int evtNum;
    double  slope_x, slope_x_err, slope_y, slope_y_err;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line[0] == '#') continue;
        std::stringstream ss(line);
        ss >> evtNum >> slope_x >> slope_x_err >> slope_y >> slope_y_err;
        v_slope_x.push_back(slope_x);
        v_slope_x_err.push_back(slope_x_err);
        v_slope_y.push_back(slope_y);
        v_slope_y_err.push_back(slope_y_err);
    }

    // for (int i = 0; i < v_slope_x.size(); i++) {
    //     std::cout << v_slope_x.at(i) << "\t" << v_slope_x_err.at(i) << "\t" << v_slope_y.at(i) << "\t" << v_slope_y_err.at(i) << std::endl;
    // }

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);

    TCanvas *cvs = new TCanvas("cvs", "cvs", 800, 400);
    cvs->Divide(2, 1);
    TH1D *h_slope_x = new TH1D("h_slope_x", "x-y slope", 100, -50, 50);
    TH1D *h_slope_y = new TH1D("h_slope_y", "y-z slope", 100, -50, 50);
    for (int i = 0; i < v_slope_x.size(); i++) {
        h_slope_x->Fill(v_slope_x.at(i));
        h_slope_y->Fill(v_slope_y.at(i));
    }
    cvs->cd(1);
    h_slope_x->SetFillColorAlpha(kBlue, 0.5);
    h_slope_x->GetXaxis()->SetTitle("slope");
    h_slope_x->GetYaxis()->SetTitle("entries");
    h_slope_x->Draw();
    cvs->cd(2);
    h_slope_y->SetFillColorAlpha(kBlue, 0.5);
    h_slope_y->GetXaxis()->SetTitle("slope");
    h_slope_x->GetYaxis()->SetTitle("entries");
    h_slope_y->Draw();

    // fit
    cvs->cd(1);
    TF1 *fit_x = new TF1("fit_x", "gaus", -50, 50);
    h_slope_x->Fit("fit_x", "R q");
    fit_x->Draw("same");
    cvs->cd(2);
    TF1 *fit_y = new TF1("fit_y", "gaus", -50, 50);
    h_slope_y->Fit("fit_y", "R q");
    fit_y->Draw("same");

    cvs->SaveAs("slopeHist.png");

    return 0;
}