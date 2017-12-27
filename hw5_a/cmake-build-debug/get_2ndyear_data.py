#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 27 16:49:18 2017

@author: wrk
"""

import pandas as pd
import numpy as np
from pandas import Series
from pandas import DataFrame
data0 = pd.read_csv('data.csv')
original_data = pd.read_csv('original_data.csv')



weight_vector = data0.ix[0]
if_in_list = [0]*947
for i in range(947):
    if weight_vector[i]>0.00001:
        if_in_list[i] = 1
        
if_in_vector = Series(if_in_list)


opt_data = Series()
dic = {}
for i in range(947):
    if if_in_list[i] == 1:
        dic[i] = original_data.ix[i].T
       


dfdic = pd.DataFrame(dic).T
#dfdic.to_csv('2second_year.csv')

detailed_weight = []
for i in range(947):
    if weight_vector[i]>0.00001:
        detailed_weight.append(weight_vector[i])
df_weight = pd.DataFrame(detailed_weight).T
df_weight.to_csv('weight.csv')