import numpy as np
import math

def entropy(bucket):
    """
    Calculates the entropy.
    :param bucket: A list of size num_classes. bucket[i] is the number of
    examples that belong to class i.
    :return: A float. Calculated entropy.
    """
    arr = np.array(bucket)
    nonzero_arr = arr[arr != 0] / np.sum(arr)
    if nonzero_arr.shape[0] == 0:
        return 0
    f = lambda t : -math.log2(t) * t
    vec = np.vectorize(f)
    ret_val = np.sum(vec(nonzero_arr))
    return ret_val


def info_gain(parent_bucket, left_bucket, right_bucket):
    """
    Calculates the information gain. A bucket is a list of size num_classes.
    bucket[i] is the number of examples that belong to class i.
    :param parent_bucket: Bucket belonging to the parent node. It contains the
    number of examples that belong to each class before the split.
    :param left_bucket: Bucket belonging to the left child after the split.
    :param right_bucket: Bucket belonging to the right child after the split.
    :return: A float. Calculated information gain.
    """
    tot = sum(parent_bucket)
    par, left, right = entropy(parent_bucket), entropy(left_bucket), entropy(right_bucket)
    ret_val = par - (left * sum(left_bucket) / tot) - (right * sum(right_bucket) / tot)
    return ret_val

def gini(bucket):
    """
    Calculates the gini index.
    :param bucket: A list of size num_classes. bucket[i] is the number of
    examples that belong to class i.
    :return: A float. Calculated gini index.
    """
    arr = np.array(bucket)
    sums = np.sum(arr)
    if sums == 0:
        return 1
    f = lambda t : ( t / sums ) ** 2
    vec = np.vectorize(f)
    ret_val = np.sum(vec(arr))
    return 1 - ret_val


def avg_gini_index(left_bucket, right_bucket):
    """
    Calculates the average gini index. A bucket is a list of size num_classes.
    bucket[i] is the number of examples that belong to class i.
    :param left_bucket: Bucket belonging to the left child after the split.
    :param right_bucket: Bucket belonging to the right child after the split.
    :return: A float. Calculated average gini index.
    """
    lg, rg, lc, rc = gini(left_bucket), gini(right_bucket), sum(left_bucket), sum(right_bucket)
    return ( (lg * lc) + (rg * rc) ) / (rc + lc)


def calculate_split_values(data, labels, num_classes, attr_index, heuristic_name):
    """
    For every possible values to split the data for the attribute indexed by
    attribute_index, it divides the data into buckets and calculates the values
    returned by the heuristic function named heuristic_name. The split values
    should be the average of the closest 2 values. For example, if the data has
    2.1 and 2.2 in it consecutively for the values of attribute index by attr_index,
    then one of the split values should be 2.15.
    :param data: An (N, M) shaped numpy array. N is the number of examples in the
    current node. M is the dimensionality of the data. It contains the values for
    every attribute for every example.
    :param labels: An (N, ) shaped numpy array. It contains the class values in
    it. For every value, 0 <= value < num_classes.
    :param num_classes: An integer. The number of classes in the dataset.
    :param attr_index: An integer. The index of the attribute that is going to
    be used for the splitting operation. This integer indexs the second dimension
    of the data numpy array.
    :param heuristic_name: The name of the heuristic function. It should either be
    'info_gain' of 'avg_gini_index' for this homework.
    :return: An (L, 2) shaped numpy array. L is the number of split values. The
    first column is the split values and the second column contains the calculated
    heuristic values for their splits.
    """
    sort_arr = np.argsort(data[:, attr_index])
    sort_data = data[sort_arr]
    sort_label = labels[sort_arr]
    att = sort_data[:, attr_index]

    ret_arr = np.empty((len(att) - 1,2))
    for i in range(len(att) - 1):
        left_b = np.zeros(num_classes)
        right_b = np.zeros(num_classes)
        parent_b = np.zeros(num_classes)
        avg = (att[i] + att[i+1]) / 2
        ind = np.searchsorted(att, avg)
        l, r = sort_label[:ind], sort_label[ind:]
        p = np.copy(sort_label)
        lb, rb, pb  = np.bincount(l), np.bincount(r), np.bincount(p)
        lb = np.pad(lb, (0,num_classes - len(lb)))
        rb = np.pad(rb, (0,num_classes - len(rb)))
        pb = np.pad(pb, (0,num_classes - len(pb)))
        left_b[:len(lb) + 1], right_b[:len(rb) + 1], parent_b[:len(pb) + 1] = lb, rb, pb
        if heuristic_name == 'info_gain':
            res = info_gain(parent_b, left_b, right_b)
            ret_arr[i] = np.array([avg, res])
        else:
            res = avg_gini_index(left_b, right_b)
            ret_arr[i] = np.array([avg, res])
    return ret_arr


def chi_squared_test(left_bucket, right_bucket):
    """
    Calculates chi squared value and degree of freedom between the selected attribute
    and the class attribute. A bucket is a list of size num_classes. bucket[i] is the
    number of examples that belong to class i.
    :param left_bucket: Bucket belonging to the left child after the split.
    :param right_bucket: Bucket belonging to the right child after the split.
    :return: A float and and integer. Chi squared value and degree of freedom.
    """
    l_sum, r_sum = np.sum(left_bucket), np.sum(right_bucket)
    p_sum = l_sum + r_sum
    chi = 0
    dof = len(right_bucket) - 1
    stacked = np.transpose(np.vstack((left_bucket, right_bucket)))
    dof -= np.sum(np.all(stacked == [0,0], axis = 1))
    for i in range(len(left_bucket)):
        if  left_bucket[i] or right_bucket[i] :
            observed_l = left_bucket[i]
            expected_l =  (left_bucket[i] + right_bucket[i]) * l_sum / p_sum
            observed_r = right_bucket[i]
            expected_r = (right_bucket[i] + left_bucket[i]) * r_sum / p_sum
            chi += ( (observed_r - expected_r)**2) / expected_r
            chi += ( (observed_l - expected_l)**2 ) / expected_l
    return chi, dof