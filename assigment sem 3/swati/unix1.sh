#!/bin/bash

echo "Parent PID = $$"

(
    # This is CHILD (subshell)
    echo "Child PID = $$"
)

# Parent continues
echo "Parent finished"
