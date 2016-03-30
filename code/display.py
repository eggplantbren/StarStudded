import matplotlib as mpl
from pylab import *
import os
import dnest4.deprecated as dn

# Piecewise linear stretch
def stretch(x):
    return log(abs(x) + 1.)

os.system('rm Frames/*.png')

# Load files (DNest4 output and data files)
metadata = loadtxt('Data/test_metadata.txt')
num_images = int(metadata[0])
ni = int(metadata[1])
nj = int(metadata[2])
max_num_stars = 300
num_pixels = ni*nj*num_images

posterior_sample = atleast_2d(dn.my_loadtxt('posterior_sample.txt'))
data = np.reshape(loadtxt('Data/test_image.txt'), (num_images, ni, nj))
sig = np.reshape(loadtxt('Data/test_sigma.txt'), (num_images, ni, nj))

stars = posterior_sample[:,(num_pixels + 3 + 2*num_images):(num_pixels + 3 + 2*num_images + max_num_stars*(2 + num_images))]
stars_x = stars[:, 0:max_num_stars]
stars_y = stars[:, max_num_stars:2*max_num_stars]
print(stars.shape)

figure(figsize=(12, 12))
for i in range(0, posterior_sample.shape[0]):
    for j in range(0, num_images):
        subplot(num_images, 2, 1 + 2*j)
        img = posterior_sample[i, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
        try:
            imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='viridis')
        except:
            imshow(img, extent=metadata[3:7], interpolation='nearest', cmap='Blues')

        which = stars_x[i, :] != 0.0
        plot(stars_x[i, which], stars_y[i, which], 'wo', markersize=3)
        axis(metadata[3:7])

        title('Model {i}'.format(i=(i+1)))
        gca().set_xticks([])
        gca().set_yticks([])

        subplot(num_images, 2, 2 + 2*j)
        imshow((img - data[j, :, :])*(sig[j, :, :] < 1E100), interpolation='nearest', cmap='coolwarm')
        title('Residuals')
        gca().set_xticks([])
        gca().set_yticks([])

    savefig('Frames/' + '%0.6d' % (i + 1) + '.png', bbox_inches='tight')
    print('Saved Frames/' + '%0.6d' % (i + 1) + '.png')
show()

