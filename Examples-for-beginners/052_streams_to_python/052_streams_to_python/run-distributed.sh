s=0	# Number of options to shift out
while getopts "d:i:z:h" options; do
    case $options in
    d) streams_domain_name=$OPTARG
       let s=s+2
       ;;
       
    i) streams_instance_name=$OPTARG
       let s=s+2
       ;;

    z) zk_conn_string=$OPTARG
       let s=s+2
       ;;

    h | * ) echo "
Command line arguments
  -d STRING     streamsDomainName   (big_space)
  -i STRING     streamsInstanceName (silver_stream)
  -z STRING     zkConnectionString  (myhost:21810)
 
  e-g:
  -d big_space
  -i silver_stream
  -z myhost:21810
"
        exit 1
        ;;
    esac
done
shift $s

# Validate the streams domain name entered by the user.
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

if [ "$zk_conn_string" == "" ];
then
   echo "Missing or wrong zk connection string via the -z option."
   echo "Your zookeeper connection string must be specified."
   echo ""
   echo "Get help using -h option."
   exit 1
fi


# Now, we can go ahead and start the instance (if not already running) and then
# start the NxCax applications.
echo "Ensuring that the Streams instance '$streams_instance_name' is running ..."
# If Streams instance is already started and running, it will display a warning on
# the stderr console. Let us suppress that warning by redirecting stderr and 
# stdout to a null device.
streamtool startinstance -d $streams_domain_name -i $streams_instance_name &> /dev/null


streamtool submitjob -d $streams_domain_name -i $streams_instance_name --zkconnect $zk_conn_string ./output/python.wrapper.example.streams_to_python/Distributed/python.wrapper.example.streams_to_python.sab

echo "****** You can check the results from this run inside this file: data/UrlToIpAddress-Result.csv ******"
