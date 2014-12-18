JsPCRE
======

This is the full PCRE library implemented in JavaScript. It has been converted using Emscriptem.

Run
====

See test.gs/js on how to run.

Compile
=======

Requirements
============

Emscripten

Steps
=====

To compile from scratch:

1. Download the PCRE library from their website and extract it.
2. Edit config.sh to point to the PCRE source directory.
3. Run `compile.sh`
4. Run `build.sh`

Notes
=====

Compiling has only been done on Mac OS X. Pretty sure it won't work on anything else (given that I'm using the dylib file).