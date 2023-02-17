import numpy as np
import sys
import matplotlib.pyplot as plt

def assign_clusters(data, cluster_centers):
    """
    Assigns every data point to its closest (in terms of Euclidean distance) cluster center.
    :param data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param cluster_centers: A (K, D) shaped numpy array where K is the number of clusters
    and D is the dimension of the data
    :return: An (N, ) shaped numpy array. At its index i, the index of the closest center
    resides to the ith data point.
    """
    total_arr = []
    for elem in cluster_centers:
        distance = np.linalg.norm(data - elem, axis=1)
        total_arr.append(distance)
    arr = np.vstack(total_arr)
    ls = np.argmin(arr, axis=0)
    return ls

def calculate_cluster_centers(data, assignments, cluster_centers, k):
    """
    Calculates cluster_centers such that their squared Euclidean distance to the data assigned to
    them will be lowest.
    If none of the data points belongs to some cluster center, then assign it to its previous value.
    :param data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param assignments: An (N, ) shaped numpy array with integers inside. They represent the cluster index
    every data assigned to.
    :param cluster_centers: A (K, D) shaped numpy array where K is the number of clusters
    and D is the dimension of the data
    :param k: Number of clusters
    :return: A (K, D) shaped numpy array that contains the newly calculated cluster centers.
    """
    new_centroid_ls = []
    for i in range(k):
        indices = np.where(assignments == i)[0]
        if indices.size == 0:
            new_centroid = cluster_centers[i]
        else:
            new_centroid = np.sum(data[indices], axis=0) / (data[indices].shape[0])
        new_centroid_ls.append(new_centroid)
    return np.array(new_centroid_ls)



def kmeans(data, initial_cluster_centers):
    """
    Applies k-means algorithm.
    :param data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param initial_cluster_centers: A (K, D) shaped numpy array where K is the number of clusters
    and D is the dimension of the data
    :return: cluster_centers, objective_function
    cluster_center.shape is (K, D).
    objective function is a float. It is calculated by summing the squared euclidean distance between
    data points and their cluster centers.
    """
    clusters = initial_cluster_centers
    is_objective_changed = True
    obj_func = 0.0
    while is_objective_changed:
        assignments = assign_clusters(data, clusters)
        new_obj_func = 0.0
        for i in range(len(clusters)):
            indices = np.where(assignments == i)[0]
            if indices.size == 0:
                continue
            else:
                new_data = data[indices]
                dist = np.linalg.norm(new_data - clusters[i], axis=1)
                dist = dist*dist/2
                new_obj_func += np.sum(dist)
        if new_obj_func == obj_func:
            is_objective_changed = False
        else:
            obj_func = new_obj_func
        clusters = calculate_cluster_centers(data, assignments, clusters, len(clusters))
    return clusters, obj_func

def initialize_clusters_plusplus(datas, k):
    indice = np.random.randint(datas.shape[0])
    initial_clusters = [datas[indice]]
    data = np.copy(datas)
    data = np.delete(data, np.where(data == initial_clusters[0]), axis=0)
    for i in range(k-1): 
        distances = []
        for j in range(len(initial_clusters)):
            dist = np.linalg.norm(data - initial_clusters[j], axis=1)
            distances.append(dist)
        all_dist = np.concatenate(distances)
        sums = np.sum(all_dist, axis=0)
        new_center_ind = np.argmax(sums)
        initial_clusters.append(data[new_center_ind])
        data = np.delete(data, np.where(data == initial_clusters[-1]), axis=0)
    return np.array(initial_clusters)

def main():
    args = sys.argv
    dataset_choice, k = int(args[1]), int(args[2])
    dataset = np.load("kmeans/dataset{}.npy".format(dataset_choice))
    print("Dataset {} is choosen with k = {}, starting clustering. \n".format(dataset_choice, k))
    init_clusters = initialize_clusters_plusplus(dataset, k)
    cluster_centers, obj_f = kmeans(dataset, init_clusters)
    assignments = assign_clusters(dataset, cluster_centers) 
    for i in range(k):
        ind = np.where(assignments == i)
        if ind != 0:
            vals = dataset[ind]
            print(vals.shape)
            plt.scatter(vals[:,0],vals[:,1])
    plt.show() 
main()
