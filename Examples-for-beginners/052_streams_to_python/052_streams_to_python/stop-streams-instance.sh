#!/bin/sh
# ========================
# Use this script to stop your streams instance.
# ========================
s=0	# Number of options to shift out
while getopts "d:i:h" options; do
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

if [ "$streams_instance_name" == "" ];
then
   echo "Missing or wrong streams instance name via the -i option."
   echo "Your streams instance name must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi

# Let us stop the instance now.
streamtool stopinstance -d $streams_domain_name -i $streams_instance_name

date
