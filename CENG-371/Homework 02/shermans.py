import time

from scipy.linalg import hilbert
import numpy as np


def shermans(A: np.matrix) -> [np.matrix, np.matrix]:
    k = len(A)
    if k == 1:
        return np.ones((1,1)), np.matrix([A[0, 0]])  # L, U
    else:
        A_k = A[:k-1, 0:-1]
        a_k = A[k-1, k-1]
        a_1k = np.matrix(A[k-1, 0:k-1])
        a_k1 = np.matrix(A[0:k-1, k-1].transpose())
        L_11, U_11 = shermans(A_k)
        u_1k = np.matrix(np.matmul(np.linalg.inv(L_11), a_1k.transpose()))
        l_k1 = np.matrix(np.matmul(np.linalg.inv(U_11.transpose()), a_k1.transpose()))
        u_kk = a_k - np.matmul(l_k1.transpose(), u_1k)
        L = np.block([
            [L_11, np.zeros((k-1, 1))],
            [l_k1.transpose(), np.ones((1, 1))]
        ])
        U = np.block([
            [U_11, u_1k],
            [np.zeros((1, k-1)), u_kk]
        ])
        return L, U


def test():
    out = open("shermans_result.csv", "a")
    out.write("N,Relative Error, Time\n")
    for n in range(1, 301):
        H = hilbert(n)
        st = time.time()
        L, U = shermans(H)
        en = time.time()
        rel_err_mat = np.linalg.norm(H - np.matmul(L, U))/np.linalg.norm(H)
        out.write("{},{},{}\n".format(n, rel_err_mat, en - st))
    out.close()

#test()