#! /usr/bin/env python
# -*- coding: utf-8 -*-

from sklearn import tree
from data_load import DataManager

if __name__ == '__main__':
    dm_train = DataManager('data1/train12')
    dm_train.load_file()
    model = tree.DecisionTreeClassifier(criterion='entropy', min_samples_leaf=16) 
    model.fit(dm_train.X, dm_train.Y)
    model.score(dm_train.X, dm_train.Y)
    dm_test = DataManager('data1/test12')
    dm_test.load_file()
    predicted = model.predict(dm_test.X)
    l = len(predicted)
    for i in predicted:
        print i
