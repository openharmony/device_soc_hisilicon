# Copyright (c) 2026 Nanjing Qinuo Information Technology Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import xml.etree.ElementTree as ET
import os
import re
import sys
import argparse

# --- Configuration Constants ---
SCRIPTS_SUBDIR = 'scripts'
LOAD_ADDR_HEX = '0x41000000'
MMC_DEV_ID = 0
BLOCK_SIZE_BYTES = 512
MAX_CHUNK_SIZE = 256 * 1024 * 1024  # 256MB per chunk to fit in most DDR configs

def parse_size_to_bytes(size_str):
    """Converts size strings like '512K', '20M' or hex '0x1000' to integer byte counts."""
    if not size_str:
        return 0
    size_str = size_str.upper().strip()
    
    # Handle hex strings
    if size_str.startswith('0X'):
        try:
            return int(size_str, 16)
        except ValueError:
            return 0
            
    units = {'K': 1024, 'M': 1024**2, 'G': 1024**3}
    match = re.match(r'(\d+)([KMG]?)', size_str)
    if match:
        number = int(match.group(1))
        unit = match.group(2)
        return number * units.get(unit, 1)
    return 0

def split_file(source_path, chunk_size, output_dir):
    """Splits a file into multiple chunks and returns a list of filenames."""
    file_size = os.path.getsize(source_path)
    if file_size <= chunk_size:
        return [os.path.basename(source_path)]
    
    base_name = os.path.basename(source_path)
    chunk_names = []
    print(f"[*] Large file detected ({file_size / (1024**2):.1f} MB). Splitting into {chunk_size / (1024**2):.0f}MB chunks...")
    
    with open(source_path, 'rb') as f:
        part_num = 0
        while True:
            chunk_data = f.read(chunk_size)
            if not chunk_data:
                break
            chunk_name = f"{base_name}.part{part_num}"
            chunk_path = os.path.join(output_dir, chunk_name)
            with open(chunk_path, 'wb') as out_f:
                out_f.write(chunk_data)
            chunk_names.append(chunk_name)
            part_num += 1
            
    return chunk_names

def generate_update_scripts(xml_file):
    # Determine the output base directory based on the XML file location
    output_root = os.path.dirname(os.path.abspath(xml_file))
    scripts_path = os.path.join(output_root, SCRIPTS_SUBDIR)
    
    print(f"[*] Processing: {xml_file}")
    print(f"[*] Output root: {output_root}")

    if not os.path.exists(scripts_path):
        os.makedirs(scripts_path)

    try:
        with open(xml_file, 'rb') as f:
            raw_content = f.read().decode('gb2312', errors='ignore')
        
        if not raw_content.strip():
            print(f"[!] Error: XML file is empty.")
            sys.exit(1)
            
        root = ET.fromstring(raw_content)
        if root.tag != 'Partition_Info':
            print(f"[!] Error: Invalid XML format.")
            sys.exit(1)
            
    except Exception as e:
        print(f"[!] XML Error: {e}")
        sys.exit(1)

    index_script_lines = []
    partitions = root.findall('Part')
    
    for part in partitions:
        name = part.get('PartitionName')
        start_str = part.get('Start')
        length_str = part.get('Length')
        source_path_xml = part.get('SelectFile')
        
        if not all([name, start_str, length_str, source_path_xml]):
            continue
            
        # Resolve actual file path relative to XML location
        xml_dir = os.path.dirname(os.path.abspath(xml_file))
        actual_source_path = os.path.join(xml_dir, source_path_xml.replace('\\', os.sep))
        
        if not os.path.exists(actual_source_path):
            print(f"[!] Warning: Source file not found: {actual_source_path}. Skipping.")
            continue

        # 1. Split file if it exceeds MAX_CHUNK_SIZE
        chunk_filenames = split_file(actual_source_path, MAX_CHUNK_SIZE, output_root)
        
        # 2. Calculate block addresses
        start_bytes = parse_size_to_bytes(start_str)
        start_blk = start_bytes // BLOCK_SIZE_BYTES

        is_dynamic = (length_str.strip() == '-')
        if is_dynamic:
            calc_cmds = [
                f"setexpr count_blk ${{emmc_total_blk}} - {hex(start_blk)}",
            ]
            max_count_var = "${count_blk}"
        else:
            length_bytes = parse_size_to_bytes(length_str)
            if length_bytes == 0: continue
            calc_cmds = []
            max_count_var = hex(length_bytes // BLOCK_SIZE_BYTES)
        
        # Construct sub-script
        sub_script_name = f"[[{name}"
        sub_script_rel_path = f"{SCRIPTS_SUBDIR}/{sub_script_name}"
        
        content = [
            f"# Auto-generated burn script for: {name}",
            f"echo '>>> Partition: {name} ({length_str})'"
        ]
        content.extend(calc_cmds)
        
        # 3. Generate multi-stage burning logic for chunks
        current_offset_blk = start_blk
        for i, chunk_file in enumerate(chunk_filenames):
            chunk_size = os.path.getsize(os.path.join(output_root, chunk_file))
            
            content.extend([
                f"echo '>>> Chunk {i}: {chunk_file}'",
                f"mw.b {LOAD_ADDR_HEX} 0xFF {hex(MAX_CHUNK_SIZE)}",
                f"tftp {LOAD_ADDR_HEX} {chunk_file}",
                f"crc32 {LOAD_ADDR_HEX} ${{filesize}}",
                f"setexpr actual_blkcnt ${{filesize}} + 0x1ff",
                f"setexpr actual_blkcnt ${{actual_blkcnt}} / {hex(BLOCK_SIZE_BYTES)}",
                # Per-chunk boundary check (Assertion style for compatibility)
                f"setexpr current_limit {max_count_var} - {hex(current_offset_blk - start_blk)}",
                f"echo '>>> Checking boundary...'",
                f"itest.l ${{actual_blkcnt}} <= ${{current_limit}}",
                f"mmc write {hex(MMC_DEV_ID)} {LOAD_ADDR_HEX} {hex(current_offset_blk)} ${{actual_blkcnt}}",
            ])
            current_offset_blk += (chunk_size + BLOCK_SIZE_BYTES - 1) // BLOCK_SIZE_BYTES

        content.append(f"echo '>>> {name} write complete.'")
        
        with open(os.path.join(scripts_path, sub_script_name), 'w') as f:
            f.write('\n'.join(content) + '\n')
            
        index_script_lines.append(f"histart {sub_script_rel_path}")

    # Finalize main index script
    index_script_lines.extend([
        "echo '====================================='",
        "echo '  FIRMWARE UPDATE COMPLETED SUCCESS  '",
        "echo '  REBOOTING IN 3 SECONDS...         '",
        "echo '====================================='",
        "sleep 3",
        "reset",
    ])

    with open(os.path.join(output_root, 'histart.txt'), 'w') as f:
        f.write('\n'.join(index_script_lines) + '\n')

    print(f"[+] Success: Scripts and chunks generated in '{output_root}'")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='HiSilicon hstart Script Generator')
    parser.add_argument('xml', help='Path to the emmc_burn_table.xml file')
    args = parser.parse_args()
    generate_update_scripts(args.xml)
