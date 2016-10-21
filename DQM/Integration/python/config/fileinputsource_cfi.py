import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	# run 281613 added by Emma
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/613/00000/003CF22A-FA82-E611-B844-02163E0119BD.root",
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/613/00000/00415F47-F882-E611-9B36-FA163E4FF519.root", 
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/613/00000/0041AF2C-FA82-E611-AAB1-02163E0143F2.root",
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/613/00000/00D95357-F282-E611-A6B6-02163E0143CB.root",
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/613/00000/00F85B10-0883-E611-BDC5-02163E01341C.root",

	# run 283478 added by Emma
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/478/00000/04406CE8-D495-E611-BD89-02163E0142E1.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/478/00000/04EB6113-D795-E611-8F1A-02163E0144E2.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/478/00000/06B6D69A-D695-E611-A17C-02163E011EBF.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/478/00000/06CCDF07-D795-E611-B164-02163E0133FD.root",
	"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/478/00000/06D1CB45-D595-E611-914A-02163E01389B.root"
    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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
