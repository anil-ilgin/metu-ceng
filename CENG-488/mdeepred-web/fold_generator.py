import pandas as pd
import numpy as np
import random
import sys
import simplejson

def main_split():
    pf = pd.read_csv("comp_targ_affinity.csv", sep="\t", header=0, low_memory=False, skipinitialspace=True)
    total_rows = len(pf.index)
    file = open("train_fold_setting1.txt", "a")
    array = list(range(0, total_rows))
    random.shuffle(array)
    five_split = np.array_split(array, 5)
    total_array = []
    for ls in five_split:
        total_array.append(list(ls))
    sys.stdout = file
    print(total_array)
    file.close()

def main_test():
    pf = pd.read_csv("test_comp_targ_affinity.csv", sep="\t", header=0, low_memory=False, skipinitialspace=True)
    total_rows = len(pf.index)
    file = open("test_fold_setting1.txt", "a")
    array = list(range(0, total_rows))
    random.shuffle(array)
    sys.stdout = file
    print(array)
    file.close()

main_split()
main_test()