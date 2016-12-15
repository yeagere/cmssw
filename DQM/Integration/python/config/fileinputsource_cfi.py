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
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/02DA1D23-4092-E611-AA04-FA163E35234A.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/02E9BAA7-3C92-E611-95C6-02163E01198B.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/04213E9F-EC91-E611-BBE3-02163E013425.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0432FCA5-EE91-E611-8E1E-02163E011970.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/043FB294-3C92-E611-A4CA-FA163E2C38F6.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0453FA53-EA91-E611-981F-02163E0140EB.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/047A8447-4092-E611-B151-02163E0118E6.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/067F96A8-EE91-E611-9FC2-FA163EB6B8B4.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/06B3158E-3C92-E611-84E2-FA163EA0D418.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/06BCCC0B-EB91-E611-BE14-02163E0118A5.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/06C9C586-3C92-E611-81A5-FA163ECD93DC.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/083B8F9E-3C92-E611-A9A3-02163E0146CA.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/086F7994-3C92-E611-8014-FA163E073540.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/0870D7A6-EE91-E611-B822-02163E011AB4.root",
	"/store/data/Run2016H/ZeroBiasIsolatedBunch0/RAW/v1/000/283/171/00000/08FE108A-EE91-E611-A031-FA163EE1765D.root",
    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000)
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
