StarStudded
===========

StarStudded produces probabilistic catalogs ― that is, a posterior distribution
for the number of stars, and their positions and fluxes,
from noisy images of a crowded field of stars.

(c) 2015–2018 Brendon J. Brewer, Alex Malz, and Daisy Leung.

![movie](movie.gif)

## License

The contents of this repository are licensed under the GNU General Public
Licence, version 3. See the LICENCE file for details.

## Acknowledgements

This work was supported by a Marsden Fast-Start grant
from the Royal Society of New Zealand. It also benefited from my being invited
to the 2015 edition of Astro Hack Week.
If you use this software, please give the URL of this repository _and_ cite
the following paper, which details the foundations of the methodology:

Brewer, Brendon J., Daniel Foreman-Mackey, and David W. Hogg.
"Probabilistic catalogs for crowded stellar fields."
The Astronomical Journal 146, no. 1 (2013): 7.

You can find the paper for free on the arxiv:
https://arxiv.org/pdf/1211.5805.pdf

If I write a paper about the software specifically, I'll update this advice.

## Dependencies

So far, just the following:

* [yaml-cpp](https://github.com/jbeder/yaml-cpp).
* [boost](https://www.boost.org)
* [DNest4](https://github.com/eggplantbren/DNest4) (but it's bundled).

You can probably get the first two from your operating system's package manager.
Don't forget the header files, which might be in a separate package
(e.g. with a suffix -dev on Ubuntu, or -devel on Fedora).

You'll need git to obtain the source code, and a non-ancient version of
g++ for it to compile properly.

There are some associated Python scripts, which use Python 3, numpy,
matplotlib, yaml, and the DNest4 python package. Anaconda's distribution of
Python 3 should work well.

If you want the postprocessing scripts to be able to make a movie of posterior
samples, you'll need [ffmpeg](http://ffmpeg.org/).

## Downloading and compiling

First, clone the repository recursively:

```
git clone --recursive https://github.com/eggplantbren/StarStudded
```

Then compile all the C++:

```
cd StarStudded
make
```

If you don't already have the DNest4 Python package on your system,
follow the instructions [here](https://github.com/eggplantbren/DNest4)
(just the bits about Python) to install it.

## Running the example data

To run StarStudded on the example data using 8 threads (recommended), use

```
./StarStudded -t 8
```

StarStudded will run and you will see DNest4 output in the terminal.
DNest4 output
will also be written to some text files.
These computations are expensive, so give it an hour or so of runtime before
expecting anything interesting from the postprocessing. It's harmless to try
it at any time, though. If you want to try your luck with some more
aggressive DNest4 settings, you can do this instead:

```
./StarStudded -t 8 -o OPTIONS_AGGRESSIVE
```

## Postprocessing

StarStudded will run for a long time.
The longer you run it, the more reliable the
output will be (i.e., more posterior samples will have been generated, as
long as everything went to plan).
You can manually terminate it when you like, or you can
do the postprocessing without terminating the main process.

The postprocessing will require
that you've installed the DNest4 Python package
(see [here](https://github.com/eggplantbren/DNest4) for instructions).
Simply invoke

```
python showresults.py
```

This will generate a bunch of output plots
(first the three canonical DNest4 output plots, then more plots that are
specific to StarStudded).
As you close each plot, more will appear.
Posterior samples will also be saved in a text file
`posterior_sample.txt`. Catalogs sampled to the posterior distribution are
saved to a bunch of YAML files in the OutputCatalogs directory.
These include the number of stars and the positions and fluxes of the stars.
Other things such as hyperparameters are only in `posterior_sample.txt`;
the excessively long header in that file labels the columns.

## The modelling assumptions

Some of the model assumptions have changed a bit since the paper was published.
Ask me for details. Also, some things are still hard-coded. But the maximum
number of stars and the level of padding (how far outside the image stars can
appear) can be controlled via `setup.yaml`.

# Running other datasets
To run other datasets, inspect `setup.yaml` to see how to set up a run.
You need to provide a YAML file of metadata (see `Data/test_metadata.yaml` for
an example), and plain text files with the images (row-major order, one band
after another) and the sigma images (row major order, one band after another,
these can also be zero).

Images provided to StarStudded should be clear of any features such as
galaxies, any non-linear background, etc. You can also use the sigma map to
mask out any non-modellable features by settings those pixels to a very high
standard deviation (> 1E100, and the plotting scripts will treat those pixels
as having been totally masked).

Some example datasets are supplied in the Data subdirectory. The _100_ example
has 100 stars, and is the simulated dataset from the original paper.
The _1000_ dataset is the 1000-star data from the original paper.
The _fink_ dataset is something Doug Finkbeiner sent me once.

