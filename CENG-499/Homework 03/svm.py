from statistics import mean
import numpy as np
import matplotlib.pyplot as plt
import sklearn.svm as svm
import sklearn.model_selection as ms

train_set_1 = np.load('svm/task1/train_set.npy')
train_labels_1 = np.load('svm/task1/train_labels.npy')

train_set_2 = np.load('svm/task2/train_set.npy')
train_labels_2 = np.load('svm/task2/train_labels.npy')

train_set_3 = np.load('svm/task3/train_set.npy')
train_labels_3 = np.load('svm/task3/train_labels.npy')

train_set_4 = np.load('svm/task4/train_set.npy')
train_labels_4 = np.load('svm/task4/train_labels.npy')

def draw_svm(clf, x, y, x1_min, x1_max, x2_min, x2_max, target=None):
    """
    Draws the decision boundary of an svm.
    :param clf: sklearn.svm.SVC classifier
    :param x: data Nx2
    :param y: label N
    :param x1_min: minimum value of the x-axis of the plot
    :param x1_max: maximum value of the x-axis of the plot
    :param x2_min: minimum value of the y-axis of the plot
    :param x2_max: maximum value of the y-axis of the plot
    :param target: if target is set to path, the plot is saved to that path
    :return: None
    """
    y = y.astype(bool)
    xx, yy = np.meshgrid(np.linspace(x1_min, x1_max, 500),
                         np.linspace(x2_min, x2_max, 500))
    z = clf.decision_function(np.c_[xx.ravel(), yy.ravel()])
    z = z.reshape(xx.shape)
    disc_z = z > 0
    plt.clf()
    plt.imshow(disc_z, interpolation='nearest',
               extent=(xx.min(), xx.max(), yy.min(), yy.max()), aspect='auto',
               origin='lower', cmap=plt.cm.RdBu, alpha=.3)
    plt.contour(xx, yy, z, levels=[-1, 1], linewidths=2,
                linestyles='dashed', colors=['red', 'blue'], alpha=0.5)
    plt.contour(xx, yy, z, levels=[0], linewidths=2,
                linestyles='solid', colors='black', alpha=0.5)
    positives = x[y == 1]
    negatives = x[y == 0]
    plt.scatter(positives[:, 0], positives[:, 1], s=50, marker='o', color="none", edgecolor="black")
    plt.scatter(negatives[:, 0], negatives[:, 1], s=50, marker='s', color="none", edgecolor="black")
    sv_label = y[clf.support_]
    positive_sv = x[clf.support_][sv_label]
    negative_sv = x[clf.support_][~sv_label]
    plt.scatter(positive_sv[:, 0], positive_sv[:, 1], s=50, marker='o', color="white", edgecolor="black")
    plt.scatter(negative_sv[:, 0], negative_sv[:, 1], s=50, marker='s', color="white", edgecolor="black")
    plt.xlim(x1_min, x1_max)
    plt.ylim(x2_min, x2_max)
    plt.gca().set_aspect('equal', adjustable='box')
    
    if target is None:
        plt.show()
    else:
        plt.savefig(target)

c_vals_1 = [0.01, 0.1, 1, 10, 100]
kernels = ["linear", "rbf", "poly", "sigmoid"]
gamma_value = [0.001, 0.01, 0.1, 1, 10]
dicts = {'C': c_vals_1, 'kernel': kernels, 'gamma': gamma_value}
parameters, mean_test_scores, mean_train_scores  = None, None, None
def q1(data, labels, c):
    svc = svm.SVC(C=c, kernel='linear')
    svc.fit(data, labels)
    min_x = np.min(np.ndarray.flatten(data[:,0]))
    min_y = np.min(np.ndarray.flatten(data[:,1]))
    max_x = np.max(np.ndarray.flatten(data[:,0]))
    max_y = np.max(np.ndarray.flatten(data[:,1]))
    draw_svm(svc, data, labels, min_x, max_x, min_y, max_y)

def q2(data, labels, kernel):
    svc = svm.SVC(C=1, kernel=kernel)
    svc.fit(data, labels)
    min_x = np.min(np.ndarray.flatten(data[:,0]))
    min_y = np.min(np.ndarray.flatten(data[:,1]))
    max_x = np.max(np.ndarray.flatten(data[:,0]))
    max_y = np.max(np.ndarray.flatten(data[:,1]))
    draw_svm(svc, data, labels, min_x, max_x, min_y, max_y)

def q3(data, labels):
    global dicts, parameters, mean_test_scores, mean_train_scores
    new_data = np.ndarray((data.shape[0], (data.shape[1] * data.shape[2])) )
    for i in range(data.shape[0]):
        new_data[i] = np.ndarray.flatten(data[i]) / float(256)
    svc = svm.SVC()
    grid_search = ms.GridSearchCV(svc, dicts, cv=5, return_train_score=True)
    grid_search.fit(new_data, labels)
    parameters = grid_search.cv_results_['params']
    mean_test_scores = grid_search.cv_results_['mean_test_score']
    mean_train_scores = grid_search.cv_results_['mean_train_score']
    svc = svm.SVC(C=grid_search.best_params_['C'], kernel=grid_search.best_params_['kernel'], gamma=grid_search.best_params_['gamma'])
    svc.fit(new_data, labels)
    train_set = np.ndarray((train_set_3.shape[0], (train_set_3.shape[1] * train_set_3.shape[2])) )
    for i in range(data.shape[0]):
        train_set[i] = np.ndarray.flatten(train_set_3[i]) / float(256)
    acc = svc.score(train_set, train_labels_3)
    return acc


def main(inp):
    if inp == 1:
        for elem in c_vals_1:
            q1(train_set_1, train_labels_1, elem)
    elif inp == 2:
        for elem in kernels:
            q2(train_set_2, train_labels_2, elem)
    elif inp == 3:
        global parameters, mean_train_scores, mean_test_scores
        acc = q3(train_set_3, train_labels_3)
        #printing out
        for elem in range(mean_train_scores.shape[0]):
            print(dicts[elem], "->", mean_train_scores[elem], " " , mean_test_scores[elem])
        print('Accuracy is {}'.format(acc))
    else:
        print("Sorry, couldnt implemented the q4 :/ ")


#Change main func input to question number to generate results again.
main(3)