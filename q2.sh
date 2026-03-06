#!/bin/bash

echo "-------------------------------------------"
echo "       LINUX KERNEL & CPU INFORMATION      "
echo "-------------------------------------------"

# Kernel version
echo "Kernel Version:"
uname -r
echo ""

# Complete kernel info
echo "Full Kernel Details:"
uname -a
echo ""

# CPU type and model
echo "CPU Information:"
grep -m 1 "model name" /proc/cpuinfo
grep -m 1 "vendor_id" /proc/cpuinfo
grep -m 1 "cpu MHz" /proc/cpuinfo
echo ""

# Number of CPU cores
echo "Total CPU Cores:"
grep -c "^processor" /proc/cpuinfo
echo ""

echo "-------------------------------------------"
echo "Report Completed."
