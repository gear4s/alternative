#!/bin/bash

[ ! -d "b" ] & mkdir b

cd b
cmake ../altplay
make