#! /usr/bin/env python
# -*- coding: utf-8 -*-

from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MultiLabelBinarizer
from sklearn.preprocessing import PolynomialFeatures
from data_load import DataManager
import numpy as np

if __name__ == '__main__':
    dm_train = DataManager('data1/train15')
    dm_train.load_file()
    model = LogisticRegression(penalty='l2')
    mb = MultiLabelBinarizer()
    ret=np.hsplit(dm_train.X, (5,7))
    TX = mb.fit_transform(ret[0])
    train_X = np.hstack((TX, ret[1]))
    model.fit(train_X, dm_train.Y)
    model.score(train_X, dm_train.Y)
    dm_test = DataManager('data1/test15')
    dm_test.load_file()
    ret = np.hsplit(dm_test.X, (5,7))
    test_TX = mb.fit_transform(ret[0])
    test_X = np.hstack((test_TX, ret[1]))
    predicted = model.predict(test_X)
    l = len(predicted)
    for i in predicted:
        print i
