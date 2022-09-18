#!/usr/bin/env bash

set -exo

echo '```' > cpuinfo.txt
./cpufp --thread_pool=[0] | tail -n +3 >> cpuinfo.txt
echo '```' >> cpuinfo.txt
