#! /usr/bin/env python
# -*- coding: utf-8 -*-

from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MultiLabelBinarizer
from sklearn.preprocessing import PolynomialFeatures
from data_load import DataManager
import numpy as np

if __name__ == '__main__':
    dm_train = DataManager('data1/train13')
    dm_train.load_file()
    model = LogisticRegression(penalty='l1')
    mb = MultiLabelBinarizer()
    TX = mb.fit_transform(dm_train.X)
    model.fit(TX, dm_train.Y)
    model.score(TX, dm_train.Y)
    dm_test = DataManager('data1/test13')
    dm_test.load_file()

    test_TX = mb.fit_transform(dm_test.X)

    predicted = model.predict(test_TX)
    l = len(predicted)
    for i in predicted:
        print i
