#!/bin/sh

pandoc ./RELAZIONE.md -o ./RELAZIONE.pdf --include-in-header=./header.tex
