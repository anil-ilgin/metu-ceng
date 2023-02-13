import time

import numpy as np
from scipy.io import mmread

def subspace_iteration(A: np.matrix, k: int):
    V = np.identity(len(A))
    q, r = np.linalg.qr(V)
    MAX_COUNT = 100
    prev = q[:, 0]
    eigs = {}
    for i in range(MAX_COUNT):
        W = np.matmul(A, q)
        q, r = np.linalg.qr(W)
        if np.linalg.norm(q[:,0] - prev) < 0.0000001:
            break
        prev = q[:, 0]
    for i in range(k):
        v1 = q[:,i]
        eigval = np.matmul(np.matmul(np.transpose(v1), A), v1) / np.matmul(np.transpose(v1), v1)
        eigs[eigval.item()] = v1
    return eigs

"""
val = mmread("can_229.mtx")
arr = val.todense()
start = time.time()
eigs = subspace_iteration(arr, 200)
end = time.time()
print(end - start)"""