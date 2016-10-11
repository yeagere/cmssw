import FWCore.ParameterSet.Config as cms

l1tStage2Emtf = cms.EDAnalyzer(
    "L1TStage2EMTF",
    emtfSource = cms.InputTag("emtfStage2Digis"),
    monitorDir = cms.untracked.string("L1T2016/L1TStage2EMTF"),
    #isData = cms.untracked.bool(False), #added by Emma 
    verbose = cms.untracked.bool(False), #altered by Emma, was False
)

