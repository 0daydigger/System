#!/bin/sh
module="11301125"
device="chardev"

# invoke rmmod with all arguments we got
/sbin/rmmod $module $* || exit 1

# Remove stale nodes

rm -f /dev/${device} /dev/${device}[0-3]





