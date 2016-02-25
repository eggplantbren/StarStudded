#!/bin/bash
ffmpeg -r 10 -f image2 -i '%06d.png' -b:v 16384k movie.webm



