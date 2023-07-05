import numpy as np

def monte_carlo_pi(n_trials: int):
    """
    Computes an approximation of pi using the Monte Carlo method.
    Parameters:
    - n_trials (int): the number of trials to run
    Returns:
    - approximation (float): the approximation of pi
    - error (float): the error of the approximation
    """
    # Generate points in the square 
    Xco = np.random.uniform(-1, 1, (n_trials,))
    Yco = np.random.uniform(-1, 1, (n_trials,))
    # calculate the distance from the origin
    dist = np.linalg.norm(np.column_stack((Xco, Yco)), axis=1)
    # count the number of points inside the circle
    count = np.sum(dist <= 1)
    # calculate the approximation of pi
    approximation = 4 * count / n_trials
    # calculate the error of the approximation
    error = (np.pi - approximation) / np.pi * 100
    return approximation, error

n_trials = 10**5
approximation, error = monte_carlo_pi(n_trials)
print("Points Generated = ", n_trials)
print("Points Included = ", count)
print("Approximated Pi = {:.5f}".format(approximation))
