import numpy as np

def vocabulary(data):
    """
    Creates the vocabulary from the data.
    :param data: List of lists, every list inside it contains words in that sentence.
                 len(data) is the number of examples in the data.
    :return: Set of words in the data
    """
    flat_arr = sum(data, [])
    uniques = np.unique(np.array(flat_arr))
   # print(uniques.shape)
    return set(uniques)
    

def estimate_pi(train_labels):
    """
    Estimates the probability of every class label that occurs in train_labels.
    :param train_labels: List of class names. len(train_labels) is the number of examples in the training data.
    :return: pi. pi is a dictionary. Its keys are class names and values are their probabilities.
    """
    flat = np.array(train_labels)
    elems, counts = np.unique(flat, return_counts=True)
    pi = dict(zip(elems, counts/np.sum(counts)))
    return pi
    
def estimate_theta(train_data, train_labels, vocab):
    """
    Estimates the probability of a specific word given class label using additive smoothing with smoothing constant 1.
    :param train_data: List of lists, every list inside it contains words in that sentence.
                       len(train_data) is the number of examples in the training data.
    :param train_labels: List of class names. len(train_labels) is the number of examples in the training data.
    :param vocab: Set of words in the training set.
    :return: theta. theta is a dictionary of dictionaries. At the first level, the keys are the class names. At the
             second level, the keys are all the words in vocab and the values are their estimated probabilities given
             the first level class name.
    """
    s_constant = 1
    unique_labels = np.unique(np.array(train_labels))
    len_unique_labels = len(unique_labels)
    empty_lists = [[] for i in range(len_unique_labels)]
    dct = dict(zip(unique_labels, empty_lists))

    for i in range(len(train_labels)):
        dct[train_labels[i]] += list(train_data[i])

    len_vocab = len(vocab)
    theta = dict(zip(unique_labels, [{} for i in range(len_unique_labels)]))

    #a = datetime.now()
    for key in theta:
        #print(datetime.now() - a)
        #a = datetime.now()
        y = len(dct[key]) + len_vocab
        const_val = s_constant / y
        filtered_words = np.unique(np.array(dct[key])).tolist()
        filtered_words = list(filter(lambda x: x in vocab, filtered_words))
        const_arr = [const_val]*len_vocab
        theta[key] = dict(zip(vocab,const_arr.copy()))
        for v in filtered_words:
            theta[key][v] += dct[key].count(v) / y
    return theta
        
        


def test(theta, pi, vocab, test_data):
    """
    Calculates the scores of a test data given a class for each class. Skips the words that are not occurring in the
    vocabulary.
    :param theta: A dictionary of dictionaries. At the first level, the keys are the class names. At the second level,
                  the keys are all of the words in vocab and the values are their estimated probabilities.
    :param pi: A dictionary. Its keys are class names and values are their probabilities.
    :param vocab: Set of words in the training set.
    :param test_data: List of lists, every list inside it contains words in that sentence.
                      len(test_data) is the number of examples in the test data.
    :return: scores, list of lists. len(scores) is the number of examples in the test set. Every inner list contains
             tuples where the first element is the score and the second element is the class name.
    """
    scores = [[] for i in range(len(test_data))]
    f = lambda key,v, i: np.log(theta[key][v]) * test_data[i].count(v)
    for i in range(len(test_data)):
       # print(i)
        filtered_words = np.unique(np.array(test_data[i])).tolist()
        filtered_words = list(filter(lambda x: x in vocab, filtered_words))
        indices = [i] * len(filtered_words)
        for key in pi:
            keys = [key] * len(filtered_words)

            total = sum(list(map(f, keys, filtered_words, indices))) + np.log(pi[key])
            scores[i].append( (total, key) )
    return scores