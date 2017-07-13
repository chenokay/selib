#! /usr/bin/env python
# -*- coding: utf-8 -*-

from sklearn import tree
from data_load import DataManager

if __name__ == '__main__':
    dm_train = DataManager('data/train')
    dm_train.load_file()
    model = tree.DecisionTreeClassifier(criterion='gini') 
    model.fit(dm_train.X, dm_train.Y)
    model.score(dm_train.X, dm_train.Y)
    dm_test = DataManager('data/test')
    dm_test.load_file()
    predicted = model.predict(dm_test.X)
    l = len(predicted)
    tp = 0
    fp = 0
    tn = 0
    fn = 0
    for i in xrange(l):
        if predicted[i] == dm_test.Y[i] and predicted[i] == 1:
            tp += 1
        elif predicted[i] != dm_test.Y[i] and predicted[i] == 1:
            fp += 1
        elif predicted[i] == dm_test.Y[i] and predicted[i] == 0:
            tn += 1
        elif predicted[i] != dm_test.Y[i] and predicted[i] == 0:
            fn += 1

    print tp, fp, tp*1.0/(tp+fn), tp*1.0/(tp+fp)
    print tn, fn, tn*1.0/(tn+fp), tn*1.0/(tn+fn)

