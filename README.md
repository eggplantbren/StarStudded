StarStudded
===========

Produce probabilistic catalogs ― that is, a posterior distribution
for the number of stars, and their positions and fluxes,
from noisy images of a crowded field.

(c) 2015–2018 Brendon J. Brewer, Alex Malz, and Daisy Leung.

## License

The contents of this repository are licensed under the GNU General Public
Licence, version 3. See the LICENCE file for details.

## Acknowledgements

This work was supported by a Marsden Fast-Start grant
from the Royal Society of New Zealand. If you use this software, please cite
the following paper, which details the foundations of the methodology:

Brewer, Brendon J., Daniel Foreman-Mackey, and David W. Hogg.
"Probabilistic catalogs for crowded stellar fields."
The Astronomical Journal 146, no. 1 (2013): 7.

You can find the paper for free on the arxiv:
https://arxiv.org/pdf/1211.5805.pdf

## Dependencies

You'll need git to obtain the source code, and a non-ancient version of
g++ for it to compile properly.

There are some associated Python scripts, which use Python 3, numpy,
matplotlib, yaml, and the DNest4 python package. Anaconda's distribution of
Python 3 should work well.

If you want the postprocessing scripts to be able to make a movie of posterior
samples, you'll need ffmpeg.

## Downloading and compiling

First, clone the repository recursively:
```
git clone --recursive https://github.com/eggplantbren/StarStudded
```

Then compile all the C++:
```
cd StarStudded/src
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
