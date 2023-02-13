from scipy.linalg import hilbert
import numpy as np

# This is recursive method I've tried to come up, but I failed horribly.
def croutsRecursive(A: np.matrix) -> [np.matrix, np.matrix]:
    k = len(A)
    if k == 1:
        return np.matrix(A[0, 0]), np.ones((1, 1))
    else:
        a_11 = np.matrix([A[0, 0]])
        a_1k = np.matrix(A[0, 1:])
        a_k1 = np.matrix(A[1:, 0])
        A_kk = A[1:, 1:]
        L_k1 = a_k1
        L_11 = a_11
        u_1k = np.matmul(np.linalg.inv(L_11), a_1k)
        u_11 = np.matrix(np.ones((1, 1)))
        LKK, UKK = croutsRecursive(A_kk)
        L = np.block([
            [L_11, np.zeros((1, k - 1))],
            [L_k1.transpose(), LKK]
        ])
        U = np.block([
            [u_11, u_1k],
            [np.zeros((k - 1, 1)), UKK]
        ])
        return L, U


# Crouts Iterative uses the algorithm I obtained at wikipedia.com
# I tried to implement this in recursive method yet I failed, error is horrendous at that :D
def croutsIterative(A: np.matrix) -> [np.matrix, np.matrix]:
    n = len(A)
    U, L = np.ndarray((n, n)), np.ndarray((n, n))
    for i in range(n):
        U[i][i] = 1
    for j in range(n):
        for i in range(j, n):
            sumx = 0
            for k in range(j):
                sumx += L[i, k] * U[k, j]
            L[i, j] = A[i, j] - sumx
        for i in range(j, n):
            sumx = 0
            for k in range(j):
                sumx += L[j, k] * U[k, i]
            U[j, i] = (A[j, i] - sumx) / L[j, j]
    return L, U