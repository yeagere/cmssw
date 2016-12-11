import FWCore.ParameterSet.Config as cms

#from Configuration.StandardSequences.Eras import eras #commented out by Emma
#process = cms.Process("L1TStage2DQM", eras.Run2_2016) #commented out by Emma
process = cms.Process("L1TStage2DQM") #added by Emma

#--------------------------------------------------
# Event Source and Condition

# Live Online DQM in P5
#process.load("DQM.Integration.config.inputsource_cfi")

# Testing in lxplus
process.load("DQM.Integration.config.fileinputsource_cfi") #added by Emma
process.load("DQM.Integration.config.FrontierCondition_GT_Offline_cfi") #added by Emma

# Required to load Global Tag
#process.load("DQM.Integration.config.FrontierCondition_GT_cfi") #commented out by Emma

# Required to load EcalMappingRecord
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

#--------------------------------------------------
# DQM Environment

process.load("DQM.Integration.config.environment_cfi")

process.dqmEnv.subSystemFolder = "L1T2016"
process.dqmSaver.tag = "L1T2016"
#process.DQMStore.referenceFileName = "/dqmdata/dqm/reference/l1t_reference.root" #commented out by Emma

process.dqmEndPath = cms.EndPath(process.dqmEnv * process.dqmSaver)

#--------------------------------------------------
# Standard Unpacking Path

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")    

process.rawToDigiPath = cms.Path(process.RawToDigi)

# Remove Unpacker Modules #added by Emma
process.rawToDigiPath.remove(process.siStripDigis) #added by Emma
process.rawToDigiPath.remove(process.gtDigis) #added by Emma
process.rawToDigiPath.remove(process.gtEvmDigis) #added by Emma

#--------------------------------------------------
# Stage2 Unpacker and DQM Path

# Filter fat events
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltFatEventFilter = hltHighLevel.clone()
process.hltFatEventFilter.throw = cms.bool(False)
process.hltFatEventFilter.HLTPaths = cms.vstring('HLT_L1FatEvents_v*')

# This can be used if HLT filter not available in a run
process.selfFatEventFilter = cms.EDFilter("HLTL1NumberFilter",
        invert = cms.bool(False),
        period = cms.uint32(107),
        rawInput = cms.InputTag("rawDataCollector"),
        fedId = cms.int32(1024)
        )

process.load("DQM.L1TMonitor.L1TStage2_cff")

process.l1tMonitorPath = cms.Path(
#    process.hltFatEventFilter + #commented out by Emma
#    process.selfFatEventFilter +
    process.l1tStage2Unpack +
    process.l1tStage2OnlineDQM
)

# Remove DQM Modules
#process.l1tStage2online.remove(process.l1tStage2CaloLayer1)
#process.l1tStage2online.remove(process.l1tStage2CaloLayer2)
#process.l1tStage2online.remove(process.l1tStage2Bmtf)
#process.l1tStage2online.remove(process.l1tStage2Emtf)
#process.l1tStage2online.remove(process.l1tStage2uGMT)
#process.l1tStage2online.remove(process.l1tStage2uGt)

# Remove DQM Modules #added by Emma
process.l1tStage2OnlineDQM.remove(process.l1tStage2CaloLayer1) #added by Emma
process.l1tStage2OnlineDQM.remove(process.l1tStage2CaloLayer2) #added by Emma
process.l1tStage2OnlineDQM.remove(process.l1tStage2Bmtf) #added by Emma
#process.l1tStageOonlineDQM.remove(process.l1tStage2Emtf) #added by Emma
process.l1tStage2OnlineDQM.remove(process.l1tStage2uGMT) #added by Emma
process.l1tStage2OnlineDQM.remove(process.l1tStage2uGt) #added by Emma

#--------------------------------------------------
# Stage2 Quality Tests
process.load("DQM.L1TMonitorClient.L1TStage2MonitorClient_cff")
process.l1tStage2MonitorClientPath = cms.Path(process.l1tStage2MonitorClient)

#--------------------------------------------------
# Legacy DQM EndPath
# TODO: Is lumi scalers still relevant?

process.load("DQM.L1TMonitor.L1TMonitor_cff")
process.l1tMonitorEndPath = cms.EndPath(process.l1tMonitorEndPathSeq)

#--------------------------------------------------
# L1T Online DQM Schedule

process.schedule = cms.Schedule(
    process.rawToDigiPath,
    process.l1tMonitorPath,
    process.l1tStage2MonitorClientPath,
    process.l1tMonitorEndPath,
    process.dqmEndPath
)

#--------------------------------------------------
# Process Customizations

from DQM.Integration.config.online_customizations_cfi import *
process = customise(process)

