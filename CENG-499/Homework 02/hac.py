import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.function_base import average
import sys


def single_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the single linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    min_arr = []
    for i in c1:
        min_arr.append(np.min(np.linalg.norm(c2 - i, axis=1)))
    return np.min(np.array(min_arr))



def complete_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the complete linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    max_arr = []
    for i in c1:
        max_arr.append(np.max(np.linalg.norm(c2 - i, axis=1)))
    return np.max(np.array(max_arr))


def average_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the average linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    avg_arr = []
    for i in c1:
        avg_arr.append(np.sum(np.linalg.norm(c2 - i, axis=1))/c2.shape[0])
    return np.sum(np.array(avg_arr))/c1.shape[0]



def centroid_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the centroid linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    return np.linalg.norm(np.average(c1, axis=0) - np.average(c2, axis=0))


def hac(data, criterion, stop_length):
    """
    Applies hierarchical agglomerative clustering algorithm with the given criterion on the data
    until the number of clusters reaches the stop_length.
    :param data: An (N, D) shaped numpy array containing all of the data points.
    :param criterion: A function. It can be single_linkage, complete_linkage, average_linkage, or
    centroid_linkage
    :param stop_length: An integer. The length at which the algorithm stops.
    :return: A list of numpy arrays with length stop_length. Each item in the list is a cluster
    and a (Ni, D) sized numpy array.
    """
    clusters = [[x] for x in data]
    while len(clusters) > stop_length:
        print(len(clusters))
        min_val, ind_i, ind_j = np.Inf, -1, -1
        i = 0
        while i < len(clusters):
            c1 = clusters[i]
            for j in range(i+1,len(clusters)):
                dist = criterion(np.array(c1), np.array(clusters[j]))
                if dist < min_val:
                    min_val, ind_i, ind_j = dist, i, j
            i += 1
        clusters.append(clusters[ind_i] + clusters[ind_j])
        if ind_i < ind_j:
            clusters.pop(ind_i)
            clusters.pop(ind_j-1)
        else:
            clusters.pop(ind_i)
            clusters.pop(ind_j)
        
    return [np.array(x) for x in clusters]

def main():
    args = sys.argv
    dataset_choice, link_choice, k = int(args[1]), int(args[2]), int(args[3])
    dataset = np.load("hac/dataset{}.npy".format(dataset_choice))
    print("Dataset {} is choosen with k = {}, starting HAC. \n".format(dataset_choice, k))
    if link_choice == 0:
        clusters = hac(dataset, single_linkage, k)
    elif link_choice == 1:
        clusters = hac(dataset, complete_linkage, k)
    elif link_choice == 2:
        clusters = hac(dataset, average_linkage, k)
    else:
        clusters = hac(dataset, centroid_linkage, k)
    for i in range(k):
        vals = clusters[k]
        plt.scatter(vals[:,0],vals[:,1])
    plt.show()

main()
