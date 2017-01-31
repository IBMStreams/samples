#!/bin/sh
# ========================
# First created on: Nov/28/2011
# Last modified on: Mar/28/2015
#
# Use this script to stop the dynamic_scaleout application.
# ========================
s=0	# Number of options to shift out
while getopts "d:i:h" options; do
    case $options in
    case $options in
    d) streams_domain_name=$OPTARG
       let s=s+2
       ;;
       
    i) streams_instance_name=$OPTARG
       let s=s+2
       ;;
    h | * ) echo "
Command line arguments
  -d STRING     streamsDomainName   (big_space)
  -i STRING     streamsInstanceName (silver_stream)

  e-g:
  -d big_space  
  -i silver_stream
"
        exit 1
        ;;
    esac
done
shift $s

# Validate the streams domain name  entered by the user.
if [ "$streams_domain_name" == "" ];
then
   echo "Missing or wrong streams domain name via the -d option."
   echo "Your streams domain name must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the streams instance name  entered by the user.
if [ "$streams_instance_name" == "" ];
then
   echo "Missing or wrong streams instance name via the -i option."
   echo "Your streams instance name must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

date
# Let us collect information about all the jobs running under this streams instance.
temp_dynamic_scaleout_streams_jobs_info_file="$HOME/temp-dynamic-scaleout-streams-jobs-info.txt"
rm -f "$temp_dynamic_scaleout_streams_jobs_info_file"
streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running > "$temp_dynamic_scaleout_streams_jobs_info_file"
# Get the total number of jobs running under this instance.
total_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)

# Read the jobs info file and get the job ids running under the given streams instance.
job_id=99999
job_stopped_cnt=0

while read line
do
   job_id=$(echo $line | awk '{print $1}')
   # Cancel the Streams job in the background.
   streamtool canceljob -d $streams_domain_name -i $streams_instance_name $job_id > /dev/null &
   let job_stopped_cnt=job_stopped_cnt+1
done < "$temp_dynamic_scaleout_streams_jobs_info_file"

echo "Background Streams job cancellation done for $job_stopped_cnt jobs in the instance '$streams_instance_name' ..."
echo "Waiting for all the $job_stopped_cnt jobs to stop completely ..."
rm -f "$temp_dynamic_scaleout_streams_jobs_info_file"

if [ $job_stopped_cnt -eq 0 ];
then
   # No jobs were stopped.
   echo "No Streams jobs are active at this time for the instance $streams_instance_name."
else
   # Let us wait here for all the background "stop job" tasks to complete.
   while :
   do
      # Get the total number of jobs running under this instance.
      total_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)
   
      # If there are 0 jobs running now, we can break out of this wait. 
      if [ $total_jobs_cnt -le 0 ];
      then 
         break
      fi
   done

   echo "Completed stopping all $job_stopped_cnt job(s) in the instance '$streams_instance_name'."
fi

rm -f ./data/PricerStartStopControl.csv > /dev/null
date
