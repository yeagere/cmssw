import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	# run 283964 added by Emma
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/284/036/00000/5CFC345B-939B-E611-9C8F-02163E01418F.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/284/036/00000/240B4D37-969B-E611-BDA3-FA163EDF9ED3.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/284/036/00000/1EF69739-969B-E611-B90A-FA163EE5597C.root",
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
