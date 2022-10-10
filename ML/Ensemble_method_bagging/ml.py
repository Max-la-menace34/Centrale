
# Python external modules
import numpy as np
import sklearn.datasets
import sklearn.svm
# Local modules
import randomvar


def predictor(classifier):
    """This turns a fitted classifier into a real function.

    Args:
        classifier: a sklearn predictor in the (X, Y) space

    Returns:
        A function X->Y
    """
    return lambda X, clf=classifier: clf.predict(X)


def S_from_sklearn(XY):
    """S_from_sklearn(([x1, ... xn], [y1, ... yn])) --> [[x1, y1], ... [xn, yn]]

    Args:
        XY: Tuple of inputs, outputs as usually provided by the datasets
            of sklearn

    Return:
        A ndarray where the i-th row contains the X[i], y[i]
    """
    return np.column_stack((XY[0], XY[1]))


def x_of(S):
    """Returns the inputs of a S format dataset

    X is supposed to occupy the first columns
    Y is supposed to occupy the very last column

    Args:
        S: A 2D ndarray

    Return:
        The first columns of S
    """
    return S[..., 0:-1]


def y_of(S):
    """Return the output of a S format dataset

    X is supposed to occupy the first columns
    Y is supposed to occupy the very last column

    Args:
        S: A 2D ndarray

    Return:
        The very last column of S
    """
    return S[..., -1]


def S_to_sklearn(S):
    """S_to_sklearn([[x1, y1], ... [xn, yn]] --> [x1, ... xn], [y1, ... yn]

    Args:
        S: a dataset as a 2D ndarray

    Return:
        The tuple (X, y) made of the first columns and the very last
    """
    return x_of(S), y_of(S)
def overfitting_classifier() :
    return sklearn.svm.SVC(C=100, kernel='rbf', gamma = 20)

def good_classifier() :
    return sklearn.svm.SVC(C=100, kernel='rbf', gamma = .5)
def real_risk(h,loss,Z,n):
    y = randomvar.apply_array(lambda z : loss(h(x_of(z)), y_of(z)), Z)
    return randomvar.expectancy(y, n);

def Z_moons(n) :
    noise = .2
    if n > 100 :
        return S_from_sklearn(sklearn.datasets.make_moons(n, noise=noise))
    else:
        S = S_from_sklearn(sklearn.datasets.make_moons(1000, noise=noise))
        np.random.shuffle(S)
        return S[0:n]

def fit(good_classifier ,D):
    def learn_for_one_dataset(d):
            dx = x_of(d)
            dy= y_of(d)
            model= good_classifier()
            model.fit(dx,dy)
            return predictor(model)
    
    return randomvar.apply_retry(learn_for_one_dataset,D)

def diff(h1, h2, loss, Z, N):
    y = randomvar.apply_array(lambda z: loss(h1(x_of(z)), h2(x_of(z))), Z)
    return randomvar.expectancy(y, N)