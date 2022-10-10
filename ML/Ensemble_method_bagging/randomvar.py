
# Python external modules
import numpy as np
# Local modules


def uniform():
    """
    Builds a uniform random variable

    Returns:
        A function int->ndarray which returns a
            collection of n samples drawn according
            to a uniform distribution
    """
    return lambda nb: np.random.rand(nb)


def display(V, label, nb_samples=10):
    """
    Displays samples a random variable V

    Args:
        V: a random variable
        label : a string labeling the variable
        nb_samples: the number realisations of V
            we want to display
    """
    print('{} :'.format(label))
    for v in V(nb_samples):
        print('   {}'.format(v))


def expectancy(V, nb_samples):
    """Computes the sampled average of a random variable

    Args:
        V:a random variable (i.e. a function int->ndarray)
        nb_samples: the number of observations with which to compute
            the sampled mean

    Return:
        A float, the sampled mean
    """
    return np.mean(V(nb_samples))


def apply(f, V):
    """Computes the random variable f(V)

    Args:
        f: a function  X -> Z
        V: a random variable with realisations in X

    Return:
        The random variable f(V) with realisations in Z

    """
    # Note: given V(1) is a ndarray with a single value, we need to
    # get it with V(1)[0] before going through f
    return lambda nb, f=f, V=V: np.array([f(V(1)[0]) for i in range(nb)])


def apply_retry(f, V):
    """Computes and returns the random variable f(V).
    If an exception is thrown when getting realization of f(V), the
    computation is retried until it works fine.

    Args:
        f: a function X -> Z
        V: a random variable with realisations in X

    Return:
        The random variable f(V) with realisations in Z

    """
    def compute():
        while True:
            try:
                return f(V(1)[0])
            except Exception as e:
                pass
    return lambda nb, f=f, V=V: np.array([compute() for i in range(nb)])


def apply_array(f, V):
    """Computes the f(V) random variable from random variable V.

    This function does the same as apply(f, V) but is more efficient
    when f can be vectorized and handles ndarrays

    Args:
        f: a function X -> Z     (X in an array here).
        V: a random variable with realisations in X

    Return:
        The random variable f(V) with realisations in Z
    """
    return lambda nb, f=f, V=V: f(V(nb))


def nuplet(V, n):
    """Computes a random variable (V1, V2, ..., VN) from V

    Args:
        V: a random variable (i.e. a int -> X function)
        n: the dimension of the tuple

    Return:
        A random variable with realisations in X^n
    """
    return lambda nb, n=n: np.array([V(n) for i in range(nb)])


def bootstrap(S):
    """Computes a random variable as a bootstrap of samples S.

    Args:
        S: A 2D ndarray S-dataset

    Return:
        A random variable with realisations as bootstraps of S
    """
    return lambda nb, S=S: S[np.random.choice(np.arange(S.shape[0]), nb)]


if __name__ == "__main__":

    print()
    print()
    print("### Test uniform")
    V = uniform()
    print('E = {}'.format(expectancy(V, 1000)))
    display(V, 'uniform', 5)

    print()
    print()
    print("### Test 10*V")
    V10 = apply(lambda x: 10*x, V)
    display(V10, '10*V', 5)
    V10 = apply_array(lambda x: (10*x).astype(int), V)
    display(V10, '10*V (via array)', 5)

    print()
    print()
    print("### Test (V, ....)")
    Vn = nuplet(V10, 10)
    display(Vn, 'Vn', 5)

    print()
    print()
    print("### Bootstrap")
    S = np.arange(0, 20)
    V = bootstrap(S)
    display(nuplet(V, 10), 'Bootstrap', 10)
