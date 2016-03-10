#!/bin/bash
echo "Running normalize for Messi"
output/normalize.example/bin/standalone aliastable=etc/aliases.csv inputFile=soccer_in.txt
echo "Normalize results for soccer"
cat data/aliasout.txt
head -10 data/aliasout.txt
if [ -f "data/iliad.txt" ] 
then
    echo "Iliad found, not fetching"
else
    echo "Getting the Iliad from project gutenberg"
    wget -O data/iliad.txt https://www.gutenberg.org/cache/epub/6150/pg6150.txt
fi
echo "Tokenize where each tuple has all the tokens in a particular line"
output/tokenize.bag/bin/standalone stopwords=etc/stopwords.dict inputFile=iliad.txt
head -10 data/tokenbag.out
echo "tokenize such that each output tuple is a token"
output/tokenize.stream/bin/standalone stopwords=etc/stopwords.dict inputFile=iliad.txt
head -10 data/tokenstream.out
