import time

import numpy as np
from scipy.io import mmread

def power_method(A: np.matrix, v: np.matrix):
    """
    Calculates the largest eigenvalue and corresponding eigenvector.
    :param A: np. matrix ,matrix to be calculated
    :param v: np. matrix , starting vector x
    :return: eigenvector : np.matrix , eigenvalue : np.matrix
    """
    MAX = 100
    v1 = v.copy()
    for i in range(MAX):
        prev = v1
        v1 = np.matmul(A, v1)
        v1 = v1 / np.linalg.norm(v1)
        if np.linalg.norm(v1 - prev) < 0.0000001:
            break
    eigval = np.matmul(np.matmul(np.transpose(v1), A), v1) / np.matmul(np.transpose(v1), v1)

    return v1, eigval

def power_k(A: np.matrix, k: int):
    """

    :param A: matrix to be calculated
    :param k: count of desired eigenvalues
    :return: dictionary of { eigenvalue : eigenvector } belong to A matrix
    """
    eigs = {}
    v = np.matrix(np.ones((len(A), 1)))
    A_n = A.copy()
    for i in range(k):
        vec, eig = power_method(A_n, v)
        A_n = A_n - eig.item() * np.matmul(vec, np.transpose(vec)) /np.matmul(np.transpose(vec), vec)
        eigs[eig.item()] = np.asarray(vec)
    return eigs

"""
val = mmread("can_229.mtx")
arr = val.todense()
start = time.time()
eigs = power_k(arr, 200)
end = time.time()
print(end - start)"""