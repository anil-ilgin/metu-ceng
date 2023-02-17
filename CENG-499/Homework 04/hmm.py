import numpy as np


def forward(A, B, pi, O):
    """
    Calculates the probability of an observation sequence O given the model(A, B, pi).
    :param A: state transition probabilities (NxN)
    :param B: observation probabilites (NxM)
    :param pi: initial state probabilities (N)
    :param O: sequence of observations(T) where observations are just indices for the columns of B (0-indexed)
        N is the number of states,
        M is the number of possible observations, and
        T is the sequence length.
    :return: The probability of the observation sequence and the calculated alphas in the Trellis diagram with shape
             (N, T) which should be a numpy array.
    """
    x = [ 0.0 for i in range(O.shape[0]) ]
    alpha = np.array([x.copy() for i in range(A.shape[0])])
    f = lambda i,j : sum(alpha[:, i - 1] * A[:, j] * B[j, O[i]])
    alpha[:, 0] = pi * B[:, O[0]]
    for i in range(1, O.shape[0]):
        j = range(A.shape[0])
        ind = [i] * A.shape[0]
        res = np.transpose(np.array(list(map(f, ind, j))))
        alpha[:, i] = res

    ret_val = sum(alpha[:, O.shape[0] - 1])
    return ret_val, alpha
def viterbi(A, B, pi, O):
    """
    Calculates the most likely state sequence given model(A, B, pi) and observation sequence.
    :param A: state transition probabilities (NxN)
    :param B: observation probabilites (NxM)
    :param pi: initial state probabilities(N)
    :param O: sequence of observations(T) where observations are just indices for the columns of B (0-indexed)
        N is the number of states,
        M is the number of possible observations, and
        T is the sequence length.
    :return: The most likely state sequence with shape (T,) and the calculated deltas in the Trellis diagram with shape
             (N, T). They should be numpy arrays.
    """
    x = [0.0 for i in range(O.shape[0])]
    delta = np.array([x.copy() for i in range(A.shape[0])])
    reverse = np.array([x.copy() for i in range(A.shape[0])])
    results = [0 for i in range(O.shape[0])]
    delta_f = lambda i,j : np.max(delta[:, j - 1] * A[:, i] * B[i, O[j]])
    reverse_f = lambda i,j : np.argmax(delta[:, j-1] * A[:, i] * B[i, O[j]])
    delta[:, 0] = B[:, O[0]] * pi
    for i in range(1,O.shape[0]):
        i_ind = range(A.shape[0])
        j_ind = [i] * A.shape[0]
        delta[:, i] = np.transpose(np.array(list(map(delta_f, i_ind, j_ind))))
        reverse[:, i] = np.transpose(np.array(list(map(reverse_f, i_ind, j_ind)))) 
    results[0] = int(np.argmax(delta[:, O.shape[0] - 1]))

    for i in range(1, O.shape[0]):
        results[i] = reverse[int(results[i - 1]), O.shape[0] - i]
    results = results[::-1]
    return np.array(results), delta