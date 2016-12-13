# L1 Trigger Event Info client cfi
# 
#   The cfi can be used, with appropriate settings, for both L1T and L1TEMU. 
#   Default version in cfi: L1T event client
#
#   authors previous versions - see CVS
#
#   V.M. Ghete 2010-10-22 revised version of L1T DQM and L1TEMU DQM



import FWCore.ParameterSet.Config as cms

l1tEMTFEventInfoClient = cms.EDAnalyzer("L1TEMTFEventInfoClient",
    monitorDir = cms.untracked.string("L1T2016/L1TStage2EMTF"), ## Location of output EventInfo/reportSummaryContents
    histDir = cms.untracked.string("L1T2016/L1TStage2EMTF"),    ## Location of input histograms for quality tests
    
    # decide when to run and update the results of the quality tests
    # retrieval of quality test results must be consistent with the event / LS / Run execution
    # 
    runInEventLoop=cms.untracked.bool(False),
    runInEndLumi=cms.untracked.bool(True),
    runInEndRun=cms.untracked.bool(True),
    runInEndJob=cms.untracked.bool(False),

    #
    # for each L1 system, give:
    #     - SystemLabel:  system label
    #     - HwValLabel:   system label as used in hardware validation package 
    #                     (the package producing the ErrorFlag histogram)
    #     - SystemDisable:   system disabled: if 1, all quality tests for the system 
    #                     are disabled in the summary plot
    #     - for each quality test:
    #         - QualityTestName: name of quality test
    #         - QualityTestHist: histogram (full path)
    #         - QualityTestSummaryEnabled: 0 if disabled, 1 if enabled in summary plot 
    #
    # the position in the parameter set gives, in reverse order, the position in the reportSummaryMap
    # in the emulator column (left column)
    TrackObjects = cms.VPSet(
                    cms.PSet(
                        SystemLabel = cms.string("XRange_Err"),
                        ## HwValLabel = cms.string("ETP"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("XRange_Errors"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfErrors"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ), 
                    cms.PSet(
                        SystemLabel = cms.string("TrackBX_Noisy"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Noisy"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackBX"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),
                    cms.PSet(
                        SystemLabel = cms.string("TrackBX_Dead"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Dead"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackBX"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),
                    cms.PSet(
                        SystemLabel = cms.string("TrackPhi_Noisy"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Noisy"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackPhiHighQuality"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),  
                    cms.PSet(
                        SystemLabel = cms.string("TrackPhi_Dead"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Dead"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackPhiHighQuality"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  ## End QualityTests = cms.VPSet(
                        )  ## End cms.PSet(  ...  SystemLabel = cms.string("NoisyHot_Phi") 
                    ),  ## End TrackObjects = cms.VPSet(

    #
    # for each L1 trigger object, give:
    #     - ObjectLabel:  object label as used in enum L1GtObject
    #     - ObjectDisable: emulator mask: if 1, the system is masked in the summary plot
    #
    # the position in the parameter set gives, in reverse order, the position in the reportSummaryMap
    # in the trigger object column (right column)
    HitObjects = cms.VPSet(
                    cms.PSet(
                        ObjectLabel = cms.string("ChamberStrip_Noisy"),
                        ObjectDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Noisy"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg11a"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            ) 
                        ),
                    cms.PSet(
                        ObjectLabel = cms.string("ChamberStrip_Dead"),
                        ObjectDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Dead"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg11a"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  ## End QualityTests = cms.VPSet(
                        ),  ## End cms.PSet(  ...  ObjectLabel = cms.string("NoisyDead_MEPos22"),
                    ),  ## End HitObjects = cms.VPSet(
    #
    # fast over-mask a system: if the name of the system is in the list, the system will be masked
    # (the default mask value is given in TrackObjects VPSet)             
    #
    DisableTrackObjects = cms.vstring(),
    #
    # fast over-mask an object: if the name of the object is in the list, the object will be masked
    # (the default mask value is given in HitObjects VPSet)             
    # 
    DisableHitObjects =  cms.vstring()   

)
