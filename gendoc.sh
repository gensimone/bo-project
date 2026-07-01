#!/bin/sh

pandoc RELAZIONE_CORRETTA.md -o RELAZIONE.pdf --include-in-header=header.tex
