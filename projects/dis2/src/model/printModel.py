#!/usr/bin/python 

import KeppelModel 

# beam energy 
beam = 5.75

# QQ Binning Parametesr 
numberQQBins = 40
qqMin = 1.0
qqMax = 4.8 
qqWidth = (qqMax-qqMin)/numberQQBins

# W Binning Parameters 
numberWBins = 40
wMin = 2.05
wMax = 3.1 
wWidth = (wMax-wMin)/numberWBins

keppel = []
print "# Keppel Model for Inclusive Events"
print "# numberQQBins={0} Min={1} Max={2}".format(numberQQBins, qqMin, qqMax) 
print "# numberWBins={0} Min={1} Max={2}".format(numberWBins, wMin, wMax) 

for i in range(1,numberQQBins):
    for j in range(1, numberWBins):
        qq = qqMin + i*qqWidth
        w = wMin + j*wWidth 

        xs = KeppelModel.tkeppel(beam,qq,w)
        keppel.append(xs)
        print "{0} {1} {2}".format(i, j, xs)

