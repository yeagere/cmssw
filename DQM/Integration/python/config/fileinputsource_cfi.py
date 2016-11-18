import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	# run 283964 added by Emma
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/00673035-E79A-E611-B12D-02163E012AC7.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/006CF568-E99A-E611-A3B2-02163E011DBA.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/00706973-E99A-E611-B7C9-02163E01221D.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/00FDE9C7-E69A-E611-B262-02163E011B12.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/00FE06BD-EC9A-E611-BBD3-02163E0137A4.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/02347858-E99A-E611-AEEA-02163E0143F9.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/025925C4-EC9A-E611-A973-02163E0142B0.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/964/00000/04664A96-E69A-E611-967A-02163E0128DC.root",
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
