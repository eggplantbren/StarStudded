import numpy as np
import matplotlib.pyplot as plt
import os
import dnest4.classic as dn4

print("WARNING! This will delete\
 movie.mkv and the Frames/ directory, if these exist.")
ch = input("Continue? y/n: ")
if ch != "y" and ch != "Y":
    exit()

os.system("rm -rf Frames/ movie.mkv")
os.mkdir("Frames")

# Open run_data.txt to get data filenames used for the run
f = open("run_data.txt", "r")
a, b, c = f.readline()[:-1], f.readline()[:-1], f.readline()[:-1]
f.close()

# Load files (DNest4 output and data files)
metadata = np.loadtxt(a)
num_images = int(metadata[0])
ni = int(metadata[1])
nj = int(metadata[2])
max_num_stars = 1500
num_pixels = ni*nj*num_images

posterior_sample = dn4.my_loadtxt('posterior_sample.txt', single_precision=True)
indices = dn4.load_column_names("posterior_sample.txt")["indices"]
data = np.reshape(np.loadtxt(b), (num_images, ni, nj))
sig = np.reshape(np.loadtxt(c), (num_images, ni, nj))

stars = posterior_sample[:,(num_pixels + 3 + 2*num_images):(num_pixels + 3 + 2*num_images + max_num_stars*(2 + num_images))]
stars_x = stars[:, 0:max_num_stars]
stars_y = stars[:, max_num_stars:2*max_num_stars]

fig = plt.figure(figsize=(12, 8))

for i in range(0, posterior_sample.shape[0]):
    for j in range(0, num_images):
        plt.subplot(num_images, 2, 1 + 2*j)
        plt.cla()
        img = posterior_sample[i, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
        try:
            plt.imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='viridis')
        except:
            plt.imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='Blues')

        which = stars_x[i, :] != 0.0
        plt.plot(stars_x[i, which], stars_y[i, which], 'wo', markersize=3)
        plt.axis(metadata[3:7])

        plt.title('Model {i}'.format(i=(i+1)))
        plt.gca().set_xticks([])
        plt.gca().set_yticks([])
        ax = plt.gca()

        plt.subplot(num_images, 2, 2 + 2*j)
        var = sig[j, :, :]**2\
                + posterior_sample[i, indices["sigma0[{j}]".format(j=j)]]\
                + posterior_sample[i, indices["sigma1[{j}]".format(j=j)]]*img
        resid = (img - data[j, :, :])/np.sqrt(var)

        plt.imshow(resid*(sig[j, :, :] < 1E100), interpolation='nearest', cmap='coolwarm')
        plt.title('Standardised Residuals')
        plt.gca().set_xticks([])
        plt.gca().set_yticks([])

    plt.savefig('Frames/' + '%0.6d' % (i + 1) + '.png', bbox_inches='tight')
    print('Saved Frames/' + '%0.6d' % (i + 1) + '.png')
plt.show()

os.system('ffmpeg -r 10 -i Frames/%06d.png -c:v h264 -b:v 4192k movie.mkv')


