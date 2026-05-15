#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$REPO_ROOT"

IMAGE_NAME="customos-build-image"
CONTAINER_NAME="customos-build-runner"

rm -rf build
mkdir -p build
rm -f src.tar.gz

tar -czf src.tar.gz src

docker build --tag "$IMAGE_NAME" .
docker rm -f "$CONTAINER_NAME" >/dev/null 2>&1 || true
docker run -d --name "$CONTAINER_NAME" "$IMAGE_NAME" tail -f /dev/null >/dev/null

cleanup() {
  docker rm -f "$CONTAINER_NAME" >/dev/null 2>&1 || true
}
trap cleanup EXIT

docker cp src.tar.gz "$CONTAINER_NAME":/root/src.tar.gz
docker cp tests/test.elf "$CONTAINER_NAME":/root/test.elf
docker exec "$CONTAINER_NAME" /bin/bash /root/build.sh

docker cp "$CONTAINER_NAME":/root/src/build/disk.img build/floppy.img
docker cp "$CONTAINER_NAME":/root/src/build/customos.iso build/customos-cd.iso

gzip -9 -c build/floppy.img > build/floppy.img.gz

echo "Artifacts generated:"
ls -lh build/floppy.img build/floppy.img.gz build/customos-cd.iso
