#!/bin/bash
# test.sh - Automated test script for the harness program
# This script pipes predefined inputs into ./harness and checks the output

# Use printf to send each input on its own line, piped into the program
# The sequence is: "hello" first, then "exit" to end the program
printf "hello\nexit\n" | ./harness
