#! /usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np

class DataManager:
    def __init__(self, file):
        self.fea_file = file

    def load_file(self):
        ## data format 
        ## feature_lines fea_number
        ## fea_type_name1   fea_type_name2  ... target_variable
        with open(self.fea_file, "r") as fd:
            lines, fea_count = fd.next().split('\t') 
            data = np.empty(( int(lines), int(fea_count) ))
            target = np.empty( (int(lines), ) )
            fea_names = np.array(fd.next().split('\t')[:-1])

            for i in xrange(int(lines)):
                line = fd.next()
                d = line.split('\t')
                data[i] = np.asarray(d[:-1], dtype=np.float)
                target[i] = np.asarray(d[-1], dtype=np.int)

            self.X = data
            self.Y = target
            self.names = fea_names
