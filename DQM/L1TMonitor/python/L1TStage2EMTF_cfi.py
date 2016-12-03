import FWCore.ParameterSet.Config as cms

qTesterStage2Emtf = cms.EDAnalyzer("QualityTester", #added by Emma
    qtList=cms.untracked.FileInPath('DQM/L1TMonitorClient/data/L1TStage2EMTFQualityTests.xml'), #added by Emma
    QualityTestPrescaler=cms.untracked.int32(1), #added by Emma
    getQualityTestsFromFile=cms.untracked.bool(True), #added by Emma
    testInEventloop=cms.untracked.bool(False), #added by Emma
    qtestOnEndLumi=cms.untracked.bool(True), #added by Emma
    qtestOnEndRun=cms.untracked.bool(True), #added by Emma
    qtestOnEndJob=cms.untracked.bool(False), #added by Emma
    reportThreshold=cms.untracked.string(""), #added by Emma
    verboseQT=cms.untracked.bool(True) #added by Emma
)

l1tStage2Emtf = cms.EDAnalyzer("L1TStage2EMTF",
    emtfSource = cms.InputTag("emtfStage2Digis"),
    monitorDir = cms.untracked.string("L1T2016/L1TStage2EMTF"),
    verbose = cms.untracked.bool(False),
)


l1tStage2EmtfSeq = cms.Sequence( #added by Emma
    qTesterStage2Emtf * #added by Emma
    l1tStage2Emtf #added by Emma
)



