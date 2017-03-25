#!/bin/bash

if [ ! -d "b" ]; then
  mkdir b
fi

cd b
cmake ../altplay
make