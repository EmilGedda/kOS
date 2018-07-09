#!/usr/bin/env bash
docker run -d -p 5900:5900 -t -e DISPLAY=:0 -v /tmp/.X11-unix:/tmp/.X11-unix kos:latest
