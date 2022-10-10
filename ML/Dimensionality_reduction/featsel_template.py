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
from nltk.stem import *
from nltk.stem.porter import *
import nltk
nltk.download('stopwords')
import nltk.corpus as corpus
from nltk.stem.snowball import SnowballStemmer
from nltk.stem.arlstem import ARLSTem
from sklearn.model_selection import cross_validate
from sklearn.base import BaseEstimator
from sklearn.metrics import make_scorer, accuracy_score
from sklearn.preprocessing import MaxAbsScaler
from sklearn.linear_model import LogisticRegression
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_selection import SelectKBest, chi2, RFECV, RFE
from sklearn.feature_selection._base import SelectorMixin
from sklearn.model_selection import cross_val_score, GridSearchCV
from sklearn.pipeline import Pipeline
from sklearn.datasets import load_digits
from sklearn.feature_selection import SelectKBest, chi2

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
    # To lower case
    #print("To lower case")
    txt = txt.lower()
    #print(txt.lower())

    # Remove links
    #print("Removing the links")
    url_regexp="https?://[\w\/\.]+"
    txt = re.sub(url_regexp, ' ', txt)
    #print(re.sub(url_regexp, ' ', txt))
    

    # Remove the HTML tags <.../>
    #print("Removing the HTML tags")
    txt = re.sub('<.*?>', '', txt)
    #print(re.sub('<.*?>', '', txt))

    # Remove punctuation
    #print("Removing the punctuation")
    txt = re.sub('[%s]' % re.escape(string.punctuation), ' ', txt)
    #print(re.sub('[%s]' % re.escape(string.punctuation), ' ', txt))

    # Remove linebreaks
    #print("Removing the linebreaks")
    tkt = re.sub('\n', ' ', txt)
    #print(re.sub('\n', ' ', txt))

    # Remove words containing numbers
    #print("Removing words containing numbers")
    txt = re.sub('\w*\d\w*', ' ', txt)
    #print(re.sub('\w*\d\w*', ' ', txt))

    # Remove duplicated characters that are present more than 2 times
    # like : reaaaaaaally => really
    #print("Removing duplicated letters")
    txt = re.sub(r'(.)\1{2,}', r'\1', txt)
    #print(txt = re.sub(r'(.)\1{2,}', r'\1', txt))    
    nltk_stop_words = corpus.stopwords.words('english')
    nltk_stop_words.remove("not")
    stemmer = SnowballStemmer("english", ignore_stopwords=True)
    txt = " ".join([stemmer.stem(w)for w in txt.split()])
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
    
    traindata, unsupdata, testdata = preprocess_imdb(num_jobs=-1)
    pipe = Pipeline([('vectorizer', CountVectorizer(ngram_range=ngram_range,min_df=min_df)),
        ('Max_abs', MaxAbsScaler()),
        ('Logistic_reg', LogisticRegression(solver="liblinear")),
        ])
    scores = cross_validate(pipe, traindata.data, traindata.target, cv=3,return_train_score=True)
    pipe.fit(traindata.data,traindata.target)
    print(scores['test_score'])

    print(pipe.score(testdata.data, testdata.target))
    logging.info("===> [END] Simple model")


def imdb_filter(num_features, C, ngram_range, min_df, num_jobs=-1):
    """
    Univariate filtering
    """
    logging.info("===> [BEGIN] Filter")
    traindata, unsupdata, testdata = preprocess_imdb(num_jobs=-1)
    chiiiiii_deux = chi2
    pipe = Pipeline([('vectorizer', CountVectorizer(ngram_range=ngram_range,min_df=min_df,binary=True)),
        ("filter", SelectKBest(chiiiiii_deux, k=num_features)),
        ('Max_abs', MaxAbsScaler()),
        ('Logistic_reg', LogisticRegression(solver="liblinear")),
        ])
    pipe.fit(traindata.data,traindata.target)
    acc_train = pipe.score(traindata.data, traindata.target)
    acc_test = pipe.score(testdata.data, testdata.target)
    print("acc_train= ", acc_train,"acc_test = ",acc_test)
    scores = cross_val_score(pipe,
                             traindata.data, traindata.target,
                             n_jobs=-1,
                             verbose=0)
    print(f"Real risk by {scores.size}-fold CV : {scores.mean():.2} (+/- {scores.std():.2})")
    # ^^^^^^^^^
    # vvvvvvvvv
    # Do your coding here
    # Extract the selected dimensions
    counter = pipe["vectorizer"]
    chi2_filter = pipe["filter"]
    selected_dims = chi2_filter.get_support()
    selected_scores = chi2_filter.scores_[selected_dims]
    sorted_idx = np.argsort(selected_scores)
    selected_terms = np.array(counter.get_feature_names_out())[selected_dims]
    print(selected_terms)
    logging.info("===> [Done] Filter")


def imdb_embedded(ngram_range=(1, 2), min_df=2, C=0.5, n_folds=5, num_jobs=-1):
    """
    LogisticRegression with L1 penalty
    """
    logging.info("===> [BEGIN] Embedded")
    traindata, unsupdata, testdata = preprocess_imdb(num_jobs=-1)
    pipe = Pipeline([('vectorizer', CountVectorizer(ngram_range=ngram_range,min_df=min_df,binary=True)),
        ('Max_abs', MaxAbsScaler()),
        ('Logistic_reg', LogisticRegression(C=C,penalty="l1",solver="liblinear")),
        ])
    logging.info("Fitting the classifier")
    pipe.fit(traindata.data, traindata.target)

    sparsity = sparsity_scorer(pipe)
    logging.info(f"Sparsity (fraction of zeros) : {100*sparsity:.2f}%")

    acc_train = pipe.score(traindata.data, traindata.target)
    acc_test = pipe.score(testdata.data, testdata.target)
    logging.info(
        f"""Train acc : {100*acc_train:.2f}%
                  Test acc : {100*acc_test:.2f}%"""
    )
    # Extract and save the selected vocabulary
    vocabulary = np.array(pipe.named_steps["vectorizer"].get_feature_names_out())
    selected_dims = pipe.named_steps["clf"].coef_.ravel() != 0
    selected_terms = vocabulary[selected_dims]
    weights = pipe.named_steps["Logistic_reg"].coef_.ravel()[selected_dims]
    sorted_idx = np.argsort(weights)

    logging.info(f"Original vocabulary size : {len(vocabulary)}")
    logging.info(f"Selected vocabulary size : {len(weights)}")
    logging.info("===> [Done] Embedded")




def imdb_wrapper(num_features=10000, step=10000, ngram=2, num_jobs=-1):
    """
    Recursive feature elimination with logistic regression as estimator
    """
    logging.info("===> [BEGIN] imdb_wrapper")
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
    # ^^^^^^^^^
    logging.info("===> [END] imdb_wrapper")


if __name__ == "__main__":
    # Test the preprocessing on one example review
    test_preprocess()

    # A simple baseline
    #simple_model(ngram_range=(1, 2), min_df=2)

    # Filters
    #imdb_filter(num_features=60000, C=0.5, ngram_range=(1, 2), min_df=2)

    # Embedded
    imdb_embedded(ngram_range=(1, 2), C=0.4)

    # Wrapper
   # imdb_wrapper(num_features=100000)

