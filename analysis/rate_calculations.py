import numpy as np
import pandas as pd
import ROOT
import glob
import sys
import os


data_dir = '/scratch/user/ajbiffl3/doublecap/data/rate_sim/'


default_num = 25e7 # number of primaries per job, typically

# files with other numbers of primaries
primary_dict = {
    'simdata_20250131_145827': 5e7,
    'simdata_20250131_155936_0001': 5e7,
    'simdata_20250131_155936_0002': 5e7,
    'simdata_20250203_100526': 2e4,
}


data_files_found = {}
filenames = os.listdir(data_dir)

Nprimaries = 0. # progressively add to this as more files read

for fname in filenames:
    fbase = ''.join(fname.split('_t')[:-1])
    idx = int(fname.split('_t')[-1].split('.')[0])

    if fbase not in data_files_found:
        data_files_found[fbase] = []
    
    data_files_found[fbase].append(idx)

for fbase, idxs in data_files_found.items():
    if all(i in idxs for i in range(max(idxs)+1)):
        numfiles = len(idxs)
        if numfiles != 8:
            print(fbase, ': numfiles =', numfiles)
    else:
        print(fbase, ': not all found')
    if fbase in primary_dict:
        Nprimaries += primary_dict[fbase]
        #print('found', fbase)
    else:
        Nprimaries += default_num

print(f'total {Nprimaries:.4g} primaries')


mCu = 37000000 # one millicurie in Bq
total_activity = mCu # total activity of sample

T = Nprimaries/total_activity
print(f'{T:.4g} seconds exposure time')


detectors = ['lowmass', 'highmass1', 'highmass2']
def get_rates(*files):
    counts = {det: 0 for det in detectors}
    ncounts = {det: 0 for det in detectors}


    for file in files:
        last_event = -1
        hits = {det: 0 for det in detectors}
        tfile = ROOT.TFile.Open(data_dir + file, 'READ')
        tree = tfile.Get('tree')
        N = tree.GetEntries()

        for k in range(N):
            tree.GetEntry(k)
            EventNum = int(getattr(tree, 'EventNum'))
            VolName = getattr(tree, 'VolName')
            ProcName = getattr(tree, 'ProcName')
            hits[VolName] += 1


            if ProcName == 'nCapture':
                ParentVol = getattr(tree, 'ParentVol')

                if (ParentVol == VolName) and (getattr(tree, 'PName') != 'gamma'):
                    ncounts[VolName] += 1

            if abs(EventNum - last_event) > 1e-6: # new event
                last_event = EventNum
                for det in detectors:
                    if hits[det] > 0:
                        counts[det] += 1
                
                hits = {det: 0 for det in detectors}

        tfile.Close()

    return counts, ncounts

counts, ncounts = get_rates(*filenames)

for det in detectors:
    print(f'{det}: {counts[det]/T/1e3:.4g} kHz ({counts[det]:.3g} counts)')
    print(f'    captures: {ncounts[det]/T:.4g} Hz ({ncounts[det]:.3g} counts)')


print('counts:')
print(counts)

print('ncounts:')
print(ncounts)

