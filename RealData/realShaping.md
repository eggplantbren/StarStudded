change level in OPTION to ~ 70
run `make` if we change anything in C++ code
./main 
---> posterior_sample.txt

Display in python:
for 1 band: 
e.g. dependeing on the dimension specified in the _metadata.txt; here e.g. 100 X 100
  im = x[0,100**2].reshape((100,100))
  imshow(im, interpolation='nearest')
  imshow(log(im)+1, interpolation='nearest')


for multiple bands:
ndim = blah
reshape into (num_band,ndim,ndim)
x = loadtxt('posteriord_sample.txt').reshape((2,ndim,ndim))
then, you can display
j band:
for j in np.arange(0, np.shape(x)[0]):
    im_1 = x[0, j*ndim**2:(j+1)*ndim**2].reshape((ndim,ndim))
    imshow(stretch(im_1), interpolation='nearest', cmap='afmhot')
    title('Model {j}'.format(j=j))
    
etc.





