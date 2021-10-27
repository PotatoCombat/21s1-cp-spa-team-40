"""
Usage:

python3 xml-extractor.py affects_complex_source.txt affects_complex_queries.txt

python3 xml-extractor.py multiclause_source.txt multiclause_2st1p_queries.txt
"""
import sys
import os

import xml.etree.cElementTree as et

def extractTimings(file_path):
	tree = et.parse(file_path)

	root = tree.getroot()
	timings = []
	for t in root.iter('time_taken'):
		timings.append(float(t.text))
	return timings


def getAverage(timings):
	return sum(timings) / len(timings)

def getMin(timings):
	return min(timings)

def getMax(timings):
	return max(timings)

def printAverageTiming(timings):
	avg = getAverage(timings)
	print("Average timing: %.2f" % avg)

def printMinMaxTiming(timings):
	print("Minimum: %d" % min(timings))
	print("Maximum: %d" % max(timings))

def concatenateTimings(subset):
	output = ""
	for i in subset:
		output += ("%.2f + " % i)
	return output[:-3]


def run(source_path, queries_path, xml_path):
	cmd = r"Team40\Code40\build_win\x86-Release\src\autotester\autotester.exe Team40\Tests40\{} Team40\Tests40\{} {}".format(source_path, queries_path, xml_path)
	os.system(cmd)
	print()

if __name__ == "__main__":
	args = sys.argv[1:]
	source_path = args[0]
	queries_path = args[1]
	xml_path = r"Team40\Code40\tests\out.xml"
	n = 5 # args[2]
	avgs = []
	mins = []
	maxs = []
	
	for i in range(n):
		run(source_path, queries_path, xml_path)
		timings = extractTimings(xml_path)
		avg = getAverage(timings)
		avgs.append(avg)
		min_val = getMin(timings)
		mins.append(min_val)
		max_val = getMax(timings)
		maxs.append(max_val)
	
	print(source_path)
	print(queries_path)
	print("    Average:  %.2f  (%s)" % (getAverage(avgs), concatenateTimings(avgs)))
	print("    Shortest: %.2f  (%s)" % (getAverage(mins), concatenateTimings(mins)))
	print("    Longest:  %.2f  (%s)" % (getAverage(maxs), concatenateTimings(maxs)))
