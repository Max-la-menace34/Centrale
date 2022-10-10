
# Python external modules
import numpy as np
# Local modules


def majority_vote(votes):
    """Returns the most frequent value in a list of votes

    Args:
        votes: List[TVal]

    Return:
        The most frequent TVal in votes
    """
    count = dict()
    for vote in votes:
        if vote in count:
            count[vote] += 1
        else:
            count[vote] = 1
    res = 0
    max_score = -1
    for value, score in count.items():
        if score > max_score:
            max_score = score
            res = value
    return res


def merge(merge_results, functions):
    """Returns a function that apply each function in functions to X and
    then merge the results with merge_result.

    Args:
        merge_results: a function Ndarray[Y] -> Y
        functions: A list of X->Y functions

    Return:
        A function m: NdArray[X] -> NdArray[Y]
    """
    def m(X):
        results = np.array([f(X) for f in functions])
        t_results = np.transpose(results)
        return np.array([merge_results(line) for line in t_results])
    return m


def majority_merge(functions):
    """
    Returns a function that returns the majority vote of the functions

    Args:
        functions: a list of X->Y functions

    Return:
        A function m: NdArray[X] -> NdArray[Y]
    """
    return merge(majority_vote, functions)


def average_merge(functions):
    """
    Returns a function that returns the majority vote of the functions

    Args:
        functions: a list of X->Y functions

    Return:
        A function m: NdArray[X] -> NdArray[Y]
    """
    return merge(lambda y: np.mean(y), functions)
