import numpy as np


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
