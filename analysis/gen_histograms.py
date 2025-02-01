import numpy as np
import pandas as pd
import ROOT
import glob
import sys
import os

data_dir = '/scratch/user/ajbiffl3/doublecap/data/rate_sim/'
output_dir = '/home/ajbiffl3/soft/doublecap/analysis/analysis_data/'

default_num = 5e7

primary_dict = {
    'simdata_20250131_145827': 5e7,
    'simdata_20250131_155936_0001': 5e7,
    'simdata_20250131_155936_0002': 5e7
}

# dict to hold [file path -- thread counts]
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



Emin = 0.5e-6
Emax = 20 # MeV
Nbins = 200

# energy bins
Ebins = np.geomspace(Emin, Emax, Nbins+1, endpoint = False) # bin edges (MeV)
dE = (Ebins[1:] - Ebins[:-1]).mean()
Emids = Ebins + dE/2 # bin centers


detectors = ['lowmass', 'highmass1', 'highmass2'] # detectors
codetectors = ['lowmassxhighmass1', 'lowmassxhighmass2', 'highmass1xhighmass2', 'lowmassxhighmass1xhighmass2'] # coincidences
Qs = ['NR', 'ER', 'Ep', 'Eq'] # recoil types; Ep = total phonon ER+NR, Eq = total charge ER+Lindhard(NR)

Ncaptures = {det: 0 for det in detectors} # number of neutron captures in each detector
Nhits = {det: 0 for det in detectors} # total number of entries in trees

ROOT.TH1.AddDirectory(false)

# all hits
# all_events[q][d] = histogram of events (true energy) of q recoils in detector d
all_events = {Q: {det: ROOT.TH1D(
        det + '_evts_' + Q, 
        det + ' all ' + Q + ' events',
        Nbins, Ebins
        ) for det in detectors} for Q in Qs}

# hits in coincidence with a hit in the lowmass
# coincidence_events[axb][c][q] = histogram of events in detector c that have coincidence in a & b (c must be either a or b) of recoil type q
coincidence_events = {detdet: {det: {Q: ROOT.TH1D(
            det + '_coinc_' + detdet , 
            detdet + ' coincidence in ' + det + ' ' + Q + ' events ',
            Nbins, Ebins
        ) for Q in Qs
    } for det in detdet.split('x')
} for detdet in codetectors}





def get_evt_type(PName):
    if PName in 'e- e+ gamma proton alpha deuteron':
        return 'ER'
    elif PName in 'P31 ' or PName[:2] in 'Si Al Mg Ge Ga As Se': 
        return 'NR'
    else:
        raise Exception('unknown PName = ' + PName + ', ProcName = ' + getattr(tree, 'ProcName'))


def yLind(Er):
    Er *= 1e3 # convert to keV from MeV
    Z = 28
    k = 0.15
    eps = 11.5*Er/np.cbrt(Z)**7
    g = 3*eps**0.15 + 0.7*eps**0.6 + eps
    Y = k*g/(1+k*g) # yield
    return Y 




for det in detectors:
    for Q in Qs:
        all_events[Q][det].Reset()

for detdet in codetectors:
    for det in detdet.split('x'):
        for Q in Qs:
            coincidence_events[detdet][det][Q].Reset()


lowmass_captures = {}

for file in filenames:
    #print(file)

    last_event = -1

    Edep_evt = {Q: {det: 0. for det in detectors} for Q in Qs} # Edep in each detector
    
    # open file
    tfile = ROOT.TFile.Open(data_dir + file, 'READ')
    tree = tfile.Get('tree')
    N = tree.GetEntries()
    n = tree.GetEntries('IsCapture') # number of captures in lowmass

    N = tree.GetEntries()

    
    for k in range(N):

        tree.GetEntry(k)

        EventNum = int(getattr(tree, 'EventNum'))
        TrkNum = int(getattr(tree, 'TrkNum'))
        Edep = float(getattr(tree, 'Edep'))
        VolName = getattr(tree, 'VolName')
        PName = getattr(tree, 'PName')
        ProcName = getattr(tree, 'ProcName')
        ParentVol = getattr(tree, 'ParentVol')
        IsCapture = int(getattr(tree, 'IsCapture'))

        q = get_evt_type(PName)

        Nhits[VolName] += 1
        if (ProcName == 'nCapture') and (ParentVol in detectors):
            Ncaptures[ParentVol] += 1
            if ParentVol == 'lowmass':
                if file not in lowmass_captures:
                    lowmass_captures[file] = [EventNum]
                elif EventNum not in lowmass_captures[file]:
                    lowmass_captures[file].append(EventNum)



        if abs(EventNum - last_event) > 1e-6: # if new event
            last_event = EventNum 

            # list of detectors where there were hits
            yes_hit = [det for det in detectors if any(Edep_evt[Q][det] > 0 for Q in Qs)]

            co = ''
            if len(yes_hit) > 1:
                co = 'x'.join(yes_hit)
            
            # fill data into histograms
            # Edep_evt[det] holds the total energy [MeV] from the event
            for det in detectors:
                # fill histograms 
                for Q in Qs:
                    all_events[Q][det].Fill(Edep_evt[Q][det])

                    if co and (det in co):
                        coincidence_events[co][det][Q].Fill(Edep_evt[Q][det])


            # reset variables
            for det in detectors:
                for Q in Qs:

                    Edep_evt[Q][det] = 0.
           
        
        # all events: add current Edep (of the correct type) to the totals

        Edep_evt[q][VolName] += Edep # add to corresponding event type

        # charge channels: NR get lindhard yield
        # phonon channels: Er plus charge channels times V/eps
        # assume 4 volts
        V = 4
        eps = 3.8
        if q == 'ER':
            Eq = Edep
        elif q == 'NR':
            Eq = Edep*yLind(Edep)

        Edep_evt['Eq'][VolName] += Eq
        Edep_evt['Ep'][VolName] += Edep + Eq*V/eps
        
    tfile.Close()


for det in detectors:
    print(f'{det}: {Nhits[det]:.3g} hits total, {Ncaptures[det]} capture events in {det}')



# array to hold data
cts = np.zeros(Nbins+1)

# all hits
for Q in Qs:
    for det in detectors:
        hist = all_events[Q][det]
        name = hist.GetName()
        
        for i in range(1, Nbins+1):
            cts[i-1] = hist.GetBinContent(i)
        
        df = pd.DataFrame.from_dict({'E': Ebins, 'count': cts})
        df.to_csv(output_dir + name + '.txt', index = False)


# coincidences
for detdet in codetectors:
    for det in detdet.split('x'):
        for Q in Qs:
            hist = coincidence_events[detdet][det][Q]
            name = hist.GetName()
            
            for i in range(1, Nbins+1):
                cts[i-1] = hist.GetBinContent(i)
            
            df = pd.DataFrame.from_dict({'E': Ebins, 'count': cts})
            df.to_csv(output_dir + name + '.txt', index = False)



print('completed')
