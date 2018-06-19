rm -rf results
mkdir results
python test_download.py $1 > failure_log
val=$?
echo $val
exit $val
