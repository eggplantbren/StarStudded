#!/bin/bash
ffmpeg -r 10 -i %06d.png -c:v theora -b:v 4192k movie.ogv

