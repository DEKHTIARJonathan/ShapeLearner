#!/bin/sh
sed -e '/^HnProperties/,/\]/d' \
    -e '/^Searching/d' \
    -e 's/:.*$//' \
    -e '/^\*\*\*\* Processing/,$d'
