import dt
import numpy as np

train_set = np.load( 'dt/train_set.npy')
train_labels = np.load( 'dt/train_labels.npy')
test_set = np.load( 'dt/test_set.npy')
test_labels = np.load( 'dt/test_labels.npy')
num_classes =  len(np.unique(train_labels))
to_print_list = []
id = 0

class Node:
    def __init__(self, data, label, id):
        self.id = id
        self.data = data
        self.label = label
        self.left = None
        self.right = None
        self.split_val = None
        self.list = None
        self.split_index = None
        self.is_leaf = None
        self.decision = None
    def print(self):
            print("--Printing Node {} --".format(self.id))
            if self.left :
                print("Left of this node is {}".format(self.left.id))
            if self.right :
                print("Right of this node is {}".format(self.right.id))
            if self.split_val is not None:
                print("x[{}] < {}".format(self.split_index, self.split_val[0]))
            print(self.list, "\n")

def tree(data, labels, mode, preprune):
    global id
    core_labels = np.unique(np.ndarray.flatten(labels))
    id += 1
    node = Node(data, labels, id)
    ls = np.bincount(labels)    
    node.list = ls
    if np.count_nonzero(node.list) == 1:
        #print(node.list)
        node.is_leaf = True
        node.decision = np.argmax(node.list)
        to_print_list.append(node)
        return node
    else:
        node.is_leaf = False
        max_info_index, max_info, arr_index = -1, -1, -1
        if mode == "avg_gini_index":
            max_info = np.array([0, np.inf])
        else:
            max_info = np.array([0, -np.inf])
        attribute_count = data.shape[1]
        for i in range(attribute_count):
            splits = dt.calculate_split_values(data, labels, num_classes, i, mode)
            if mode == "avg_gini_index":
                index = np.argmin(splits[:,1])
                info = splits[index, 1]
                if info < max_info[1]:
                    max_info = splits[index]
                    arr_index = i
            else:
                index = np.argmax(splits[:,1])
                info = splits[index, 1]
                if info > max_info[1]:
                    max_info = splits[index]
                    arr_index = i
        node.split_val = max_info
        if mode == "info_gain":
            mask = data[:,arr_index] < max_info[0]
            left_node = data[mask]
            left_labels = labels[mask]
            mask = data[:, arr_index] >= max_info[0]
            right_node = data[mask]
            right_labels = labels[mask]
            #print(left_labels, right_labels)
        else:
            mask = data[:, arr_index] > max_info[0]
            right_node = data[mask]
            right_labels = labels[mask]
            mask = data[:, arr_index] <= max_info[0]
            left_node = data[mask]
            left_labels = labels[mask]
        node.split_index = arr_index
        left, right = np.zeros(num_classes), np.zeros(num_classes)
        for label in core_labels:
            left[label] = np.count_nonzero(left_labels == label)
            right[label] = np.count_nonzero(right_labels == label)
        #chi, dof = dt.chi_squared_test(left, right)
        #Couldn't Complete chi_squared_test_algorithm version.
        if preprune == 0 :
            if left_node.shape[0] != 0:
                node.left = tree(left_node, left_labels, mode, preprune)
            if right_node.shape[0] != 0:
                node.right = tree(right_node, right_labels, mode, preprune)
            node.decision = np.argmax(node.list)
            to_print_list.append(node)
        else:
            print('I couldnt completed this, get ready for lots of errors.' )
        return node

def predict(data, node):
    if not node.is_leaf:
        if data[node.split_index] < node.split_val[0]:
            return predict(data, node.left)
        else:
            return predict(data, node.right)
    else:
        #print(node.decision)
        return node.decision

def results(node):
    score = 0
    for i in range(test_set.shape[0]):
        if test_labels[i] == predict(test_set[i], node):
            score += 1
    print(score / test_set.shape[0])
    return score / test_set.shape[0]


#mode = "avg_gini_index"
mode = "info_gain"
preprune = 0
root = tree(train_set, train_labels, mode, preprune)
results(root)
#run decision_tree.py > test.txt or sth you like to see node connections.
for elem in to_print_list:
    elem.print()




