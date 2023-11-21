import pygatt
import time
import struct
import mysql.connector
from datetime import datetime

device_name = "Group15"

custom_char_uuid= "1526-1212-efde-1523-785feabcd123"
custom_sevice_uuid= "00001526-1212-efde-1523-785feabcd123"

adapter = pygatt.GATTToolBackend()

def import_data(sensor_data):
	try:
		connection = mysql.connector.connect(
			host = "172.20.241.9",
			user = "dbaccess_rw",
			password = "fasdjkf2389vw2c3k234vk2f3",
			database = "measurements")
		cursor = connection.cursor()
		sql_query = "INSERT INTO rawdata (timestamp, groupid, from_mac, to_mac, sensorvalue_a, sensorvalue_b, sensorvalue_c, sensorvalue_d, sensorvalue_e, sensorvalue_f) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
		timestamp = datetime.now()
		cursor.execute(sql_query, (timestamp, 15, "nrf5340", "raspi", 
			sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], 0, "fuck IT with a stick" ))
		connection.commit()
	except mysql.connector.Error as error:
		print(f"Error: {error}")
	finally:
		if connection.is_connected():
			cursor.close()
			connection.close()

def handle_discover(handle, value):
	print (handle)
	print (value)
	iQty_of_Values = len(value)/2
	h = "h" * int(iQty_of_Values)
	ivalues = struct.unpack("<iiii", value)
	
	print (ivalues)
	import_data(ivalues)

try:
	adapter.start()

	
	devices_scan = adapter.scan(run_as_root=True, timeout=9)	
	devices = next((device for device in devices_scan if device["name"] == device_name), None)
	print (devices)
	if not devices:
		print("no devises")
	else:
		device = adapter.connect(devices['address'], address_type=pygatt.BLEAddressType.random, timeout=12)
		while True:
			device.subscribe(custom_sevice_uuid, callback=handle_discover)
			
finally:
	adapter.stop()			

