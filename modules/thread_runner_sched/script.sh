#!/bin/bash
sudo trace-cmd reset
rm -rf trace.dat
sudo trace-cmd record -e sched:sched_switch 

kernelshark
