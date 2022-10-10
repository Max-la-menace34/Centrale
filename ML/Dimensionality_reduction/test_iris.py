#!/usr/bin/env python3

# Standard modules
import logging
# External modules
from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler
from sklearn.datasets import load_iris
from sklearn.pipeline import Pipeline
from sklearn.model_selection import train_test_split, cross_val_score


def main():
    # Set up loggin stuff
    logger = logging.getLogger('expipeline')
    FORMAT = '%(asctime)s %(levelname)-8s %(message)s'
    handler = logging.StreamHandler()
    handler.setFormatter(logging.Formatter(FORMAT, datefmt='%H:%M:%S'))
    logger.addHandler(handler)
    logger.setLevel(logging.INFO)

    # Load the data
    iris = load_iris()

    # Make the pipeline
    clf = Pipeline([
        ('scaler', StandardScaler()),
        ('clf', LinearSVC())
    ])

    # Split the data to get a training and test sets
    X_train, X_test, y_train, y_test = train_test_split(iris.data,
                                                        iris.target)
    # Fit the classifier
    logger.info("Fitting the classifier on a train/test split")
    clf.fit(X_train, y_train)

    # Compute and print the metrics
    logger.info("Computing the metrics")
    acc_train = clf.score(X_train, y_train)
    acc_test = clf.score(X_test, y_test)
    logger.info(f"""Train acc : {100*acc_train:.2f}%
                  Test acc : {100*acc_test:.2f}%""")

    logger.info("Cross validation")
    scores = cross_val_score(clf,
                             iris.data, iris.target,
                             n_jobs=-1,
                             verbose=0)
    logger.info(f"Real risk by {scores.size}-fold CV : {scores.mean():.2} (+/- {scores.std():.2})")


if __name__ == '__main__':
    main()
