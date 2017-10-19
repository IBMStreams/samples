# https://github.com/ibm-watson-iot/iot-java/tree/master#migration-from-release-015-to-021
# Comment the following to use the post-0.2.1 WIoTP client behavior.
#

USE_OLD_EVENT_FORMAT=-Dcom.ibm.iotf.enableCustomFormat=false

VM_OPTS=${USE_OLD_EVENT_FORMAT}
        
java  ${VM_OPTS} -jar weather-station-simulator.jar $1 $2
