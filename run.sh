#!/usr/bin/env bash
set -euo pipefail
make
exec ./bash_n_dash "$@"
