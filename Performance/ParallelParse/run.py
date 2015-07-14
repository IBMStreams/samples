#!/usr/bin/env python

# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2015  

import sys, time, collections, csv
from subprocess import call, Popen, PIPE

def exec_no_fail(seq):
    p = Popen(seq, stdout=PIPE, stderr=PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print 'err: ' + stderr
        print str(seq) + ' failed.'
    return stdout

def exec_fail(seq):
    p = Popen(seq, stdout=PIPE, stderr=PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print 'err: ' + stderr
        print str(seq) + ' failed.'
        sys.exit()
    return stdout

def exec_raise(seq):
    p = Popen(seq, stdout=PIPE, stderr=PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print 'err: ' + stderr
        print str(seq) + ' failed.'
        raise Exception('exec_throw failure')
    return stdout

def finish_exec(p):
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print 'err: ' + stderr
        print "'" + str(p) + "' failed."
        sys.exit()
    return stdout

def run_standalone(exp, params):
    exec_raise(['./output/' + exp + '/bin/standalone'] + [params])

Experiment = collections.namedtuple('Experiment', ['name', 'argument_name', 'argument_list'])

def main():
    sample_size = 5
    experiments = [Experiment('FileSourceOnly', '', ['']),
                   Experiment('FileSourceString', '', ['']), 
                   Experiment('SequentialParse', '', ['']), 
                   Experiment('ParallelParse', 'parallelism=', [str(i) for i in [1, 2, 4, 8, 16]]), 
                   Experiment('ParallelParseOrdered', 'parallelism=', [str(i) for i in [1, 2, 4, 8, 16]])]

    exec_raise(['make'])

    results_file = open('results.csv', 'w')
    results = csv.writer(results_file)

    for exp in experiments:
        for arg in exp.argument_list:
            row = [exp.name, arg]
            print exp.name, arg,
            for i in range(sample_size):
                before = time.time()
                run_standalone(exp.name, exp.argument_name + arg)
                after = time.time()
                row.append(after - before)
                print after - before,
            results.writerow(row)
            print

    results_file.close()

if __name__ == "__main__":
    main()

