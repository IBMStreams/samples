#!/usr/bin/env python

import csv, random

tuples = 1000000

def main():
    out = open('./data/data.csv', "w")
    writer = csv.writer(out)
    for i in range(tuples):
        writer.writerow([str(random.random()) for i in range(50)] + 
                        [random.random() for i in range(50)])
    out.close()

if __name__ == "__main__":
    main()
