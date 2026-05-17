#!/bin/bash
set -e

PWMCHIP=/sys/class/pwm/pwmchip3
PERIOD=33333333
DUTY=1000000

if [ ! -d "${PWMCHIP}/pwm0" ]; then
    echo 0 > ${PWMCHIP}/export
    sleep 0.1
fi

echo ${PERIOD}  > ${PWMCHIP}/pwm0/period
echo ${DUTY}    > ${PWMCHIP}/pwm0/duty_cycle
echo 1          > ${PWMCHIP}/pwm0/enable

echo "FSYNC enabled at $(( 1000000000 / PERIOD ))Hz"