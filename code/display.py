import numpy as np
import matplotlib.pyplot as plt
import os
import dnest4.deprecated as dn

os.system('rm Frames/*.png')

# Load files (DNest4 output and data files)
metadata = np.loadtxt('Data/test_metadata.txt')
num_images = int(metadata[0])
ni = int(metadata[1])
nj = int(metadata[2])
max_num_stars = 300
num_pixels = ni*nj*num_images

posterior_sample = np.atleast_2d(dn.my_loadtxt('posterior_sample.txt'))
data = np.reshape(np.loadtxt('Data/test_image.txt'), (num_images, ni, nj))
sig = np.reshape(np.loadtxt('Data/test_sigma.txt'), (num_images, ni, nj))

stars = posterior_sample[:,(num_pixels + 3 + 2*num_images):(num_pixels + 3 + 2*num_images + max_num_stars*(2 + num_images))]
stars_x = stars[:, 0:max_num_stars]
stars_y = stars[:, max_num_stars:2*max_num_stars]

for i in range(0, posterior_sample.shape[0]):
    plt.figure(figsize=(12, 12))

    for j in range(0, num_images):
        plt.subplot(num_images, 2, 1 + 2*j)
        img = posterior_sample[i, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
        try:
            plt.imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='viridis')
        except:
            plt.imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='Blues')

        plt.hold(True)
        which = stars_x[i, :] != 0.0
        plt.plot(stars_x[i, which], stars_y[i, which], 'wo', markersize=3)
        plt.axis(metadata[3:7])

        plt.title('Model {i}'.format(i=(i+1)))
        plt.gca().set_xticks([])
        plt.gca().set_yticks([])
        ax = plt.gca()

        plt.subplot(num_images, 2, 2 + 2*j)
        plt.imshow((img - data[j, :, :])*(sig[j, :, :] < 1E100), interpolation='nearest', cmap='coolwarm')
        plt.title('Residuals')
        plt.gca().set_xticks([])
        plt.gca().set_yticks([])

    plt.savefig('Frames/' + '%0.6d' % (i + 1) + '.png', bbox_inches='tight')
    print('Saved Frames/' + '%0.6d' % (i + 1) + '.png')
plt.show()

