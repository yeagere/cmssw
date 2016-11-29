#ifndef DQM_L1TMonitor_L1TStage2EMTF_h
#define DQM_L1TMonitor_L1TStage2EMTF_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/L1TMuon/interface/EMTFDaqOut.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"

//_______________________________ADDED BY EMMA____________________________________________________________
//added by Emma for quality test readout from DQM/L1TMonitorClient/interface/L1TEventInfoClient
// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"

class DQMStore; //forward declarations
/*
class L1TStage2EMTF : public DQMEDHarvester {
  public:
    L1TStage2EMTF(const edm::ParameterSet&);
    virtual ~L1TStage2EMTF();
  private:
    void readQtResults(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter); //added by Emma
}
*/
//________________________________________________________________________________________________________

class L1TStage2EMTF : public DQMEDAnalyzer {

 public:

  L1TStage2EMTF(const edm::ParameterSet& ps);
  virtual ~L1TStage2EMTF();

 protected:

  virtual void dqmBeginRun(const edm::Run&, const edm::EventSetup&);
  virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
  virtual void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override;
  //virtual void analyze(const edm::Event&, const edm::EventSetup&, DQMStore::IGetter &) override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  void dqmEndLuminosityBlock(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter, const edm::LuminosityBlock&, const edm::EventSetup&); //added by Emma

 private:

  void readQtResults(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter); //added by Emma

  edm::EDGetTokenT<l1t::EMTFDaqOutCollection> daqToken;
  edm::EDGetTokenT<l1t::EMTFHitCollection> hitToken;
  edm::EDGetTokenT<l1t::EMTFTrackCollection> trackToken;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> muonToken;
  std::string monitorDir;
  bool verbose;

  MonitorElement* emtfErrors;

  MonitorElement* emtfHitBX;
  MonitorElement* emtfHitStrip[20]; //altered by Emma
  MonitorElement* emtfHitWire[20]; //altered by Emma
  MonitorElement* emtfChamberStrip[20]; //altered by Emma
  MonitorElement* emtfChamberWire[20]; //altered by Emma
  MonitorElement* emtfHitOccupancy;
  
  MonitorElement* emtfnTracks;
  MonitorElement* emtfTracknHits;
  MonitorElement* emtfTrackBX;
  MonitorElement* emtfTrackPt;
  MonitorElement* emtfTrackEta;
  MonitorElement* emtfTrackPhi;
  MonitorElement* emtfTrackPhiHighQuality;
  MonitorElement* emtfTrackOccupancy;
  MonitorElement* emtfTrackMode;
  MonitorElement* emtfTrackQuality;
  MonitorElement* emtfTrackQualityVsMode;

  MonitorElement* emtfMuonBX;
  MonitorElement* emtfMuonhwPt;
  MonitorElement* emtfMuonhwEta;
  MonitorElement* emtfMuonhwPhi;
  MonitorElement* emtfMuonhwQual;

//=========QUALITY TESTER (added by Emma)=================
  MonitorElement* emtfChamberStrip_QT[20]; //have to reallocate for two additional histograms
  MonitorElement* emtfChamberStrip_QT_hot[20];
  MonitorElement* emtfChamberStrip_QT_dead[20];

  MonitorElement* emtfTrackBX_QT;
  MonitorElement* emtfTrackBX_QT_hot;
  MonitorElement* emtfTrackBX_QT_dead;

  MonitorElement* emtfTrackPhi_QT;
  MonitorElement* emtfTrackPhi_QT_hot;
  MonitorElement* emtfTrackPhi_QT_dead;

  MonitorElement* emtfQualityTester;
 //========================================

};

#endif
