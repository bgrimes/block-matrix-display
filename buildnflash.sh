#!/bin/sh
rm tcp.bin
particle compile photon . --saveTo tcp.bin && particle flash <DEVICE_ID> tcp.bin