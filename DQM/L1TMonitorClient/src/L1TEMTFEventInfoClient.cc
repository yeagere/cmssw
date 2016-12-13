/**
 * \class L1TEMTFEventInfoClient
 *
 *
 * Description: see header file.
 *
 *
 * \author: Vasile Mihai Ghete   - HEPHY Vienna
 *
 *
 */

// this class header
#include "DQM/L1TMonitorClient/interface/L1TEMTFEventInfoClient.h"

// system include files
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <math.h>
#include <memory>

#include <vector>
#include <string>

// user include files
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DQMServices/Core/interface/QReport.h"
#include "DQMServices/Core/interface/DQMStore.h"
# include "DQMServices/Core/interface/DQMDefinitions.h"

#include <TH2F.h>
#include "TROOT.h"

// constructor
L1TEMTFEventInfoClient::L1TEMTFEventInfoClient(const edm::ParameterSet& parSet) :
            m_verbose(parSet.getUntrackedParameter<bool>("verbose", false)),
            m_monitorDir(parSet.getUntrackedParameter<std::string>("monitorDir", "")),
            m_histDir(parSet.getUntrackedParameter<std::string>("histDir", "")),
            m_runInEventLoop(parSet.getUntrackedParameter<bool>("runInEventLoop", false)),
            m_runInEndLumi(parSet.getUntrackedParameter<bool>("runInEndLumi", false)),
            m_runInEndRun(parSet.getUntrackedParameter<bool>("runInEndRun", false)),
            m_runInEndJob(parSet.getUntrackedParameter<bool>("runInEndJob", false)),
            m_trackObjects(parSet.getParameter<std::vector<edm::ParameterSet> >("TrackObjects")),
            m_hitObjects(parSet.getParameter<std::vector<edm::ParameterSet> >("HitObjects")),
            m_disableTrackObjects(parSet.getParameter<std::vector<std::string> >("DisableTrackObjects")),
            m_disableHitObjects(parSet.getParameter<std::vector<std::string> >("DisableHitObjects")),
            m_nrTrackObjects(0),
            m_nrHitObjects(0),
            m_totalNrQtSummaryEnabled(0) {

    initialize();
}

// destructor
L1TEMTFEventInfoClient::~L1TEMTFEventInfoClient() {
    //empty
}

void L1TEMTFEventInfoClient::initialize() {
  
    if (m_verbose) {
        std::cout << "\nMonitor directory =             " << m_monitorDir
                << std::endl;
    }

    // L1 systems

    m_nrTrackObjects = m_trackObjects.size();

    m_trackLabel.reserve(m_nrTrackObjects);
    // m_trackLabelExt.reserve(m_nrTrackObjects);  // Not needed? - AWB 05.12.16
    m_trackDisable.reserve(m_nrTrackObjects);

    // on average five quality test per system - just a best guess
    m_trackQualityTestName.reserve(5*m_nrTrackObjects);  // Not needed? - AWB 05.12.16
    m_trackQualityTestHist.reserve(5*m_nrTrackObjects);  // Not needed? - AWB 05.12.16
    m_trackQtSummaryEnabled.reserve(5*m_nrTrackObjects);  // Not needed? - AWB 05.12.16

    int indexSys = 0;

    int totalNrQualityTests = 0;

    for (std::vector<edm::ParameterSet>::const_iterator itTrack =
            m_trackObjects.begin(); itTrack != m_trackObjects.end(); ++itTrack) {

        m_trackLabel.push_back(itTrack->getParameter<std::string>(
                "SystemLabel"));

        // m_trackLabelExt.push_back(itTrack->getParameter<std::string>(  // Not needed? - AWB 05.12.16
        //         "HwValLabel"));

        m_trackDisable.push_back(itTrack->getParameter<unsigned int>(
                "SystemDisable"));
        // check the additional disable flag from m_disableTrackObjects
        for (std::vector<std::string>::const_iterator itSys =
                m_disableTrackObjects.begin(); itSys != m_disableTrackObjects.end(); ++itSys) {

            if (*itSys == m_trackLabel[indexSys]) {
                m_trackDisable[indexSys] = 1;

            }
        }

        std::vector < edm::ParameterSet > qTests = itTrack->getParameter<
                std::vector<edm::ParameterSet> > ("QualityTests");
        size_t qtPerSystem = qTests.size();

        std::vector < std::string > qtNames;
        qtNames.reserve(qtPerSystem);

        std::vector < std::string > qtFullPathHists;
        qtFullPathHists.reserve(qtPerSystem);

        std::vector<unsigned int> qtSumEnabled;
        qtSumEnabled.reserve(qtPerSystem);

	std::cout << "\nLooping over track quality tests" << std::endl;
        for (std::vector<edm::ParameterSet>::const_iterator itQT =
                qTests.begin(); itQT != qTests.end(); ++itQT) {

            totalNrQualityTests++;

            qtNames.push_back(
                    itQT->getParameter<std::string> ("QualityTestName"));

            // qtFullPathHists.push_back( m_histDir + "/" + itQT->getParameter<std::string> ("QualityTestHist"));
            qtFullPathHists.push_back( itQT->getParameter<std::string> ("QualityTestHist"));
	    std::cout << qtFullPathHists.back() << std::endl;

            unsigned int qtEnabled = itQT->getParameter<unsigned int> (
                    "QualityTestSummaryEnabled");

            qtSumEnabled.push_back(qtEnabled);

            if (qtEnabled) {
                m_totalNrQtSummaryEnabled++;
            }
        }

        m_trackQualityTestName.push_back(qtNames);
        m_trackQualityTestHist.push_back(qtFullPathHists);
        m_trackQtSummaryEnabled.push_back(qtSumEnabled);

        indexSys++;

    }

    // L1 objects

    //
    m_nrHitObjects = m_hitObjects.size();

    m_hitLabel.reserve(m_nrHitObjects);
    m_hitDisable.reserve(m_nrHitObjects);

    // on average five quality test per object - just a best guess
    m_hitQualityTestName.reserve(5*m_nrHitObjects);
    m_hitQualityTestHist.reserve(5*m_nrHitObjects);
    m_hitQtSummaryEnabled.reserve(5*m_nrHitObjects);

    int indexObj = 0;

    for (std::vector<edm::ParameterSet>::const_iterator itObject =
            m_hitObjects.begin(); itObject != m_hitObjects.end(); ++itObject) {

        m_hitLabel.push_back(itObject->getParameter<std::string>(
                "ObjectLabel"));

        m_hitDisable.push_back(itObject->getParameter<unsigned int>(
                "ObjectDisable"));
        // check the additional disable flag from m_disableHitObjects
        for (std::vector<std::string>::const_iterator itObj =
                m_disableHitObjects.begin(); itObj != m_disableHitObjects.end(); ++itObj) {

            if (*itObj == m_hitLabel[indexObj]) {
                m_hitDisable[indexObj] = 1;

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

	std::cout << "\nLooping over hit quality tests" << std::endl;
        for (std::vector<edm::ParameterSet>::const_iterator itQT =
                qTests.begin(); itQT != qTests.end(); ++itQT) {

            totalNrQualityTests++;

            qtNames.push_back(
                    itQT->getParameter<std::string> ("QualityTestName"));

            // qtFullPathHists.push_back( m_histDir + "/" + itQT->getParameter<std::string> ("QualityTestHist") );
            qtFullPathHists.push_back( itQT->getParameter<std::string> ("QualityTestHist") );
	    std::cout << qtFullPathHists.back() << std::endl;

            unsigned int qtEnabled = itQT->getParameter<unsigned int> (
                    "QualityTestSummaryEnabled");

            qtSumEnabled.push_back(qtEnabled);

            if (qtEnabled) {
                m_totalNrQtSummaryEnabled++;
            }
        }

        m_hitQualityTestName.push_back(qtNames);
        m_hitQualityTestHist.push_back(qtFullPathHists);
        m_hitQtSummaryEnabled.push_back(qtSumEnabled);

        indexObj++;

    }

    m_summaryContent.reserve(m_nrTrackObjects + m_nrHitObjects);
    m_meReportSummaryContent.reserve(totalNrQualityTests);

}

void L1TEMTFEventInfoClient::dqmEndLuminosityBlock(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter, const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& evSetup) {

  std::cout << "\nInside void L1TEMTFEventInfoClient::dqmEndLuminosityBlock" << std::endl;
  if (m_runInEndLumi) {
    
    book(ibooker, igetter);
    readQtResults(ibooker, igetter);

    if (m_verbose) {

        std::cout << "\n  L1TEMTFEventInfoClient::endLuminosityBlock\n"
                << std::endl;
        dumpContentMonitorElements(ibooker, igetter);
    }

  }
}


void L1TEMTFEventInfoClient::dqmEndJob(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

  std::cout << "\nInside void L1TEMTFEventInfoClient::dqmEndJob" << std::endl;
  book(ibooker, igetter);

  readQtResults(ibooker, igetter);

  if (m_verbose) {

    std::cout << "\n  L1TEMTFEventInfoClient::endRun\n" << std::endl;
    dumpContentMonitorElements(ibooker, igetter);
  }
}

void L1TEMTFEventInfoClient::dumpContentMonitorElements(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

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

    std::cout << "\nL1 systems: " << m_nrTrackObjects << " systems included\n"
            << "\n Summary content size: " << (m_summaryContent.size())
            << std::endl;

    for (unsigned int iTrackObj = 0; iTrackObj < m_nrTrackObjects; ++iTrackObj) {

        std::cout << std::setw(10) << m_trackLabel[iTrackObj] << std::setw(10)
	  // << m_trackLabelExt[iTrackObj] << " \t" << m_trackDisable[iTrackObj]
                << m_trackDisable[iTrackObj]
                << " \t" << std::setw(25) << " m_summaryContent["
                << std::setw(2) << iTrackObj << "] = " << meReportSummaryMap[0][iTrackObj]
                << std::endl;
    }

    std::cout << "\n L1 trigger objects: " << m_nrHitObjects
            << " objects included\n" << std::endl;

    for (unsigned int iMon = m_nrTrackObjects; iMon < m_nrTrackObjects
            + m_nrHitObjects; ++iMon) {

        std::cout << std::setw(20) << m_hitLabel[iMon - m_nrTrackObjects]
                << " \t" << m_hitDisable[iMon - m_nrTrackObjects] << " \t"
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



void L1TEMTFEventInfoClient::book(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

  std::cout << "\nInside void L1TEMTFEventInfoClient::book" << std::endl;
  std::string dirEventInfo = m_monitorDir + "/EventInfo";
  std::cout << dirEventInfo << std::endl;

  ibooker.setCurrentFolder(dirEventInfo);
  std::cout << "Ran ibooker.setCurrentFolder(dirEventInfo;" << std::endl;

    // remove m_meReportSummary if it exists
    if ((m_meReportSummary = igetter.get(dirEventInfo + "/reportSummary"))) {
      std::cout << "Removing m_meReportSummary" << std::endl;
      igetter.removeElement(m_meReportSummary->getName());
      std::cout << "Ran igetter.removeElement(m_meReportSummary->getName());" << std::endl;
    }

    // ...and book it again
    m_meReportSummary = ibooker.bookFloat("reportSummary");
    std::cout << "Ran m_meReportSummary = ibooker.bookFloat" << std::endl;

    // initialize reportSummary to 1

    if (m_meReportSummary) {
      std::cout << "Initializing reportSummary to 1" << std::endl;
      m_meReportSummary->Fill(1);
      std::cout << "Ran m_meReportSummary->Fill(1);" << std::endl;
    }

    // define float histograms for reportSummaryContents (one histogram per quality test),
    // initialize them to zero
    // initialize also m_summaryContent to dqm::qstatus::DISABLED

    ibooker.setCurrentFolder(dirEventInfo + "/reportSummaryContents");
    std::cout << "Ran ibooker.setCurrentFolder(dirEventInfo" << std::endl;
    // general counters:
    //   iAllQTest: all quality tests for all systems and objects
    //   iAllMon:   all monitored systems and objects
    int iAllQTest = 0;
    int iAllMon = 0;
    
    std::cout << "m_nrTrackObjects = " << m_nrTrackObjects << std::endl;
    for (unsigned int iMon = 0; iMon < m_nrTrackObjects; ++iMon) {
      std::cout << "  * iMon = " << iMon << std::endl;

        m_summaryContent.push_back(dqm::qstatus::DISABLED);
	std::cout << "Ran m_summaryContent.push_back(dqm::qstatus::DISABLED);" << std::endl;

        const std::vector<std::string>& trackObjQtName = m_trackQualityTestName[iMon];
	std::cout << "Ran const std::vector<std::string>& trackObjQtName = m_trackQualityTestName[iMon];" << std::endl;

        for (std::vector<std::string>::const_iterator itQtName =
	       trackObjQtName.begin(); itQtName != trackObjQtName.end(); ++itQtName) {

	  std::cout << "    - m_monitorDir = " << m_monitorDir << ", m_trackLabel[iMon] = " << m_trackLabel[iMon]
		    << ", (*itQtName) = " << (*itQtName) << std::endl;

	  // Avoid error in ibooker.bookFloat(hStr))
	  std::string m_mon_mod = m_monitorDir;	  
	  std::replace( m_mon_mod.begin(), m_mon_mod.end(), '/', '_' );

	  const std::string hStr = m_mon_mod + "_L1Sys_" +m_trackLabel[iMon] + "_" + (*itQtName);
	    std::cout << "    - " << hStr << std::endl;

            m_meReportSummaryContent.push_back(ibooker.bookFloat(hStr));
	    std::cout << "    - Ran m_meReportSummaryContent.push_back(ibooker.bookFloat(hStr));" << std::endl;
            m_meReportSummaryContent[iAllQTest]->Fill(0.);
	    std::cout << "    - Ran m_meReportSummaryContent[iAllQTest]->Fill(0.);" << std::endl;

            iAllQTest++;
        }

        iAllMon++;
    }


    for (unsigned int iMon = 0; iMon < m_nrHitObjects; ++iMon) {
      std::cout << "  * iMon = " << iMon << std::endl;

      m_summaryContent.push_back(dqm::qstatus::DISABLED);

        const std::vector<std::string>& objQtName =
                m_hitQualityTestName[iMon];

        for (std::vector<std::string>::const_iterator itQtName =
                objQtName.begin(); itQtName != objQtName.end(); ++itQtName) {

	  // Avoid error in ibooker.bookFloat(hStr))
	  std::string m_mon_mod = m_monitorDir;	  
	  std::replace( m_mon_mod.begin(), m_mon_mod.end(), '/', '_' );

            const std::string hStr = m_mon_mod + "_L1Obj_" + m_hitLabel[iMon] + "_" + (*itQtName);
	    std::cout << "    - " << hStr << std::endl;

            m_meReportSummaryContent.push_back(ibooker.bookFloat(hStr));
	    std::cout << "    - Ran m_meReportSummaryContent.push_back(ibooker.bookFloat(hStr));" << std::endl;
            m_meReportSummaryContent[iAllQTest]->Fill(0.);
	    std::cout << "    - Ran m_meReportSummaryContent[iAllQTest]->Fill(0.);" << std::endl;

            iAllQTest++;
        }

        iAllMon++;

    }

    std::cout << "Setting current folder to " << dirEventInfo << std::endl;
    ibooker.setCurrentFolder(dirEventInfo);
    std::cout << "Ran ibooker.setCurrentFolder(dirEventInfo);" << std::endl;

    // Should this be a "==" ?  - AWB 03.12.16
    if ((m_meReportSummaryMap = igetter.get(dirEventInfo + "/reportSummaryMap"))) {
        igetter.removeElement(m_meReportSummaryMap->getName());
    }

    // define a histogram with two bins on X and maximum of m_nrTrackObjects, m_nrHitObjects on Y

    int nBinsY = std::max(m_nrTrackObjects, m_nrHitObjects);

    m_meReportSummaryMap = ibooker.book2D("reportSummaryMap",
            "reportSummaryMap", 2, 1, 3, nBinsY, 1, nBinsY + 1);

    if (m_monitorDir == "L1TEMU") {
        m_meReportSummaryMap->setTitle(
                "L1TEMU: L1 Emulator vs Data Report Summary Map");

    } else if (m_monitorDir == "L1T") {
        m_meReportSummaryMap->setTitle(
                "L1T: L1 Trigger Data Report Summary Map");
    } else if (m_monitorDir == "L1T2016") {
        m_meReportSummaryMap->setTitle(
                "L1T2016: L1 Trigger Data Report Summary Map");
    } else {
        // do nothing
    }

    m_meReportSummaryMap->setAxisTitle("", 1);
    m_meReportSummaryMap->setAxisTitle("", 2);

    m_meReportSummaryMap->setBinLabel(1, "Track Objects", 1);
    m_meReportSummaryMap->setBinLabel(2, "Hit objects", 1);

    for (int iBin = 0; iBin < nBinsY; ++iBin) {

        m_meReportSummaryMap->setBinLabel(iBin + 1, " ", 2);
    }

}


void L1TEMTFEventInfoClient::readQtResults(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {

    // initialize summary content, summary sum and ReportSummaryContent float histograms
    // for all L1 systems and L1 objects

  std::cout << "\nInside L1TEMTFEventInfoClient::readQtResults" << std::endl; // Extra printout - AWB 03.12.16

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

    for (unsigned int iTrackObj = 0; iTrackObj < m_nrTrackObjects; ++iTrackObj) {

        // get the reports for each quality test

        const std::vector<std::string>& trackObjQtName =
                m_trackQualityTestName[iTrackObj];
        const std::vector<std::string>& trackObjQtHist =
                m_trackQualityTestHist[iTrackObj];
        const std::vector<unsigned int>& trackObjQtSummaryEnabled =
                m_trackQtSummaryEnabled[iTrackObj];

        // pro system counter for quality tests
        int iTrackObjQTest = 0;

        for (std::vector<std::string>::const_iterator itQtName =
                trackObjQtName.begin(); itQtName != trackObjQtName.end(); ++itQtName) {

            // get results, status and message

	  std::cout << "  *itQtName = " << (*itQtName) << std::endl; // Extra printout - AWB 03.12.16

            MonitorElement* qHist = igetter.get(trackObjQtHist[iTrackObjQTest]);

            if (qHist) {
                const std::vector<QReport*> qtVec = qHist->getQReports();
                const std::string hName = qHist->getName();

                // if (m_verbose) {
		if (true) {  // Force printout - AWB 03.12.16

                    std::cout << "  - Number of quality tests"
		    // std::cout << "\nNumber of quality tests"
                            << " for histogram " << trackObjQtHist[iTrackObjQTest]
                            << ": " << qtVec.size() << "\n" << std::endl;
                }

                const QReport* sysQReport = qHist->getQReport(*itQtName);
                if (sysQReport) {
                    const float trackObjQtResult = sysQReport->getQTresult();
                    const int trackObjQtStatus = sysQReport->getStatus();
                    const std::string& trackObjQtMessage = sysQReport->getMessage();

                    // if (m_verbose) {
		    if (true) {  // Force printout - AWB 03.12.16
                        std::cout << "\n" << (*itQtName) << " quality test:"
                                << "\n  result:  " << trackObjQtResult
                                << "\n  status:  " << trackObjQtStatus
                                << "\n  message: " << trackObjQtMessage << "\n"
                                << "\nFilling m_meReportSummaryContent["
                                << iAllQTest << "] with value "
                                << trackObjQtResult << "\n" << std::endl;
                    }

                    m_meReportSummaryContent[iAllQTest]->Fill(trackObjQtResult);

                    // for the summary map, keep the highest status value ("ERROR") of all tests
                    // which are considered for the summary plot
                    if (trackObjQtSummaryEnabled[iTrackObjQTest]) {

                        if (trackObjQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = trackObjQtStatus;
                        }

                        m_summarySum += trackObjQtResult;
                    }


                } else {

                    // for the summary map, if the test was not found but it is assumed to be
                    // considered for the summary plot, set it to dqm::qstatus::INVALID

                    int trackObjQtStatus = dqm::qstatus::INVALID;

                    if (trackObjQtSummaryEnabled[iTrackObjQTest]) {

                        if (trackObjQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = trackObjQtStatus;
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

                int trackObjQtStatus = dqm::qstatus::INVALID;

                if (trackObjQtSummaryEnabled[iTrackObjQTest]) {

                    if (trackObjQtStatus > m_summaryContent[iAllMon]) {
                        m_summaryContent[iAllMon] = trackObjQtStatus;
                    }
                }

                m_meReportSummaryContent[iAllQTest]->Fill(0.);

                if (m_verbose) {

                    std::cout << "\nHistogram " << trackObjQtHist[iTrackObjQTest]
                            << " not found\n" << std::endl;
                }

            }

            // increase counters for quality tests
            iTrackObjQTest++;
            iAllQTest++;

        }

        iAllMon++;

    }

    // quality tests for all L1 objects

    for (unsigned int iHitObj = 0; iHitObj < m_nrHitObjects; ++iHitObj) {

        // get the reports for each quality test

        const std::vector<std::string>& hitObjQtName =
                m_hitQualityTestName[iHitObj];
        const std::vector<std::string>& hitObjQtHist =
                m_hitQualityTestHist[iHitObj];
        const std::vector<unsigned int>& hitObjQtSummaryEnabled =
                m_hitQtSummaryEnabled[iHitObj];

        // pro object counter for quality tests
        int iHitObjQTest = 0;

        for (std::vector<std::string>::const_iterator itQtName =
                hitObjQtName.begin(); itQtName != hitObjQtName.end(); ++itQtName) {

            // get results, status and message

            MonitorElement* qHist = igetter.get(hitObjQtHist[iHitObjQTest]);

            if (qHist) {
                const std::vector<QReport*> qtVec = qHist->getQReports();
                const std::string hName = qHist->getName();

                if (m_verbose) {

                    std::cout << "\nNumber of quality tests "
                            << " for histogram " << hitObjQtHist[iHitObjQTest]
                            << ": " << qtVec.size() << "\n" << std::endl;
                }

                const QReport* objQReport = qHist->getQReport(*itQtName);
                if (objQReport) {
                    const float hitObjQtResult = objQReport->getQTresult();
                    const int hitObjQtStatus = objQReport->getStatus();
                    const std::string& hitObjQtMessage = objQReport->getMessage();

                    if (m_verbose) {
                        std::cout << "\n" << (*itQtName) << " quality test:"
                                << "\n  result:  " << hitObjQtResult
                                << "\n  status:  " << hitObjQtStatus
                                << "\n  message: " << hitObjQtMessage << "\n"
                                << "\nFilling m_meReportSummaryContent["
                                << iAllQTest << "] with value "
                                << hitObjQtResult << "\n" << std::endl;
                    }

                    m_meReportSummaryContent[iAllQTest]->Fill(hitObjQtResult);

                    // for the summary map, keep the highest status value ("ERROR") of all tests
                    // which are considered for the summary plot
                    if (hitObjQtSummaryEnabled[iHitObjQTest]) {

                        if (hitObjQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = hitObjQtStatus;
                        }

                        m_summarySum += hitObjQtResult;
                    }

                } else {

                    // for the summary map, if the test was not found but it is assumed to be
                    // considered for the summary plot, set it to dqm::qstatus::INVALID

                    int hitObjQtStatus = dqm::qstatus::INVALID;

                    if (hitObjQtSummaryEnabled[iHitObjQTest]) {

                        if (hitObjQtStatus > m_summaryContent[iAllMon]) {
                            m_summaryContent[iAllMon] = hitObjQtStatus;
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

                int hitObjQtStatus = dqm::qstatus::INVALID;

                if (hitObjQtSummaryEnabled[iHitObjQTest]) {

                    if (hitObjQtStatus > m_summaryContent[iAllMon]) {
                        m_summaryContent[iAllMon] = hitObjQtStatus;
                    }
                }

                m_meReportSummaryContent[iAllQTest]->Fill(0.);

                if (m_verbose) {
                    std::cout << "\nHistogram " << hitObjQtHist[iHitObjQTest]
                            << " not found\n" << std::endl;
                }

            }

            // increase counters for quality tests
            iHitObjQTest++;
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
    for (unsigned int iTrackObj = 0; iTrackObj < m_nrTrackObjects; ++iTrackObj) {

        double summCont = static_cast<double>(m_summaryContent[iTrackObj]);
        m_meReportSummaryMap->setBinContent(1, iTrackObj + 1, summCont);
    }

    // fill the ReportSummaryMap for L1 objects (bin 2 on X)
    for (unsigned int iMon = m_nrTrackObjects; iMon < m_nrTrackObjects
            + m_nrHitObjects; ++iMon) {

        double summCont = static_cast<double>(m_summaryContent[iMon]);
        m_meReportSummaryMap->setBinContent(2, iMon - m_nrTrackObjects + 1, summCont);

    }
}



