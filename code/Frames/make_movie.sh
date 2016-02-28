#!/bin/bash
ffmpeg -r 10 -i %06d.png -c:v mjpeg -qscale:v 0 movie.avi

