import pandas as pd
import numpy as np
import autosklearn.regression as autocl
from sklearn.model_selection import train_test_split
import sklearn.metrics

import sklearn.datasets
import matplotlib.pyplot as plt
from scipy import stats
import xgboost as xgb
from autosklearn.metrics import balanced_accuracy, precision, recall, f1
from sklearn import (manifold, datasets, preprocessing, model_selection, decomposition, metrics, multioutput)
from sklearn.preprocessing import StandardScaler
import sklearn.preprocessing


# select user input
user_input = int(input("Which user's result do you want to look at?: "))

# read the dataframe
data = pd.read_csv('data.csv')
df = data[data['user_number'] == user_input]

# drop all Null data (filtering null values)
df.dropna()


# split the dataset into training and test data
X = df.iloc[:,:-2].values
y = df["symptom_value"].values

n_samples = X.shape[0]
n_train = int(n_samples * 0.8)
indices = np.arange(n_samples)
np.random.shuffle(indices)

# pre processing the data and normalizing the data
def preprocessing(X):
    from sklearn.preprocessing import StandardScaler
    import sklearn.preprocessing
    std_scaler = StandardScaler()
    X = std_scaler.fit_transform(X)
    X = sklearn.preprocessing.normalize(X, norm='l2')
    return X
X = preprocessing(X)

# split the dataset into training and test data
train_indices = indices[:n_train]
test_indices = indices[n_train:]

X_train = X[train_indices]
y_train = y[train_indices]

X_test = X[test_indices]
y_test = y[test_indices]

# train the module and fit and refit the model
automl = autocl.AutoSklearnRegressor(
    time_left_for_this_task = 120,
    per_run_time_limit = 30,
    disable_evaluator_output = False,
    resampling_strategy = "cv",
    resampling_strategy_arguments = {"folds": 10},
)
automl.fit(X_train, y_train, dataset_name='IBS')
print("Before re-fit")
predictions = automl.predict(X_test)
print("Test R2 score:", sklearn.metrics.r2_score(y_test, predictions))
print("#" * 80)
print("After re-fit")
automl.refit(X_train.copy(), y_train.copy())
# pickle.dump(automl, open('model.sav', 'wb'))
predictions = automl.predict(X_test)
print("Test R2 score:", sklearn.metrics.r2_score(y_test, predictions))
print("#" * 80)

# View the models found by auto-sklearn
print(automl.leaderboard())

