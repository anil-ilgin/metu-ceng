# Will use bag of features
import sys
import cv2
import random
import os
import numpy as np
from cyvlfeat.sift import dsift
from sklearn.cluster import KMeans
from sklearn.metrics import confusion_matrix
import seaborn as sn
import matplotlib.pyplot as plt

# need sift_on_image, dsift_on_image
# need BoF dictionary builder
# need k_means, KNN

# Args : Batch_size, is_dense,
def read_images():
    images = []
    grays = []
    full_path = os.getcwd()
    train_path = full_path + "/the2_data/train"
    im_names = list(open("image_names.txt", 'r'))
    im_names = [elem.strip('\n') for elem in im_names]
    # print(im_names)
    rand_sample = random.sample(range(101, 501), int(sys.argv[2]))
    #print(rand_sample)
    for i in range(len(im_names)):
        for j in range(int(sys.argv[2])):
            path_img = train_path + "/{}/{}.png".format(im_names[i], str(rand_sample[j]).zfill(4))
            #print(path_img)
            img = cv2.imread(path_img)
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            grays.append(gray)
            images.append(img)
    # print(len(images), len(im_names)*len(rand_sample), type(images[0]))
    return images, grays


def read_test_path():
    images = []
    grays = []
    full_path = os.getcwd()
    train_path = full_path + "/the2_data/validation"
    im_names = list(open("image_names.txt", 'r'))
    im_names = [elem.strip('\n') for elem in im_names]
    # print(im_names)
    for i in range(len(im_names)):
        for j in range(1, int(sys.argv[2]) + 1):
            path_img = train_path + "/{}/{}.png".format(im_names[i], str(j).zfill(4))
            #print(path_img)
            img = cv2.imread(path_img)
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            grays.append(gray)
            images.append(img)
    # print(len(images), len(im_names)*len(rand_sample), type(images[0]))
    return images, grays


# Does Dense-SIFT or SIFT on Image
def image_sift(gray, image, is_dense):
    if is_dense:
        frames, descs = dsift(gray, step=int(sys.argv[5]), fast=True)
    else:

        sift = cv2.SIFT_create(nfeatures=0, nOctaveLayers=3, contrastThreshold=0.04,
                               edgeThreshold=15, sigma=1.6)
        kp = sift.detect(gray, None)
        cv2.drawKeypoints(gray, kp, image)
        kp, descs = sift.detectAndCompute(gray, None)
        if len(kp) == 0:
            # print("Zero Keypoint detected.")
            descs = np.zeros((1, 128))
    # print(len(descs), len(descs[0]))
    return descs


def local_cluster_database(images, grays, is_dense):
    list_of = []
    all_descs = np.ndarray(shape=(0, 128))
    for i in range(len(images)):
        #print(i)
        descs = image_sift(grays[i], images[i], is_dense)
        list_of.append(descs)
        all_descs = np.concatenate((all_descs, descs))
    # print(all_descs.shape)
    return all_descs, list_of


# get clusters by kmeans
def kmeans(all_descs, k):
    clusters = KMeans(n_clusters=k).fit(all_descs)
    np.savetxt("clusters.txt", clusters.cluster_centers_, delimiter=" ", newline="\n")
    #f = open('clusters.txt', 'a')
    #f.write(" ".join(str(item) for item in clusters.cluster_centers_))
    #f.close()
    return clusters.cluster_centers_


# get BoF of a given Image
def bag_of_feature(descs, clusters,k):
    hist = np.zeros(k)
    for elem in descs:
        dist = np.linalg.norm(clusters - elem, axis=1)
        index = np.argmin(dist)
        hist[index] += 1
    norm_hist = hist / len(descs)
    return norm_hist


def bof_database_extractor():
    dense = sys.argv[1]
    bof_database = []
    if dense == 'DENSE':
        is_dense = True
    else:
        is_dense = False
    images, grays = read_images()
    all_descs, list_of_all = local_cluster_database(images, grays, is_dense)
    clusters = kmeans(all_descs, int(sys.argv[4]))
    i = 0
    for elem in list_of_all:
        print(i)
        i += 1
        hist = bag_of_feature(elem, clusters, int(sys.argv[4]))
        bof_database.append(hist)
    f = open("bof_database.txt", 'a')
    for elem in bof_database:
        f.write(" ".join(str(item) for item in elem) + "\n")
    f.close()


def calculate_distances(train_data, test_instance):
    distance = np.linalg.norm(train_data - test_instance, axis=1)
    return distance


def majority_voting(distances, labels, k):
    parts = np.argpartition(distances, k)[:k]
    votes = np.array(labels)[parts]
    unique, counts = np.unique(votes, return_counts=True)
    return unique[np.argmax(counts)]


def knn(train_data, train_labels, test_data, k):
    dist = calculate_distances(train_data, test_data)
    label = majority_voting(dist, train_labels, k)
    return label


def test_all(is_dense, k):
    print(sys.argv[1:])
    bof = np.loadtxt('bof_database.txt', delimiter=' ')
    train_labels = []
    test_labels = []
    im_names = list(open("image_names.txt", 'r'))
    im_names = [elem.strip('\n') for elem in im_names]
    test_images, gray = read_test_path()
    clusters = np.loadtxt('clusters.txt', delimiter=' ')
    for elem in im_names:
        for i in range(int(len(bof) / len(im_names))):
            train_labels.append(elem)
        for i in range(100):
            test_labels.append(elem)
    tot = 0
    all_predictions = []
    for i in range(len(test_images)):
        descs = image_sift(gray[i], test_images[i], is_dense)
        test_bof = bag_of_feature(descs, clusters, int(sys.argv[4]))
        label = knn(bof, train_labels, test_bof, k)
        all_predictions.append(label)
        if label == test_labels[i]:
            tot += 1
    print("Acc is {}".format(tot/len(test_images)))
    return all_predictions, test_labels

def tester(image, is_dense, k):
    bof = np.loadtxt('bof_database.txt', delimiter=' ')
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    descs = image_sift(gray, image, is_dense)
    im_names = list(open("image_names.txt", 'r'))
    im_names = [elem.strip('\n') for elem in im_names]
    train_labels = []
    for elem in im_names:
        for i in range(int(len(bof) / len(im_names))):
            train_labels.append(elem)

    label = knn(bof, train_labels, descs, k)
    return label

def main():
    if sys.argv[1] == 'DENSE':
        is_dense = True
    else:
        is_dense = False
    if sys.argv[3] == 'train':
        print(sys.argv[1:])
        bof_database_extractor()
    elif sys.argv[3] == 'validation':
        r1, r2 = test_all(is_dense, int(sys.argv[6]))
        #confusion_matrix_(r2, r1)

def confusion_matrix_(truth, prediction):
    graph = confusion_matrix(truth, prediction)
    sn.set(font_scale=1.4)
    sn.heatmap(graph, annot=True, annot_kws={"size": 16})
    plt.show()

def read_testing_set():
    images = []
    grays = []
    full_path = os.getcwd()
    test_path = full_path + "/test/"
    im_names = list(open("image_names.txt", 'r'))
    im_names = [elem.strip('\n') for elem in im_names]
    # print(im_names)
    for elem in sorted(os.listdir(test_path)):
        path_img = test_path + elem
        img = cv2.imread(path_img)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        grays.append(gray)
        images.append(img)
    # print(len(images), len(im_names)*len(rand_sample), type(images[0]))
    return images, grays, sorted(os.listdir(test_path))

def testing_test_set():
    print(sys.argv[1:])
    f = open('predictions.txt', 'a')
    bof = np.loadtxt('bof_database.txt', delimiter=' ')
    train_labels = []
    test_labels = []
    im_names = list(open("image_names.txt", 'r'))
    im_names = [elem.strip('\n') for elem in im_names]
    test_images, gray, image_names = read_testing_set()
    clusters = np.loadtxt('clusters.txt', delimiter=' ')
    for elem in im_names:
        for i in range(int(len(bof) / len(im_names))):
            train_labels.append(elem)
    tot = 0
    all_predictions = []
    for i in range(len(test_images)):
        descs = image_sift(gray[i], test_images[i], True)
        test_bof = bag_of_feature(descs, clusters, int(sys.argv[4]))
        label = knn(bof, train_labels, test_bof, 64)
        all_predictions.append(label)
    for i in range(len(all_predictions)):
        f.write(image_names[i] + " \t " + all_predictions[i] + '\n')
    return all_predictions, test_labels


testing_test_set()
