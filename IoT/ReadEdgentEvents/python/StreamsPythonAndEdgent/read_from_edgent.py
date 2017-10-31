
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2017

from streamsx.topology.topology import Topology

from streamsx.rest import StreamsConnection
from streamsx.topology import schema
import json
from streamsx.topology.context import *


# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2017

def get_config_object():

     config = {
         ConfigParams.VCAP_SERVICES: "credentials.cfg",
         #vcap_conf,
         ConfigParams.SERVICE_NAME: "Streaming-Analytics",
         ConfigParams.FORCE_REMOTE_BUILD: True
     }
     return config



def submit_to_service(topo, local, username="streamsadmin",pwd="passw0rd"):
    """
       Takes a Topology object and submits it to the Streaming analytics service
       Returns the SubmissionResult
    """
    # build config object to authenticate with the service
    if not local:
      cfg = get_config_object()
      submission_result = submit('STREAMING_ANALYTICS_SERVICE', topo, cfg)
    else:
      sc = StreamsConnection(username, pwd)
      sc.session.verify = False
      submission_result = submit('DISTRIBUTED',topo, {ConfigParams.STREAMS_CONNECTION:sc}, username,pwd)
    return submission_result


def get_cmd(tuple):
    #build the message you wish to send as a dictionary
    payload = {}

    payload["action"] = "Reboot"
    payload["msg"] = "Message to Edgent from Streams:\n Alert code: 249"

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
   local = sys.argv[1] == "local"


   #define needed variables
   COMMANDS_TOPIC = "streamsx/iot/device/commands/send" #topic to publish commands to
   EVENTS_TOPIC = "streamsx/iot/device/events" #topic to subscribe to for events
   incoming_schema =  schema.StreamSchema("tuple <rstring typeId, rstring deviceId, rstring eventId,rstring jsonString>")
   cmd_schema = schema.StreamSchema('tuple<rstring typeId, rstring deviceId, rstring cmdId, rstring jsonString>')


   topo = Topology('ReadingsFromIot')

   #Subscribe to  events
   events = topo.subscribe(EVENTS_TOPIC, incoming_schema,"AllEventsAsJSON")
   sensor_events = events.filter(lambda tuple: tuple["eventId"] == "sensors","SensorEventsAsJSON")
   readings = sensor_events.map(get_event_data,"ReadingsStream")
   readings.print()

   #send a command
   cmd_stream = sensor_events.map(get_cmd, "CommandsAsJSON")
   #convert the commands stream to a SPL structured schema
   commands_to_publish = cmd_stream.map(lambda x : (x["typeId"],x["deviceId"],x["cmdId"],x["jsonString"],), schema = cmd_schema, name="CommandsToPublish")

   commands_to_publish.publish(COMMANDS_TOPIC, cmd_schema)

   if local and len(sys.argv) > 2:
      username = sys.argv[2]
      password = sys.argv[3]
      result = submit_to_service(topo, local, username, password)
   else:
   	  result = submit_to_service(topo, local)

   print("Submitted job to the service, job id = " + str(result.job.id))



if __name__ == '__main__':
    num_args = len(sys.argv[1:])
    if num_args == 1 or num_args == 3:
       main()
    else:
       print("Usage: read_from_edgent.py bluemix | local [username password]")
