#! /usr/bin/env python

import sklearn
import os
import math

## n rows 
## n class for every row
## {"k1":[], "k2":[], "k3":[], ... }
def entroy(ratio_vec):
    en_dict = {}
    for key, ele in ratio_vec.items():
        sum = 0
        for i in xrange(len(ele)):
            if float(ele[i]) > 0:
                sum = sum + (float(ele[i]) * math.log(float(ele[i]), 2) * (-1))

        en_dict[key] = sum

    return en_dict

## file has n column
## last column is target
## n-1 columns is feature
def chi_test(file, sep, col_num):
    err_line_count = 0
    tar_vec = []
    fea_vec = []
    with open(file, "r") as fd:
        for line in fd:
            items = line.split(sep)
            if len(items) != col_num:
                continue
            target = items.pop()
            fea_vec.append(items)      
            tar_vec.append(target)

    return sklearn.feature_selection.univariate_selection.chi2(fea_vec, tar_vec)

if __name__ == '__main__':
    print entroy({"k1":[5.0/6, 1.0/6], "k2":[4.0/5, 1.0/5], "k3":[3.0/4, 1.0/4], "k4":[1.0/2, 1.0/2], "k5":[1.0/4, 3.0/4], "k6":[1.0/5, 4.0/5], "k7":[1.0/6, 5.0/6] })
    #print chi_test()
