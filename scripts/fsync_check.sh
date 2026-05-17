#!/bin/bash

set -euo pipefail

sudo cat /sys/class/pwm/pwmchip3/pwm0/enable
sudo cat /sys/class/pwm/pwmchip3/pwm0/period
sudo cat /sys/class/pwm/pwmchip3/pwm0/duty_cycle
