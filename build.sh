#!/usr/bin/env bash

gcc \
-o build/main src/main.c \
`pkg-config --cflags gtk+-3.0 webkit2gtk-4.0` \
`pkg-config --libs gtk+-3.0 webkit2gtk-4.0`
