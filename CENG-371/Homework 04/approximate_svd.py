import matplotlib.pyplot as plt
import numpy as np
import scipy as spy
import sys

p = 5


def approximate_svd(A: np.matrix, k: int):
    global p
    n = A.shape[1]
    gaussRM = np.random.normal(size=(n, k+p))
    Y = np.matmul(A, gaussRM)
    Q = spy.linalg.orth(Y)
    qT = np.transpose(Q)
    sMat = np.matmul(qT, A)
    ret = spy.linalg.svd(sMat)
    u, vk = ret[0], ret[2]
    uk = np.matmul(Q, u)
    sk = spy.linalg.diagsvd(ret[1], k+p, n)
    return uk, sk, vk


def get_image(im_name: str):
    img = plt.imread(im_name)
    A = np.asmatrix(img)
    return A


def svds_svd_imshow(A: np.matrix, k):
    ret2 = spy.sparse.linalg.svds(np.asarray(A).astype(float), k=k)
    u2, v2 = np.matrix(ret2[0]), np.matrix(ret2[2])
    s2 = spy.linalg.diagsvd(ret2[1], ret2[1].shape[0],ret2[1].shape[0])
    u2s2 = np.matrix(np.matmul(u2,s2))
    img_svds = np.matmul(u2s2, v2)
    plt.imshow(img_svds, cmap="gray")
    plt.title("k={} for SVDS".format(k))
    plt.show()


def approximate_imshow(A: np.matrix, k):
    ret = spy.linalg.svd(A)
    u, s, v = ret[0], spy.linalg.diagsvd(ret[1], A.shape[0], A.shape[1]), ret[2]
    uk, sk, vk = approximate_svd(A, k)
    img_approx = np.matmul(np.matmul(uk, sk), vk)
    plt.imshow(img_approx, cmap="gray")
    plt.title("k={} for Approximate SVD".format(k))
    plt.show()


def svd_imshow(A: np.matrix):
    ret = spy.linalg.svd(A)
    u, s, v = ret[0], spy.linalg.diagsvd(ret[1], A.shape[0], A.shape[1]), ret[2]
    img_svd = np.matmul(np.matmul(u, s), v)
    plt.imshow(img_svd, cmap="gray")
    plt.title("Pure SVD")
    plt.show()

def graph():
    err_approx = np.recfromcsv("errors_approx.csv")
    err_svds = np.recfromcsv("errors_svds.csv")
    x1, y1, x2, y2 = [i[0] for i in err_approx], [i[1] for i in err_approx], [i[0] for i in err_svds], [i[1] for i in err_svds]
    plt.title("Error plot for SVDs for Cameraman.JPG")
    plt.xlabel("k from 1 to rank of image")
    plt.ylabel("Relative errors")
    plt.plot(x1, y1, label="Approximate SVD")
    plt.plot(x2, y2, label="SVDS")
    plt.legend()
    plt.show()


def main():
    im_name = sys.argv[1]
    k = int(sys.argv[2])
    A = get_image(im_name)
    u, s, v = approximate_svd(A, k)
