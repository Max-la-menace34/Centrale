
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

def Z_moons(n) :
    noise = .2
    if n > 100 :
        return S_from_sklearn(sklearn.datasets.make_moons(n, noise=noise))
    else:
        S = S_from_sklearn(sklearn.datasets.make_moons(1000, noise=noise))
        np.random.shuffle(S)
        return S[0:n]

def overfitting_classifier() :
  return sklearn.svm.SVC(C=100, kernel='rbf', gamma = 20)

def good_classifier() :
  return sklearn.svm.SVC(C=100, kernel='rbf', gamma = .5)


def real_risk(h, loss, Z, nb_samples) :
    """ Estimates the real risk of k.

    Args:
        h          : the hypothesis y = h(X) (X is the arry of inputs, y the array of labels).
        loss       : loss(Y, Y') is the errors of predicting array Y while label is array Y'.
        Z          : a random variable : Z(n) = [[x1, y1], ... [xn, yn]]
        nb_samples : used to estimate the expectancy.

    Returns: 
        the real risk of h (estimated).
    """
    return randomvar.expectancy(randomvar.apply_array(lambda z_s : loss(h(x_of(z_s)), y_of(z_s)), Z), nb_samples)


def fit(make_sklearner, D) :
    """ Computes a functional random variable that is h_n,    
        the functions learned from  n-sized dataset.          
    
    Args:
        make_learner : a function such as make_learner() returns a new scikit algorithm.
        D            : a random variable such as D(1)[0] is a dataset.

    Returns:
        The h_n functional random variable.
    """
    def learn(d) :
        algo = make_sklearner()
        algo.fit(x_of(d), y_of(d))
        return predictor(algo)
    return randomvar.apply_retry(learn, D)

def diff(h1, h2, loss, Z, nb_samples) :
    """Computes E[loss(h1(X), h2(X))], with Z=(X,Y)

    Args:
        h1, h2     : the two hypothesis functions. h([x1,...,xn]) = [y1,...yn]
        loss       : the loss function : loss([y1,...yn], [y'1,...,y'n]) = [e1,...,en]
        Z          : The input random variable. Z(1)[0] = (xy) (single array).
        nb_samples : used to estimate the expectancy.

    Return:
        An estimation of E[loss(h1(X), h2(X))]
    """
    return randomvar.expectancy(randomvar.apply_array(lambda z_s : loss(h1(x_of(z_s)), h2(x_of(z_s))), Z), nb_samples)
