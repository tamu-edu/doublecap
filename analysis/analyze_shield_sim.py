import numpy as np
import ROOT
import pickle
from shield_optimization import ShieldOptimization


if __name__ == '__main__':

    data_folder = '/scratch/user/ajbiffl3/doublecap/data/shield_sim/'

    opt = ShieldOptimization(folder = data_folder, Ndefault = 2.5e8)
    opt.analyze()
    print(opt.rates)

    with open('/home/ajbiffl3/soft/doublecap/analysis/analysis_data/shield_opt.dat', 'wb') as file:
        pickle.dump(opt, file)

    print('done')
