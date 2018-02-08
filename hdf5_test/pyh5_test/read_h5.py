#!/usr/bin/python2.7
#_*_encoding=utf-8_*_

import h5py
import numpy as np

with h5py.File('/home/yz/myPrograms/peopleCounting/gst_video_player/cmake-build-debug/vidf1_33_000.y_data.h5','r') as f:
    data = f['data']
    a = data[0:20]
    print np.sum(a[0,1])
    pass