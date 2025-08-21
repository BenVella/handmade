#!/bin/bash
export DISPLAY=:1
dbus-uuidgen > $PREFIX/var/lib/dbus/machine-id
