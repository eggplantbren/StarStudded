from pylab import *

import os

# Piecewise linear stretch
def stretch(x):
	y = x.copy()
	y = (y - y.min())/(y.max() - y.min())
	y[y > 0.1] = 0.1 + 0.05*(y[y > 0.1] - 0.1)
	return y

saveFrames = False # For making movies
if saveFrames:
	os.system('rm Frames/*.png')

posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))
data = loadtxt('Data/test_image.txt')
sig = loadtxt('Data/test_sigma.txt')

ion()
hold(False)
for i in xrange(0, posterior_sample.shape[0]):
	img = posterior_sample[i, 0:200**2].reshape((200, 200))
	subplot(1, 2, 1)
	imshow(stretch(img))
	title('Model {i}'.format(i=i))
	gca().set_xticks([-0.5, 99.5, 199.5])
	gca().set_yticks([-0.5, 99.5, 199.5])
	gca().set_xticklabels(['-1', '0', '1'])
	gca().set_yticklabels(['1', '0', '-1'])

	subplot(1, 2, 2)
	sigma = sqrt(sig**2 + posterior_sample[i,-2]**2)
	imshow((img - data)/sigma)
	title('Standardised Residuals')
	gca().set_xticks([-0.5, 99.5, 199.5])
	gca().set_yticks([-0.5, 99.5, 199.5])
	gca().set_xticklabels(['-1', '0', '1'])
	gca().set_yticklabels(['1', '0', '-1'])

	draw()

	if saveFrames:
		savefig('Frames/' + '%0.4d'%(i+1) + '.png', bbox_inches='tight')
		print('Frames/' + '%0.4d'%(i+1) + '.png')

ioff()
show()

