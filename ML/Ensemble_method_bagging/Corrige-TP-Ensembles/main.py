import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import sys

import ml
import draw
import randomvar
import functional

def good_and_overfitting():
    Z    = ml.Z_moons
    S    = Z(20) 

    clf1 = ml.good_classifier()
    clf1.fit(ml.x_of(S), ml.y_of(S))
    h1   = ml.predictor(clf1)

    clf2 = ml.overfitting_classifier()
    clf2.fit(ml.x_of(S), ml.y_of(S))
    h2   = ml.predictor(clf2)
    
    plt.figure(figsize = (10,5))
    xbounds = (-2, 3)
    ybounds = (-1.5, 2)

    ax = plt.subplot(1, 2, 1)
    ax.set_title('good classifier')
    draw.equal(ax)
    draw.no_tics(ax)
    draw.decision_boundary_2d(ax, h1, xbounds, ybounds, Nsamples=200)
    draw.samples_2d(ax, S)

    ax = plt.subplot(1, 2, 2)
    ax.set_title('overfitting classifier')
    draw.equal(ax)
    draw.no_tics(ax)
    draw.decision_boundary_2d(ax, h2, xbounds, ybounds, Nsamples=200)
    draw.samples_2d(ax, S)
    plt.show();

def learning() : # This is the code given in the subjet.
    Z = ml.Z_moons
    D = randomvar.nuplet(Z, 20)
    h_n = ml.fit(ml.good_classifier, D)
    
    plt.figure(figsize = (10, 10))
    xbounds = (-2, 3)
    ybounds = (-1.5, 2)
    for i, h in enumerate(h_n(25)):
        ax = plt.subplot(5, 5, i+1)
        draw.no_tics(ax)
        draw.decision_boundary_2d(ax, h, xbounds, ybounds, Nsamples=200)
    plt.show()

def statistical_difference() : # This is the code given in the subjet.
    Z = ml.Z_moons
    S = Z(100)
    D = randomvar.nuplet(Z, 20)
    h_n = ml.fit(ml.good_classifier, D)
    loss = lambda y1, y2 : y1 != y2
    N = 1000 # for expectancy estimation
    
    h1, h2 = h_n(2)
    h3 = lambda X, f=h1, g=h2: loss(f(X), g(X))
    
    plt.figure(figsize = (9, 3))
    xbounds = (-2, 3)
    ybounds = (-1.5, 2)

    ax = plt.subplot(1, 3, 1)
    draw.no_tics(ax)
    draw.decision_boundary_2d(ax, h1, xbounds, ybounds, Nsamples=200)
    
    ax = plt.subplot(1, 3, 2)
    draw.no_tics(ax)
    draw.decision_boundary_2d(ax, h2, xbounds, ybounds, Nsamples=200)
    
    ax = plt.subplot(1, 3, 3)
    ax.set_title('statistical difference = {}'.format(ml.diff(h1, h2, loss, Z, N)))
    draw.no_tics(ax)
    draw.decision_boundary_2d(ax, h3, xbounds, ybounds, Nsamples=200)
    draw.prediction_2d(ax, h3, ml.x_of(S))
    
    plt.show()
    
def aggregating() :
    n     = 20
    k     = 20
    Z     = ml.Z_moons
    D     = randomvar.nuplet(Z, n)
    h_n   = ml.fit(ml.overfitting_classifier, D)
    loss  = lambda y1, y2 : y1 != y2
    H_nk  = randomvar.nuplet(h_n, k)
    a_nk  = randomvar.apply(functional.majority_merge, H_nk)
    
    plt.figure(figsize = (12, 12))
    xbounds = (-2, 3)
    ybounds = (-1.5, 2)

    
    ax = plt.subplot(5, 5, 1)
    ax.set_xlim(xbounds)
    ax.set_ylim(ybounds)
    ax.set_title('a d, n={}'.format(n))
    draw.no_tics(ax)
    draw.samples_2d(ax, D(1)[0], dotsize=3)
    
    ax = plt.subplot(5, 5, 2)
    ax.set_title('a h_{}'.format(n))
    draw.no_tics(ax)
    draw.decision_boundary_2d(ax, h_n(1)[0], xbounds, ybounds, Nsamples=200)

    for i, aggreg in enumerate(a_nk(23)):
        print('plot {:02d}/25'.format(i+3))
        ax = plt.subplot(5, 5, i+3)
        ax.set_title('a {}-sized aggreg.'.format(k))
        draw.no_tics(ax)
        draw.decision_boundary_2d(ax, aggreg, xbounds, ybounds, Nsamples=200)
    plt.show()
    
def variance_analysis_warmup() :
    ks    = [k for k in range(1,26)]
    Z     = ml.Z_moons
    loss  = lambda y1, y2 : y1 != y2

    N = 1000 # for expectancy estimation (diff)
    M = 50   # for distribution plotting

    n = 10
    D   = randomvar.nuplet(Z, n)
    h_n = ml.fit(ml.overfitting_classifier, D)
    dots_x = []
    dots_y = []
    avgs   = []
    stds   = []
    for k in ks :
        print('{:02d}/{}'.format(k, ks[-1]))
        a_nk    = randomvar.apply(functional.majority_merge, randomvar.nuplet(h_n, k))
        diff    = randomvar.apply(lambda h1h2 : ml.diff(h1h2[0], h1h2[1], loss, Z, N),
                               randomvar.nuplet(a_nk, 2))
        diffs   = diff(M)
        dots_x += [k]*M
        dots_y += list(diffs)
        avgs.append(np.mean(diffs))
        stds.append(np.std(diffs))
    avgs = np.array(avgs)
    stds = np.array(stds)
    plt.figure()
    plt.title('2 by 2 statistical differences among the a_nk')
    plt.xlabel('k')
    plt.scatter(dots_x, dots_y, s=3, alpha=.5, zorder=1)
    plt.plot(ks, avgs, 'r-', zorder=3)
    plt.fill_between(ks, avgs - stds, avgs + stds, color='k', alpha=.1, zorder=2)
    plt.show()
    

def variance_analysis_hn() :
    Z     = ml.Z_moons
    loss  = lambda y1, y2 : y1 != y2

    N = 1000 # for expectancy estimation (diff)
    K =   50 # for expectancy estimation (E(h_n))
    M =   50 # for expectancy estimation (E(h_n - E))
    ns = [n for n in range(10, 101, 10)];
    stddevs = []
    for n in ns:
        print('{:02d}/{}'.format(n, ns[-1]))
        D    = randomvar.nuplet(Z, n)
        h_n  = ml.fit(ml.overfitting_classifier, D)
        a_nK = randomvar.apply(functional.majority_merge, randomvar.nuplet(h_n, K))
        E_hn = a_nK(1)[0]
        diff = randomvar.apply(lambda h : ml.diff(h, E_hn, loss, Z, N), h_n)
        stddevs.append(randomvar.expectancy(diff, M))
    plt.figure()
    plt.title('Std. dev. of h_n')
    plt.xlabel('n')
    plt.plot(ns, stddevs, 'r-')
    plt.show()
    
    
def variance_analysis_ank() :
    Z     = ml.Z_moons
    loss  = lambda y1, y2 : y1 != y2
    n     = 10
    D     = randomvar.nuplet(Z, n)
    h_n   = ml.fit(ml.overfitting_classifier, D)

    N  = 1000 # for expectancy estimation (diff)
    K  =   50 # for expectancy estimation (E(h_n) or E(a_nk))
    M  =   50 # for expectancy estimation (E(h_n - E) or E(a_nk - E))

    # First, let us compute de stddev of h_n as previously, for a
    # single n. This will be a reference stddev, i.e. the one of a_nk
    # should be reduced.
    
    a_nK       = randomvar.apply(functional.majority_merge, randomvar.nuplet(h_n, K))
    E_hn       = a_nK(1)[0]
    diff       = randomvar.apply(lambda h : ml.diff(h, E_hn, loss, Z, N), h_n)
    stddev_ref = randomvar.expectancy(diff, M)

    print()
    print('stddev(h_{}) = {}'.format(n, stddev_ref))
    print()
    
    stddevs = []
    ks = [k for k in range(1, 52, 5)]
    for k in ks :
        print('{:02d}/{}'.format(k, ks[-1]))
        a_nk  = randomvar.apply(functional.majority_merge, randomvar.nuplet(h_n, k))
        A_nK  = randomvar.apply(functional.majority_merge, randomvar.nuplet(a_nk, K))
        E_ank = A_nK(1)[0]
        diff  = randomvar.apply(lambda h : ml.diff(h, E_ank, loss, Z, N), a_nk)
        stddevs.append(randomvar.expectancy(diff, M))
    plt.figure()
    plt.title('Std. dev. of a_nk, n={}'.format(n))
    plt.xlabel('k')
    plt.axhline(stddev_ref, color='k', ls='--', alpha=.2)
    plt.plot(ks, stddevs, 'r-')
    plt.show()
    
    
def bootstrapping() :
    ks    = [k for k in range(1, 52, 5)]
    def variance_analysis(D, Z, tag): # This is similar to the previous code
        loss  = lambda y1, y2 : y1 != y2
        h_n   = ml.fit(ml.overfitting_classifier, D)
        N     = 1000 # for expectancy estimation (diff)
        K     =   10 # for expectancy estimation (E(h_n) or E(a_nk))
        M     =   10 # for expectancy estimation (E(h_n - E) or E(a_nk - E))
        stddevs = []
        for k in ks :
            print('{} : {:02d}/{}'.format(tag, k, ks[-1]))
            a_nk  = randomvar.apply(functional.majority_merge, randomvar.nuplet(h_n, k))
            A_nK  = randomvar.apply(functional.majority_merge, randomvar.nuplet(a_nk, K))
            E_ank = A_nK(1)[0]
            diff  = randomvar.apply(lambda h : ml.diff(h, E_ank, loss, Z, N), a_nk)
            stddevs.append(randomvar.expectancy(diff, M))
        return np.array(stddevs)
    n          = 10
    Z          = ml.Z_moons
    D          = randomvar.nuplet(Z, n)
    stddev_iid = variance_analysis(D, Z, 'iid')
    B          = randomvar.bootstrap(randomvar.nuplet(Z, 2*n)(1)[0])
    D          = randomvar.nuplet(B, n)
    stddev_bst = variance_analysis(D, Z, 'bootstrap')
    plt.figure()
    plt.title('Std. dev. of a_nk, n={}'.format(n))
    plt.xlabel('k')
    plt.plot(ks, stddev_iid, label='iid')
    plt.plot(ks, stddev_bst, label='bootstrap')
    plt.legend()
    plt.show()
        
    
        
    

if __name__ == "__main__":
    
    functions = [
        good_and_overfitting,
        learning,
        statistical_difference,
        aggregating,
        variance_analysis_warmup,
        variance_analysis_hn,
        variance_analysis_ank,
        bootstrapping
    ]
    
    if len(sys.argv) != 2 :
        print('usage : {} <function id>'.format(sys.argv[0]))
        print()
        print('id | function')
        print('---+---------')
        for id, f in enumerate(functions) :
            print('{:2d} | {}'.format(id, f.__name__))
        print()
        sys.exit()

    id = int(sys.argv[1])
    if(id < 0 or id >= len(functions)) :
        print('Function id {} is invalid (should be in [0, {}])'.format(id, len(functions)-1))
        sys.exit()
    functions[id]()
