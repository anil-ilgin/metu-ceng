import math

import numpy as np
import matplotlib.pyplot as plt
import time

# Constants and lambda functions
f = lambda n: n * ((n + 1) / n - 1) - 1
g = lambda n: f(n) / np.finfo(float).eps
mil_single = np.single(10 ** 6)
m_8_single = np.single(10 ** 8)
mil_double = np.double(10 ** 6)
m_8_double = np.double(10 ** 8)
nums_single = lambda n: np.single(1) + (mil_single - np.single(n) + np.single(1)) / m_8_single
nums_double = lambda n: np.double(1) + (mil_double - np.double(n) + np.double(1)) / m_8_double

# Change N for a worse Pairwise Summation Error e.g 9 or 17
N = 3


def q1():
    x_axis = np.arange(1, 1001)
    y_axis = g(x_axis)
    q_1b = np.where(y_axis == 0)[0] + 1
    print(f"Q1 b) : {q_1b}")
    plt.plot(x_axis, y_axis)
    plt.xlabel("n")
    plt.ylabel("g(n)")
    plt.savefig("q1_a.png", dpi=500)


def naive_sum(arr, is_single):
    start = time.time()
    if is_single:
        sums = np.single(0)
        for elem in arr:
            sums += elem
        end = time.time()
        print(f"Run-time of naive-sum is {end - start} \n")
        return sums

    else:
        sums = np.double(0)
        for elem in arr:
            sums += elem
        end = time.time()
        print(f"Run-time of naive-sum is {end - start} \n")
        return sums



def compensated_sum(arr, is_single):
    start = time.time()
    if is_single:
        sums = np.single(0)
        c = np.single(0)
        for elem in arr:
            y = elem - c
            t = sums + y
            c = (t - sums) - y
            sums = t
        end = time.time()
        print(f"Run-time of compensated-sum is {end - start} \n")
        return sums

    else:
        sums = np.double(0)
        c = np.double(0)
        for elem in arr:
            y = elem - c
            t = sums + y
            c = (t - sums) - y
            sums = t
        end = time.time()
        print(f"Run-time of compensated-sum is {end - start} \n")
        return sums


def pairwise_sum(arr, is_single):
    if is_single:
        if len(arr) < N:
            sum = np.single(0)
            for elem in arr:
                sum += elem
            return sum
        else:
            m = math.floor(len(arr) / 2)
            return pairwise_sum(arr[0:m + 1], True) + pairwise_sum(arr[m + 1:], True)
    else:
        if len(arr) < N:
            sum = np.double(0)
            for elem in arr:
                sum += elem
            return sum
        else:
            m = math.floor(len(arr) / 2)
            return pairwise_sum(arr[0:m + 1], False) + pairwise_sum(arr[m + 1:], False)


def q2():
    x_axis = np.arange(np.single(1), mil_single + np.single(1))
    y_axis = nums_single(x_axis)
    n, c, p = naive_sum(y_axis, True), compensated_sum(y_axis, True), pairwise_sum(y_axis, True)
    print(f"The Naive Sum for Single Precision is {n} \n"
          f"The Compensated Sum for Single Precision is {c} \n"
          f"The Pairwise Sum for Single Precision is {p}\n")

    x_axis = np.arange(np.double(1), mil_double + np.double(1))
    y_axis = nums_double(x_axis)
    n, c = naive_sum(y_axis, False), compensated_sum(y_axis, False)
    start = time.time()
    p = pairwise_sum(y_axis, False)
    end = time.time()

    print(f"The Naive Sum for Double Precision is {n} \n"
          f"The Compensated Sum for Double Precision is {c} \n"
          f"The Pairwise Sum for Double Precision is {p}\n")

    print(f"Run-time of pairwise-sum is {end - start} \n")

def main():
    while (True):
        inp = input("Please Enter q1 or q2 for results of question 1 or 2:")
        if inp == "q1":
            q1()
            return
        elif inp == "q2":
            q2()
            return
        else:
            print("You have given an invalid input, please try again.")


if __name__ == "__main__":
    main()
