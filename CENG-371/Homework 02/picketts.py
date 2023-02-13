from scipy.linalg import hilbert
import numpy as np
import time


def picketts(A: np.ndarray) -> [np.matrix, np.matrix]:
    l, k = A.shape
    if k == 1:
        return A, np.ones((1, 1))  # L, U
    else:
        a_kk = np.matrix(A[k - 1:, k - 1]).transpose()
        a_1k = np.matrix(A[:k - 1, k - 1]).transpose()
        L1, U11 = picketts(A[:, :k - 1])
        L_11 = np.matrix(L1[:k - 1, :k - 1])
        L_k1 = np.matrix(L1[k - 1:, :k - 1])
        u_1k = np.matrix(np.matmul(np.linalg.inv(L_11), a_1k))
        l_kk = a_kk - np.matmul(L_k1, u_1k)
        u_kk = l_kk[0]
        l_kk = l_kk / u_kk
        L = np.block([
            [L_11, np.zeros((k - 1, 1))],
            [L_k1, l_kk]
        ])
        U = np.block([
            [U11, u_1k],
            [np.zeros((1, k - 1)), u_kk]
        ])
        return L, U


def test():
    out = open("picketts_result.csv", "a")
    out.write("N,Relative Error, Time\n")
    for n in range(1, 301):
        print(n)
        H = hilbert(n)
        st = time.time()
        L, U = picketts(H)
        en = time.time()
        rel_err_mat = np.linalg.norm(H - np.matmul(L, U)) / np.linalg.norm(H)
        out.write("{},{},{}\n".format(n, rel_err_mat, en-st))
    out.close()

#test()
