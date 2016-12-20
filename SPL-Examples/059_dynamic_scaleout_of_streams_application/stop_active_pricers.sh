#!/bin/sh
# ========================
# First created on: Jan/08/2012
# Last modified on: Mar/28/2015
#
# Use this script to stop actively running pricers.
# At this time, you can only stop x number of pricers
# that were started most recently as a group.
# 
# For example, you may have started bunch of pricers 
# in this sequence.
# 1) Start 3 pricers
# 2) Start 5 pricers
# 3) Start 7 pricers
# 4) Start 6 pricers
#
# After a while, if you want to stop some of the pricers,
# you can only bring them down in the most recent order
# and group in which they got started.
# (Sorry, we don't allow arbitrary stopping of random
#  pricers at this time).
# You must issue stop pricer script in the reverse order
# in which you started the pricers as shown below.
#
# 1) Stop 6 pricers
# 2) Stop 7 pricers
# 3) Stop 5 pricers
#
# When these active pricers are taken off-line, the main
# driver application will automatically stop sending
# pricing calculations to those pricers.
# We will give ample time for the currently running
# price calculations to complete on those pricers that are marked for
# shutdown before they really go off-line.
#
# If you want to stop all the pricers, then you
# simply stop the application by using the script
# stop_dynamic_scaleout_app.sh 
#
# We will enhance this script later to be able to
# stop x number of pricers on a specific list of machines.
# ========================
pricer_engines_to_be_stopped=0
s=0	# Number of options to shift out
while getopts "d:i:s:e:m:h" options; do
    case $options in
     d) streams_domain_name=$OPTARG
       let s=s+2
       ;;
    i) streams_instance_name=$OPTARG
       let s=s+2
       ;;
    s) dynamic_scaleout_spl_project_dir=$OPTARG
       let s=s+2
       ;;
    # -e specifies the number of engines that will be stopped.
    # We will stop x number of engines that were most recently started.
    e) let pricer_engines_to_be_stopped=$OPTARG
       let s=s+2
       ;;
    m) node_for_the_main_app=$OPTARG
       let s=s+2
       ;;
    h | * ) echo "
Command line arguments
  -d STRING       streamsDomainName            (big_space)
  -i STRING       streamsInstanceName          (silver_stream)
  -s STRING       dynamicScaleOutSplProjectDir   (Your dynamic_scaleout SPL project directory)
  -e INTEGER      pricerEnginesToBeStopped     (10)   (# of most recently started engines will be stopped)
  -m STRING       nodeForTheMainApp            (delaware)

  e-g:
  -d big_space
  -i silver_stream
  -s /homes/usr1/workspace/splproj
  -e 10
  -m delaware
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

# Validate the streams instance name entered by the user.
if [ "$streams_instance_name" == "" ];
then
   echo "Missing or wrong streams instance name via the -i option."
   echo "Your streams instance name must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the dynamic_scaleout SPL project directory entered by the user.
if [ "$dynamic_scaleout_spl_project_dir" == "" ];
then
   echo "Missing or wrong dynamic_scaleout SPL project dir via the -s option."
   echo "dynamic_scaleout SPL project directory must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the number of pricer engines per node entered by the user.
if [ $pricer_engines_to_be_stopped -le 0 ];
then
   echo "Missing or wrong pricer engines to be stopped via the -e option."
   echo "Total number of pricer engines to be stopped must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the node for the main app entered by the user.
if [ "$node_for_the_main_app" == "" ];
then
   echo "Missing or wrong node name for the main application via the -m option."
   echo "Node name for the main application must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# ================
# echo $streams_instance_name
# echo $dynamic_scaleout_spl_project_dir
# echo $pricer_engines_to_be_stopped
# echo $node_for_the_main_app
# exit 1
# ================
date
# ================
cd $dynamic_scaleout_spl_project_dir

# This script can be ONLY be run after the dynamic_scaleout application has been started.
# Ensure that there are at least two jobs running under our instance.
# i.e. A minimum of 1 pricer job and 1 data ingestion/exgestion job must already be running.
current_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)

if [ $current_jobs_cnt -lt 2 ];
then
   echo "You can't stop active pricers now. First, you must run the main application using the start_dynamic_scaleout.sh script." 
   exit 1
fi

# Ensure that user didn't ask us to delete all the actively running pricers.
# We will not allow that. We need at least one pricer running at any given time.
let potential_pricer_jobs_cnt=current_jobs_cnt-1
if [ $pricer_engines_to_be_stopped -ge $potential_pricer_jobs_cnt ];
then
   echo "You are trying to stop all the actively running pricer jobs."
   echo "You must have at least one pricer actively running at any given time."
   exit 1
fi

# Let us collect information about all the jobs running under this streams instance.
temp_dynamic_scaleout_streams_jobs_info_file="$HOME/temp-dynamic-scaleout-streams-jobs-info.txt"
rm -f "$temp_dynamic_scaleout_streams_jobs_info_file"
streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running > "$temp_dynamic_scaleout_streams_jobs_info_file"

# Read the jobs info file and get only the Pricer job ids running under the given streams instance.
# Store them in an array.
pricer_job_ids_array=()
pricer_job_index=0

job_id=99999
job_name=""

while read line
do
   job_id=$(echo $line | awk '{print $1}')
   job_name=$(echo $line | awk '{print $6}')
   
   if [ "$job_name" == "com.ibm.streams.pricing::Pricer" ];
   then
      pricer_job_ids_array[$pricer_job_index]=$job_id
      let pricer_job_index=pricer_job_index+1
   fi   
done < "$temp_dynamic_scaleout_streams_jobs_info_file"

rm -f "$temp_dynamic_scaleout_streams_jobs_info_file"
pricer_jobs_stopped_cnt=0

# We have to now send a control signal to the dynamicScaleout main driver application and
# inform that we are going to stop one or more pricers. That will allow the main driver
# application not to send any new price calculation requests to those pricers.
# External control signal is sent by writing a new line to a CSV file with an
# int32 negative value indicating the total number of pricer engines that will be stopped shortly.
let negative_pricer_engines_to_be_stopped=0-pricer_engines_to_be_stopped
`ssh -xa $node_for_the_main_app "echo $negative_pricer_engines_to_be_stopped >> /tmp/PricerStartStopControl.csv" > /dev/null 2> /dev/null`

# Wait long enough for the main driver application to catch the control signal and stop
# sending any further pricing calculation requests to those pricers.
echo "Signaling the running application that $pricer_engines_to_be_stopped pricer engine(s) will be stopped shortly."
let starting_pricer_job_idx=pricer_job_index-1
capture_state_output_file="$HOME/pricer-capture-state.txt"

# Iterate over the pricer job ids array in reverse order and stop the requested
# number of active pricers that were most recently started.
for (( jobIdx=$starting_pricer_job_idx; jobIdx>=0; jobIdx-- ))
do 
   let job_id=pricer_job_ids_array[$jobIdx]
   echo "Stopping pricer job $job_id ..."
   ntuples_submitted_previous_token=""
   ntuple_submitted_current_token=""
   
   # Let us first ensure that the job to be stopped has finished processing any pending price calculation requests.
   # We can do streamtool capture state 5 times for this pricer job and ensure its number of tuples submitted on its output port stays the same.      
   for (( csCnt=1; csCnt<=5; csCnt++  ))
   do
      let line_number=0
      rm -f $capture_state_output_file > /dev/null
      streamtool capturestate -d $streams_domain_name -i $streams_instance_name --select jobs=metrics --jobs $job_id > $capture_state_output_file
      # Parse the line in the capture state output file that will give us the value for nTuplesSubmitted.
      # We are looking for the following block of output lines in the capture state file.
      # For the pricer job, capture state output for the following XML block should always start at line 232.
      # That means, we can simply parse line 234 to get the actual value of nTuplesSubmitted metric.
      # Since there is no other elegant way to detect if a pricer has stopped processing its pending requests, 
      # we are using this technique for this purpose.
      #  <outputPort index="0" name="PricerFinalResult" streamName="PricerFinalResult">
      #     <metric name="nTuplesSubmitted" lastChangeObserved="1326313189" userDefined="false">
      #        <metricValue xsi:type="streams:longType" value="133"/>
      #     </metric>   
   
      while read line
      do
         let line_number=line_number+1
   
         if [ $line_number -eq 234 ];
         then
            # Parse the 3rd token that has the nTuplesSubmitted value.
            if [ "$ntuples_submitted_previous_token" == "" ];
            then
               ntuples_submitted_previous_token=$(echo $line | awk '{print $3}')
            else
               ntuples_submitted_current_token=$(echo $line | awk '{print $3}')
               
               if [ "$ntuples_submitted_previous_token" != "$ntuples_submitted_current_token" ];
               then
                  # Previous value read and the current value read are not matching.
                  # Let us reset the loop count so that we can match 10 consecutive times for no value change in the nTuplesSubmitted metric.
                  # echo "Resetting loop..."
                  # echo $ntuples_submitted_previous_token
                  # echo $ntuples_submitted_current_token
                  let csCnt=1
                  ntuples_submitted_previous_token=""
                  ntuples_submitted_current_token=""
               fi               
            fi
                   
            break
         fi   
      done < "$capture_state_output_file"  
   done
   
   
   
   # Cancel the Streams job in the background.
   streamtool canceljob -d $streams_domain_name -i $streams_instance_name $job_id > /dev/null &
   let pricer_jobs_stopped_cnt=pricer_jobs_stopped_cnt+1

   if [ $pricer_jobs_stopped_cnt -ge $pricer_engines_to_be_stopped ];
   then
      break
   fi
done

# Since we did background job cancellations, let us now
# wait for all those jobs to get stopped.
rm -f $capture_state_output_file > /dev/null
total_jobs_cnt=0
echo "Waiting for all the $pricer_engines_to_be_stopped pricer engine(s) to get stopped ..."

let remaining_jobs_that_should_be_active=current_jobs_cnt-pricer_engines_to_be_stopped

while :
do
   # Get the total number of jobs running under this instance.
   total_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)
   
   if [ $total_jobs_cnt -eq $remaining_jobs_that_should_be_active ];
   then 
      break
   fi
done

echo "Successfully stopped $pricer_engines_to_be_stopped pricer engine(s)."

# We are done stopping the active pricers as requested.
# ==========================
date
