import matplotlib as mpl
mpl.use('TkAgg')
from pylab import *
import os

# Piecewise linear stretch
def stretch(x):
    y = x.copy()
#   y = (y - y.min())/(y.max() - y.min())
#   y[y > 0.1] = 0.1 + 0.05*(y[y > 0.1] - 0.1)
    return log(abs(y) + 1.)

saveFrames = False  # For making movies
if saveFrames:
    os.system('rm Frames/*.png')

metadata = np.loadtxt('Data/test_metadata.txt').astype(int)
# print(metadata)

num_images = metadata[0]
ni = metadata[1]
nj = metadata[2]

# num_sam_acccepted X nbands*xdim*ydim
posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))
# print(np.shape(posterior_sample))

# generate after running showresults.py
# posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))
# print(np.shape(posterior_sample))
# posterior_sample show have same shape as _image.txt

data = np.reshape(loadtxt('Data/test_image.txt'), (num_images, ni, nj))
print((data.shape))
sig = np.reshape(loadtxt('Data/test_sigma.txt'), (num_images, ni, nj))

ion()
hold(False)

for i in xrange(0, posterior_sample.shape[0]):
    for j in xrange(0, num_images):
        subplot(num_images, 2, 1 + j*num_images)
        img = posterior_sample[i, j * ni * nj:(j + 1) * ni * nj].reshape((ni, nj))
        imshow(-img, interpolation='nearest', cmap='gray')
        title('Model {i}'.format(i=(i+1)))

        subplot(num_images, 2, 2 + j*num_images)
        imshow(-(img - data[j, :, :]), interpolation='nearest', cmap='gray')
        title('Residuals')
    draw()

    if saveFrames:
        savefig('Frames/' + '%0.4d' % (i + 1) + '.png', bbox_inches='tight')
        print('Frames/' + '%0.4d' % (i + 1) + '.png')

ioff()
show()
