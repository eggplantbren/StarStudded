import numpy as np
import matplotlib.pyplot as plt
import os
import yaml
import dnest4.classic as dn4

# Remove existing output images
os.system("rm -rf OutputImages/*.png")
os.system("rm -rf OutputImages/movie.mkv")
os.system("rm -rf OutputCatalogs/*.yaml")

# Set up fonts
plt.rcParams["font.family"] = "serif"
plt.rcParams["font.size"] = 12
plt.rc("text", usetex=True)

# Open setup file to get data filenames used for the run
f = open("setup.yaml", "r")
setup = yaml.load(f)
f.close()
a, b, c = setup["data_files"]["metadata_file"],\
          setup["data_files"]["images_file"],\
          setup["data_files"]["sigmas_file"]

# Load files (DNest4 output and data files)
f = open(a)
metadata = yaml.load(f)
f.close()
num_images = metadata["num_images"]
ni = metadata["ni"]
nj = metadata["nj"]
max_num_stars = setup["assumptions"]["max_num_stars"]
num_pixels = ni*nj*num_images
# Convert back to list
metadata = [num_images, ni, nj, metadata["x_min"], metadata["x_max"],\
                metadata["y_min"], metadata["y_max"]]

posterior_sample = dn4.my_loadtxt('posterior_sample.txt', single_precision=True)
indices = dn4.load_column_names("posterior_sample.txt")["indices"]
data = np.reshape(np.loadtxt(b), (num_images, ni, nj))
sig = np.reshape(np.loadtxt(c), (num_images, ni, nj))

stars = posterior_sample[:,(num_pixels + 3 + 2*num_images):(num_pixels + 3 + 2*num_images + max_num_stars*(2 + num_images))]
stars_x = stars[:, 0:max_num_stars]
stars_y = stars[:, max_num_stars:2*max_num_stars]
stars_f = stars[:, 2*max_num_stars:2*max_num_stars + num_images*max_num_stars]

# Plot posterior for number of stars
# Histogram-like.
num_stars = posterior_sample[:, indices["num_stars"]].astype("int64")
n = np.arange(0, max_num_stars+1)
count = np.zeros(len(n))
plt.figure()
for i in range(len(num_stars)):
    count[num_stars[i]] += 1
for i in range(len(n)):
    plt.plot([n[i], n[i]], [0, count[i]], "b-")
plt.xlim([-0.5, max_num_stars+0.5])
plt.ylim([0, 1.1*max(count)])
plt.xlabel("Number of stars")
plt.ylabel("Number of posterior samples")
plt.show()

for i in range(0, posterior_sample.shape[0]):

    # Output the catalog
    filename = "OutputCatalogs/catalog{k}.yaml".format(k=i+1)
    f = open(filename, "w")
    f.write("---\n")
    f.write("num_stars: {n}\n".format(n=num_stars[i]))
    f.write("stars:\n")
    for j in range(0, num_stars[i]):
        f.write("    -\n")
        f.write("        position: ")
        f.write(str([stars_x[i, j], stars_y[i, j]]))
        f.write("\n")
        f.write("        fluxes: ")
        fluxes = []
        for k in range(0, num_images):
            fluxes.append(stars_f[i, j + max_num_stars*k])
        f.write(str(fluxes))
        f.write("\n")
    f.close()
    print("Saved {filename}".format(filename=filename))

    plt.clf()

    for j in range(0, num_images):
        ax = plt.subplot(num_images, 2, 1 + 2*j)
        ax.cla()
        img = posterior_sample[i, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
        ax.imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='viridis')

        which = stars_x[i, :] != 0.0
        ax.plot(stars_x[i, which], stars_y[i, which],
                'wo', markersize=2, alpha=0.3)
        ax.axis(metadata[3:7])

        if j==0:
            ax.set_title('Catalog {i}'.format(i=(i+1)))
        ax.set_xticks([])
        ax.set_yticks([])

        ax = plt.subplot(num_images, 2, 2 + 2*j)
        var = sig[j, :, :]**2\
                + posterior_sample[i, indices["sigma0[{j}]".format(j=j)]]\
                + posterior_sample[i, indices["sigma1[{j}]".format(j=j)]]*img
        resid = (img - data[j, :, :])/np.sqrt(var)

        ax.imshow(resid*(sig[j, :, :] < 1E100), interpolation='nearest', cmap='coolwarm')
        if j==0:
            ax.set_title('Standardised Residuals')
        ax.set_xticks([])
        ax.set_yticks([])

    plt.savefig('OutputImages/' + '%0.6d' % (i + 1) + '.png')
    print('Saved OutputImages/' + '%0.6d' % (i + 1) + '.png\n')
plt.show()

os.system('ffmpeg -r 10 -i OutputImages/%06d.png -c:v libvpx-vp9 -b:v 4192k OutputImages/movie.mkv')

