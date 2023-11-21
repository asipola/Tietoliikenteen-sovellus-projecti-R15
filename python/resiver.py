import pygatt
import time
import struct

device_name = "Group15"

custom_char_uuid= "1526-1212-efde-1523-785feabcd123"
custom_sevice_uuid= "00001526-1212-efde-1523-785feabcd123"

adapter = pygatt.GATTToolBackend()

def handle_discover(handle, value):
	print (handle)
	print (value)
	iQty_of_Values = len(value)/2
	h = "h" * int(iQty_of_Values)
	ivalues = struct.unpack("<iiii", value)
	
	print (ivalues)

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

