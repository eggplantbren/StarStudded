#!/bin/bash
ffmpeg -r 10 -i %06d.png -c:v h264 -b:v 4192k movie.mkv

