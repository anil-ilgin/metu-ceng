from nb import *

def main():
    #I simply read train_set and test_set from files, split it by new_line character
    #and then also split them by space so train_set is a list of lists consist of words. 
    #Then I just called functions from nb.py  in order.

    
    train_set = open('nb_data/train_set.txt', 'r').read().splitlines()
    train_set = [elem.split(' ') for elem in train_set]
    train_labels = open('nb_data/train_labels.txt', 'r').read().splitlines()

    test_set = open('nb_data/test_set.txt', 'r').read().splitlines()
    test_set = [elem.split(' ') for elem in test_set]
    test_labels = open('nb_data/test_labels.txt', 'r').read().splitlines()
    print('Starting Vocab Construction')
    vocab = vocabulary(train_set)
    print('Ended Vocab, Starting Pi Estimation')
    pi = estimate_pi(train_labels)
    print('Ended Pi Estimation, Starting Theta Estimation')
    theta = estimate_theta(train_set, train_labels, vocab)
    print('Ended Theta Est., Starting Generating Scores')
    scores = test(theta, pi, vocab, test_set)
    preds = 0
    print('Starting Accuracy Check')
    for i in range(len(scores)):
        sorted_score = sorted(scores[i], reverse=True)
        if test_labels[i] == sorted_score[0][1]:
            preds += 1
    print('Accuracy: {}'.format(preds/len(test_labels)))



main()