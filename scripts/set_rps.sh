#! /bin/bash

ETH=$1

sh -c 'echo 32767 > /proc/sys/net/core/rps_sock_flow_entries'

for i in `ls /sys/class/net/${ETH}/queues/ | grep 'rx'`
do
    sh -c "echo 4096 > /sys/class/net/${ETH}/queues/${i}/rps_flow_cnt"
    #sh -c "echo 2 > /sys/class/net/${ETH}/queues/${i}/rps_cpus"
done
