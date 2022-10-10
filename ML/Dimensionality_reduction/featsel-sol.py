#!/usr/bin/python3

"""
Sample solution for the practical on preprocessing and dimensionality
reduction.
"""

# standard modules
import logging

logging.basicConfig(
    format="%(asctime)s %(levelname)-8s %(message)s",
    datefmt="%H:%M:%S",
    level=logging.INFO,
)
from joblib import Parallel, delayed
import re
import string

# external modules
from sklearn.base import BaseEstimator
from sklearn.metrics import make_scorer, accuracy_score
from sklearn.preprocessing import MaxAbsScaler
from sklearn.linear_model import LogisticRegression
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_selection import SelectKBest, chi2, RFECV, RFE
from sklearn.feature_selection._base import SelectorMixin
from sklearn.model_selection import cross_val_score, GridSearchCV
from sklearn.pipeline import Pipeline

import scipy.sparse
import numpy as np
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

# Local modules
import stanford_sentiment
import utils


def sparsity_scorer(clf, X=None, y=None, scale_coef=1.0):
    """
    Evaluates the sparsity of the estimator within a pipeline
    Can work with a LogisticRegression or a pipeline containing
    a LogisticRegression

    Parameters:
    -----------
        clf: a LogisticRegression or a Pipeline
        X: ndarray (unused)
        y: ndarray (unused)

    Returns
    -------
        sparsity: float in [0., 1.]
                  0. means all the dimensions are used,
                  1. means no dimension are used
    """
    if isinstance(clf, Pipeline):
        clf_item = None
        coef = scale_coef
        for k, v in clf.named_steps.items():
            if isinstance(v, SelectorMixin):
                coef *= v.get_support().sum() / v.get_support().size
            elif isinstance(v, LogisticRegression):
                clf_item = v
        if clf_item is None:
            raise RuntimeError("Cannot estimate the sparsity of the pipeline")
        else:
            return sparsity_scorer(clf_item, X, y, coef)
    elif isinstance(clf, LogisticRegression):
        non_null_coefs = clf.coef_.ravel() != 0
        sparsity = 1.0 - non_null_coefs.sum() / non_null_coefs.size * scale_coef
        return sparsity
    else:
        raise RuntimeError(f"Cannot estimate the sparsity of a {type(clf)}")


class LinearPipeline(BaseEstimator):
    def __init__(self, pipeline, clf_key):
        super().__init__()
        self.pipeline = pipeline
        self.clf_key = clf_key

    def fit(self, X, y=None):
        return self.pipeline.fit(X, y)

    def transform(self, X):
        return self.pipeline.transform(X)

    def score(self, X, y):
        return self.pipeline.score(X, y)

    @property
    def coef_(self):
        return self.pipeline.named_steps[self.clf_key].coef_


# @SOL
# class Vectorizer(TfidfVectorizer):
#     """TfIdf Vectorizer making use of unlabeled data
#     """

#     def __init__(self, unsupdata,
#                  input='content', encoding='utf-8',
#                  decode_error='strict', strip_accents=None, lowercase=True,
#                  preprocessor=None, tokenizer=None, analyzer='word',
#                  stop_words=None, token_pattern=r"(?u)\b\w\w+\b",
#                  ngram_range=(1, 1), max_df=1.0, min_df=1,
#                  max_features=None, vocabulary=None, binary=False,
#                  dtype=np.float64, norm='l2', use_idf=True, smooth_idf=True,
#                  sublinear_tf=False):
#         super().__init__(input=input, encoding=encoding,
#                          decode_error=decode_error,
#                          strip_accents=strip_accents,
#                          lowercase=lowercase, preprocessor=preprocessor,
#                          tokenizer=tokenizer, analyzer=analyzer,
#                          stop_words=stop_words, token_pattern=token_pattern,
#                          ngram_range=ngram_range, max_df=max_df, min_df=min_df,
#                          max_features=max_features, vocabulary=vocabulary,
#                          binary=binary, dtype=dtype, norm=norm,
#                          use_idf=use_idf, smooth_idf=smooth_idf,
#                          sublinear_tf=sublinear_tf)
#         self.unsupdata = unsupdata

#     def fit(self, X, y=None):
#         return super().fit(X + self.unsupdata)
# SOL@


def preprocess(txt):
    """Takes a document (str) as input and apply preprocessing on it :
        - to lowercase
        - remove URLs
        - remove html tags such as <br />
        - remove punctuation
        - remove linebreaks
        - remove words with numbers inside
        - remove words which contain repeated letters

    Parameters
    ----------
        txt : str
              The text to process

    Returns
    -------
        txt : str
              A preprocessed string
    """

    # vvvvvvvvv
    # Do your preprocessing on one document
    # @SOL
    # Change to lower case
    txt = txt.lower()

    # Remove links
    url_regexp = "https?://[\w\/\.]+"
    # url_regexp = 'https?://\S+'
    txt = re.sub(url_regexp, " ", txt)

    # Remove the HTML tags <...> (shortest with the '?'
    # character in the regexp)
    # i.e. matches '< br/>' in both '< br/>' and '< br/> >'
    txt = re.sub("<[ a-zA-Z]*/?>", " ", txt)
    # txt = re.sub('<.*?>+', ' ', txt)

    # Remove punctuation
    txt = re.sub("[%s]" % re.escape(string.punctuation), " ", txt)

    # Remove linebreaks
    txt = re.sub("\n", " ", txt)

    # Remove words containing numbers
    txt = re.sub("\w*\d\w*", " ", txt)

    # Remove duplicated characters that are present more than 2 times
    # like : reaaaaaaally => really
    txt = re.sub(r"(.)\1{2,}", r"\1", txt)
    # SOL@
    # ^^^^^^^^^
    return txt


def test_preprocess():
    """Test the preprocessing on an example review"""

    txt = """Is it worth seeing? If you can look past the technical and budgetary limitations, and get into the story, I think you
    will enjoy this, especially if you've actually read the original H G Wells novel. 
    If, however, you are easily put off by cheap production values, you'd best pass on this (unless you're a MST3K fan).
    Be warned, however that the film runs a full 3 hours, so I don't recommend watching it all in one sitting.<br /><br />
    BTW: An entirely different version of War of the Worlds (aka "INVASION") came out on DVD the same month that
    Spielberg's hit the theatres: http://imdb.com/title/tt0449040/.
    This was also made on a budget, but is updated to the present day like the Spielberg film - but it's much better! And to top it off, Jeff Wayne is making an animated film of his best-selling album from 1978, but that won't be out until 2007."""

    # Apply a preprocessing that is independent on the training set
    preprocessed = preprocess(txt)

    logging.info(
        f"""Example of text :
                    {txt}

                  Result of the preprocessing :
                    {preprocessed}
                """
    )


def preprocess_imdb(num_jobs=-1):
    """Loads and preprocess the IMDB stanford data
    if there is no cache to load.

    Returns
    -------

        traindata, unsupdata, testdata: sklearn.utils.Bunch
                These dictionnaries have a data and target
                attributs which hold what their name stands for.
                unsupdata has no target

    See also
    --------
    preprocess
    """

    cache_name = "imdb.pkz"
    # Preprocess all the texts
    try:
        traindata, unsupdata, testdata = utils.load_cache(
            cache_name, ["traindata", "unsupdata", "testdata"]
        )
        logging.info("Using cached dataset")
    except RuntimeError as err:
        traindata = stanford_sentiment.fetch_imdb(subset="train")
        unsupdata = stanford_sentiment.fetch_imdb(subset="unsup")
        testdata = stanford_sentiment.fetch_imdb(subset="test")

        def preprocess_data(data):
            return Parallel(n_jobs=num_jobs)(delayed(preprocess)(d) for d in data)

        logging.info("Preprocessing the training data")
        traindata.data = preprocess_data(traindata.data)
        logging.info("Preprocessing the test data")
        testdata.data = preprocess_data(testdata.data)
        logging.info("Preprocessing the unsupervised data")
        unsupdata.data = preprocess_data(unsupdata.data)

        utils.save_cache(
            cache_name,
            dict(traindata=traindata, unsupdata=unsupdata, testdata=testdata),
        )

    return traindata, unsupdata, testdata


def simple_model(ngram_range=(1, 2), min_df=2, num_folds=4, num_jobs=-1):
    """
    A simple baseline model with bag-of-words on n-grams and
    a logistic regression
    """

    logging.info("===> [BEGIN] Simple model")
    # vvvvvvvvv
    # Do your coding here

    # @SOL
    # Load the datasets
    traindata, _, testdata = preprocess_imdb(num_jobs=num_jobs)

    # Fit a simple linear model
    logging.info("Fitting the model")
    clf = Pipeline(
        [
            ("vectorizer", CountVectorizer(ngram_range=ngram_range, min_df=2)),
            ("scaler", MaxAbsScaler()),
            ("clf", LogisticRegression(solver="liblinear")),
        ]
    )
    clf.fit(traindata.data, traindata.target)

    # Extract and save the words and feature space size
    vectorizer = clf["vectorizer"]
    logging.info("Saving the vocabulary in words_simple.txt")
    vocabulary = vectorizer.get_feature_names_out()
    with open("words_simple.txt", "w") as f:
        f.write("\n".join(vocabulary))

    logging.info("Feature space size : {}".format(len(vocabulary)))
    # for i, wi in enumerate(vocabulary):
    #     print(f"Dim {i}, word {vocabulary[i]}, is present at max  {X_train[:,i].max()} times")

    # Compute and print the metrics
    logging.info("Computing the metrics")
    acc_train = clf.score(traindata.data, traindata.target)
    acc_test = clf.score(testdata.data, testdata.target)
    logging.info(
        f"""Train acc : {100*acc_train:.2f}%
                  Test acc : {100*acc_test:.2f}%"""
    )

    logging.info("Cross validation")
    scores = cross_val_score(
        clf,
        traindata.data,
        traindata.target,
        # X_train, y_train,
        cv=num_folds,
        n_jobs=num_jobs,
        verbose=0,
    )
    logging.info(
        f"Real risk by {num_folds}-fold CV : {scores.mean():.2} (+/- {scores.std():.2})"
    )
    # SOL@
    # ^^^^^^^^^
    logging.info("===> [END] Simple model")


def imdb_filter(num_features, C, ngram_range, min_df, num_jobs=-1):
    """
    Univariate filtering
    """
    logging.info("===> [BEGIN] Filter")

    # vvvvvvvvv
    # Do your coding here
    # @SOL
    traindata, unsupdata, testdata = preprocess_imdb(num_jobs)
    filter_function = chi2

    classifier = Pipeline(
        [
            (
                "vectorizer",
                CountVectorizer(binary=False, ngram_range=ngram_range, min_df=min_df),
            ),
            ("filter", SelectKBest(filter_function, k=num_features)),
            ("scaler", MaxAbsScaler()),
            ("clf", LogisticRegression(C=C, solver="liblinear")),
        ]
    )
    # The vectorizer is using binary features (presence/absence of a n-gram)
    # but we could use the count of n-grams . Maximizing the mean test acc :
    # Binary : True gave
    # {'clf__C': 0.1, 'filter__k': 40000}, 'sparsity': 0.8899269651588451, 'mean_accuracy': 0.89868}
    # Binary : False gave
    # {'clf__C': 0.5, 'filter__k': 60000}, 'sparsity': 0.8348904477382677, 'mean_accuracy': 0.9040800000000001}

    # Fit the pipeline
    logging.info("Fitting the classifier")
    classifier.fit(traindata.data, traindata.target)

    # Extract the selected dimensions
    counter = classifier["vectorizer"]
    chi2_filter = classifier["filter"]
    selected_dims = chi2_filter.get_support()
    selected_scores = chi2_filter.scores_[selected_dims]
    sorted_idx = np.argsort(selected_scores)
    selected_terms = np.array(counter.get_feature_names_out())[selected_dims]
    with open("words_filter.txt", "w") as f:
        f.write("\n".join(selected_terms[sorted_idx][::-1]))
    logging.info(
        f"The {selected_dims.sum()}/{counter.get_feature_names_out().size} selected words are saved in words_filter.txt"
    )

    logging.info("Measuring the metrics")
    acc_train = classifier.score(traindata.data, traindata.target)
    acc_test = classifier.score(testdata.data, testdata.target)

    logging.info(
        f"""Train acc : {100*acc_train:.2f}%
                 Test acc : {100*acc_test:.2f}%
                 Sparsity (fraction of dropped words) of the pipeline: {100*sparsity_scorer(classifier):.2f}%"""
    )

    # Doing a GridSearch for the regularization and sparsity
    param_grid = {
        "filter__k": [1000, 5000, 10000, 15000, 20000, 40000, 60000, 80000],
        "clf__C": [0.01, 0.05, 0.1, 0.5, 1.0, 2.0, 4.0],
    }
    grid = GridSearchCV(
        classifier,
        n_jobs=num_jobs,
        param_grid=param_grid,
        scoring={"sparsity": sparsity_scorer, "accuracy": make_scorer(accuracy_score)},
        verbose=2,
        refit=False,
    )

    logging.info("Grid searching")
    grid.fit(traindata.data, traindata.target)

    logging.info("Best parameters found")
    results = grid.cv_results_

    # Get the order of the predictors
    rank = np.array(results["rank_test_accuracy"]).argmin()
    best_pred_metrics = {
        "params": results["params"][rank],
        "sparsity": results["mean_test_sparsity"][rank],
        "mean_accuracy": results["mean_test_accuracy"][rank],
    }
    logging.info(
        f"Properties of the best predictor w.r.t. to cv mean accuracy : {best_pred_metrics}"
    )

    c_values = np.array(param_grid["clf__C"])
    k_values = np.array(param_grid["filter__k"])
    K, C = np.meshgrid(k_values, c_values)
    mean_sparsity = results["mean_test_sparsity"].reshape(
        (c_values.size, k_values.size)
    )
    mean_accuracy = results["mean_test_accuracy"].reshape(
        (c_values.size, k_values.size)
    )
    logging.info("The best CV test accuracy is ")
    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")
    ax.plot_wireframe(
        K, C, mean_sparsity.reshape((c_values.size, k_values.size)), color="tab:blue"
    )
    ax.plot_wireframe(
        K, C, mean_accuracy.reshape((c_values.size, k_values.size)), color="tab:red"
    )
    ax.set_xlabel(r"Number of features kepts by $\chi^2$")
    ax.set_ylabel("Inverse of regularization (C)")
    ax.set_zlabel("Sparsity (blue) / Accuracy (red)")
    plt.xticks(rotation=15)

    plt.tight_layout()
    plt.savefig("filter.pdf")
    logging.info("Plots saved in filter.pdf")
    # SOL@

    logging.info("===> [Done] Filter")


def imdb_embedded(ngram_range=(1, 2), min_df=2, C=0.5, n_folds=5, num_jobs=-1):
    """
    LogisticRegression with L1 penalty
    """
    logging.info("===> [BEGIN] Embedded")

    # vvvvvvvvv
    # Do your coding here
    # @SOL
    traindata, _, testdata = preprocess_imdb(num_jobs=num_jobs)

    classifier = Pipeline(
        [
            ("vectorizer", CountVectorizer(min_df=min_df, ngram_range=ngram_range)),
            # No big improvement with TfIdf and the unsup data
            # ('vectorizer', Vectorizer(unsupdata.data,
            #                           ngram_range=ngram_range,
            #                           min_df=min_df)),
            ("scaler", MaxAbsScaler()),
            ("clf", LogisticRegression(C=C, penalty="l1", solver="liblinear")),
        ]
    )

    # Fit the classifier
    logging.info("Fitting the classifier")
    classifier.fit(traindata.data, traindata.target)

    sparsity = sparsity_scorer(classifier)
    logging.info(f"Sparsity (fraction of zeros) : {100*sparsity:.2f}%")

    # Compute its metrics
    acc_train = classifier.score(traindata.data, traindata.target)
    acc_test = classifier.score(testdata.data, testdata.target)
    logging.info(
        f"""Train acc : {100*acc_train:.2f}%
                  Test acc : {100*acc_test:.2f}%"""
    )

    # Extract and save the selected vocabulary
    vocabulary = np.array(classifier.named_steps["vectorizer"].get_feature_names_out())
    selected_dims = classifier.named_steps["clf"].coef_.ravel() != 0
    selected_terms = vocabulary[selected_dims]
    weights = classifier.named_steps["clf"].coef_.ravel()[selected_dims]
    sorted_idx = np.argsort(weights)

    logging.info(f"Original vocabulary size : {len(vocabulary)}")
    logging.info(f"Selected vocabulary size : {len(weights)}")

    logging.info("Saving the selected words in words_embedded.txt")
    with open("words_embedded.txt", "w") as f:
        f.write(
            "{}".format(
                "\n".join(
                    [
                        "{} ({})".format(w, weight)
                        for w, weight in zip(
                            selected_terms[sorted_idx], weights[sorted_idx]
                        )
                    ]
                )
            )
        )
    logging.info("Selected words : {}".format(selected_terms[sorted_idx]))

    # Evaluate the risk of the pipeline
    logging.info("Cross validation")
    scores = cross_val_score(
        classifier,
        traindata.data,
        traindata.target,
        cv=n_folds,
        n_jobs=num_jobs,
        verbose=1,
    )
    logging.info(
        f"Real risk by {n_folds}-fold CV : {scores.mean():.2} (+/- {scores.std():.2})"
    )

    # Grid search
    param_grid = {"clf__C": [0.01, 0.05, 0.1, 0.5, 1.0]}
    grid = GridSearchCV(
        classifier,
        n_jobs=num_jobs,
        param_grid=param_grid,
        scoring={"sparsity": sparsity_scorer, "accuracy": make_scorer(accuracy_score)},
        verbose=1,
        refit=False,
    )
    grid.fit(traindata.data, traindata.target)

    results = grid.cv_results_

    fig, ax1 = plt.subplots()
    ax1.set_title("Sparsity and accuracy of a logistic regression with L1 penalty")
    ax1.set_xlabel("Inverse of the regularization coefficient (C)")
    color = "tab:red"
    ax1.set_ylabel("Mean accuracy on the test folds", color=color)
    ax1.plot(results["param_clf__C"].data, results["mean_test_accuracy"], color=color)
    # ax1.set_xticks(results['param_clf__C'].data.tolist())
    ax1.tick_params(axis="y", labelcolor=color)

    ax2 = ax1.twinx()
    color = "tab:blue"
    ax2.set_ylabel("Mean sparsity", color=color)
    ax2.plot(results["param_clf__C"].data, results["mean_test_sparsity"], color=color)
    ax2.tick_params(axis="y", labelcolor=color)

    fig.tight_layout()
    plt.show()
    # SOL@
    logging.info("===> [Done] Embedded")


# @SOL
def test_independance(ngram_range=(1, 2), min_df=2):

    cache_name = "test_independance.kz"

    try:
        counts_train, y_train, vocabulary = utils.load_cache(
            cache_name, ["counts_train", "y_train", "vocabulary"]
        )
    except RuntimeError as err:
        traindata, unsupdata, testdata = preprocess_imdb()

        # Check the unique words per document
        logging.info("Identifying the n-grams")
        counter = CountVectorizer(binary=True, ngram_range=ngram_range, min_df=min_df)
        counts_train = counter.fit_transform(traindata.data)
        vocabulary = counter.get_feature_names_out()
        y_train = traindata.target
        # counts[0].sum()  gives the number of words in the first document
        # print(traincounts.shape)
        utils.save_cache(
            cache_name,
            {
                "counts_train": counts_train,
                "y_train": y_train,
                "vocabulary": np.array(vocabulary),
            },
        )

    # We compute the contingency matrix for every feature
    t0_idx = y_train == 0
    num_t0 = t0_idx.sum()
    t1_idx = np.logical_not(t0_idx)
    num_t1 = t1_idx.sum()

    f1t0 = np.squeeze(np.asarray(counts_train[t0_idx].sum(axis=0)))
    f0t0 = (num_t0 - f1t0).ravel()
    f1t1 = np.squeeze(np.asarray(counts_train[t1_idx].sum(axis=0)))
    f0t1 = (num_t1 - f1t1).ravel()

    frequencies = np.column_stack([f0t0, f0t1, f1t0, f1t1])
    N = num_t0 + num_t1
    observed = frequencies / N
    expected = np.zeros_like(frequencies, dtype=float)
    # expected p(f=0, t=0)
    expected[:, 0] = ((f0t0 + f0t1) / N) * ((f0t0 + f1t0) / N)
    # expected p(f=0, t=1)
    expected[:, 1] = ((f0t0 + f0t1) / N) * ((f0t1 + f1t1) / N)
    # expected p(f=1, t=0)
    expected[:, 2] = ((f1t0 + f1t1) / N) * ((f0t0 + f1t0) / N)
    # expected p(f=1, t=1)
    expected[:, 3] = ((f1t0 + f1t1) / N) * ((f0t1 + f1t1) / N)

    chi2 = np.sum((N * observed - N * expected) ** 2.0 / (N * expected), axis=1)

    ratio = observed / expected
    ratio.ravel()[observed.ravel() == 0] = 1
    mi = np.sum(observed * np.log(ratio), axis=1)

    # sorted_idx = np.argsort(chi2)
    sorted_idx = np.argsort(mi)

    mystr = "\n".join(
        ",".join(map(str, v))
        for v in zip(
            vocabulary[sorted_idx],
            f0t0[sorted_idx],
            f0t1[sorted_idx],
            f1t0[sorted_idx],
            f1t1[sorted_idx],
            chi2[sorted_idx],
            mi[sorted_idx],
        )
    )

    with open("contingency.data", "w") as f:
        f.write("n-gram, f0t0, f0t1, f1t0, f1t1, chi2, MI\n")
        f.write(mystr)

    # plt.figure()
    # plt.plot(chi2[sorted_idx])
    # plt.plot(mi[sorted_idx])
    # plt.show()

    logging.info("Initial vocabulary of size {}".format(len(vocabulary)))


# SOL@


def imdb_wrapper(num_features=10000, step=10000, ngram=2, num_jobs=-1):
    """
    Recursive feature elimination with logistic regression as estimator
    """
    logging.info("===> [BEGIN] imdb_wrapper")

    # vvvvvvvvv
    # Do your coding here
    # @SOL
    # Load the datasets
    logging.info("Loading the data")

    cache_name = "imdb_wrapper.pkz"
    try:
        X_train, y_train, X_test, y_test, vocabulary = utils.load_cache(
            cache_name, ["X_train", "y_train", "X_test", "y_test", "vocabulary"]
        )
    except RuntimeError as err:
        traindata, _, testdata = preprocess_imdb(num_jobs=num_jobs)

        logging.info("Vectorizing the data")
        vectorizer = CountVectorizer(ngram_range=(1, ngram), min_df=2)
        X_train = vectorizer.fit_transform(traindata.data)
        y_train = traindata.target
        X_test = vectorizer.transform(testdata.data)
        y_test = testdata.target
        vocabulary = np.array(vectorizer.get_feature_names_out())

        utils.save_cache(
            cache_name,
            {
                "X_train": X_train,
                "y_train": y_train,
                "X_test": X_test,
                "y_test": y_test,
                "vocabulary": vocabulary,
            },
        )
    logging.info(f"Original vocabulary size : {len(vocabulary)}")

    classifier = Pipeline(
        [("scaler", MaxAbsScaler()), ("clf", LogisticRegression(solver="liblinear"))]
    )
    classifier = LinearPipeline(classifier, "clf")

    selector = RFE(classifier, n_features_to_select=num_features, step=step, verbose=1)
    logging.info("Performing the recursive feature elimination")
    selector.fit(X_train, y_train)

    # Compute its metrics
    acc_train = selector.score(X_train, y_train)
    acc_test = selector.score(X_test, y_test)
    logging.info(
        f"""Train acc : {100*acc_train:.2f}%
                  Test acc : {100*acc_test:.2f}%"""
    )

    selected_dims = selector.get_support()
    selected_terms = vocabulary[selected_dims]
    weights = selector.estimator_.pipeline.named_steps["clf"].coef_.ravel()
    sorted_idx = np.argsort(weights)

    logging.info(f"Original vocabulary size : {len(vocabulary)}")
    logging.info(f"Selected vocabulary size : {len(weights)}")

    with open("words_wrapper.txt", "w") as f:
        f.write(
            "{}".format(
                "\n".join(
                    [
                        "{} ({})".format(w, weight)
                        for w, weight in zip(
                            selected_terms[sorted_idx], weights[sorted_idx]
                        )
                    ]
                )
            )
        )
    logging.info("Selected words : {}".format(selected_terms[sorted_idx]))

    # Evaluate the risk of the pipeline
    logging.info("Cross validation")
    scores = cross_val_score(
        selector, X_train, y_train, cv=5, n_jobs=num_jobs, verbose=1
    )
    logging.info(f"Real risk by 5-fold CV : {scores.mean():.2} (+/- {scores.std():.2})")
    # SOL@
    # ^^^^^^^^^
    logging.info("===> [END] imdb_wrapper")


if __name__ == "__main__":
    # Test the preprocessing on one example review
    #test_preprocess()

    # A simple baseline
    #simple_model(ngram_range=(1, 2), min_df=2)

    # Filters
    #imdb_filter(num_features=60000, C=0.5, ngram_range=(1, 2), min_df=2)

    # Embedded
    imdb_embedded(ngram_range=(1, 2), C=0.4)

    # Wrapper
    #imdb_wrapper(num_features=100000)

    # @SOL
    # For the example done during the lecture
    #test_independance()
    # SOL@
