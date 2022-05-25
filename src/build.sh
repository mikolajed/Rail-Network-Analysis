#! /bin/sh

cmake -S . -B build; cd build; make; cp PROJECT ../ ; cp TEST ../
