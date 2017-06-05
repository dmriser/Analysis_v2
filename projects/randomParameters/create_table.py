import numpy as np 
import glob

header = ["FILE","EL_VZ_MIN", "EL_VZ_MAX", "EL_ECU_MAX", "EL_ECU_MIN", 
        "EL_ECV_MAX", "EL_ECW_MAX", "EL_CC_FIDA", "EL_CC_FIDC", 
        "EL_DCR1_FIDA", "EL_DCR1_FIDH", "EL_DCR3_FIDA", "EL_DCR3_FIDH", 
        "EL_EC_NSIGMA", "EL_CCT_NSIGMA", "KP_TOFMASS_MIN"]

pars = ["EL_VZ_MIN", "EL_VZ_MAX", "EL_ECU_MAX", "EL_ECU_MIN", 
        "EL_ECV_MAX", "EL_ECW_MAX", "EL_CC_FIDA", "EL_CC_FIDC", 
        "EL_DCR1_FIDA", "EL_DCR1_FIDH", "EL_DCR3_FIDA", "EL_DCR3_FIDH", 
        "EL_EC_NSIGMA", "EL_CCT_NSIGMA", "KP_TOFMASS_MIN"]

print ', '.join(header) 

files = glob.glob('random_pars/*.pars')

for file in files:
    values = []
    filenumber = [s for s in file if s.isdigit()]
    filenum = ''.join(filenumber)

    values.append(filenum)

    for par in pars:
        matches = [ line.split()[3] for line in open(file) if par in line ]
        values.append(matches[0])


    print ', '.join(values)


