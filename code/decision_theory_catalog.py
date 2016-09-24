import numpy as np
import dnest4.classic as dn4
import matplotlib.pyplot as plt

# Load both sample.txt and posterior_sample.txt
sample = dn4.my_loadtxt("sample.txt", single_precision=True)
posterior_sample = dn4.my_loadtxt("posterior_sample.txt", single_precision=True)
metadata = np.loadtxt('Data/1000_metadata.txt')
indices = dn4.load_column_names("sample.txt")["indices"]
num_images = int(metadata[0])
ni = int(metadata[1])
nj = int(metadata[2])
max_num_stars = 2000
num_pixels = ni*nj*num_images

kernel = np.zeros((ni, nj))
ii = np.arange(0, ni)
jj = np.arange(0, nj)
[jj, ii] = np.meshgrid(ii, jj)

rsq = (ii - ni/2)**2 + (jj - nj/2)**2
f = 1.0/(1.0 + rsq/10.0**2)
f /= f.sum()
f = np.fft.fftshift(f)
_f = np.fft.fft2(f)

def convolve(img):
    _img = np.fft.fft2(img)
    _result = _img*_f
    result = np.real(np.fft.ifft2(_result))
    return result

def utility(asserted_catalog, true_catalog):
    """
    Calculate the utility of publishing asserted_catalog
    when the truth is true_catalog.
    """
    return utility2(asserted_catalog, true_catalog)

def utility1(asserted_catalog, true_catalog):
    """
    Calculate the utility of publishing asserted_catalog
    when the truth is true_catalog and we only care about
    the number of stars (with absolute loss).

    This should result in all catalogs with num_stars
    close to the posterior median (as close as they get)
    being equally good. A pretty dumb idea without any
    further constraints...
    """
    index = indices["num_stars"]
    asserted_num_stars = asserted_catalog[index]
    true_num_stars = true_catalog[index]
    return -np.abs(asserted_num_stars - true_num_stars)

def utility2(asserted_catalog, true_catalog):
    """
    Calculate the utility of publishing asserted_catalog
    when the truth is true_catalog and we care about the
    divergence of the true image from a catalog-produced image.
    """
    img1 = asserted_catalog[j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
    img2 = true_catalog[j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
    u, w = img1.copy(), img2.copy()
    u -= asserted_catalog[indices["bg[0]"]]
    w -= true_catalog[indices["bg[0]"]]
    u = convolve(u)
    w = convolve(w)
    return -np.sum(u - w + w*np.log(w/u))

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

best = np.nonzero(U == np.max(U[np.logical_not(np.isnan(U))]))[0][0]
plt.imshow(sample[best, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj)),\
            interpolation="nearest", cmap="viridis")
plt.show()

