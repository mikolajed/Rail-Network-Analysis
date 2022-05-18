#! /bin/sh

cmake -S . -B build; cd build; make; cp PRELIMINARY ../ ; cd .. ; ./PRELIMINARY
