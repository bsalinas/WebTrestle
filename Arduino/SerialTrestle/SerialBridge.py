import serial, urllib
#replace this with your serial device.
# ser = serial.Serial('/dev/tty.usbmodemfd141', 9600)


ser = serial.Serial('/dev/tty.usbserial-A800I2RW', 9600)
siteStartChar = 0x04
siteEndChar = 0x05
postStartChar = 0x06
postEndChar = 0x07
dataStartChar = 0x08
dataEndChar = 0x03
postData = ""
postRequest = ""
site = ""
def readValue(endChar):
	tmp = ""
	while 1:
		val = ser.read()
		if ord(val) == endChar:
			return tmp
		tmp += val
		#print tmp
def post(s, r, d):
	f = urllib.urlopen(s+r, d)
	resp = f.read()
	print resp;
	ser.write(resp)

while 1:
	val = ser.read()
	if ord(val) == siteStartChar:
		site = ""
		postRequest = ""
		postData = ""
		site = readValue(siteEndChar)
		print 'postSite=', site
	elif ord(val) == postStartChar:
		postRequest = ""
		postRequest = readValue(postEndChar)
		print 'postRequest=', postRequest
	elif ord(val) == dataStartChar:
		postData = ""
		postData = readValue(dataEndChar)
		print 'postData=', postData
		post(site, postRequest, postData)
	else:
		print val
