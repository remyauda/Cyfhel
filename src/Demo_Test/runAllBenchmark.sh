#!/bin/bash
# since Bash v4

# Run all the executables begining with Benchmark_ and ending with _x in the current directory.

# Put the current directory in a variable.
pwd=$(pwd)
# Use the program run-parts to run all the executables begining with Benchmark_ and ending with _x in the current directory (for details about run-parts, run: man run-parts or run-parts --help).
run-parts --verbose --new-session --regex '^Benchmark_.*_x$' $pwd

# If you just want to see the program that will be executed by run-parts but without run them effectively, just uncomment the following line and comment the line above.
# run-parts --list --verbose --new-session --regex '^Benchmark_.*_x$' $pwd
