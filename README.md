# BIOS Interrupt Calls
## Which INT xxh Values the BIOS understood

https://en.wikipedia.org/wiki/BIOS_interrupt_call


# Protected Mode
## Interrupts Descriptor Table (IDT)
https://alex.dzyoba.com/blog/os-interrupts/
http://www.osdever.net/
## PCI
https://wiki.osdev.org/PCI


# Making custom Tools
## Python3 Script to parse the elf files of previous asm builds, to combine together

The actual make Bashscript is not optimal, but it could build later also the asm -> bin files. But for better reading and not everytime manipulating the files, searching for new addresses and updating them, it would be really much easier, if there will be a script, which updates, the addresses in the asm files, before building the new assembler files.

Test of Command: "readelf -s build/stage2.elf" gives me this sample Output:

<code>
Symbol table '.symtab' contains 44 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000     0 FILE    LOCAL  DEFAULT  ABS src/bootloader/s[...]
     2: 00008000     4 OBJECT  LOCAL  DEFAULT    1 print_string
     3: 00008004     4 OBJECT  LOCAL  DEFAULT    1 wait_for_key_reboot
     4: 00008008     4 OBJECT  LOCAL  DEFAULT    1 read_fat32
     5: 0000800c     4 OBJECT  LOCAL  DEFAULT    1 DAP
     6: 00008010     4 OBJECT  LOCAL  DEFAULT    1 var_32_zeros
     7: 00008014     4 OBJECT  LOCAL  DEFAULT    1 Result_Read_File
     8: 00008018     0 NOTYPE  LOCAL  DEFAULT    1 init
     9: 00008023     0 NOTYPE  LOCAL  DEFAULT    1 start
    10: 00008024     0 NOTYPE  LOCAL  DEFAULT    1 start.assign_values
    11: 00008067     0 NOTYPE  LOCAL  DEFAULT    1 Execute
    12: 00008071     0 NOTYPE  LOCAL  DEFAULT    1 hlt
    13: 00008073     1 OBJECT  LOCAL  DEFAULT    1 Line
    14: 00008077     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile
    15: 00008077     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.read_fa[...]
    16: 0000807f     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.compare[...]
    17: 0000808a     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.check_i[...]
    18: 0000808a     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.check_l[...]
    19: 0000808a     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.check_i[...]
    20: 0000809c     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.found_file
    21: 000080ae     0 NOTYPE  LOCAL  DEFAULT    1 ReadFile.file_no[...]
    22: 000080bc     0 NOTYPE  LOCAL  DEFAULT    1 LoadFile
    23: 000080bc     0 NOTYPE  LOCAL  DEFAULT    1 LoadFile.loading_file
    24: 00008179     0 NOTYPE  LOCAL  DEFAULT    1 LoadFile.finish_[...]
    25: 00008183     0 NOTYPE  LOCAL  DEFAULT    1 print_DAP_Values
    26: 00008187     0 NOTYPE  LOCAL  DEFAULT    1 print_DAP_Values[...]
    27: 000081ad     1 OBJECT  LOCAL  DEFAULT    1 msg_new_line
    28: 000081b0     0 NOTYPE  LOCAL  DEFAULT    1 file_not_found
    29: 000081bc     0 NOTYPE  LOCAL  DEFAULT    1 print_hex_word
    30: 000081c8     0 NOTYPE  LOCAL  DEFAULT    1 print_hex_word.loop
    31: 000081dd     0 NOTYPE  LOCAL  DEFAULT    1 set_in_dap_value
    32: 000081dd     0 NOTYPE  LOCAL  DEFAULT    1 set_in_dap_value[...]
    33: 000081de     0 NOTYPE  LOCAL  DEFAULT    1 set_in_dap_value[...]
    34: 000081e2     0 NOTYPE  LOCAL  DEFAULT    1 set_in_dap_value.loop
    35: 000081eb     0 NOTYPE  LOCAL  DEFAULT    1 set_in_dap_value.end
    36: 000081ed     1 OBJECT  LOCAL  DEFAULT    1 load_msg
    37: 0000820c     1 OBJECT  LOCAL  DEFAULT    1 msg_file_not_found
    38: 00008233     1 OBJECT  LOCAL  DEFAULT    1 msg_file_found
    39: 00008259     1 OBJECT  LOCAL  DEFAULT    1 filename_stage2_part2
    40: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _start
    41: 00009000     0 NOTYPE  GLOBAL DEFAULT    1 __bss_start
    42: 00009000     0 NOTYPE  GLOBAL DEFAULT    1 _edata
    43: 00009000     0 NOTYPE  GLOBAL DEFAULT    1 _end
</code>

To read the full name, use --wide as parameter.

This was a really good sample, and there should be maybe a elf parser Python Script. I want to use Python, cause Python is a fast and good modifieable customizable script.

This could be done by calling this, maybe like writing when needed for example: "stage2.print_hex_word". Then the Python Script would replace this string with this value: 0x81bc from the above sample.
## GitHub Build Artifact (Floppy)

A GitHub Actions workflow is available at `.github/workflows/build-floppy.yml`.
You can run it manually via **Actions → Build Floppy Image → Run workflow**.
After it finishes, download the artifact `customos-images` which contains:

- `floppy.img` (raw floppy disk image for QEMU)
- `floppy.img.gz` (compressed image)
- `customos-cd.iso` (bootable CD ISO image)

## Browser Emulator (wie cloudvm.app)
Der Workflow erzeugt zusätzlich das Artifact `customos-browser-emulator`.
Darin sind enthalten:

- `index.html` (v86-basierter Browser-Emulator)
- `floppy.img` (dein Boot-Image)

So nutzt du es:
1. In GitHub Actions den Workflow **Build Floppy Image** starten.
2. Artifact `customos-browser-emulator` herunterladen und entpacken.
3. Inhalte auf einen statischen Webhost laden (z. B. GitHub Pages, Netlify, Cloudflare Pages).
4. `index.html` im Browser öffnen — die VM startet direkt vom Floppy-Image.

Wenn du stattdessen cloudvm.app nutzen willst, lade dort einfach das erzeugte `floppy.img` hoch.


## Unified Release (single tag)

The workflow also publishes all build outputs to one stable GitHub release tag: `build-artifacts`.

Release page:
- `https://github.com/<owner>/<repo>/releases/tag/build-artifacts`

Direct download links (same tag each run):
- `https://github.com/<owner>/<repo>/releases/download/build-artifacts/floppy.img`
- `https://github.com/<owner>/<repo>/releases/download/build-artifacts/floppy.img.gz`
- `https://github.com/<owner>/<repo>/releases/download/build-artifacts/customos-cd.iso`
- `https://github.com/<owner>/<repo>/releases/download/build-artifacts/customos-browser-emulator.tar.gz`
