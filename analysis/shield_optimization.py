"""
shield_optimization - module for analysis of shield optimization simulations

includes classes:

ShieldOptimization - general utility class for finding, storing, and organizing simulation output files, reading output files and calculating rates, and storing aggregate data
"""

import numpy as _np
import ROOT as _ROOT
import glob as _glob



class ShieldOptimization:

    __primary_dict__ = {
        #
    }

    _mCu = 37000000 # one millicurie in Bq

    def __init__(self, match = 'Lead*_PE*_*_t?.root', folder = '../build/shield_sim/', Ndefault = 1e8, activity = _mCu, verbose = 1, part = 10):
        self.verbosity = verbose
        self.part = part

        self.match = match
        self.dir = folder
        self.glob_list = _glob.glob(self.dir + self.match)
        self.filename_list = [path.split('/')[-1] for path in self.glob_list]
        self.N = len(self.filename_list)

        self.default_Nprimaries = Ndefault

        self.activity = activity

        self.detectors = ['highmass1', 'highmass2', 'lowmass']
        self.processes = ['hadElastic', 'nCapture']

        self.rates_accessor = {det: {proc: (i,j) for j, proc in enumerate(self.processes)} for i, det in enumerate(self.detectors)}


    def exposure_time(self, Nprimaries):
        return Nprimaries/self.activity


    def analyze(self):
        """
        do all the main analysis steps
        """
        self.pre_analyze()
        self.read_rates()
        self.collect_rates()


    def pre_analyze(self):
        self.collect_files()
        self.calculate_primaries()
        self.assign_primaries()
        self.get_exposure_times()


    
    def collect_files(self):
        """
        - find and categorize all files found in data directory
        - add file information (lead thickness, poly thickness, timestamp, etc) to lists
        - collect file "bases" (core of filename corresponding to single run with multiple threads) into dictionary
        """

        if self.verbosity > 0:
            print(f'collecting {self.N} files')

        self.leads = []
        self.polys = []
        self.threads = []
        self.timestamps = []
        self.files_found = {}
        self.measurements_list = []
        self.measurements = []

        for file in self.filename_list:

            fbase = ''.join(file.split('_t')[:-1])
            if fbase not in self.files_found:
                self.files_found[fbase] = []

            comps = file.split('_')
            if 'Lead' in comps[0]:
                lead = float(comps[0][4:])
            elif 'Air' in comps[0]:
                lead = float(comps[0][3:])
            else:
                print('problem with ' + file + ': no lead')

            if 'PE' in comps[1]:
                poly = float(comps[1][2:])
            else:
                print('problem with ' + file + ': no poly')
            
            if '.root' in comps[-1]:
                t = int(comps[-1][1:-5])

            timestamp = '_'.join(comps[2:-1])

            self.leads.append(lead)
            self.polys.append(poly)
            self.threads.append(t)
            self.timestamps.append(timestamp)
            self.files_found[fbase].append(t)

            if (lead,poly) not in self.measurements_list:
                self.measurements_list.append((lead,poly))
            self.measurements.append((lead,poly))

    def calculate_primaries(self):
        """
        take dictionary of file bases and corresponding thread indices and assign number of primaries in each file to 
        """
        self.primaries_dict = {}
        for fbase, idxs in self.files_found.items():
            Nfiles = len(idxs)
            if any(i not in idxs for i in range(max(idxs)+1)):
                print(fbase, ': not all found')
            if fbase in self.__primary_dict__:
                Nprimaries = self.__primary_dict__[fbase] 
            else:
                Nprimaries = self.default_Nprimaries
            self.primaries_dict[fbase] = Nprimaries/Nfiles

    def assign_primaries(self):
        """
        take calculated primaries for each file base and assign to each file
        """
        self.primaries = []
        for file in self.filename_list:
            fbase = ''.join(file.split('_t')[:-1])
            self.primaries.append(self.primaries_dict[fbase])

    def calculate_rate(self, i):
        """
        calculate rates of hadElastic/nCapture in highmass1/highmass2/lowmass in the file at the specified index i

        returns 3x2 array (rows hm1/hm2/hm3, columns hadElastic/nCapture)
        """
        time = self.exposure_time(self.primaries[i])
        Nevts = _np.zeros((3,2))

        tfile = _ROOT.TFile.Open(self.glob_list[i], 'READ')
        tree = tfile.Get('tree')
        N = tree.GetEntries()

        last_event = -1
        hit = _np.zeros((3,2))

        for k in range(N):
            tree.GetEntry(k)
            EventNum = int(getattr(tree, 'EventNum'))
            VolName = getattr(tree, 'VolName')
            ProcName = getattr(tree, 'ProcName')
            Time = float(getattr(tree, 'Time'))

            if abs(EventNum - last_event) > 1e-6: # new event

                Nevts += hit
                hit = _np.zeros((3,2))
                last_event = EventNum

            if (Time < 1e9*360*24*365*100):
                for i, volname in enumerate(self.detectors):
                    for j, procname in enumerate(self.processes):
                        if VolName == volname and ProcName == procname:
                            hit[i,j] = 1
                        

        tfile.Close()

        rate = Nevts/time

        return Nevts, rate, time


    def get_exposure_times(self):
        """
        prepare rate data lists
        """
        self.exposure_time_list = []

        for i in range(self.N):
            time = self.exposure_time(self.primaries[i])
            self.exposure_time_list.append(time)

        self.meas_exposures = {meas: 0 for meas in self.measurements_list}
        for i in range(self.N):
            self.meas_exposures[self.measurements[i]] += self.exposure_time_list[i]


    def read_rates(self):
        """
        prepare rate data lists
        """
        self.rate_list = []
        self.count_list = []

        for i in range(len(self.filename_list)):
            cts, rt, time = self.calculate_rate(i)
            self.count_list.append(cts)
            self.rate_list.append(rt)
            if i%(self.N//self.part) == 0 and self.verbosity > 2:
                print(f'{i+1}/{self.N} done')


    def collect_rates(self):
        """
        calculate rates for specified event types for specific measurements
        """
        self.rates = {meas: _np.zeros((3,2)) for meas in self.measurements_list}
        self.drates = {meas: _np.zeros((3,2)) for meas in self.measurements_list}
        self.totals = {meas: _np.zeros((3,2)) for meas in self.measurements_list}
        self.total_exposure = {meas: 0 for meas in self.measurements_list}

        for i in range(len(self.filename_list)):
            self.totals[self.measurements[i]] += self.count_list[i]
            self.total_exposure[self.measurements[i]] += self.exposure_time_list[i]

        for meas in self.measurements_list:
            self.rates[meas] = self.totals[meas]/self.total_exposure[meas]
            self.drates[meas] = _np.sqrt(self.totals[meas])/self.total_exposure[meas]


    def get_rate(self, lead, poly, det = 'lowmass', proc = 'nCapture'):
        return self.rates[(lead,poly)][self.rates_accessor[det][proc]]

    def get_drate(self, lead, poly, det = 'lowmass', proc = 'nCapture'):
        return self.drates[(lead,poly)][self.rates_accessor[det][proc]]


    def get_capture_rate(self):
        lead = []
        poly = []
        rate = []
        drate = []
        for l,p in self.measurements_list:
            lead.append(l)
            poly.append(p)
            rate.append(self.rates[(l,p)][(2,1)])
            drate.append(self.drates[(l,p)][(2,1)])

        return _np.array(lead), _np.array(poly), _np.array(rate), _np.array(drate)


class BackgroundRates(ShieldOptimization):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.processes = ['all', 'nCapture']


    def calculate_rate(self, i):
        """
        calculate rates of hadElastic/nCapture in highmass1/highmass2/lowmass in the file at the specified index i

        returns 3x2 array (rows hm1/hm2/hm3, columns total/nCapture)
        """
        time = self.exposure_time(self.primaries[i])
        Nevts = _np.zeros((3,2))

        tfile = _ROOT.TFile.Open(self.glob_list[i], 'READ')
        tree = tfile.Get('tree')
        N = tree.GetEntries()

        last_event = -1
        hit = _np.zeros((3,2))

        for k in range(N):
            tree.GetEntry(k)
            EventNum = int(getattr(tree, 'EventNum'))
            VolName = getattr(tree, 'VolName')
            ParentVol = getattr(tree, 'ParentVol')
            ProcName = getattr(tree, 'ProcName')
            Time = float(getattr(tree, 'Time'))

            if abs(EventNum - last_event) > 1e-6: # new event

                Nevts += hit
                hit = _np.zeros((3,2))
                last_event = EventNum

            if (Time < 1e9*360*24*365*100):
                for i, volname in enumerate(self.detectors):
                    if VolName == volname:
                        hit[i,0] = 1
                        if ProcName == 'nCapture' and VolName == ParentVol:
                            hit[i,1] = 1

        tfile.Close()

        rate = Nevts/time

        return Nevts, rate, time

