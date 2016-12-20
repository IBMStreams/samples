#!/bin/sh -x
# ========================
# First created on: Nov/28/2011
# Last modified on: Mar/29/2015
#
# Use this script to start the dynamic_scaleout application on 
# either a single machine or on multiple machines.
# During start-up, you can provide a grand total
# number of the pricer engines that you will likely need for the
# current application run. You can start the application with
# x number of pricer engines per machine. As new machines
# become available, you can keep starting additional pricer
# engines or stop the currently active pricer engines using two
# other shell scripts. (start_new_pricers.sh, stop_active_pricers.sh)
# ========================
# Initialize an array to store the execution nodes, where the pricer engines will be deployed.
nodes_for_pricer_engines=()
nodes_cnt=0
grand_total_of_pricers=0
pricer_engines_per_node=0
node_for_the_main_app=""
total_spot_prices=0
source_operator_init_delay=0
beginning_spot_price=""
addition_to_current_spot_price=""
s=0	# Number of options to shift out
while getopts "d:i:s:g:e:n:m:p:l:b:a:h" options; do
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
    # -g specifies the grand total of pricers that will likely be started during this application run.
    g) let grand_total_of_pricers=$OPTARG
       let s=s+2
       ;;
    # -e specifies the number of engines that will be started on every specified node during application start-up.
    # Total engines that will be started during application start-up must be less than or equal to grand_total_of_pricers.
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
    p) let total_spot_prices=$OPTARG
       let s=s+2
       ;;
    l) let source_operator_init_delay=$OPTARG
       let s=s+2
       ;;
    b) beginning_spot_price=$OPTARG
       let s=s+2
       ;;
    a) addition_to_current_spot_price=$OPTARG
       let s=s+2
       ;;
    h | * ) echo "
Command line arguments
  -d STRING       streamsDomainName   (big_space)
  -i STRING       streamsInstanceName   (silver_stream)
  -s STRING       dynamicScaleOutSplProjectDir   (Your dynamic_scaleout SPL project directory)
  -g INTEGER      grandTotalOfPricers   (200)  (# of pricers that will likely be started during this application run)
  -e INTEGER      pricerEnginesPerNode  (10)   (# of engines per node to be started at the time of application start-up)
  -n STRING       nodesForPricerEngines (sacramento concord hudson potomac)
  -m STRING       nodeForTheMainApp (delaware)
  -p INTEGER      totalSpotPrices (1000)
  -l INTEGER      sourceOperatorInitDelay (10)
  -b FLOAT        beginningSpotPrice (58.00)
  -a FLOAT        additionToCurrentSpotPrice (0.05)

  e-g:
  -d big_space
  -i silver_stream
  -s $HOME/SPL-Examples-For-Beginners/059_dynamic_scaleout_of_streams_application
  -g 200
  -e 10
  -n sacramento -n concord -n hudson -n potomac
  -m delaware
  -p 1000
  -l 10
  -b 58.00
  -a 0.05
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

# Validate the grand total of pricer engines entered by the user.
if [ $grand_total_of_pricers -le 0 ] ||  [ $grand_total_of_pricers -gt "2000" ];
then
   echo "Missing or wrong grand total of pricers via the -g option."
   echo "Grand total number of pricer engines must be specified."
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

# Now, ensure that the total number of engines that are requested during application
# start-up is less than or equal to the grand_total_of_pricers.
let number_of_engines_during_app_startup=$nodes_cnt*$pricer_engines_per_node

if [ $number_of_engines_during_app_startup -gt $grand_total_of_pricers ];
then
   echo "Error. Attempting to start more pricer engines than the grand total of pricers planned."
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

# Validate the total number of spot prices entered by the user.
if [ $total_spot_prices -le 0 ];
then
   echo "Missing or wrong number of total spot prices via the -p option."
   echo "Total spot prices must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the source operator initDelay entered by the user.
if [ $source_operator_init_delay -le 0 ];
then
   echo "Missing or wrong value for the source operator initDelay via the -d option."
   echo "Source operator initial delay must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the beginning spot price entered by the user.
if [ "$beginning_spot_price" == "" ];
then
   echo "Missing or wrong beginning spot price via the -b option."
   echo "Beginning spot price must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Validate the addition to current spot price entered by the user.
if [ "$addition_to_current_spot_price" == "" ];
then
   echo "Missing or wrong added value to current spot price via the -a option."
   echo "Addition to current spot price must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# ================
# echo $streams_instance_name
# echo $dynamic_scaleout_spl_project_dir
# echo $grand_total_of_pricers
# echo $pricer_engines_per_node
# echo $nodes_cnt
# echo ${nodes_for_pricer_engines[*]}
# echo $node_for_the_main_app
# echo $total_spot_prices
# echo $source_operator_init_delay
# echo $beginning_spot_price
# echo $addition_to_current_spot_price
# exit 1
# ================
date
# ================
cd $dynamic_scaleout_spl_project_dir

# At first, we are going to dynamically configure the PERL variables (grandTotalOfPricers, pricersPerMachine, numberOfMachinesUsed, and others) in the splmm file.
echo "Dynamically configuring the number of pricing engines inside the SPL code ..."
# Take a copy of the original splmm file before we change the PERL variables.
cp -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm.org
# Replace the first PERL variable now.
sed -e "s/grandTotalOfPricers = 1/grandTotalOfPricers = $grand_total_of_pricers/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test
# Replace the second PERL variable now.
sed -e "s/pricersPerMachine = 1/pricersPerMachine = $pricer_engines_per_node/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test
# Replace the third PERL variable now.
sed -e "s/numberOfMachinesUsed = 1/numberOfMachinesUsed = $nodes_cnt/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test
# Replace the fourth PERL variable now.
sed -e "s/totalSpotPrices = 1/totalSpotPrices = $total_spot_prices/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test
# Replace the fifth PERL variable now.
sed -e "s/beaconInitialDelay = 1/beaconInitialDelay = $source_operator_init_delay.0/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test
# Replace the sixth PERL variable now.
sed -e "s/beginningSpotPrice = 1/beginningSpotPrice = $beginning_spot_price/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test
# Replace the seventh PERL variable now.
sed -e "s/additionToCurrentSpotPrice = 1/additionToCurrentSpotPrice = $addition_to_current_spot_price/g"    ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm   >  ./DynamicScaleOut.splmm
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./DynamicScaleOut.splmm  ./com.ibm.streams.pricing.test

# Let us now build the splmm file, in which we just configured the PERL variables.
./mk > /dev/null
# We are done with configuring the PERL variables in the splmm file and rebuilding that splmm file.
# We can put back the original splmm file now.
rm -f ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm
mv ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm.org   ./com.ibm.streams.pricing.test/DynamicScaleOut.splmm

# Let us create an empty CSV file that the application will source for 
# detecting the external events that start new pricers or stop currently active pricers.
`ssh -xa $node_for_the_main_app 'echo "Positive number for new pricers getting started. Negative number for currently active pricers getting stopped." > /tmp/PricerStartStopControl.csv' > /dev/null 2> /dev/null`

# We are now going to take a copy of the ADL files for all the two jobs (DynamicScaleOut main composite and Pricer main composite).
cp -f ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl  ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl.org

cp -f ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl  ./output/com.ibm.streams.pricing.Pricer/Distributed/com.ibm.streams.pricing.Pricer.adl.org

node_name=""

# Since we are going to start a bunch of pricing engines in parallel in the background.
# We can start the source/sink job only after all the pricing engines have started and
# ready for import/export of streams with the source/sink job.
# Otherwise, it is very hard to guess the initDelay for the source operator.
# We have to hold off the source operator from sending tuples before all the
# pricing engines are ready. In order to do this, we have implemented a hack inside
# the pricing engine code. When a pricing engine starts, it will create a dummy file
# named (data/PricingEngine-Started-JobId.txt). We will count all such files here 
# in the script to ensure that all the pricing engines have really started.
# After that condition is satisfied, this script will start the source/sink job.
# Before we do anything, let us delete any left-over files from the previous application run.
rm -f ./data/PricingEngine-Started-*.txt &> /dev/null

# Now, we can go ahead and start the instance (if not already running) and then
# start the dynamic_scaleout applications.
echo "Ensuring that the Streams instance '$streams_instance_name' is running ..."
# If Streams instance is already started and running, it will display a warning on
# the stderr console. Let us suppress that warning by redirecting stderr and 
# stdout to a null device.
# You can learn more about bash redirection from this URL:
# http://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO-3.html
streamtool startinstance -d $streams_domain_name -i $streams_instance_name &> /dev/null

# Purposefully, we assigned "pricer_dummy_machine" as the execution node for all the two ADL files at the time of development.
# Now, we are going to stay in a loop and change the adl with the user provided node names and start the job with the substituted node name.
# ==========================
# At first, let us start as many pricer engines per machine that the user asked for. 
# Stay in a for loop and start the pricer engines on all the machines provided to this script.
pricerEngineId=0
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
echo "Waiting for all the $totalPricersToBeStarted pricing engines to come alive ..."

while :
do
   # Get the total number of jobs running under this instance.
   total_jobs_cnt=$(streamtool lsjobs -d $streams_domain_name -i $streams_instance_name | grep -i Running | wc -l)
   
   if [ $total_jobs_cnt -ge $totalPricersToBeStarted ];
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
# Before starting the source/sink job, let us ensure that all the pricing engines have
# started and they are ready for export/import of streams with the source/sink job.
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

echo "All the $totalPricersToBeStarted pricing engines are up and running now."

# We are now going to do this for the DynamicScaleOut job and start it only once on the machine for the main application provided by the user.
cp -f ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl.org    ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl
# Let us do the string substitution of the node name in the ADL file contents.
sed -e "s/pricer_dummy_machine/$node_for_the_main_app/g"    ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl > ./com.ibm.streams.pricing.test.DynamicScaleOut.adl
rm -f ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl 
mv ./com.ibm.streams.pricing.test.DynamicScaleOut.adl   ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed

# We now have massaged the ADL file with the user requested node name.
# We can now start the DynamicScaleOut job ONLY ONCE on the node dedicated for the main application.
streamtool submitjob -i $streams_instance_name ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl > /dev/null
echo "Started the DynamicScaleOut main application on $node_for_the_main_app ..."

# We have started the DynamicScaleOut job only once on the very first user-provided node.
# Let us now put back the original ADL file for this Streams job.
rm -f ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl
mv ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl.org    ./output/com.ibm.streams.pricing.test.DynamicScaleOut/Distributed/com.ibm.streams.pricing.test.DynamicScaleOut.adl

# Let us delete the start-up signal files created by the pricing engines.
rm -f ./data/PricingEngine-Started-*.txt &> /dev/null
# We are done starting the full dynamic_scaleout application as requested.
# ==========================
date
