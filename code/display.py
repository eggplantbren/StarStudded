from pylab import *

import os

# Piecewise linear stretch
def stretch(x):
	y = x.copy()
#	y = (y - y.min())/(y.max() - y.min())
#	y[y > 0.1] = 0.1 + 0.05*(y[y > 0.1] - 0.1)
	return log(abs(y) + 1.)

saveFrames = False # For making movies
if saveFrames:
	os.system('rm Frames/*.png')

posterior_sample = atleast_2d(loadtxt('sample.txt'))
data = np.reshape(loadtxt('Data/test_image.txt'),(2,200,200))
print(np.shape(data))
sig = np.reshape(loadtxt('Data/test_sigma.txt'),(2,200,200))
print(np.shape(sig))

ion()
hold(False)
print(np.shape(posterior_sample))
print(np.shape(posterior_sample[0, 0:200**2].reshape((200, 200))))
for j in xrange(0,np.shape(data)[0]):
  print(j)
  print(np.shape(posterior_sample[0, j*200**2:(j+1)*200**2]))
  print(np.shape(posterior_sample[0, j*200**2:(j+1)*200**2].reshape((200, 200))))
  img = posterior_sample[0, j*200**2:(j+1)*200**2].reshape((200, 200))
  for i in xrange(0, posterior_sample.shape[0]):
	  #img = posterior_sample[0, j*200**2:(j+1)*200**2].reshape((200, 200))
	  subplot(1, 2, 1)
	  imshow(stretch(img), interpolation='nearest', cmap='afmhot')
	  title('Model {i}'.format(i=i))
	  gca().set_xticks([-0.5, 99.5, 199.5])
	  gca().set_yticks([-0.5, 99.5, 199.5])
	  gca().set_xticklabels(['-1', '0', '1'])
	  gca().set_yticklabels(['1', '0', '-1'])

	  subplot(1, 2, 2)
	  imshow(img - data[j], interpolation='nearest', cmap='gray')
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

