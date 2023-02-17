import numpy as np

import numpy as np
from numpy.core.numeric import cross
import matplotlib.pyplot as plt


def calculate_distances(train_data, test_instance, distance_metric):
    """
    Calculates Manhattan (L1) / Euclidean (L2) distances between test_instance and every train instance.
    :param train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data.
    :param test_instance: A (D, ) shaped numpy array.
    :param distance_metric: A string which indicates the distance metric, it can be either 'L1' or 'L2'
    :return: An (N, ) shaped numpy array that contains distances.
    """
    if distance_metric == 'L1':
        distance = np.sum(np.absolute((train_data - test_instance)), axis=1)
    else:
        distance = np.linalg.norm(train_data-test_instance, axis=1)
    return distance



def majority_voting(distances, labels, k):
    """
    Applies majority voting. If there are more then one major class, returns the smallest label.
    :param distances: An (N, ) shaped numpy array that contains distances
    :param labels: An (N, ) shaped numpy array that contains labels
    :param k: An integer. The number of nearest neighbor to be selected.
    :return: An integer. The label of the majority class.
    """

    parts = np.argpartition(distances, k)[:k]
    votes = labels[parts]
    unique, counts = np.unique(votes, return_counts=True)
    return unique[np.argmax(counts)]


def knn(train_data, train_labels, test_data, test_labels, k, distance_metric):
    """
    Calculates accuracy of knn on test data using train_data.
    :param train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param train_labels: An (N, ) shaped numpy array that contains labels
    :param test_data: An (M, D) shaped numpy array where M is the number of examples
    and D is the dimension of the data
    :param test_labels: An (M, ) shaped numpy array that contains labels
    :param k: An integer. The number of nearest neighbor to be selected.
    :param distance_metric: A string which indicates the distance metric, it can be either 'L1' or 'L2'
    :return: A float. The calculated accuracy.
    """
    acc_tot = 0
    for i in range(len(test_data)):
        dist = calculate_distances(train_data, test_data[i], distance_metric)
        label = majority_voting(dist, train_labels, k)
        if label == test_labels[i]:
            acc_tot += 1
    return acc_tot / len(test_data)

def split_train_and_validation(whole_train_data, whole_train_labels, validation_index, k_fold):
    """
    Splits training dataset into k and returns the validation_indexth one as the
    validation set and others as the training set. You can assume k_fold divides N.
    :param whole_train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param whole_train_labels: An (N, ) shaped numpy array that contains labels
    :param validation_index: An integer. 0 <= validation_index < k_fold. Specifies which fold
    will be assigned as validation set.
    :param k_fold: The number of groups that the whole_train_data will be divided into.
    :return: train_data, train_labels, validation_data, validation_labels
    train_data.shape is (N-N/k_fold, D).
    train_labels.shape is (N-N/k_fold, ).
    validation_data.shape is (N/k_fold, D).
    validation_labels.shape is (N/k_fold, ).
    """
    train_sets = np.array_split(whole_train_data, k_fold)
    label_sets = np.array_split(whole_train_labels, k_fold)
    ret_train_sets = np.delete(train_sets, validation_index, 0)
    ret_train_sets = np.concatenate(ret_train_sets)

    ret_label_sets = np.delete(label_sets, validation_index, 0)
    ret_label_sets = np.concatenate(ret_label_sets)
    return ret_train_sets, ret_label_sets, train_sets[validation_index],label_sets[validation_index]


def cross_validation(whole_train_data, whole_train_labels, k_fold, k, distance_metric):
    """
    Applies k_fold cross-validation and averages the calculated accuracies.
    :param whole_train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param whole_train_labels: An (N, ) shaped numpy array that contains labels
    :param k_fold: An integer.
    :param k: An integer. The number of nearest neighbor to be selected.
    :param distance_metric: A string which indicates the distance metric, it can be either 'L1' or 'L2'
    :return: A float. Average accuracy calculated.
    """
    acc_arr = []
    for i in range(k_fold):
        train_set, label_train, test_set, label_test = \
        split_train_and_validation(whole_train_data, whole_train_labels, i, k_fold)
        acc = knn(train_set, label_train, test_set, label_test, k, distance_metric)
        acc_arr.append(acc)
    return np.average(acc_arr)

def plotter():
    
    train_set = np.load( 'knn/train_set.npy')
    train_labels = np.load( 'knn/train_labels.npy')
    test_set = np.load('knn/test_set.npy')
    test_labels = np.load( 'knn/test_labels.npy')
    
    maxval = 0
    max_k = 0
    x_axis = []
    y_axis = []
    f = open("KNN_Results.txt", 'a')
    for elem in range(1,179):
        cv = cross_validation(train_set, train_labels, 10,elem, 'L2')
        x_axis.append(elem)
        y_axis.append(cv)
        if cv > maxval:
            maxval, max_k = cv, elem
        f.write("Accuracy for k={} is {} \n".format(elem, cv) )
    f.write("\n Max Acc is k={} with {} \n".format(maxval, max_k))
    cv = knn(train_set, train_labels, test_set, test_labels, max_k, 'L2')
    f.write("\n Test Accuracy for Best k={} is {} \n".format(max_k, cv))
    plt.xlabel("K Values")
    plt.ylabel("10-Fold Average Accuracy")
    plt.plot(x_axis, y_axis)
    plt.show()

plotter()
