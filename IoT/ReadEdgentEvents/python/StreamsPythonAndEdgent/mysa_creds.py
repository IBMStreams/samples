import json
from streamsx.topology.context import *


# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2017

def build_streams_config(service_name, credentials):
     vcap_conf = {
         'streaming-analytics': [
             {
                 'name': service_name,
                 'credentials': credentials,
             }
         ]
     }
     config = {
         ConfigParams.VCAP_SERVICES: vcap_conf,
         ConfigParams.SERVICE_NAME: service_name,
         ConfigParams.FORCE_REMOTE_BUILD: True
     }
     return config

def get_config_object():
    #Paste the credentials you get from the dashboard here

    service_name="service name" #Change this to your service name
   
    credentials = {}
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
    
    streams_conf = build_streams_config(service_name, credentials)
    
    return streams_conf


