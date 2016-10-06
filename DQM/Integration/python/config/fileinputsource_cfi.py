import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/613/00000/003CF22A-FA82-E611-B844-02163E0119BD.root" #added by Emma
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/976/00001/00B12511-1787-E611-B3EA-02163E012699.root"

    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Parameters for runType
import FWCore.ParameterSet.VarParsing as VarParsing
import sys
from dqmPythonTypes import *

options = VarParsing.VarParsing("analysis")

options.register(
    "runkey",
    "cosmic_run",
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "Run Keys of CMS"
)

options.parseArguments()

# Fix to allow scram to compile
#if len(sys.argv) > 1:
#  options.parseArguments()

runType = RunType()
if not options.runkey.strip():
    options.runkey = "pp_run"

runType.setRunType(options.runkey.strip())
