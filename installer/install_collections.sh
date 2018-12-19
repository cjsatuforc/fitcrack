#!/bin/bash

# Make collections directory tree
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

echo "Creating directories for common collections..."
# Create collections dir (if not exists)
if ! [ -d "/usr/share/collections" ]; then
  mkdir /usr/share/collections
  exit
fi

# Create collections subdirs
if ! [ -d "/usr/share/collections/charsets" ]; then
  mkdir /usr/share/collections/charsets
fi

if ! [ -d "/usr/share/collections/dictionaries" ]; then
  mkdir /usr/share/collections/dictionaries
fi

if ! [ -d "/usr/share/collections/markov" ]; then
  mkdir /usr/share/collections/markov
fi

if ! [ -d "/usr/share/collections/encrypted-files" ]; then
  mkdir /usr/share/collections/encrypted-files
fi

if ! [ -d "/usr/share/collections/masks" ]; then
  mkdir /usr/share/collections/masks
fi

if ! [ -d "/usr/share/collections/rules" ]; then
  mkdir /usr/share/collections/rules
fi

# Set ownership and permissions
chown -R $APACHE_USER:$APACHE_GROUP /usr/share/collections
chmod -R 777 /usr/share/collections

echo "Directories for common collections created."
