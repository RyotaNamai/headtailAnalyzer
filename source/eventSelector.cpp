#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

int main(int argc, char** argv) {
    gStyle->SetOptStat(0);
    // open root file
    TFile *file = new TFile("./cvvar.root", "READ");
    if (!file->IsOpen()) {
        std::cerr << ">>> Error: rootfile not found." << std::endl;
        return 1;
    } else {
        std::cout << ">>> rootfile opened." << std::endl;
    }
    TTree *tree = (TTree*)file->Get("cvvar_tree");

    // read config
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("../config/cut.json", pt);
    if (pt.empty()) {
        std::cerr << ">>> Error: config file not found." << std::endl;
        return 1;
    } else {
        std::cout << ">>> config file opened." << std::endl;
    }
    const bool   bool_unixtime = pt.get<bool>("activate.unixtime");
    const bool   bool_length   = pt.get<bool>("activate.length"  );
    const bool   bool_x        = pt.get<bool>("activate.x"       );
    const bool   bool_y        = pt.get<bool>("activate.y"       );
    const bool   bool_x_ave    = pt.get<bool>("activate.x_ave"   );
    const bool   bool_y_ave    = pt.get<bool>("activate.y_ave"   );
    const bool   bool_ene_low  = pt.get<bool>("activate.ene_low" );
    const bool   bool_ene_high = pt.get<bool>("activate.ene_high");
    const double unixtime_min = pt.get<double>("unixtime_min");
    const double unixtime_max = pt.get<double>("unixtime_max");
    const double length_min   = pt.get<double>("length_min"  );
    const double length_max   = pt.get<double>("length_max"  );
    const double x_min        = pt.get<double>("x_min"       );
    const double x_max        = pt.get<double>("x_max"       );
    const double y_min        = pt.get<double>("y_min"       );
    const double y_max        = pt.get<double>("y_max"       );
    const double x_ave_min    = pt.get<double>("x_ave_min"   );
    const double x_ave_max    = pt.get<double>("x_ave_max"   );
    const double y_ave_min    = pt.get<double>("y_ave_min"   );
    const double y_ave_max    = pt.get<double>("y_ave_max"   );
    const double ene_low_min  = pt.get<double>("ene_low_min" );
    const double ene_low_max  = pt.get<double>("ene_low_max" );
    const double ene_high_min = pt.get<double>("ene_high_min");
    const double ene_high_max = pt.get<double>("ene_high_max");

    int evtNum;
    double length;
    double x_xz_min, x_xz_max;
    double y_yz_min, y_yz_max;
    double x_xz_ave, y_yz_ave;
    double ene_low, ene_high;
    int unixtime;

    tree->SetBranchAddress("evtNum"  , &evtNum  );
    tree->SetBranchAddress("length"  , &length  );
    tree->SetBranchAddress("x_xz_min", &x_xz_min);
    tree->SetBranchAddress("x_xz_max", &x_xz_max);
    tree->SetBranchAddress("y_yz_min", &y_yz_min);
    tree->SetBranchAddress("y_yz_max", &y_yz_max);
    tree->SetBranchAddress("x_xz_ave", &x_xz_ave);
    tree->SetBranchAddress("y_yz_ave", &y_yz_ave);
    tree->SetBranchAddress("ene_low" , &ene_low );
    tree->SetBranchAddress("ene_high", &ene_high);
    tree->SetBranchAddress("unixtime", &unixtime);

    const int nEntries = tree->GetEntries();

    // dump selected event number
    std::ofstream ofs("selectedEvent.dat");
    std::cout << ">>> electedEvent.dat created." << std::endl;
    ofs << "# evtNum" << std::endl;

    std::cout << ">>> Start event selection." << std::endl;
    for (int i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        if (bool_length   == true && (length   < length_min   || length   > length_max  )) continue;
        if (bool_x        == true && (x_xz_min < x_min        || x_xz_max > x_max       )) continue;
        if (bool_y        == true && (y_yz_min < y_min        || y_yz_max > y_max       )) continue;
        if (bool_x_ave    == true && (x_xz_ave < x_ave_min    || x_xz_ave > x_ave_max   )) continue;
        if (bool_y_ave    == true && (y_yz_ave < y_ave_min    || y_yz_ave > y_ave_max   )) continue;
        if (bool_ene_low  == true && (ene_low  < ene_low_min  || ene_low  > ene_low_max )) continue;
        if (bool_ene_high == true && (ene_high < ene_high_min || ene_high > ene_high_max)) continue;
        if (bool_unixtime == true && (unixtime < unixtime_min || unixtime > unixtime_max)) continue;
        ofs << evtNum << std::endl;
        std::cout << "Scanning : " << i << " / " << nEntries << "\t[" << Form("%.2f %%]", (double)i / (double)nEntries * 100) << "\r" << std::flush;
    }
    std::cout << ">>> Event selection finished." << std::endl;
    file->Close();

    return 0;
}