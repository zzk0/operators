#!/usr/bin/env bash

set -exo

output_file=$1
echo '```' >> $output_file
echo 'lscpu: ' >> $output_file
lscpu > $output_file
echo 'free -mh' >> $output_file
free -mh >> $output_file
echo 'Cache Info' >> $output_file
./csrc/cache_info >> $output_file
echo 'Peak Performance' >> $output_file
./cpufp --thread_pool=[0] | tail -n +3 >> $output_file
echo '```' >> $output_file
cat $output_file
