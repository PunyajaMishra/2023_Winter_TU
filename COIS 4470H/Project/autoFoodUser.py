#imports
import pandas as pd
import numpy as np
import autosklearn.classification as autocl
from sklearn.model_selection import train_test_split

CONSTRAINT = 0.3

def bars(shap_values):
    # summarize the SHAP values for each feature
    shap.summary_plot(shap_values, X, plot_type='bar', show=False)

    # plot the SHAP values for each feature
    fig, ax = plt.gcf(), plt.gca()
    ax.set_xlim(-0.5, 2)
    ax.set_title("Food Group Impact")
    plt.show()

# select user input
user_input = int(input("Which user's result do you want to look at?: "))

# read the dataframe
data = pd.read_csv('data.csv')
df = data[data['user_number'] == user_input]

# drop all Null data (filtering null values)
df.dropna()

# feature seletion to determine correllation between the colummn and symtpom val
# using corrwidth to compare all values of columns F1-F20 to the symptom_value
# for each data point. Using method - "Spearman Correlation"
#correlation = df.iloc[:,:-2].corrwith(df['symptom_value'],method='spearman').abs()
# filtering all food categories with vorrelation value >0.3 and indexing to get names from df header
#high_corr_categories = correlation[correlation>CONSTRAINT].index.tolist()

# print the dataframe
# df2 = df[high_corr_categories]
# df2 = df2.join(df.iloc[:,-2:])
# print("Highest affecting food group categories:", high_corr_categories)
# print(df2)

# split the dataset into training and test data
X = df.iloc[:,:-2].values
y = df["symptom_value"].values

n_samples = X.shape[0]
n_train = int(n_samples * 0.8)
indices = np.arange(n_samples)
np.random.shuffle(indices)

train_indices = indices[:n_train]
test_indices = indices[n_train:]

X_train = X[train_indices]
y_train = y[train_indices]

X_test = X[test_indices]
y_test = y[test_indices]

# create a DMatrix for XGBoost
# dtrain = xgb.DMatrix(X, label=y)

# specify XGBoost parameters
# params = {'max_depth': 3, 'eta': 0.1, 'objective': 'reg:squarederror'}

# train the model
#X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=1)

# model = xgb.train(params, dtrain)
automl = autocl.AutoSklearnClassifier(
    time_left_for_this_task=120,
    per_run_time_limit=30,
)
automl.fit(X_train, y_train)

# print the results
print(automl.leaderboard())

# print the accuracy
predictions = automl.predict(X_test)
print("Accuracy score:", sklearn.metrics.accuracy_score(y_test, predictions))

# create an explainer object for SHAP
#explainer = shap.Explainer(model, X)

# calculate SHAP values for each feature for each instance
#shap_values = explainer(X)

# call the bars() function
#bars(shap_values)

# plot the heatmap
#shap.plots.beeswarm(shap_values)


