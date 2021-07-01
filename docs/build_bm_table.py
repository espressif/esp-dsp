#!/usr/bin/env python
#
# Copyright 2021 Espressif Systems (Shanghai) PTE LTD
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
#
# This is python script to process various types trace data streams in SystemView format.
# Trace data can be provided in multiple trace files (one per CPU). After processing phase
# script prints report for every type of trace data stream which was found.
#

from tabulate import tabulate
from pathlib import Path
import argparse


def read_csv(csv_file):
    data = []
    with open(csv_file, 'r') as f:
        # create a list of rows in the CSV file
        rows = f.readlines()
        # strip white-space and newlines
        rows = list(map(lambda x:x.strip(), rows))
        for row in rows:
            # further split each row into columns assuming delimiter is comma
            row = row.split(',')
            # append to data-frame our new row-object with columns
            data.append(row)
    return data


def main():

    argparser = argparse.ArgumentParser("Generate benchmarks table for esp-dsp libray docu")
    argparser.add_argument('--data', '-d', help='Benchmarks data file', type=argparse.FileType('r'))
    argparser.add_argument('--template', '-t', help='Docu template file', type=argparse.FileType('r'))
    argparser.add_argument('--output', '-o', help='Benchmarks output file', type=argparse.FileType('w'))

    args = argparser.parse_args()

    csvFile = args.data.name
    templateFile = args.template.name
    outputFile = args.output.name

    rts_doc = Path(templateFile).read_text()

    data = read_csv(csvFile)

    table_dict = dict()
    max_unused_length = 7

    for i, item in enumerate(data):
        item = data[i]
        # Check if the line contain the valid data and it's not a comment
        if ((len(item) == 0) | (len(item[0]) == 0)):
            continue
        if ((len(item) > 0) & (len(item[0]) > 0) & (item[0][0] == '#')):
            continue
        # We check, if the item hase only one field and has a length more then length Esp32 or Esp32S3
        if ((len(item) == 1) & (len(item[0]) > max_unused_length)):
            table_dict[item[0]] = dict()

    for i, item in enumerate(data):
        item = data[i]
        # Check if the line contain the valid data and it's not a comment
        if ((len(item) == 0) | (len(item[0]) == 0)):
            continue
        if ((len(item) > 0) & (len(item[0]) > 0) & (item[0][0] == '#')):
            continue
        # We check, if the item hase only one field and has a length more then length Esp32 or Esp32S3
        if ((len(item) == 1) & (len(item[0]) > max_unused_length)):
            active_dict = table_dict[item[0]]
        # We check if the line has a name and data values inside
        if ((len(item) == 5) & (len(item[0]) > max_unused_length)):
            active_dict[item[0]] = [0] * 6

    for i, item in enumerate(data):
        item = data[i]
        # Check if the line contain the valid data and it's not a comment
        if ((len(item) == 0) | (len(item[0]) == 0)):
            continue
        if ((len(item) > 0) & (len(item[0]) > 0) & (item[0][0] == '#')):
            continue
        # We check, if the item hase only one field and has a length more then length Esp32 or Esp32S3
        if ((len(item) == 1) & (len(item[0]) > max_unused_length)):
            active_dict = table_dict[item[0]]
        # We check if the line has a name and data values inside
        if ((len(item) == 5) & (len(item[0]) > max_unused_length)):
            current_dict = active_dict[item[0]]
            opt  = int(item[1])
            ansi = int(item[2])
            flag  = int(item[3]) - 1  # o2/os
            cpu  = int(item[4])  # 1 + esp32/esp32s2/esp32s3
            index = flag * 3 + int(round(cpu / 3))
            index_ansi = flag * 3 + 2
            current_dict[index] = opt
            current_dict[index_ansi] = ansi

    table = []

    for item in table_dict.items():
        dic2 = item[1]
        table.append(' ')
        table.append([item[0]])
        for key_data in dic2.items():
            data = [key_data[0]]
            data_array = key_data[1]
            for n in range(len(data_array)):
                data.append(data_array[n])
            table.append(data)

    headers = ["Function\n\rOptimization", "ESP32\n\rO2", "ESP32S3\n\rO2", "ANSI\n\rO2", "ESP32\n\rOs", "ESP32S3\n\rOs", "ANSI\n\rOs"]
    text_table = tabulate(table, headers, tablefmt="grid")

    rts_doc = rts_doc.replace("MAIN_TABLE", text_table)

    with open(outputFile, "w") as benchmarks_file:
        benchmarks_file.write(rts_doc)
        benchmarks_file.close()

    print(rts_doc)
    return 0


if __name__ == '__main__':
    exit(main())
