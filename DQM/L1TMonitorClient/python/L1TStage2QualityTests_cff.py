# Quality tests for L1 Trigger DQM (L1T)

import FWCore.ParameterSet.Config as cms

# L1 systems quality tests

# Stage 2 L1 Trigger Quality tests
from DQM.L1TMonitorClient.L1TStage2CaloLayer1QualityTests_cfi import *

# edit out quality tests to check impact #added by Emma
from DQM.L1TMonitorClient.L1TStage2EMTFQualityTests_cfi import * #added by Emma

# L1 objects quality tests

# sequence for L1 systems
l1TriggerSystemQualityTests = cms.Sequence(
                                l1TStage2CaloLayer1QualityTests +
                                l1TStage2EMTFQualityTests #added by Emma
                                )

# sequence for L1 objects
l1TriggerObjectQualityTests = cms.Sequence(
                                )


# general sequence
l1TStage2QualityTests = cms.Sequence(
                                      l1TriggerSystemQualityTests + 
                                      l1TriggerObjectQualityTests
                                      )

