#!/bin/sh
# ========================
# First created on: Jan/20/2012
# Last modified on: Mar/29/2015
#
# Use this script to run the dynamic_scaleout demo interactively
# by gradually adding and removing pricer engines on one or multiple machines.
# This interactive script wraps other shell script commands to 
# demonstrate the features to dynamically scale-up and scale-down
# pricer engines. As you perform these steps, it is highly recommended
# that you monitor the application topology using
# the Streams Live Graph facility available in the Streams Studio eclipse tool.
#(i.e. In the "Streams Explorer" view, right click on your running instance name and
#select "Show Instance Graph" to bring up the Streams Instance Graph tool.)
# ========================
s=0	# Number of options to shift out
while getopts "d:i:s:h" options; do
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
    h | * ) echo "
Command line arguments
  -d STRING       streamsDomainName   (big_space)
  -i STRING       streamsInstanceName (silver_stream)
  -s STRING       dynamicScaleOutSplProjectDir   (Your dynamic_scaleout SPL project directory)

  e-g:
  -d big_space
  -i silver_stream
  -s $PWD
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


# ================
# echo $dynamic_scaleout_spl_project_dir
# exit 1
# ================
date
# ================
cd $dynamic_scaleout_spl_project_dir
echo "=========================="
echo "1)  Start 2 pricer engines on machine1: $HOSTNAME"
read -p "Press any key to continue..."
./start_dynamic_scaleout_app.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -g 32 -e 2 -n $HOSTNAME -m $HOSTNAME -p 4000000 -b 58.00 -a 0.00 -l 7

echo "=========================="
echo "2)  Start 2 pricer engines on machine1: $HOSTNAME"
read -p "Press any key to continue..."
./start_new_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 2 -n $HOSTNAME -m $HOSTNAME

echo "=========================="
echo "3)  Start 2 pricer engines on machine1: $HOSTNAME"
read -p "Press any key to continue..."
./start_new_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 2 -n $HOSTNAME -m $HOSTNAME

echo "=========================="
echo "4)  Start 2 pricer engines on machine1: $HOSTNAME"
read -p "Press any key to continue..."
./start_new_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 2 -n $HOSTNAME -m $HOSTNAME

echo "=========================="
echo "5)  Stop the 2 pricers that we just started on: $HOSTNAME"
read -p "Press any key to continue..."
./stop_active_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 2  -m $HOSTNAME

echo "=========================="
echo "6)  Start 1 pricer engine on $HOSTNAME"
read -p "Press any key to continue..."
./start_new_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 1 -n $HOSTNAME -m $HOSTNAME

echo "=========================="
echo "7)  Stop the 4 most recent pricers that we started on: $HOSTNAME"
read -p "Press any key to continue..."
./stop_active_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 4  -m $HOSTNAME

echo "=========================="
echo "8)  Start 6 pricer engines on: $HOSTNAME"
read -p "Press any key to continue..."
./start_new_pricers.sh -d $streams_domain_name -i $streams_instance_name -s $dynamic_scaleout_spl_project_dir -e 6 -n $HOSTNAME  -m $HOSTNAME

echo "=========================="
echo "9)  Stop the dynamic_scaleout application on all the machine()s."
read -p "Press any key to continue..."
./stop_dynamic_scaleout_app.sh -d $streams_domain_name -i $streams_instance_name

echo "=========================="
echo "10) Stop the Streams instance on all the machine(s)."
read -p "Press any key to continue..."
./stop_streams_instance.sh -d $streams_domain_name -i $streams_instance_name

# We are done running the demo of dynamic_scaleout application.
# ==========================
date
