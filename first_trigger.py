import ROOT

file = "~/local_SUEP_stuff/misc_root_files/1000to1400.root"
tree = ROOT.TChain("tree_name")
tree.Add(file)

num = tree.GetEntries()
