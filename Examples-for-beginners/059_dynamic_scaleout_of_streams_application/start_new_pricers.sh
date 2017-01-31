#!/bin/sh
# ========================
# First created on: Jan/08/2012
# Last modified on: Mar/29/2015
#
# Use this script to start new pricers on 
# either a single machine or on multiple machines.
# These additional pricers would typically be started after the 
# initial set of pricers that were started during application
# start-up are not sufficient to do a high volume price calculation.
# When, these new pricers come up, already running main composite will
# automatically include them in load sharing.
# ========================
# Initialize an array to store the execution nodes, where the pricer engines will be deployed.
nodes_for_pricer_engines=()
nodes_cnt=0
pricer_engines_per_node=0
node_for_the_main_app=""
s=0	# Number of options to shift out
while getopts "d:i:s:e:n:m:h" options; do
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
    # -e specifies the number of engines that will be started on every specified node.
    e) let pricer_engines_per_node=$OPTARG
       let s=s+2
       ;;
    n) for i in ${OPTARG}
       do
          nodes_for_pricer_engines[$nodes_cnt]=$i
          let nodes_cnt=nodes_cnt+1
       done
       ;;
    m) node_for_the_main_app=$OPTARG
       let s=s+2
       ;;
    h | * ) echo "
Command line arguments
  -d STRING       streamsDomainName     (big_space)
  -i STRING       streamsInstanceName   (silver_stream)
  -s STRING       dynamicScaleOutSplProjectDir   (Your dynamic_scaleout SPL project directory)
  -e INTEGER      pricerEnginesPerNode  (10)   (# of engines per node to be started at the time of application start-up)
  -n STRING       nodesForPricerEngines (sacramento concord hudson potomac)
  -m STRING       nodeForTheMainApp     (delaware)

  e-g:
  -d big_space
  -i silver_stream
  -s /homes/usr1/workspace/splproj
  -e 10
  -n sacramento -n concord -n hudson -n potomac
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
if [ $pricer_engines_per_node -le 0 ] ||  [ $pricer_engines_per_node -gt "64" ];
then
   echo "Missing or wrong pricer engines per node via the -e option."
   echo "Total number of pricer engines per node must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the nodes for pricer engines entered by the user.
if [ $nodes_cnt -le 0 ];
then
   echo "Missing or wrong node names for pricer engines via the -n option."
   echo "Node names for pricer engines  must be specified."
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
# echo $pricer_engines_per_node
# echo $nodes_cnt
# echo ${nodes_for_pricer_engines[*]}
# echo $node_for_the_main_app
# exit 1
# ================
date
# ================
cd $dynamic_scaleout_spl_project_dir

# This script can be ONLY be run after the dynamic_scaleout application has been started.
# Ensure that there are at least two jobs running under our instance.
# i.e. A minimum of 1 pricer job and 1 data ingestion/exgestion job must already be running.
currently_active_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)

if [ $currently_active_jobs_cnt -lt 2 ];
then
   echo "You can't start new pricers now. First, you must run the main application using the start_dynamic_scaleout.sh script." 
   exit 1
fi

# We are now going to take a copy of the ADL file for the Pricer main composite job.
# NOTE: Starting in Streams 4.x, .sab file is the preferred mechansim over the .adl file.
# ADL file is still there in Streams 4.x, but it may be completely removed in one of the future releases of Streams.
# At that point, the following technique of manipulating the ADL file will stop working. 
# This approach needs to be changed for this example to keep working beyond 2015.
cp -f ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl  ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl.org

node_name=""
# Before we do anything, let us delete any left-over files from the previous application run.
rm -f ./data/PricingEngine-Started-*.txt &> /dev/null

# Purposefully, we assigned "pricer_dummy_machine" as the execution node inside the ADL file at the time of development.
# Now, we are going to stay in a loop and change the adl with the user provided node names and start the job with the substituted node name.
# ==========================
# At first, let us start as many pricer engines per machine that the user asked for. 
# Stay in a for loop and start the pricer engines on all the machines provided to this script.
# 
# Since, few pricers are already running, we have to assign pricer ids by skipping the
# ids owned by already running pricer jobs.
# In the currently running application, there is only one non pricer job that does
# data ingestion and data exgestion. All the rest are pricer jobs.
# Let us pick the next available pricer id.
pricerEngineId=0
let pricerEngineId=$currently_active_jobs_cnt-1
let totalPricersToBeStarted=$nodes_cnt*$pricer_engines_per_node

for (( nodeIdx=0; nodeIdx<$nodes_cnt; nodeIdx++ ))
do
   node_name=${nodes_for_pricer_engines[$nodeIdx]}
   cp -f ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl.org    ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl
   # Let us do the string substitution of the node name in the ADL file contents.
   sed -e "s/pricer_dummy_machine/$node_name/g"   ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl   > ./com.ibm.streams.pricing.Pricer.adl
   rm -f ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl
   mv ./com.ibm.streams.pricing.Pricer.adl  ./output/com.ibm.streams.pricing.Pricer/Distributed
   # Copy the modified ADL file as a file that carries the node index in its name.
   cp ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl   ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl$nodeIdx
      
   # We now have massaged the ADL file with the user requested node name.
   # We can now stay in another loop and start as many engines as requested on that node.
   for (( engineIdx=0; engineIdx<$pricer_engines_per_node; engineIdx++ ))
   do
      # Remember to pass the pricerId as a submission time value to the pricer engine job being started at this time.
      let pricerEngineId=pricerEngineId+1
      # Launch this streams job in the background.
      streamtool submitjob -d $streams_domain_name -i $streams_instance_name ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl$nodeIdx  -P com.ibm.streams.pricing::Pricer.pricerId=$pricerEngineId > /dev/null &      
   done
   
   echo "Background Streams job submission done for $engineIdx pricing engines on $node_name ..."
   # read -p "Press any key to continue"
done

# Since we did background job submissions on all the requested nodes, let us now
# wait for all those jobs to get started.
total_jobs_cnt=0
let expected_new_total_jobs_cnt=$currently_active_jobs_cnt+$totalPricersToBeStarted
echo "Waiting for all the $totalPricersToBeStarted pricing engines to come alive ..."

while :
do
   # Get the total number of jobs running under this instance.
   total_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)
   
   if [ $total_jobs_cnt -ge $expected_new_total_jobs_cnt ];
   then 
      break
   fi
done

# We can now delete the temporary ADL files we created.
for (( nodeIdx=0; nodeIdx<$nodes_cnt; nodeIdx++ ))
do
   rm -f ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl$nodeIdx
done

# We have started the pricer engines requested number of times on all the user-provided nodes.
# Let us now put back the original ADL file for this Streams job.
rm -f ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl
mv ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl.org    ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl
# ==========================
# Let us ensure that all the pricing engines have started and they are 
# ready for export/import of streams with the source/sink job.
total_startup_signal_files_cnt=0
while :
do
   # Get the total number of start-up signal files created by the pricing engines.
   total_startup_signal_files_cnt=$(ls -la ./data/PricingEngine-Started-*.txt 2> /dev/null | wc -l)
   
   if [ $total_startup_signal_files_cnt -ge $totalPricersToBeStarted ];
   then 
      break
   fi
done

echo "$totalPricersToBeStarted new pricing engines are up and running now."

# We have to now send a control signal to the dynamicScaleout main driver application and
# inform that we started one or more new pricers that should be put to good use.
# External control signal is sent by writing a new line to a CSV file with an
# int32 value indicating the total number of new pricer engines that were just started.
`ssh -xa $node_for_the_main_app "echo $totalPricersToBeStarted >> /tmp/PricerStartStopControl.csv" > /dev/null 2> /dev/null`

# Let us delete the start-up signal files created by the pricing engines.
rm -f ./data/PricingEngine-Started-*.txt &> /dev/null
# We are done starting the additional pricers as requested.
# ==========================
date
