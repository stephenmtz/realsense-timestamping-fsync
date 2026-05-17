#!/bin/bash

PWMCHIP=/sys/class/pwm/pwmchip3

if [ $EUID -ne 0 ]; then
    echo "Error: must be run as root" >&2
    exit 1
fi

if [ ! -d "${PWMCHIP}/pwm0" ]; then
    echo "pwm0 not exported, nothing to disable"
    exit 0
fi

echo 0 > "${PWMCHIP}/pwm0/enable"     || { echo "Error: failed to disable PWM" >&2; exit 1; }
echo 0 > "${PWMCHIP}/unexport"        || { echo "Error: failed to unexport pwm0" >&2; exit 1; }

echo "FSYNC disabled"