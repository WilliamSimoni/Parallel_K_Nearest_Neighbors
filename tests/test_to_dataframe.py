import sys
import re
import pandas as pd

#check arguments
if len(sys.argv) != 2:
    print("Error usage: python plot_test.py <filename>")
    sys.exit()

#read file
lines = None
with open(sys.argv[1]) as file:
    lines = file.readlines()

#removing new line character
def remove_new_line(lines):
    new_lines = []
    for line in lines:
        new_lines.append(re.sub("\n", "", line))
    return new_lines
lines = remove_new_line(lines)

#find starting positions of each sub test in test file
start_positions = []
for i, line in enumerate(lines):
    if re.search("-{7}.+-{7}", line):
        start_positions.append(i)
#append length of lines to start_positions
start_positions.append(len(lines))

#create pandas data frame for each test
for i, subtest_start in enumerate(start_positions[:-1]):
    test_name = re.sub("\s", "_", re.sub("-", "", lines[subtest_start]).lower())[1:-1]
    #Retrieving test results

    num_workers = []
    parallel_time = []
    serial_time = []
    entire_program_time = []
    perc_serial = []

    for line in lines[subtest_start+1:start_positions[i+1]]:
        if re.search("Number of Workers", line):
            num_workers.append(int(re.findall("\d+", line)[0]))
            continue
        if re.search("parallel time", line):
            parallel_time.append(int(re.findall("\d+", line)[0]))
            continue
        if re.search("serial time", line):
            serial_time.append(int(re.findall("\d+", line)[0]))
            continue
        if re.search("entire program time", line):
            entire_program_time.append(int(re.findall("\d+", line)[0]))
            continue
        if re.search("\% serial", line):
            perc_serial.append(int(re.findall("\d+", line)[0]))
            continue
    
    pd_subtest = pd.DataFrame()
    pd_subtest["num workers"] = num_workers
    pd_subtest["parallel time"] = parallel_time
    pd_subtest["serial time"] = serial_time
    pd_subtest["entire program time"] = entire_program_time
    pd_subtest["% serial"] = perc_serial

    pd_subtest.to_csv(test_name + ".csv")


"""
for i, line in enumerate(lines):
    if re.search("Number of Workers", line):
        print(line)
"""
