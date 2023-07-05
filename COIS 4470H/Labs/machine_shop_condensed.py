import math, random, numpy as np, pandas as pd

LAST, START, M, P = 10000, 0.0, 50, [0.1, 0.2, 0.5, 0.7, 0.9]

class Sum: wait, delay, service, interarrival = 0.0, 0.0, 0.0, 0.0

get_failure = lambda: -100 * np.log(1.0 - random.uniform(0,1))
next_failure = lambda failure: np.amin(failure)
get_service = lambda: np.random.uniform(10, 20)
quotient = lambda a, b: a / b if b != 0 else 0.0

def main(p):
    index, arrival, delay, service, wait, departure, n_tasks, t_tasks = 0, START, 0.0, 0.0, 0.0, START, 0, 0
    failure = np.array([START + get_failure() for _ in range(M)])
    sum = Sum()
    while index < LAST:
        index += 1
        arrival = next_failure(failure)
        delay = departure - arrival if arrival < departure else 0.0
        n_tasks = np.random.geometric(p) + 1
        t_tasks = np.random.uniform(10, 20, size=n_tasks).sum()
        service = t_tasks
        wait = delay + service
        departure = arrival + wait
        m = np.argmin(failure)
        failure[m] = departure + get_failure()
        sum.wait += wait
        sum.delay += delay
        sum.service += service
    sum.interarrival = arrival - START
    return [quotient(sum.delay, index), quotient(sum.service, index), quotient(sum.service, departure) * 100]

results = {p: main(p) for p in P}
df = pd.DataFrame.from_dict(results, orient='index', columns=['Average delay time', 'Average service time', 'Server utilization'])
df.index.name = 'P'
print(df)