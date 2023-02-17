import numpy as np
import cv2 as cv
import os
import sys

eps = 0.00001
sup_hists = []
query_hists = []
sup_3d_hists = []
query_3d_hists = []
grid_sup_hists = []
grid_query_hists = []
grid_sup_3d_hists = []
grid_query_3d_hists = []
p_query_3d, p_query, sup_3d, sup = [], [], [], []
name_list = []
path = os.getcwd()
images = []
query_images = []

def color_histogram(img, interval):
    bin_c = int(256 / interval)
    hist_1 = np.full(bin_c, eps)
    hist_2 = np.full(bin_c, eps)
    hist_3 = np.full(bin_c, eps)
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            hist_1[ int(img[i][j][0] / interval) ] += 1
            hist_2[ int(img[i][j][1] / interval) ] += 1
            hist_3[ int(img[i][j][2] / interval) ] += 1
    return hist_1, hist_2, hist_3

def color_3d_histogram(img, interval):
    bin_c = int(256 / interval)
    hist = np.full((bin_c, bin_c, bin_c), eps)
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            hist[ int(img[i][j][0] / interval) ][int(img[i][j][1] / interval) ] [ int(img[i][j][2] / interval) ] += 1
    return np.ndarray.flatten(hist)


def dataset_prep(query, interval, selection, grid):
    names = open('InstanceNames.txt')
    for row in names:
        name_list.append(row.replace('\n',''))
    
    for elem in name_list:
        img = cv.imread(path + '/support_96/' + elem)
        images.append(img)
        img_2 = cv.imread(path + '/' + query + '/' + elem)
        query_images.append(img_2)
        
        if selection == 'N':
            hist_1, hist_2, hist_3 = color_histogram(img, interval)
            h_1, h_2, h_3 = color_histogram(img_2, interval)
            query_hists.append((h_1, h_2, h_3))
            sup_hists.append((hist_1, hist_2, hist_3))
        
        elif selection == '3D':
            hist_1 = color_3d_histogram(img, interval)
            h_1 = color_3d_histogram(img_2, interval)
            sup_3d_hists.append(hist_1)
            query_3d_hists.append(h_1)


def dissimilarity(hist_1, hist_2):
    flat1 = np.ndarray.flatten(hist_1)
    flat2 = np.ndarray.flatten(hist_2)
    divlog = np.log(np.divide(flat1, flat2))
    divsum = np.sum(np.multiply(flat1, divlog))
    return divsum

def q1(index):
    all_dissims = []
    q_hist = query_hists[index]
    p1 = (q_hist[0])/np.linalg.norm(q_hist[0], 1)
    p2 = (q_hist[1])/np.linalg.norm(q_hist[1], 1)
    p3 = (q_hist[2])/np.linalg.norm(q_hist[2], 1)
    for i in range(len(images)):
        hist = sup_hists[i]
        p_ele1 = (hist[0])/np.linalg.norm(hist[0], 1)
        p_ele2 = (hist[1])/np.linalg.norm(hist[1], 1)
        p_ele3 = (hist[2])/np.linalg.norm(hist[2], 1)
        dis1, dis2, dis3 = dissimilarity(p1, p_ele1), dissimilarity(p2, p_ele2), dissimilarity(p3, p_ele3)
        disavg = (dis1 + dis2 + dis3)/3
        all_dissims.append(disavg)
    index = all_dissims.index(min(all_dissims))
    return index

def q2(index):
    all_dissims = []
    q_hist = query_3d_hists[index]
    p = (q_hist)/np.linalg.norm(q_hist, 1)
    for i in range(len(images)):
        hist = sup_3d_hists[i]
        p_2 = (hist)/np.linalg.norm(hist, 1)
        dis = dissimilarity(p, p_2)
        all_dissims.append(dis)
    index = all_dissims.index(min(all_dissims))
    return index

def main():
    args = sys.argv

    f = open("Test_Results_{}.txt".format(args[1]), "a")
    if args[1] == 'q1':
        acc = 0
        interval = int(args[3])
        dataset_prep(args[2], interval, '3D', 0)
        f.write('\n' + 'Test for Q1, with ' + args[2] + ' interval of ' + str(args[3]) + '\n')
        for i in range(len(query_images)):
            guess = q1(i)
            f.write('Match for index ' + str(i) + ' is ' + str(guess) + '\n')
            if guess == i:
                acc += 1
            print(i)
        f.write("Accuracy is " + str(acc/200) + "\n")
        print ("Accuracy is " + str(acc/200) + "\n")
        f.close()
    
    if args[1] == 'q2':
        acc = 0
        interval = int(args[3])
        dataset_prep(args[2], interval, 'N', 0)
        f.write('\n' + 'Test for Q2, with ' + args[2] + ' interval of ' + str(args[3]) + '\n')
        for i in range(len(query_images)):
            guess = q2(i)
            f.write('Match for index ' + str(i) + ' is ' + str(guess) + '\n')
            if guess == i:
                acc += 1
            print(i)
        f.write("Accuracy is " + str(acc/200) + "\n")
        print ("Accuracy is " + str(acc/200) + "\n")
        f.close()

#Write Q3

def image_split(img, interval, grid, selection):
    step_count = 96 // grid
    hists = []
    for i in range(step_count):
        for j in range(step_count):
            imx = img[i*grid : (i+1)*grid][j*grid : (j+1)*grid]
            if selection == '3D':
             hists.append(color_3d_histogram(imx, interval))
            else:
                hist_1, hist_2, hist_3 = color_histogram(imx, interval)
                hists.append((hist_1, hist_2, hist_3))
    return hists




main()