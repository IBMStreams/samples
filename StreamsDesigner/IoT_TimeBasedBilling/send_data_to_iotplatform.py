import ibmiotf.device

import time
from random import random
from datetime import datetime, timezone,timedelta

def connect():
    #Before we can send data, we need to authenticate using the credentials for the device we registered
    #Paste credentials here
    organization = "myorg" # Your organization, available from Top Right corner of IoT platform dashboard
    deviceId = "device_1" # id of device you registered
    authToken = "meter_1_token" #authentication token you chose
    deviceType="Smart_Meters" #device type

    options = {
        "org": organization,
        "type": deviceType,
        "id": deviceId,
        "auth-method": "token", # leave as is
        "auth-token": authToken,
        "clean-session": True
    }
    #Create the client object
    client = ibmiotf.device.Client(options)
    client.connect()
    return client


# Peak hours (after 9am and before 6pm) have higher usage, so this code just
# generates a higher reading if the current time is within peak hours

def compute_usage_by_time(curtime, high_user):

    if curtime.hour >= 9 and curtime.hour < 18:
        #at home user has more usage during day time hours
        if high_user :
            return random() * 40.75
        else:
            return  random() * 0.3
    elif curtime.hour < 5:
        return random() * 0.5
    else:
        return random() *0.3

# Generate a device id for one of the 100 simulated devices
def get_device_id(ct):
    if (ct % 10) == 0:
        return "iot_device_178"
    return "iot_device_1" + str(random()*100)[-2:]

#simple loop that sends  some simulated data for an hour (change the loop count for more time.)
def start(client):
    print("Sending data to Watson IoT Platform")

    for i in range(144600):
        current_time = datetime.now()
        d_id = get_device_id(i)

        normal_usage = d_id == "iot_device_178"

        reading ={'device_id' : d_id,
                 'serial_number':"XF438YQ",
                 'firmware_version':"2.1.5_20180114",
                 "usage_date":current_time.strftime("%Y-%m-%d %H:%M:%S")
        }

        reading["usage_in_kwH"] = compute_usage_by_time(current_time, normal_usage)

        #To generate data in different hours, we also send a reading in a different time zone PST
        #This gives more variety to the data without having to run for a long time
        reading_pst = dict(reading)
        pst_time = datetime.now(timezone(timedelta(hours=-9)))
        reading_pst["usage_period_end_time"] = pst_time.strftime("%Y-%m-%d %H:%M:%S")
        # reading_pst has a different reading value for that time of day

        reading_pst["usage_in_kwH"] = compute_usage_by_time(pst_time, normal_usage)

        time.sleep(0.25)

        #send the reading to the platform using the client api
        client.publishEvent(event="meter_readings", msgFormat="json", data=reading)
        client.publishEvent(event="meter_readings", msgFormat="json", data=reading_pst)
    print("Done")

client = connect()
start(client)
client.disconnect()
