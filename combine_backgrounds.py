import ROOT
import glob

ROOT.gROOT.SetBatch(1)

def getHist(t, hname, value_name, binstr="", sel_name="", binning="", verbose=False):
    #hname = hname.replace("(","-").replace(")","-")
    hname_nobin = hname
    if binning != "": 
        hname = hname + "(" + binning +")"
    print (hname)
    draw_cmd = '%s>>%s' % (value_name, hname)
    if binstr != "":
        draw_cmd += "(%s)" % (binstr)
    
    print (draw_cmd)
    print (sel_name)
    if sel_name != "": t.Draw( draw_cmd, '(%s)' % (sel_name) ,"COLZ")
    else: t.Draw( draw_cmd )
    if verbose: print ("\"%s\", \"%s\""%(draw_cmd, sel_name))
    print (h)
    return h


floc = "/Users/benthornberry/local_SUEP_stuff/outputs"
fnames = [
    #"QCD_Pt_15to30_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    #"QCD_Pt_30to50_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    #"QCD_Pt_50to80_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    #"QCD_Pt_80to120_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    #"QCD_Pt_120to170_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2+MINIAODSIM_noJetIdWrongWeight.root",
    #"QCD_Pt_170to300_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_300to470_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_470to600_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_600to800_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_800to1000_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root",
    "QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8+RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1+MINIAODSIM_noJetIdWrongWeight.root"
]

lum=135*1000
#xsec=[311900,29070,5962,1207,119.9,25.24]
xsec=[1,1,1,1,1,1,1,1,1]
xsecdict = {}

trees = {}  #Organizes data into TTree
for k, fname in enumerate(fnames):
    fullname = floc+fname
    tname = "TreeMaker2/PreSelection"
    Range = fname.split("_")[1]
    #trees[Range] = getTree(fullname, tname)
    trees[Range] = ROOT.TChain(tname)
    trees[Range].Add(fname)
    xsecdict[Range] = xsec[k]

maxX=6000
nbins=100

hist={}

#Plots TTrees into dictionary for histogram
for j, Range in enumerate(trees):
    t=trees[Range]
    entries = t.GetEntries()
    hist[Range] = getHist(t, "QCDHistogram"+Range, "HT", "100, 0, 6000", xsecdict[Range]*lum/(entries)) #"Length$(Tracks)"
    #hist[Range] = ROOT.gROOT.FindObject("QCDHistogram"+Range)

#Creates THIF Hist out of TTree dict
fhist={}
fhist["events"]=ROOT.TH1F("Data", "QCD Histogram", nbins, 0, maxX)
h = fhist["events"]

for Range in trees:
    print("type: ",type(hist[Range]))
    fhist["events"].Add(hist[Range])

myfile = TFile('backgrounds.root', 'RECREATE' )

fhist["events"].Write()
myfile.Close()