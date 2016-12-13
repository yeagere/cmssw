import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	# run 284036 added by Emma
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/284/036/00000/5CFC345B-939B-E611-9C8F-02163E01418F.root",
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/284/036/00000/240B4D37-969B-E611-BDA3-FA163EDF9ED3.root",
	#"/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/284/036/00000/1EF69739-969B-E611-B90A-FA163EE5597C.root",

	# run 283171 from zerobiasisolatedbunch0 added by Emma
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/00A396A0-3C92-E611-9B93-02163E0145AD.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/00CEE8A0-3C92-E611-B6DE-02163E01185B.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0208FF04-EB91-E611-965C-02163E01389B.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0212579C-3C92-E611-B0ED-02163E014348.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/029C87AA-3C92-E611-956C-02163E014380.root",
    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
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
