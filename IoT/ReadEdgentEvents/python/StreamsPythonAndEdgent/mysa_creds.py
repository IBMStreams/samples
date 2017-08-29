import json
from streamsx.topology.context import *


# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2017

def get_sa_credentials():
    #Paste the credentials you got from the dashboard here
    #credentials = {}

    credentials =  { "password": "d44351fe-f4e4-46bd-b7e9-2aef1330243e",
      "rest_port": "443",
      "bundles_path": "/jax-rs/bundles/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64",
      "resources_path": "/jax-rs/resources/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64",
      "stop_path": "/jax-rs/streams/stop/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64",
      "rest_host": "streams-app-service.ng.bluemix.net",
      "size_path": "/jax-rs/streams/size/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64",
      "jobs_path": "/jax-rs/jobs/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64",
      "start_path": "/jax-rs/streams/start/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64",
      "rest_url": "https://streams-app-service.ng.bluemix.net",
      "userid": "2eedcfc9-8abf-41a5-9e89-a53a25fa5768",
      "status_path": "/jax-rs/streams/status/service_instances/f734936a-520b-48d8-8a3c-b5d334199979/service_bindings/73be4481-16aa-4ea6-a9cf-4e9e241e4e64"
    }
    """ #should look like this:
      credentials = {  "password": "212358pp8",
      "rest_port": "443",
      "bundles_path": "/jax-rs/bundles/service_instances/abcdef",
      "resources_path": "/jax-rs/resources/service_instances/abcdef",
      "stop_path": "/jax-rs/streams/stop/service_instances/",
      "rest_host": "streams-app-service.ng.bluemix.net",
      "size_path": "/jax-rs/streams/size/abcdef",
      "jobs_path": "/jax-rs/j....",
      "start_path": "/jax-rs/strea......22f",
      "rest_url": "https://streams-app-service.ng.bluemix.net",
      "userid": "abcdef",
      "status_path": "xxxxxx41cc22f"
      }
    """
    return json.dumps(credentials)

def get_service_name():
    return "sa_nd" #Change this to your Streaming Analytics service name..so in our screenshot above it would be "test1"

def get_config_object():
    _creds = get_sa_credentials()
    service_name = get_service_name()
    vs={'streaming-analytics': [{'name': service_name, 'credentials': json.loads (_creds)}]}
    cfg = {}
    cfg[ConfigParams.VCAP_SERVICES] = vs
    cfg[ConfigParams.SERVICE_NAME] = service_name

    return cfg
