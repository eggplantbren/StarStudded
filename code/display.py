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
print(metadata)

num_images = metadata[0]
ni = metadata[1]
nj = metadata[2]

# num_sam_acccepted X nbands*xdim*ydim
# sample = atleast_2d(loadtxt('sample.txt'))

# generate after running showresults.py
posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))
print(np.shape(posterior_sample))
# posterior_sample show have same shape as _image.txt

data = np.reshape(loadtxt('Data/test_image.txt'), (num_images, ni, nj))
print(np.shape(data))
sig = np.reshape(
    loadtxt('Data/test_sigma.txt'), (num_images, ni, nj))
print(np.shape(sig))

ion()
hold(False)
for j in xrange(0, num_images):
    print(j)
    print(np.shape(posterior_sample[
          0, j * ni * nj:(j + 1) * ni * nj]))
    print(np.shape(posterior_sample[0, j * ni * nj:(
        j + 1) * ni * nj].reshape((ni, nj))))
    img = posterior_sample[0, j * ni * nj
        :(j + 1) * ni * nj].reshape((ni, nj))
    for i in xrange(0, posterior_sample.shape[0]):
        #img = posterior_sample[0, j*200**2:(j+1)*200**2].reshape((200, 200))
        subplot(1, 2, 1)
        imshow(stretch(img), interpolation='nearest', cmap='gray')
        title('Model {i}'.format(i=i))
        #gca().set_xticks([-0.5, 99.5, 199.5])
        #gca().set_yticks([-0.5, 99.5, 199.5])
        gca().set_xticklabels(['-1', '0', '1'])
        gca().set_yticklabels(['1', '0', '-1'])

        subplot(1, 2, 2)
        imshow(img - data[j], interpolation='nearest', cmap='gray')
        title('Standardised Residuals')
        #gca().set_xticks([-0.5, 99.5, 199.5])
        #gca().set_yticks([-0.5, 99.5, 199.5])
        gca().set_xticklabels(['-1', '0', '1'])
        gca().set_yticklabels(['1', '0', '-1'])

        draw()

        if saveFrames:
            savefig('Frames/' + '%0.4d' % (i + 1) + '.png', bbox_inches='tight')
            print('Frames/' + '%0.4d' % (i + 1) + '.png')

    ioff()
    show()
