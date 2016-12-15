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
                        SystemLabel = cms.string("TrackBX"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Noisy_TrackBX"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackBX"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),
                    cms.PSet(
                        SystemLabel = cms.string("TrackPhi"),
                        SystemDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Noisy"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackPhi"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),  
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
                        HitLabel = cms.string("TrackBX"),
                        HitDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Dead_TrackBX"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackBX"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),
                    cms.PSet(
                        HitLabel = cms.string("TrackPhi"),
                        HitDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(
                                QualityTestName = cms.string("ContentSigma_Dead"),
                                QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfTrackPhi"),
                                QualityTestSummaryEnabled = cms.uint32(1)
                                )
                            )  
                        ),  
                    ),  ## End HitObjects = cms.VPSet(
    NoisyStrip = cms.VPSet(
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg42"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg41"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string("ChamberStrip_Noisy"), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg32"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg31"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg22"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg21"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg13"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg12"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_224"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg11b"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg11a"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos11a"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_224"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos11b"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos12"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos13"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos21"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos22"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos31"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos32"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos41"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(NoisyLabel = cms.string(" "), NoisyDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Noisy_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos42"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                    ),  ## End HitObjects = cms.VPSet(
    DeadStrip = cms.VPSet(
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg42"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg41"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg32"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg31"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg22"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg21"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg13"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg12"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_224"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg11b"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMENeg11a"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos11a"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_224"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos11b"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos12"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_128"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos13"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos21"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos22"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos31"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos32"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos41"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
                  	cms.PSet(DeadLabel = cms.string(" "), DeadDisable  = cms.uint32(0),
                        QualityTests = cms.VPSet(
                            cms.PSet(QualityTestName = cms.string("ContentSigma_Dead_160"), QualityTestHist = cms.string("L1T2016/L1TStage2EMTF/emtfChamberStripMEPos42"), QualityTestSummaryEnabled = cms.uint32(1) ) ) ),
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
    DisableHitObjects =  cms.vstring(),
    DisableNoisyStrip = cms.vstring(),
    DisableDeadStrip = cms.vstring()   

)
