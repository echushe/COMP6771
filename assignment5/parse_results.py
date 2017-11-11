import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from collections import defaultdict

from math import sqrt

def timeToSpeed(ms):
    s = float(ms) / 1000
    return (float(nitems) / 1000000) / s


means = defaultdict(lambda: [0])
errors = defaultdict(lambda: [0])
average = defaultdict(lambda: [0])

f = open('results.csv')
nitems = int(f.readline())

for line in f:
    dataset, cores, *results = line.strip().split(',')
    cores = int(cores)
    results = list(map(timeToSpeed, results))
    mean = float(sum(results)) / len(results)
    deviation = sqrt(sum((mean - x)**2 for x in results) / len(results))
    means[dataset].append(mean)
    average[cores].append(mean)
    errors[dataset].append(deviation)

for core in sorted(average):
    mean = float(sum(average[core])) / len(average[core])
    deviation = sqrt(sum((mean - x) ** 2 for x in average[core]) / len(average[core]))
    means['Average'].append(mean)
    errors['Average'].append(0)

fig = plt.figure(figsize=(20, 10))
ax = plt.subplot(111)

tests = sorted(means, key=lambda x: '' if x == 'Average' else x)
colors = dict(zip(tests, 'bgrcmyk'))
legend = []
for key in tests:
    legend.append(mpatches.Patch(color=colors[key], label=key))
    ax.errorbar(range(len(means[key])), means[key], yerr=errors[key], c=colors[key], linewidth=2)
    plt.xlim(0, len(means[key]))

plt.ylabel("Speed (millions/second)")
plt.xlabel("Cores")

box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), handles=legend)
plt.savefig('results.png')
plt.show()
