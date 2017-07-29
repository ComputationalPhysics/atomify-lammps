#!/bin/bash
cd /tmp
rm -r atomify_build
mkdir atomify_build
cd atomify_build
git clone --recursive git@github.com:ovilab/atomify
cd atomify
rm -rf .git
cd src/examples
rm -rf .git
cd ../../..
tar -cxvf atomify.tar.gz atomify
