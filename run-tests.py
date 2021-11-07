"""
Usage:

python3 run-tests.py
"""
import subprocess
import sys
import os
import platform

import xml.etree.cElementTree as et


def printByCriteria(output):
    whatToPrint = 0
    if whatToPrint == 0:  # default don't print anything
        pass
    elif whatToPrint == 1:  # print everything
        print(output)
    elif whatToPrint == 2:  # print errors
        criteria = ["Evaluating query", "Missing:", "Additional:"]
        toPrint = [1 for x in criteria if x in output]
        if toPrint:
            print(output)


def run(source_path, queries_path, xml_path):
    cmd = r"Team40\Code40\build_win\x86-Debug\src\autotester\autotester.exe Team40\Tests40\{}_source.txt Team40\Tests40\{}_queries.txt Team40\Tests40\out_{}.xml".format(
        source_path, queries_path, xml_path
    )
    os.system(cmd)
    print()


if __name__ == "__main__":
    path = [
        "affects_complex",
        "affects_simple",
        "basic1",
        "basic2",
        "basic3",
        "call",
        "next",
        "nextbip",
        "pattern",
        "pattern_extreme",
        "Sample",
        "StressTest",
        "TA",
        "tuples",
        "with_boolean_attribute",
        "multiclause_2st",
        "multiclause_2st1p",
        "multiclause_2st1w",
        "multiclause_2st1w1p",
        "multiclause_3st",
        "multiclause_3st1p",
        "multiclause_semantic",
    ]

    for i in range(len(path)):
        print("Running test %d..." % (i + 1))
        print(path[i])
        run(path[i], path[i], path[i])
    print(len(path))
