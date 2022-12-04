#!/usr/bin/env python3

# Used by:
# https://github.com/lief-project/LIEF/blob/master/examples/python/elf_json.py
# This will be modified to an better version and another file. See README.

import argparse
import sys
import lief
import json

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('binary', help='ELF binary')
    args = parser.parse_args()
    binary = lief.parse(args.binary)
    json_data = json.loads(lief.to_json(binary))
    print(json.dumps(json_data, sort_keys = True, indent = 4))

if __name__ == "__main__":
    sys.exit(main())
