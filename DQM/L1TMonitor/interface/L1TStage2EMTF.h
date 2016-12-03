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

//_______________________________ADDED BY EMMA____________________________________________________________
//added by Emma from DQM/L1TMonitorClient/interface/L1TEventInfoClient.h
// class declaration
class L1TStage2EMTFqt: public DQMEDHarvester {

public:

    /// Constructor
    L1TStage2EMTFqt(const edm::ParameterSet&);

    /// Destructor
    virtual ~L1TStage2EMTFqt();

protected:
    
    void
    dqmEndLuminosityBlock(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter, const edm::LuminosityBlock&, const edm::EventSetup&);

    /// end job
    void dqmEndJob(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter)override;

    
private:

    /// input parameters

    bool m_verbose;
    std::string m_monitorDir;

    bool m_runInEventLoop;
    bool m_runInEndLumi;
    bool m_runInEndRun;
    bool m_runInEndJob;

    std::vector<edm::ParameterSet> m_l1Systems;
    std::vector<edm::ParameterSet> m_l1Objects;
    std::vector<std::string> m_disableL1Systems;
    std::vector<std::string> m_disableL1Objects;

    /// private methods

    /// initialize properly all elements
    void initialize();

    /// dump the content of the monitoring elements defined in this module
    void dumpContentMonitorElements(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter);

    /// book histograms
    void book(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter);

    /// read quality test results
    void readQtResults(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter);

    /// number of L1 trigger systems
    size_t m_nrL1Systems;

    /// number of L1 trigger objects
    size_t m_nrL1Objects;

    /// total number of quality tests enabled for summary report for L1 trigger systems
    /// and L1 trigger objects
    size_t m_totalNrQtSummaryEnabled;

    std::vector<std::string> m_systemLabel;
    std::vector<std::string> m_systemLabelExt;
    std::vector<int> m_systemDisable;

    std::vector<std::vector<std::string> > m_systemQualityTestName;
    std::vector<std::vector<std::string> > m_systemQualityTestHist;
    std::vector<std::vector<unsigned int> > m_systemQtSummaryEnabled;

    std::vector<int> m_objectDisable;
    std::vector<std::string> m_objectLabel;
    std::vector<std::string> m_objectFolder;

    std::vector<std::vector<std::string> > m_objectQualityTestName;
    std::vector<std::vector<std::string> > m_objectQualityTestHist;
    std::vector<std::vector<unsigned int> > m_objectQtSummaryEnabled;

    /// summary report

    Float_t m_reportSummary;
    Float_t m_summarySum;
    std::vector<int> m_summaryContent;

    /// a summary report
    MonitorElement* m_meReportSummary;

    /// monitor elements to report content for all quality tests
    std::vector<MonitorElement*> m_meReportSummaryContent;

    /// report summary map
    MonitorElement* m_meReportSummaryMap;
};

//________________________________________________________________________________________________________

#endif
