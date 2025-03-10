#!/usr/bin/env bash

command -v git >/dev/null 2>&1 || exit 1
command -v make >/dev/null 2>&1 || exit 1

git clone https://github.com/KDesp73/cloak --depth=1
cd cloak || exit 1
make check_tools || exit 1
sudo make install
