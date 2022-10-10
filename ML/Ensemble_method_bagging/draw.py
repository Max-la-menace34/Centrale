# coding : utf-8

# Python external modules
import matplotlib.pyplot as plt
import numpy as np
# Local modules
import ml


def no_tics(ax):
    """Disable the tics marks on a matplotlib axis"""
    ax.get_xaxis().set_ticks([])
    ax.get_yaxis().set_ticks([])


def equal(ax):
    """Equalize the axis dimensions"""
    ax.set_aspect('equal')


def samples_2d(ax, S,
               colors=np.array(['#377eb8', '#ff7f00']),
               dotsize=None):
    """Draws a collection of samples as dots on an axis

    Args:
        ax : a matplotlib axis
        S : a 2D ndarray of size N x 3
            S[:,2] is considered as the label in {0, 1}
    """
    ax.scatter(S[:, 0], S[:, 1], c=colors[S[:, 2].astype(int)], s=dotsize)


def prediction_2d(ax, h, X,
                  colors=np.array(['#377eb8', '#ff7f00']),
                  dotsize=None):
    """Plot the predictions of h on X

    Args:
        ax : a matplotlib axis
        h : a predictor, i.e. a X -> Y function
        X : a collection of inputs
    """
    y = h(X)
    samples_2d(ax, ml.S_from_sklearn((X, y)), colors, dotsize=dotsize)


def decision_boundary_2d(ax, h, xbounds, ybounds,
                         Nsamples=100,
                         colors=np.array(['#377eb8', '#ff7f00'])):
    """This computes and plot the decision boundary of a classifier
    It does not trigger plt.show(), you must call it on your own

    By default, we suppose h is a classifier output a value in {0, 1}

    Args:
        ax : a matplotlib axis
        h : a predictor, i.e. a X -> Y function
        xbounds : pair (tuple) of xmin, xmax
        ybounds : pair (tuple) of ymin, ymax
        Nsamples : number of subdivisions of the x and y axis
        colors : optional numpy array
                 The colors to be used for plotting the
                 samples and decision boundary

    """
    dx = (xbounds[1] - xbounds[0])/float(Nsamples)
    dy = (ybounds[1] - ybounds[0])/float(Nsamples)

    xx, yy = np.meshgrid(np.arange(xbounds[0], xbounds[1]+dx, dx),
                         np.arange(ybounds[0], ybounds[1]+dy, dy))

    S = h(np.c_[xx.ravel(), yy.ravel()])
    S = S.reshape(xx.shape)
    ax.contourf(xx, yy, S, alpha=0.4,
                levels=[0, 0.5, 1.0], colors=colors)
