#!/usr/bin/env bash
set -euo pipefail       # 안전 옵션

make                    # 빌드 (변경 없으면 “Nothing to be done”)

echo "Launching Bash'n'Dash… (press q to quit)"
exec ./bashndash "$@" # 실제 실행파일 이름에 맞춰 수정
