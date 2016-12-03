#include <string>
#include <vector>

//_____________________ADDED BY EMMA___________________________________________________________________________
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <math.h>
#include <memory>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DQMServices/Core/interface/QReport.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/DQMDefinitions.h"

#include "TMath.h" 
Double_t pi=TMath::Pi(); 
//_____________________________________________________________________________________________________________


#include "DQM/L1TMonitor/interface/L1TStage2EMTF.h"


L1TStage2EMTF::L1TStage2EMTF(const edm::ParameterSet& ps)
    : daqToken(consumes<l1t::EMTFDaqOutCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      hitToken(consumes<l1t::EMTFHitCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      trackToken(consumes<l1t::EMTFTrackCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      muonToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TStage2EMTF::~L1TStage2EMTF() {}

void L1TStage2EMTF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}

void L1TStage2EMTF::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

void L1TStage2EMTF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {
  ibooker.setCurrentFolder(monitorDir);

  // DAQ Output Monitor Elements
  emtfErrors = ibooker.book1D("emtfErrors", "EMTF Errors", 6, 0, 6);
  emtfErrors->setAxisTitle("Error Type (Corruptions Not Implemented)", 1);
  emtfErrors->setAxisTitle("Number of Errors", 2);
  emtfErrors->setBinLabel(1, "Corruptions", 1);
  emtfErrors->setBinLabel(2, "Synch. Err.", 1);
  emtfErrors->setBinLabel(3, "Synch. Mod.", 1);
  emtfErrors->setBinLabel(4, "BX Mismatch", 1);
  emtfErrors->setBinLabel(5, "Time Misalign.", 1);
  emtfErrors->setBinLabel(6, "FMM != Ready", 1);

  // Hit (LCT) Monitor Elements
  int n_xbins;
  int n_ybins; //added by Emma
  std::string name, label;
/*  std::vector<std::string> suffix_name = {"42", "41", "32", "31", "22", "21", "13", "12", "11"};
  std::vector<std::string> suffix_label = {"4/2", "4/1", "3/2", "3/1", " 2/2", "2/1", "1/3", "1/2", "1/1"}; */ //removed by Emma 
  std::vector<std::string> suffix_name = {"42", "41", "32", "31", "22", "21", "13", "12", "11b", "11a"};
  std::vector<std::string> suffix_label = {"4/2", "4/1", "3/2", "3/1", " 2/2", "2/1", "1/3", "1/2", "1/1b", "1/1a"}; //added by Emma

  emtfHitBX = ibooker.book2D("emtfHitBX", "EMTF Hit BX", 8, -3, 5, 18, 0, 18);
  emtfHitBX->setAxisTitle("BX", 1);
  for (int xbin = 1, xbin_label = -3; xbin <= 8; ++xbin, ++xbin_label) {
    emtfHitBX->setBinLabel(xbin, std::to_string(xbin_label), 1);
  }
  for (int ybin = 1; ybin <= 9; ++ybin) {
    emtfHitBX->setBinLabel(ybin, "ME-" + suffix_label[ybin - 1], 2);
    emtfHitBX->setBinLabel(19 - ybin, "ME+" + suffix_label[ybin - 1], 2);
  }

/*  for (int hist = 0, i = 0; hist < 18; ++hist, i = hist % 9) {

    if (hist < 9) {
      name = "MENeg" + suffix_name[i];
      label = "ME-" + suffix_label[i];
    } else {
      name = "MEPos" + suffix_name[8 - i];
      label = "ME+" + suffix_label[8 - i];
    }

    if (hist < 6 || hist > 11) {
      n_xbins = (i % 2) ? 18 : 36;
    } else {
      n_xbins = 36;
    } */ //removed by Emma

  for (int hist = 0, i = 0; hist < 20; ++hist, i = hist % 10) { //added by Emma

    int yMin = 0;
    int yMax = 224;
    int n_xbinsQT = 36;

    if (hist < 10) {
      name = "MENeg" + suffix_name[i];
      label = "ME-" + suffix_label[i];
    } else {
      name = "MEPos" + suffix_name[9 - i];
      label = "ME+" + suffix_label[9 - i];
    }

    if (hist < 6) { 
      n_xbins = (i % 2) ? 18 : 36;
    } else if (hist > 13) {
      n_xbins = ((i-1) % 2) ? 18 : 36;
    } else {
      n_xbins = 36;
    } 
/*
    if (hist == 6 || hist == 9 || hist == 10 || hist == 13) {
      yMin = 0;
      yMax = 128; //8 bins
    } else if (hist == 8 || hist == 11) {
      yMin = 128;
      yMax = 224; //6 bins 
    } else {
      yMin = 0;
      yMax = 160; //10 bins
    }
*/
    n_ybins = (yMax-yMin)/16; //added by Emma

    emtfHitStrip[hist] = ibooker.book1D("emtfHitStrip" + name, "EMTF Halfstrip " + label, 256, 0, 256);
    emtfHitStrip[hist]->setAxisTitle("Cathode Halfstrip, " + label, 1);

    emtfHitWire[hist] = ibooker.book1D("emtfHitWire" + name, "EMTF Wiregroup " + label, 128, 0, 128);
    emtfHitWire[hist]->setAxisTitle("Anode Wiregroup, " + label, 1);

    emtfChamberStrip[hist] = ibooker.book2D("emtfChamberStrip" + name, "EMTF Halfstrip " + label, n_xbins, 1, 1+n_xbins, 256, 0, 256);
    emtfChamberStrip[hist]->setAxisTitle("Chamber, " + label, 1);
    emtfChamberStrip[hist]->setAxisTitle("Cathode Halfstrip", 2);

    emtfChamberWire[hist] = ibooker.book2D("emtfChamberWire" + name, "EMTF Wiregroup " + label, n_xbins, 1, 1+n_xbins, 128, 0, 128);
    emtfChamberWire[hist]->setAxisTitle("Chamber, " + label, 1);
    emtfChamberWire[hist]->setAxisTitle("Anode Wiregroup", 2);

    for (int bin = 1; bin <= n_xbins; ++bin) {
      emtfChamberStrip[hist]->setBinLabel(bin, std::to_string(bin), 1);
      emtfChamberWire[hist]->setBinLabel(bin, std::to_string(bin), 1);
    }

//________________QUALITY TESTER (added by Emma)__CHAMBER STRIP________________________________________________

  ibooker.setCurrentFolder(monitorDir + "/Quality Test Collection"); //book in subfolder
    emtfChamberStrip_QT[hist] = ibooker.book2D("emtfChamberStrip_QT" + name, "EMTF Halfstrip QT " + label, n_xbinsQT, 1, 1+n_xbinsQT, n_ybins, yMin, yMax);
    emtfChamberStrip_QT[hist]->setAxisTitle("Chamber, " + label, 1);
    emtfChamberStrip_QT[hist]->setAxisTitle("Cathode Halfstrip", 2);

    emtfChamberStrip_QT_hot[hist] = ibooker.book2D("emtfChamberStrip_QT_hot" + name, "EMTF Halfstrip QT Hot " + label, n_xbinsQT, 1, 1+n_xbinsQT, n_ybins, yMin, yMax);
    emtfChamberStrip_QT_hot[hist]->setAxisTitle("Chamber, " + label, 1);
    emtfChamberStrip_QT_hot[hist]->setAxisTitle("Cathode Halfstrip", 2);

    emtfChamberStrip_QT_dead[hist] = ibooker.book2D("emtfChamberStrip_QT_dead" + name, "EMTF Halfstrip QT Dead " + label, n_xbinsQT, 1, 1+n_xbinsQT, n_ybins, yMin, yMax);
    emtfChamberStrip_QT_dead[hist]->setAxisTitle("Chamber, " + label, 1);
    emtfChamberStrip_QT_dead[hist]->setAxisTitle("Cathode Halfstrip", 2); // 2D dead histogram

    for (int bin = 1; bin <= n_xbinsQT; ++bin) {
      emtfChamberStrip_QT[hist]->setBinLabel(bin, std::to_string(bin), 1);
      emtfChamberStrip_QT_hot[hist]->setBinLabel(bin, std::to_string(bin), 1);
      emtfChamberStrip_QT_dead[hist]->setBinLabel(bin, std::to_string(bin), 1);
    } 
  ibooker.setCurrentFolder(monitorDir); //return to main folder
//_____________________________________________________________________________________________________________

  emtfHitOccupancy = ibooker.book2D("emtfHitOccupancy", "EMTF Chamber Occupancy", 54, 1, 55, 10, -5, 5);
  emtfHitOccupancy->setAxisTitle("Sector (CSCID 1-9 Unlabelled)", 1);
  for (int bin = 1; bin <= 46; bin += 9) {
    emtfHitOccupancy->setBinLabel(bin, std::to_string(bin % 8), 1);
  }
  emtfHitOccupancy->setBinLabel(1, "ME-4", 2);
  emtfHitOccupancy->setBinLabel(2, "ME-3", 2);
  emtfHitOccupancy->setBinLabel(3, "ME-2", 2);
  emtfHitOccupancy->setBinLabel(4, "ME-1b", 2);
  emtfHitOccupancy->setBinLabel(5, "ME-1a", 2);
  emtfHitOccupancy->setBinLabel(6, "ME+1a", 2);
  emtfHitOccupancy->setBinLabel(7, "ME+1b", 2);
  emtfHitOccupancy->setBinLabel(8, "ME+2", 2);
  emtfHitOccupancy->setBinLabel(9, "ME+3", 2);
  emtfHitOccupancy->setBinLabel(10, "ME+4", 2);

  // Track Monitor Elements
  emtfnTracks = ibooker.book1D("emtfnTracks", "Number of EMTF Tracks per Event", 11, 0, 11);
  for (int bin = 1; bin <= 10; ++bin) {
    emtfnTracks->setBinLabel(bin, std::to_string(bin - 1), 1);
  }
  emtfnTracks->setBinLabel(11, "Overflow", 1);

  emtfTracknHits = ibooker.book1D("emtfTracknHits", "Number of Hits per EMTF Track", 5, 0, 5);
  for (int bin = 1; bin <= 5; ++bin) {
    emtfTracknHits->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

  emtfTrackBX = ibooker.book2D("emtfTrackBX", "EMTF Track Bunch Crossing", 12, -6, 6, 8, -3, 5);
  emtfTrackBX->setAxisTitle("Sector (Endcap)", 1);
  for (int i = 0; i < 6; ++i) {
    emtfTrackBX->setBinLabel(i + 1, std::to_string(6 - i) + " (-)", 1);
    emtfTrackBX->setBinLabel(12 - i, std::to_string(6 - i) + " (+)", 1);
  }
  emtfTrackBX->setAxisTitle("Track BX", 2);
  for (int bin = 1, i = -3; bin <= 8; ++bin, ++i) {
    emtfTrackBX->setBinLabel(bin, std::to_string(i), 2);
  }

}
//__________________QUALITY TESTER (added by Emma)___TRACK BX__________________________________________________
  ibooker.setCurrentFolder(monitorDir + "/Quality Test Collection"); //book in subfolder

  emtfTrackBX_QT = ibooker.book1D("emtfTrackBX_QT", "EMTF Track BX QT", 12, -6, 6);
  emtfTrackBX_QT->setAxisTitle("Sector (Endcap)", 1);
  for (int i = 0; i < 6; ++i) {
    emtfTrackBX_QT->setBinLabel(i + 1, std::to_string(6 - i) + " (-)", 1);
    emtfTrackBX_QT->setBinLabel(12 - i, std::to_string(6 - i) + " (+)", 1);
  }

  emtfTrackBX_QT_hot = ibooker.book1D("emtfTrackBX_QT_hot", "EMTF Track BX QT Hot", 12, -6, 6);
  emtfTrackBX_QT_hot->setAxisTitle("Sector (Endcap)", 1);
  for (int i = 0; i < 6; ++i) {
    emtfTrackBX_QT_hot->setBinLabel(i + 1, std::to_string(6 - i) + " (-)", 1);
    emtfTrackBX_QT_hot->setBinLabel(12 - i, std::to_string(6 - i) + " (+)", 1);
  }

  emtfTrackBX_QT_dead = ibooker.book1D("emtfTrackBX_QT_dead", "EMTF Track BX QT Dead", 12, -6, 6);
  emtfTrackBX_QT_dead->setAxisTitle("Sector (Endcap)", 1);
  for (int i = 0; i < 6; ++i) {
    emtfTrackBX_QT_dead->setBinLabel(i + 1, std::to_string(6 - i) + " (-)", 1);
    emtfTrackBX_QT_dead->setBinLabel(12 - i, std::to_string(6 - i) + " (+)", 1);
  }

  ibooker.setCurrentFolder(monitorDir); //return to main folder
//_____________________________________________________________________________________________________________

  emtfTrackPt = ibooker.book1D("emtfTrackPt", "EMTF Track p_{T}", 256, 1, 257);
  emtfTrackPt->setAxisTitle("Track p_{T} [GeV]", 1);

  emtfTrackEta = ibooker.book1D("emtfTrackEta", "EMTF Track #eta", 100, -2.5, 2.5);
  emtfTrackEta->setAxisTitle("Track #eta", 1);

  emtfTrackPhi = ibooker.book1D("emtfTrackPhi", "EMTF Track #phi", 126, -3.15, 3.15);
  emtfTrackPhi->setAxisTitle("Track #phi", 1);

  emtfTrackPhiHighQuality = ibooker.book1D("emtfTrackPhiHighQuality", "EMTF High Quality #phi", 126, -3.15, 3.15);
  emtfTrackPhiHighQuality->setAxisTitle("Track #phi (High Quality)", 1);

//__________________QUALITY TESTER (added by Emma)___TRACK PHI_________________________________________________
  ibooker.setCurrentFolder(monitorDir + "/Quality Test Collection"); //book in subfolder

  Double_t xMin = -(63.0/64.0)*pi;
  Double_t xMax = (65.0/64.0)*pi;

  emtfTrackPhi_QT = ibooker.book1D("emtfTrackPhi_QT", "EMTF Track #phi QT", 128, xMin, xMax);
  emtfTrackPhi_QT->setAxisTitle("Track #phi", 1);

  emtfTrackPhi_QT_hot = ibooker.book1D("emtfTrackPhi_QT_hot", "EMTF Track #phi QT Hot", 128, xMin, xMax);
  emtfTrackPhi_QT_hot->setAxisTitle("Track #phi", 1);

  emtfTrackPhi_QT_dead = ibooker.book1D("emtfTrackPhi_QT_dead", "EMTF Track #phi QT Dead", 128, xMin, xMax);
  emtfTrackPhi_QT_dead->setAxisTitle("Track #phi", 1);
  ibooker.setCurrentFolder(monitorDir); //return to main folder
//_____________________________________________________________________________________________________________

  emtfTrackOccupancy = ibooker.book2D("emtfTrackOccupancy", "EMTF Track Occupancy", 100, -2.5, 2.5, 126, -3.15, 3.15);
  emtfTrackOccupancy->setAxisTitle("#eta", 1);
  emtfTrackOccupancy->setAxisTitle("#phi", 2);

  emtfTrackMode = ibooker.book1D("emtfTrackMode", "EMTF Track Mode", 16, 0, 16);
  emtfTrackMode->setAxisTitle("Mode", 1);

  emtfTrackQuality = ibooker.book1D("emtfTrackQuality", "EMTF Track Quality", 16, 0, 16);
  emtfTrackQuality->setAxisTitle("Quality", 1);

  emtfTrackQualityVsMode = ibooker.book2D("emtfTrackQualityVsMode", "EMTF Track Quality vs Mode", 16, 0, 16, 16, 0, 16);
  emtfTrackQualityVsMode->setAxisTitle("Mode", 1);
  emtfTrackQualityVsMode->setAxisTitle("Quality", 2);

  for (int bin = 1; bin <= 16; ++bin) {
    emtfTrackMode->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfTrackQuality->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfTrackQualityVsMode->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfTrackQualityVsMode->setBinLabel(bin, std::to_string(bin - 1), 2);
  }

  // Regional Muon Candidate Monitor Elements
  ibooker.setCurrentFolder(monitorDir + "/MuonCand");

  emtfMuonBX = ibooker.book1D("emtfMuonBX", "EMTF Muon Cand BX", 7, -3, 4);
  emtfMuonBX->setAxisTitle("BX", 1);
  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    emtfMuonBX->setBinLabel(bin, std::to_string(bin_label), 1);
  }

  emtfMuonhwPt = ibooker.book1D("emtfMuonhwPt", "EMTF Muon Cand p_{T}", 512, 0, 512);
  emtfMuonhwPt->setAxisTitle("Hardware p_{T}", 1);

  emtfMuonhwEta = ibooker.book1D("emtfMuonhwEta", "EMTF Muon Cand #eta", 460, -230, 230);
  emtfMuonhwEta->setAxisTitle("Hardware #eta", 1);

  emtfMuonhwPhi = ibooker.book1D("emtfMuonhwPhi", "EMTF Muon Cand #phi", 125, -20, 105);
  emtfMuonhwPhi->setAxisTitle("Hardware #phi", 1);

  emtfMuonhwQual = ibooker.book1D("emtfMuonhwQual", "EMTF Muon Cand Quality", 16, 0, 16);
  emtfMuonhwQual->setAxisTitle("Quality", 1);
  for (int bin = 1; bin <= 16; ++bin) {
    emtfMuonhwQual->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

//_____________________QUALITY TESTER (added by Emma___________________________________________________________
  ibooker.setCurrentFolder(monitorDir); //book in main folder
  std::vector<std::string> chamber_name = {"42", "41", "32", "31", "22", "21", "13", "12", "11b", "11a"};

  emtfQualityTester = ibooker.book1D("emtfQualityTester", "Report Summary Map",23, 0, 23);

  for (int i = 1; i < 11; ++i) {
    emtfQualityTester->setBinLabel(i, "Chamber Strip ME+"+chamber_name[i - 1]);
    emtfQualityTester->setBinLabel(21 - i, "Chamber Strip ME-"+chamber_name[i - 1]);
  }
  emtfQualityTester->setBinLabel(21, "Track BX");
  emtfQualityTester->setBinLabel(22, "Track Phi");
  emtfQualityTester->setBinLabel(23, "Error");
//_____________________________________________________________________________________________________________

}

void L1TStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TStage2EMTF") << "L1TStage2EMTF: analyze..." << std::endl;

  // DAQ Output
  edm::Handle<l1t::EMTFDaqOutCollection> DaqOutCollection;
  e.getByToken(daqToken, DaqOutCollection);

  for (std::vector<l1t::EMTFDaqOut>::const_iterator DaqOut = DaqOutCollection->begin(); DaqOut != DaqOutCollection->end(); ++DaqOut) {
    const l1t::emtf::MECollection* MECollection = DaqOut->PtrMECollection();
    for (std::vector<l1t::emtf::ME>::const_iterator ME = MECollection->begin(); ME != MECollection->end(); ++ME) {
      if (ME->SE()) emtfErrors->Fill(1);
      if (ME->SM()) emtfErrors->Fill(2);
      if (ME->BXE()) emtfErrors->Fill(3);
      if (ME->AF()) emtfErrors->Fill(4);
    }

    const l1t::emtf::EventHeader* EventHeader = DaqOut->PtrEventHeader();
    if (!EventHeader->Rdy()) emtfErrors->Fill(5);
  }

  // Hits (LCTs)
  edm::Handle<l1t::EMTFHitCollection> HitCollection;
  e.getByToken(hitToken, HitCollection);

  for (std::vector<l1t::EMTFHit>::const_iterator Hit = HitCollection->begin(); Hit != HitCollection->end(); ++Hit) {
    int endcap = Hit->Endcap();
    int sector = Hit->Sector();
    int station = Hit->Station();
    int ring = Hit->Ring();
    int cscid = Hit->CSC_ID();
    int chamber = Hit->Chamber();
    int strip = Hit->Strip();
    int wire = Hit->Wire();

    int neighbor = Hit->Neighbor(); //added by Emma

    int hist_index = 0;
    int cscid_offset = (sector - 1) * 9;

    // The following logic determines the index of the monitor element
    // to which a hit belongs, exploiting the symmetry of the endcaps.
    if (station == 1) { //altered by Emma
      if (ring == 4) {
        hist_index = 8;
      } else if (ring == 1) {
        hist_index = 9;
      } else if (ring == 2) {
        hist_index = 7;
      } else if (ring == 3) {
        hist_index = 6;
      }
    } else if (ring == 1) {
      if (station == 2) {
        hist_index = 5;
      } else if (station == 3) {
        hist_index = 3;
      } else if (station == 4) {
        hist_index = 1;
      }
    } else if (ring == 2) {
      if (station == 2) {
        hist_index = 4;
      } else if (station == 3) {
        hist_index = 2;
      } else if (station == 4) {
        hist_index = 0;
      }
    }

    if (endcap > 0) hist_index = 19 - hist_index; //altered by Emma

    emtfHitBX->Fill(Hit->BX(), hist_index);

    emtfHitStrip[hist_index]->Fill(strip);
    emtfHitWire[hist_index]->Fill(wire);

    emtfChamberStrip[hist_index]->Fill(chamber, strip);
    emtfChamberWire[hist_index]->Fill(chamber, wire);

//_____________________QUALITY TESTER (added by Emma)___CHAMBER STRIP__________________________________________

    if (neighbor == 0) {
        emtfChamberStrip_QT[hist_index]->Fill(chamber, strip);
    }

for (int hist_index = 0, i = 0; hist_index < 20; ++hist_index, i = hist_index % 10) {

  int xMax, yMin, yMax;
  int xMinFiller, yMinFiller, yMaxFiller;

    if (hist_index < 6) { 
      xMax = (i % 2) ? 18 : 36;
      xMinFiller = xMax + 1;
    } else if (hist_index > 13) {
      xMax = ((i-1) % 2) ? 18 : 36;
      xMinFiller = xMax + 1;
    } else {
      xMax = 36;
      xMinFiller = xMax + 1;
    }
    if (hist_index == 6 || hist_index == 9 || hist_index == 10 || hist_index == 13) {
      yMin = 1;
      yMax = 8;
      yMinFiller = yMax + 1;
      yMaxFiller = 14;
    } else if (hist_index == 8 || hist_index == 11) {
      yMin = 9;
      yMax = 14;
      yMinFiller = 1;
      yMaxFiller = yMin - 1; 
    } else {
      yMin = 1;
      yMax = 10;
      yMinFiller = yMax + 1;
      yMaxFiller = 14;
    }

    double meanChamberStrip; double sumChamberStrip = 0;
    double meanChamberStripHot; double sumChamberStripHot = 0;
    double meanChamberStripDead; double sumChamberStripDead = 0;
    double minValue = emtfChamberStrip_QT[hist_index]->getTH1()->GetMinimum(); 
    double maxValue = emtfChamberStrip_QT[hist_index]->getTH1()->GetMaximum();

    for (int binX=1; binX <= xMax; binX++) {
      for (int binY = yMin; binY <= yMax; binY++) {
        sumChamberStrip += sqrt(emtfChamberStrip_QT[hist_index]->getTH1()->GetBinContent(binX, binY));

//        sumChamberStrip += sqrt((emtfChamberStrip_QT[hist_index]->getTH1()->GetBinContent(binX, binY) - minValue)/(maxValue - minValue));
      }
    }
    meanChamberStrip = sumChamberStrip/(xMax*(yMax-yMin+1));

    for (int binX=1; binX <= xMax; binX++) {
      for (int binY = yMin; binY <= yMax; binY++) {
	emtfChamberStrip_QT_dead[hist_index]->getTH1()->SetBinContent(binX, binY, 1 + meanChamberStrip - sqrt(emtfChamberStrip_QT[hist_index]->getTH1()->GetBinContent(binX, binY)));
	emtfChamberStrip_QT_hot[hist_index]->getTH1()->SetBinContent(binX, binY, 1 - meanChamberStrip + sqrt(emtfChamberStrip_QT[hist_index]->getTH1()->GetBinContent(binX, binY))); 

//	emtfChamberStrip_QT_dead[hist_index]->getTH1()->SetBinContent(binX, binY, 1 + meanChamberStrip - sqrt((emtfChamberStrip_QT[hist_index]->getTH1()->GetBinContent(binX, binY) - minValue)/(maxValue - minValue)));
//	emtfChamberStrip_QT_hot[hist_index]->getTH1()->SetBinContent(binX, binY, 1 - meanChamberStrip + sqrt((emtfChamberStrip_QT[hist_index]->getTH1()->GetBinContent(binX, binY) - minValue)/(maxValue - minValue))); 
      }
    }

    for (int binX=1; binX <= xMax; binX++) {
      for (int binY = yMin; binY <= yMax; binY++) {
        sumChamberStripHot += emtfChamberStrip_QT_hot[hist_index]->getTH1()->GetBinContent(binX, binY);
        sumChamberStripDead += emtfChamberStrip_QT_dead[hist_index]->getTH1()->GetBinContent(binX, binY);
      }
    }
    meanChamberStripHot = sumChamberStripHot/(xMax*(yMax-yMin+1));
    meanChamberStripDead = sumChamberStripDead/(xMax*(yMax-yMin+1));

  //fill all histograms so they maintain their error percentage, but all have 504 bins, 1 error = .198
  if (yMin > 1 || yMax < 14) {
    for (int binX = 1; binX <= 36; binX++) {
      for (int binY = yMinFiller; binY <= yMaxFiller; binY++) {
	emtfChamberStrip_QT_dead[hist_index]->getTH1()->SetBinContent(binX, binY, meanChamberStripDead);
	emtfChamberStrip_QT_hot[hist_index]->getTH1()->SetBinContent(binX, binY, meanChamberStripHot);
      }
    }
  }
  if (xMax < 36) {
    for (int binX = xMinFiller; binX <= 36; binX++) {
      for (int binY = 1; binY <= 14; binY++) {
	emtfChamberStrip_QT_dead[hist_index]->getTH1()->SetBinContent(binX, binY, meanChamberStripDead);
	emtfChamberStrip_QT_hot[hist_index]->getTH1()->SetBinContent(binX, binY, meanChamberStripHot);
      }
    }
  }
}
//_____________________________________________________________________________________________________________

    if (Hit->Subsector() == 1) {
      emtfHitOccupancy->Fill(cscid + cscid_offset, endcap * (station - 0.5));
    } else {
      emtfHitOccupancy->Fill(cscid + cscid_offset, endcap * (station + 0.5));
    }
  }

  // Tracks
  edm::Handle<l1t::EMTFTrackCollection> TrackCollection;
  e.getByToken(trackToken, TrackCollection);

  int nTracks = TrackCollection->size();

  if (nTracks <= 10) {
    emtfnTracks->Fill(nTracks);
  } else {
    emtfnTracks->Fill(10);
  }

  for (std::vector<l1t::EMTFTrack>::const_iterator Track = TrackCollection->begin(); Track != TrackCollection->end(); ++Track) {
    int endcap = Track->Endcap();
    int sector = Track->Sector();
    float eta = Track->Eta();
    float phi_glob_rad = Track->Phi_glob_rad();
    int mode = Track->Mode();
    int quality = Track->Quality();

    int has_neighbor = Track->Has_neighbor(); //added by Emma

    emtfTracknHits->Fill(Track->NumHits());
    emtfTrackBX->Fill(endcap * (sector - 0.5), Track->BX());

//__________________QUALITY TESTER (added by Emma)___TRACK BX__________________________________________________

    if (has_neighbor == 0) {
      if (Track->BX() == 0) {
        emtfTrackBX_QT->Fill(endcap * (sector - 0.5));
      }
    }

    double meanTrackBX = emtfTrackBX_QT->getTH1()->Integral() / emtfTrackBX_QT->getTH1()->GetNbinsX();


    for (int binX=1; binX <= emtfTrackBX_QT->getTH1()->GetNbinsX(); binX++) {
	emtfTrackBX_QT_dead->getTH1()->SetBinContent(binX, 1 + sqrt(meanTrackBX) - sqrt(emtfTrackBX_QT->getTH1()->GetBinContent(binX)));
	emtfTrackBX_QT_hot->getTH1()->SetBinContent(binX, 1 - sqrt(meanTrackBX) + sqrt(emtfTrackBX_QT->getTH1()->GetBinContent(binX)));
    }
//____________________________________________________________________________________________________________

    emtfTrackPt->Fill(Track->Pt());
    emtfTrackEta->Fill(eta);
    emtfTrackPhi->Fill(phi_glob_rad);

//________________QUALITY TESTER (added by Emma)___TRACK PHI__________________________________________________
    double shift = pi/64.0;
    int xMax = emtfTrackPhi_QT->getTH1()->GetNbinsX();

    if (has_neighbor == 0) 
    {
        if (mode == 15) emtfTrackPhi_QT->Fill(phi_glob_rad + shift);
    }

    double meanPhi = emtfTrackPhi_QT->getTH1()->Integral() / xMax;

    for (int binX=1; binX <= xMax; binX++) {
	emtfTrackPhi_QT_dead->getTH1()->SetBinContent(binX, 1 + sqrt(meanPhi) - sqrt(emtfTrackPhi_QT->getTH1()->GetBinContent(binX)));
	emtfTrackPhi_QT_hot->getTH1()->SetBinContent(binX, 1 - sqrt(meanPhi) + sqrt(emtfTrackPhi_QT->getTH1()->GetBinContent(binX)));
    } 
//____________________________________________________________________________________________________________
    emtfTrackOccupancy->Fill(eta, phi_glob_rad);
    emtfTrackMode->Fill(mode);
    emtfTrackQuality->Fill(quality);
    emtfTrackQualityVsMode->Fill(mode, quality);
    if (mode == 15) emtfTrackPhiHighQuality->Fill(phi_glob_rad);
   }

  // Regional Muon Candidates
  edm::Handle<l1t::RegionalMuonCandBxCollection> MuonBxCollection;
  e.getByToken(muonToken, MuonBxCollection);

  for (int itBX = MuonBxCollection->getFirstBX(); itBX <= MuonBxCollection->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator Muon = MuonBxCollection->begin(itBX); Muon != MuonBxCollection->end(itBX); ++Muon) {
      emtfMuonBX->Fill(itBX);
      emtfMuonhwPt->Fill(Muon->hwPt());
      emtfMuonhwEta->Fill(Muon->hwEta());
      emtfMuonhwPhi->Fill(Muon->hwPhi());
      emtfMuonhwQual->Fill(Muon->hwQual());
    }
  }
//_____________________QUALITY TESTER__added by Emma___________________________________________________________

const QReport * emtfTrackBXHotQReport = emtfTrackBX_QT_hot->getQReport("NoisyChannel_TrackBX"); //get QReport associated to your ME 
//const QReport * emtfTrackBXHotQReport = emtfTrackBX_QT_hot->getQReport("DQMData/Run 277420/L1T2016/Run summary/L1TStage2EMTF/NoisyChannel_TrackBX");
if(emtfTrackBXHotQReport) {
 float qtresult = emtfTrackBXHotQReport->getQTresult(); // get QT result value
 int qtstatus = emtfTrackBXHotQReport->getStatus() ; // get QT status value (see table below)
 std::string qtmessage = emtfTrackBXHotQReport->getMessage() ; // get the whole QT result message
 emtfQualityTester->getTH1()->SetBinContent(21,qtresult);
 emtfQualityTester->getTH1()->SetBinContent(22,qtstatus);
 emtfQualityTester->getTH1()->SetBinContent(2,500);
}
 emtfQualityTester->getTH1()->SetBinContent(1,500);

//_____________________________________________________________________________________________________________
}

//_____________________ADDED BY EMMA___________________________________________________________________________
// constructor
L1TStage2EMTFqt::L1TStage2EMTFqt(const edm::ParameterSet& parSet) :
            m_verbose(parSet.getUntrackedParameter<bool>("verbose", false)),
            m_monitorDir(parSet.getUntrackedParameter<std::string>("monitorDir", "")),
            m_runInEventLoop(parSet.getUntrackedParameter<bool>("runInEventLoop", false)),
            m_runInEndLumi(parSet.getUntrackedParameter<bool>("runInEndLumi", false)),
            m_runInEndRun(parSet.getUntrackedParameter<bool>("runInEndRun", false)),
            m_runInEndJob(parSet.getUntrackedParameter<bool>("runInEndJob", false)),
            m_l1Systems(parSet.getParameter<std::vector<edm::ParameterSet> >("L1Systems")),
            m_l1Objects(parSet.getParameter<std::vector<edm::ParameterSet> >("L1Objects")),
            m_disableL1Systems(parSet.getParameter<std::vector<std::string> >("DisableL1Systems")),
            m_disableL1Objects(parSet.getParameter<std::vector<std::string> >("DisableL1Objects")),
            m_nrL1Systems(0),
            m_nrL1Objects(0),
            m_totalNrQtSummaryEnabled(0) {

    initialize();
}

// destructor
L1TStage2EMTFqt::~L1TStage2EMTFqt() {

    //empty

}

void L1TStage2EMTFqt::initialize() {
  
    if (m_verbose) {
        std::cout << "\nMonitor directory =             " << m_monitorDir
                << std::endl;
    }

    // L1 systems

    m_nrL1Systems = m_l1Systems.size();

    m_systemLabel.reserve(m_nrL1Systems);
    m_systemLabelExt.reserve(m_nrL1Systems);
    m_systemDisable.reserve(m_nrL1Systems);

    // on average five quality test per system - just a best guess
    m_systemQualityTestName.reserve(5*m_nrL1Systems);
    m_systemQualityTestHist.reserve(5*m_nrL1Systems);
    m_systemQtSummaryEnabled.reserve(5*m_nrL1Systems);

    int indexSys = 0;

    int totalNrQualityTests = 0;

    for (std::vector<edm::ParameterSet>::const_iterator itSystem =
            m_l1Systems.begin(); itSystem != m_l1Systems.end(); ++itSystem) {

        m_systemLabel.push_back(itSystem->getParameter<std::string>(
                "SystemLabel"));

        m_systemLabelExt.push_back(itSystem->getParameter<std::string>(
                "HwValLabel"));

        m_systemDisable.push_back(itSystem->getParameter<unsigned int>(
                "SystemDisable"));
        // check the additional disable flag from m_disableL1Systems
        for (std::vector<std::string>::const_iterator itSys =
                m_disableL1Systems.begin(); itSys != m_disableL1Systems.end(); ++itSys) {

            if (*itSys == m_systemLabel[indexSys]) {
                m_systemDisable[indexSys] = 1;

            }
        }

        std::vector < edm::ParameterSet > qTests = itSystem->getParameter<
                std::vector<edm::ParameterSet> > ("QualityTests");
        size_t qtPerSystem = qTests.size();

        std::vector < std::string > qtNames;
        qtNames.reserve(qtPerSystem);

        std::vector < std::string > qtFullPathHists;
        qtFullPathHists.reserve(qtPerSystem);

        std::vector<unsigned int> qtSumEnabled;
        qtSumEnabled.reserve(qtPerSystem);

        for (std::vector<edm::ParameterSet>::const_iterator itQT =
                qTests.begin(); itQT != qTests.end(); ++itQT) {

            totalNrQualityTests++;

            qtNames.push_back(
                    itQT->getParameter<std::string> ("QualityTestName"));

            qtFullPathHists.push_back(
                    itQT->getParameter<std::string> ("QualityTestHist"));

            unsigned int qtEnabled = itQT->getParameter<unsigned int> (
                    "QualityTestSummaryEnabled");

            qtSumEnabled.push_back(qtEnabled);

            if (qtEnabled) {
                m_totalNrQtSummaryEnabled++;
            }
        }

        m_systemQualityTestName.push_back(qtNames);
        m_systemQualityTestHist.push_back(qtFullPathHists);
        m_systemQtSummaryEnabled.push_back(qtSumEnabled);

        indexSys++;

    }

    // L1 objects

    //
    m_nrL1Objects = m_l1Objects.size();

    m_objectLabel.reserve(m_nrL1Objects);
    m_objectDisable.reserve(m_nrL1Objects);

    // on average five quality test per object - just a best guess
    m_objectQualityTestName.reserve(5*m_nrL1Objects);
    m_objectQualityTestHist.reserve(5*m_nrL1Objects);
    m_objectQtSummaryEnabled.reserve(5*m_nrL1Objects);

    int indexObj = 0;

    for (std::vector<edm::ParameterSet>::const_iterator itObject =
            m_l1Objects.begin(); itObject != m_l1Objects.end(); ++itObject) {

        m_objectLabel.push_back(itObject->getParameter<std::string>(
                "ObjectLabel"));

        m_objectDisable.push_back(itObject->getParameter<unsigned int>(
                "ObjectDisable"));
        // check the additional disable flag from m_disableL1Objects
        for (std::vector<std::string>::const_iterator itObj =
                m_disableL1Objects.begin(); itObj != m_disableL1Objects.end(); ++itObj) {

            if (*itObj == m_objectLabel[indexObj]) {
                m_objectDisable[indexObj] = 1;

            }
        }

        std::vector < edm::ParameterSet > qTests = itObject->getParameter<
                std::vector<edm::ParameterSet> > ("QualityTests");
        size_t qtPerObject = qTests.size();

        std::vector < std::string > qtNames;
        qtNames.reserve(qtPerObject);

        std::vector < std::string > qtFullPathHists;
        qtFullPathHists.reserve(qtPerObject);

        std::vector<unsigned int> qtSumEnabled;
        qtSumEnabled.reserve(qtPerObject);

        for (std::vector<edm::ParameterSet>::const_iterator itQT =
                qTests.begin(); itQT != qTests.end(); ++itQT) {

            totalNrQualityTests++;

            qtNames.push_back(
                    itQT->getParameter<std::string> ("QualityTestName"));

            qtFullPathHists.push_back(
                    itQT->getParameter<std::string> ("QualityTestHist"));

            unsigned int qtEnabled = itQT->getParameter<unsigned int> (
                    "QualityTestSummaryEnabled");

            qtSumEnabled.push_back(qtEnabled);

            if (qtEnabled) {
                m_totalNrQtSummaryEnabled++;
            }
        }

        m_objectQualityTestName.push_back(qtNames);
        m_objectQualityTestHist.push_back(qtFullPathHists);
        m_objectQtSummaryEnabled.push_back(qtSumEnabled);

        indexObj++;

    }

    m_summaryContent.reserve(m_nrL1Systems + m_nrL1Objects);
    m_meReportSummaryContent.reserve(totalNrQualityTests);

}

void L1TStage2EMTFqt::dqmEndLuminosityBlock(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter, const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& evSetup) {

  
  if (m_runInEndLumi) {
    
    book(ibooker, igetter);
    readQtResults(ibooker, igetter);

    if (m_verbose) {

        std::cout << "\n  L1TEventInfoClient::endLuminosityBlock\n"
                << std::endl;
        dumpContentMonitorElements(ibooker, igetter);
    }

  }
}


void L1TStage2EMTFqt::dqmEndJob(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

  book(ibooker, igetter);

  readQtResults(ibooker, igetter);

  if (m_verbose) {

    std::cout << "\n  L1TEventInfoClient::endRun\n" << std::endl;
    dumpContentMonitorElements(ibooker, igetter);
  }
}

void L1TStage2EMTFqt::dumpContentMonitorElements(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

    std::cout << "\nSummary report " << std::endl;

    // summary content

    MonitorElement* me = igetter.get(m_meReportSummaryMap->getName());

    std::cout
            << "\nSummary content per system and object as filled in histogram\n  "
            << m_meReportSummaryMap->getName() << std::endl;

    if (!me) {

        std::cout << "\nNo histogram " << m_meReportSummaryMap->getName()
                << "\nNo summary content per system and object as filled in histogram.\n  "
                << std::endl;
        return;

    }

    TH2F* hist = me->getTH2F();

    const int nBinsX = hist->GetNbinsX();
    const int nBinsY = hist->GetNbinsY();
    std::cout << nBinsX << " " << nBinsY;

    std::vector<std::vector<int> > meReportSummaryMap(nBinsX, std::vector<int>(
            nBinsY));

//    for (int iBinX = 0; iBinX < nBinsX; iBinX++) {
//        for (int iBinY = 0; iBinY < nBinsY; iBinY++) {
//            meReportSummaryMap[iBinX][iBinY]
//                    = static_cast<int>(me->GetBinContent(iBinX + 1, iBinY + 1));
//        }
//    }

    std::cout << "\nL1 systems: " << m_nrL1Systems << " systems included\n"
            << "\n Summary content size: " << (m_summaryContent.size())
            << std::endl;

    for (unsigned int iSys = 0; iSys < m_nrL1Systems; ++iSys) {

        std::cout << std::setw(10) << m_systemLabel[iSys] << std::setw(10)
                << m_systemLabelExt[iSys] << " \t" << m_systemDisable[iSys]
                << " \t" << std::setw(25) << " m_summaryContent["
                << std::setw(2) << iSys << "] = " << meReportSummaryMap[0][iSys]
                << std::endl;
    }

    std::cout << "\n L1 trigger objects: " << m_nrL1Objects
            << " objects included\n" << std::endl;

    for (unsigned int iMon = m_nrL1Systems; iMon < m_nrL1Systems
            + m_nrL1Objects; ++iMon) {

        std::cout << std::setw(20) << m_objectLabel[iMon - m_nrL1Systems]
                << " \t" << m_objectDisable[iMon - m_nrL1Systems] << " \t"
                << std::setw(25) << " m_summaryContent[" << std::setw(2)
                << iMon << "] = \t" << m_summaryContent[iMon] << std::endl;
    }

    std::cout << std::endl;

    // quality tests

    std::cout << "\nQuality test results as filled in " << "\n  "
            << m_monitorDir << "/EventInfo/reportSummaryContents\n"
            << "\n  Total number of quality tests: "
            << (m_meReportSummaryContent.size()) << "\n" << std::endl;

    for (std::vector<MonitorElement*>::const_iterator itME =
            m_meReportSummaryContent.begin(); itME
            != m_meReportSummaryContent.end(); ++itME) {

        std::cout << std::setw(50) << (*itME)->getName() << " \t"
                << std::setw(25) << (*itME)->getFloatValue() << std::endl;

    }

    std::cout << std::endl;

}



void L1TStage2EMTFqt::book(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

    std::string dirEventInfo = m_monitorDir + "/EventInfo";

    //ibooker.setCurrentFolder(dirEventInfo);
    ibooker.setCurrentFolder(m_monitorDir + "/LOOK HERE"); //book in subfolder

    // remove m_meReportSummary if it exists
    if ((m_meReportSummary = igetter.get(dirEventInfo + "/reportSummary"))) {
        igetter.removeElement(m_meReportSummary->getName());
    }

    // ...and book it again
    m_meReportSummary = ibooker.bookFloat("reportSummary");

    // initialize reportSummary to 1

    if (m_meReportSummary) {
        m_meReportSummary->Fill(1);
    }

    // define float histograms for reportSummaryContents (one histogram per quality test),
    // initialize them to zero
    // initialize also m_summaryContent to dqm::qstatus::DISABLED

    //ibooker.setCurrentFolder(dirEventInfo + "/reportSummaryContents");
    ibooker.setCurrentFolder(m_monitorDir + "/LOOK HERE"); //book in subfolder
    // general counters:
    //   iAllQTest: all quality tests for all systems and objects
    //   iAllMon:   all monitored systems and objects
    int iAllQTest = 0;
    int iAllMon = 0;

    for (unsigned int iMon = 0; iMon < m_nrL1Systems; ++iMon) {

        m_summaryContent.push_back(dqm::qstatus::DISABLED);

        const std::vector<std::string>& sysQtName =
                m_systemQualityTestName[iMon];

        for (std::vector<std::string>::const_iterator itQtName =
                sysQtName.begin(); itQtName != sysQtName.end(); ++itQtName) {

            const std::string hStr = m_monitorDir + "_L1Sys_" +m_systemLabel[iMon] + "_" + (*itQtName);

            m_meReportSummaryContent.push_back(ibooker.bookFloat(hStr));
            m_meReportSummaryContent[iAllQTest]->Fill(0.);

            iAllQTest++;
        }

        iAllMon++;
    }


    for (unsigned int iMon = 0; iMon < m_nrL1Objects; ++iMon) {

        m_summaryContent.push_back(dqm::qstatus::DISABLED);

        const std::vector<std::string>& objQtName =
                m_objectQualityTestName[iMon];

        for (std::vector<std::string>::const_iterator itQtName =
                objQtName.begin(); itQtName != objQtName.end(); ++itQtName) {

            const std::string hStr = m_monitorDir + "_L1Obj_" + m_objectLabel[iMon] + "_" + (*itQtName);

            m_meReportSummaryContent.push_back(ibooker.bookFloat(hStr));
            m_meReportSummaryContent[iAllQTest]->Fill(0.);

            iAllQTest++;
        }

        iAllMon++;

    }

    ibooker.setCurrentFolder(m_monitorDir + "/LOOK HERE"); //book in subfolder
    //ibooker.setCurrentFolder(dirEventInfo);

    if ((m_meReportSummaryMap = igetter.get(dirEventInfo + "/reportSummaryMap"))) {
        igetter.removeElement(m_meReportSummaryMap->getName());
    }

    // define a histogram with two bins on X and maximum of m_nrL1Systems, m_nrL1Objects on Y

    int nBinsY = std::max(m_nrL1Systems, m_nrL1Objects);

    m_meReportSummaryMap = ibooker.book2D("reportSummaryMap",
            "reportSummaryMap", 2, 1, 3, nBinsY, 1, nBinsY + 1);

    if (m_monitorDir == "L1TEMU") {
        m_meReportSummaryMap->setTitle(
                "L1TEMU: L1 Emulator vs Data Report Summary Map");

    } else if (m_monitorDir == "L1T") {
        m_meReportSummaryMap->setTitle(
                "L1T: L1 Trigger Data Report Summary Map");
    } else {
        // do nothing
    }

    m_meReportSummaryMap->setAxisTitle("", 1);
    m_meReportSummaryMap->setAxisTitle("", 2);

    m_meReportSummaryMap->setBinLabel(1, "L1 systems", 1);
    m_meReportSummaryMap->setBinLabel(2, "L1 objects", 1);

    for (int iBin = 0; iBin < nBinsY; ++iBin) {

        m_meReportSummaryMap->setBinLabel(iBin + 1, " ", 2);
    }

}


void L1TStage2EMTFqt::readQtResults(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

    // initialize summary content, summary sum and ReportSummaryContent float histograms
    // for all L1 systems and L1 objects

    for (std::vector<int>::iterator it = m_summaryContent.begin(); it
            != m_summaryContent.end(); ++it) {

        (*it) = dqm::qstatus::DISABLED;

    }

    m_summarySum = 0.;

    for (std::vector<MonitorElement*>::iterator itME =
            m_meReportSummaryContent.begin(); itME
            != m_meReportSummaryContent.end(); ++itME) {

        (*itME)->Fill(0.);

    }


    // general counters:
    //   iAllQTest: all quality tests for all systems and objects
    //   iAllMon:   all monitored systems and objects
    int iAllQTest = 0;
    int iAllMon = 0;


    // quality tests for all L1 systems

    for (unsigned int iSys = 0; iSys < m_nrL1Systems; ++iSys) {

        // get the reports for each quality test

        const std::vector<std::string>& sysQtName =
                m_systemQualityTestName[iSys];
        const std::vector<std::string>& sysQtHist =
                m_systemQualityTestHist[iSys];
        const std::vector<unsigned int>& sysQtSummaryEnabled =
                m_systemQtSummaryEnabled[iSys];

        // pro system counter for quality tests
        int iSysQTest = 0;

        for (std::vector<std::string>::const_iterator itQtName =
                sysQtName.begin(); itQtName != sysQtName.end(); ++itQtName) {

            // get results, status and message

            MonitorElement* qHist = igetter.get(sysQtHist[iSysQTest]);

            if (qHist) {
                const std::vector<QReport*> qtVec = qHist->getQReports();
                const std::string hName = qHist->getName();

                if (m_verbose) {

                    std::cout << "\nNumber of quality tests "
                            << " for histogram " << sysQtHist[iSysQTest]
                            << ": " << qtVec.size() << "\n" << std::endl;
                }

                const QReport* sysQReport = qHist->getQReport(*itQtName);
                if (sysQReport) {
                    const float sysQtResult = sysQReport->getQTresult();
                    const int sysQtStatus = sysQReport->getStatus();
                    const std::string& sysQtMessage = sysQReport->getMessage();

                    if (m_verbose) {
                        std::cout << "\n" << (*itQtName) << " quality test:"
                                << "\n  result:  " << sysQtResult
                                << "\n  status:  " << sysQtStatus
                                << "\n  message: " << sysQtMessage << "\n"
                                << "\nFilling m_meReportSummaryContent["
                                << iAllQTest << "] with value "
                                << sysQtResult << "\n" << std::endl;
                    }

                    m_meReportSummaryContent[iAllQTest]->Fill(sysQtResult);

                    // for the summary map, keep the highest status value ("ERROR") of all tests
                    // which are considered for the summary plot
                    if (sysQtSummaryEnabled[iSysQTest]) {

                        if (sysQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = sysQtStatus;
                        }

                        m_summarySum += sysQtResult;
                    }


                } else {

                    // for the summary map, if the test was not found but it is assumed to be
                    // considered for the summary plot, set it to dqm::qstatus::INVALID

                    int sysQtStatus = dqm::qstatus::INVALID;

                    if (sysQtSummaryEnabled[iSysQTest]) {

                        if (sysQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = sysQtStatus;
                        }
                    }

                    m_meReportSummaryContent[iAllQTest]->Fill(0.);

                    if (m_verbose) {

                        std::cout << "\n" << (*itQtName)
                                << " quality test not found\n" << std::endl;
                    }
                }

            } else {
                // for the summary map, if the histogram was not found but it is assumed
                // to have a test be considered for the summary plot, set it to dqm::qstatus::INVALID

                int sysQtStatus = dqm::qstatus::INVALID;

                if (sysQtSummaryEnabled[iSysQTest]) {

                    if (sysQtStatus > m_summaryContent[iAllMon]) {
                        m_summaryContent[iAllMon] = sysQtStatus;
                    }
                }

                m_meReportSummaryContent[iAllQTest]->Fill(0.);

                if (m_verbose) {

                    std::cout << "\nHistogram " << sysQtHist[iSysQTest]
                            << " not found\n" << std::endl;
                }

            }

            // increase counters for quality tests
            iSysQTest++;
            iAllQTest++;

        }

        iAllMon++;

    }

    // quality tests for all L1 objects

    for (unsigned int iObj = 0; iObj < m_nrL1Objects; ++iObj) {

        // get the reports for each quality test

        const std::vector<std::string>& objQtName =
                m_objectQualityTestName[iObj];
        const std::vector<std::string>& objQtHist =
                m_objectQualityTestHist[iObj];
        const std::vector<unsigned int>& objQtSummaryEnabled =
                m_objectQtSummaryEnabled[iObj];

        // pro object counter for quality tests
        int iObjQTest = 0;

        for (std::vector<std::string>::const_iterator itQtName =
                objQtName.begin(); itQtName != objQtName.end(); ++itQtName) {

            // get results, status and message

            MonitorElement* qHist = igetter.get(objQtHist[iObjQTest]);

            if (qHist) {
                const std::vector<QReport*> qtVec = qHist->getQReports();
                const std::string hName = qHist->getName();

                if (m_verbose) {

                    std::cout << "\nNumber of quality tests "
                            << " for histogram " << objQtHist[iObjQTest]
                            << ": " << qtVec.size() << "\n" << std::endl;
                }

                const QReport* objQReport = qHist->getQReport(*itQtName);
                if (objQReport) {
                    const float objQtResult = objQReport->getQTresult();
                    const int objQtStatus = objQReport->getStatus();
                    const std::string& objQtMessage = objQReport->getMessage();

                    if (m_verbose) {
                        std::cout << "\n" << (*itQtName) << " quality test:"
                                << "\n  result:  " << objQtResult
                                << "\n  status:  " << objQtStatus
                                << "\n  message: " << objQtMessage << "\n"
                                << "\nFilling m_meReportSummaryContent["
                                << iAllQTest << "] with value "
                                << objQtResult << "\n" << std::endl;
                    }

                    m_meReportSummaryContent[iAllQTest]->Fill(objQtResult);

                    // for the summary map, keep the highest status value ("ERROR") of all tests
                    // which are considered for the summary plot
                    if (objQtSummaryEnabled[iObjQTest]) {

                        if (objQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = objQtStatus;
                        }

                        m_summarySum += objQtResult;
                    }

                } else {

                    // for the summary map, if the test was not found but it is assumed to be
                    // considered for the summary plot, set it to dqm::qstatus::INVALID

                    int objQtStatus = dqm::qstatus::INVALID;

                    if (objQtSummaryEnabled[iObjQTest]) {

                        if (objQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = objQtStatus;
                        }
                    }

                    m_meReportSummaryContent[iAllQTest]->Fill(0.);

                    if (m_verbose) {

                        std::cout << "\n" << (*itQtName)
                                << " quality test not found\n" << std::endl;
                    }

                }

            } else {
                // for the summary map, if the histogram was not found but it is assumed
                // to have a test be considered for the summary plot, set it to dqm::qstatus::INVALID

                int objQtStatus = dqm::qstatus::INVALID;

                if (objQtSummaryEnabled[iObjQTest]) {

                    if (objQtStatus > m_summaryContent[iAllMon]) {
                        m_summaryContent[iAllMon] = objQtStatus;
                    }
                }

                m_meReportSummaryContent[iAllQTest]->Fill(0.);

                if (m_verbose) {
                    std::cout << "\nHistogram " << objQtHist[iObjQTest]
                            << " not found\n" << std::endl;
                }
            }
            // increase counters for quality tests
            iObjQTest++;
            iAllQTest++;
        }
        iAllMon++;
    }

    // reportSummary value
    m_reportSummary = m_summarySum / float(m_totalNrQtSummaryEnabled);
    if (m_meReportSummary) {
        m_meReportSummary->Fill(m_reportSummary);
    }

    // fill the ReportSummaryMap for L1 systems (bin 1 on X)
    for (unsigned int iSys = 0; iSys < m_nrL1Systems; ++iSys) {

        double summCont = static_cast<double>(m_summaryContent[iSys]);
        m_meReportSummaryMap->setBinContent(1, iSys + 1, summCont);
    }

    // fill the ReportSummaryMap for L1 objects (bin 2 on X)
    for (unsigned int iMon = m_nrL1Systems; iMon < m_nrL1Systems
            + m_nrL1Objects; ++iMon) {

        double summCont = static_cast<double>(m_summaryContent[iMon]);
        m_meReportSummaryMap->setBinContent(2, iMon - m_nrL1Systems + 1, summCont);

    }
}
//_____________________________________________________________________________________________________________




