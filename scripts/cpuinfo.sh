#!/usr/bin/env bash

set -exo

output_file=$1

echo '## CPU' > $output_file
echo '```' >> $output_file
lscpu >> $output_file
echo '```' >> $output_file

echo '## Cache' >> $output_file
echo '```' >> $output_file
./csrc/cache_info >> $output_file
echo '```' >> $output_file

echo '## Memory' >> $output_file
echo '```' >> $output_file
free -mh >> $output_file
echo '```' >> $output_file

echo '## Peak Performance' >> $output_file
echo '```' >> $output_file
./cpufp --thread_pool=[0] | tail -n +3 >> $output_file
echo '```' >> $output_file

cat $output_file
