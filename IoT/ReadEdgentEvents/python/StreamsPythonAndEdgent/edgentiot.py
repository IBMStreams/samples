
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2017

from streamsx.topology.topology import Topology

from streamsx.rest import StreamingAnalyticsConnection
from streamsx.topology import schema
import json
from mysa_creds import *

def submit_to_service(topo):
    """
       Takes a Topology object and submits it to the Streaming analytics service
       Returns the SubmissionResult
    """
    # build config object to authenticate with the service
    cfg = get_config_object()
    submission_result = submit('STREAMING_ANALYTICS_SERVICE', topo, cfg)

    return submission_result

   
def get_cmd(tuple):
    #build the message you wish to send as a dictionary
    payload = {}
    
    payload["action"] = "Reboot"
    payload["msg"] = "Alert code: 249"
    
    command_data =  {} 
    command_data ["d"] = payload

    #convert the whole payload to json
    command_as_json = json.dumps(command_data)

    #build the command metadata. The device id and device type are on the tuple, but you could also specify them manually
    device_cmd ={}
    device_cmd["typeId"] = tuple["typeId"]
    device_cmd["cmdId"] = "display"
    device_cmd["deviceId"] = tuple["deviceId"]

    device_cmd["jsonString"] = command_as_json
    return device_cmd   
    
def get_event_data(tuple):
    payload_json = tuple["jsonString"]
    payload = json.loads(payload_json)
    return payload["d"]

def main():
   #define needed variables
   COMMANDS_TOPIC = "streamsx/iot/device/commands/send" #topic to publish commands to 
   EVENTS_TOPIC = "streamsx/iot/device/events" #topic to subscribe to for events 
   incoming_schema =  schema.StreamSchema("tuple <rstring typeId, rstring deviceId, rstring eventId,rstring jsonString>") 
   cmd_schema = schema.StreamSchema('tuple<rstring typeId, rstring deviceId, rstring cmdId, rstring jsonString>')


   topo = Topology('ReadingsFromIot')
   
   #Subscribe to  events
   events = topo.subscribe(EVENTS_TOPIC, incoming_schema)
   sensor_events = events.filter(lambda tuple: tuple["eventId"] == "sensors")
   readings = sensor_events.map(get_event_data)
   readings.print()
  
   #send a command
   cmd_stream = sensor_events.map(get_cmd)
   #convert the commands stream to a SPL structured schema
   commands_to_publish = cmd_stream.map(lambda x : (x["typeId"],x["deviceId"],x["cmdId"],x["jsonString"],), schema = cmd_schema)

   commands_to_publish.publish(COMMANDS_TOPIC, cmd_schema)
   commands_to_publish.print()
   result = submit_to_service(topo)
   print("Submitted job to the service, job id = " + str(result.job.id))



if __name__ == '__main__':
    main()


