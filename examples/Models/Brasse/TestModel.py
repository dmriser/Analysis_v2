#!/usr/bin/python 

import BrasseModel
import KeppelModel 
import matplotlib.pyplot as plt
beam = 5.75
numberQQBins = 100
qqMin = 1.5
qqMax = 4.0 
qqWidth = (qqMax-qqMin)/numberQQBins
#numberWBins = 100
#wMin = 1.1
#wMax = 2.9 
#wWidth = (wMax-wMin)/numberWBins

keppel = []

for i in range(1,numberQQBins):
    qq = qqMin + i*qqWidth
    #        w = wMin + j*wWidth 
    w = 1.21
#        xs = BrasseModel.brasse(beam,qq,w)
    xs = KeppelModel.tkeppel(beam,qq,w)
    keppel.append(xs)
    print "{0} {1} {2}".format(qq, w, xs)

plt.plot(keppel)
plt.show()
