import numpy as np
import dnest4.classic as dn4
import matplotlib.pyplot as plt

# Load indices
indices = dn4.load_column_names("sample.txt")["indices"]

def utility(asserted_catalog, true_catalog):
    """
    Calculate the utility of publishing asserted_catalog
    when the truth is true_catalog.
    """
    return utility1(asserted_catalog, true_catalog)

def utility1(asserted_catalog, true_catalog):
    """
    Calculate the utility of publishing asserted_catalog
    when the truth is true_catalog and we only care about
    the number of stars (with absolute loss).

    This should result in all catalogs with num_stars
    close to the posterior median (as close as they get)
    being equally good.
    """
    index = indices["num_stars"]
    asserted_num_stars = asserted_catalog[index]
    true_num_stars = true_catalog[index]

    return -np.abs(asserted_num_stars - true_num_stars)

# Load both sample.txt and posterior_sample.txt
sample = dn4.my_loadtxt("sample.txt", single_precision=True)
posterior_sample = dn4.my_loadtxt("posterior_sample.txt", single_precision=True)

# Utilities for each possible decision, allowing any catalog from sample.txt to
# be a possible decision. Ultimately we could use a wider decision space here,
# but this would be much more computationally intensive.
U = np.empty(sample.shape[0])
for i in range(0, sample.shape[0]):
    # Need to average over the posterior
    u = np.empty(posterior_sample.shape[0])
    for j in range(0, posterior_sample.shape[0]):
        u[j] = utility(sample[i, :], posterior_sample[j, :])
    U[i] = np.mean(u)

    print("Assessed sample {k}/{n}.".format(k=(i+1), n=sample.shape[0]))

plt.plot(U, "ko-")
plt.xlabel("Sample number")
plt.ylabel("Posterior expected utility")
plt.show()

