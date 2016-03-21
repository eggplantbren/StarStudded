import matplotlib as mpl
from pylab import *
import os

# Piecewise linear stretch
def stretch(x):
    return log(abs(x) + 1.)

os.system('rm Frames/*.png')

# Load files (DNest4 output and data files)
metadata = np.loadtxt('Data/test_metadata.txt')
num_images = int(metadata[0])
ni = int(metadata[1])
nj = int(metadata[2])

posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))
data = np.reshape(loadtxt('Data/test_image.txt'), (num_images, ni, nj))
sig = np.reshape(loadtxt('Data/test_sigma.txt'), (num_images, ni, nj))

figure(figsize=(12, 12))
for i in range(0, posterior_sample.shape[0]):
    for j in range(0, num_images):
        subplot(num_images, 2, 1 + 2*j)
        img = posterior_sample[i, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
        try:
            imshow(img, interpolation='nearest', cmap='viridis')
        except:
            imshow(img, interpolation='nearest', cmap='Blues')
        title('Model {i}'.format(i=(i+1)))
        gca().set_xticks([])
        gca().set_yticks([])

        subplot(num_images, 2, 2 + 2*j)
        imshow((img - data[j, :, :]), interpolation='nearest', cmap='coolwarm')
        title('Residuals')
        gca().set_xticks([])
        gca().set_yticks([])

    savefig('Frames/' + '%0.6d' % (i + 1) + '.png', bbox_inches='tight')
    print('Saved Frames/' + '%0.6d' % (i + 1) + '.png')
show()

