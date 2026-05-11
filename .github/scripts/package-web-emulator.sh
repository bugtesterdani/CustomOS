#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$REPO_ROOT"

if [[ ! -f build/floppy.img ]]; then
  echo "build/floppy.img is missing. Run .github/scripts/build-artifacts.sh first." >&2
  exit 1
fi

mkdir -p build/web
cp build/floppy.img build/web/floppy.img

cat > build/web/index.html <<'HTML'
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>CustomOS Browser Emulator</title>
    <style>
      body { font-family: system-ui, sans-serif; margin: 0; background: #111; color: #eee; }
      main { max-width: 980px; margin: 1rem auto; padding: 0 1rem 1rem; }
      #screen { width: 100%; min-height: 450px; border: 1px solid #333; background: #000; }
      .muted { color: #bbb; }
      code { background: #222; padding: 0.15rem 0.35rem; border-radius: 4px; }
    </style>
  </head>
  <body>
    <main>
      <h1>CustomOS Browser Emulator</h1>
      <p class="muted">Click inside the VM and wait a few seconds for BIOS + bootloader startup.</p>
      <div id="screen"></div>
      <p class="muted">If this artifact is hosted statically, open <code>index.html</code> from that host root.</p>
    </main>

    <script src="https://cdn.jsdelivr.net/npm/v86@latest/build/libv86.js"></script>
    <script>
      const emulator = new V86({
        wasm_path: "https://cdn.jsdelivr.net/npm/v86@latest/build/v86.wasm",
        screen_container: document.getElementById("screen"),
        bios: { url: "https://cdn.jsdelivr.net/npm/v86@latest/bios/seabios.bin" },
        vga_bios: { url: "https://cdn.jsdelivr.net/npm/v86@latest/bios/vgabios.bin" },
        fda: { url: "./floppy.img" },
        boot_order: 0x1,
        memory_size: 32 * 1024 * 1024,
        vga_memory_size: 2 * 1024 * 1024,
        autostart: true,
      });
      window.emulator = emulator;
    </script>
  </body>
</html>
HTML

echo "Web emulator package generated in build/web"
ls -lh build/web
