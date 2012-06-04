import time
import httplib
import simplejson as json

client_id = "91d827904e3c452aab4936d9658b88fd"
lat       = "55.751943"
lng       = "37.617123"
distance  = "5000"

f = open('out.csv', 'w')
f.write("timestamp, id, latitude, longitude\n")

t = round(time.time()/60)*60
step = 60
for c in range(0, 60*24*7): 

	min_timestamp = t-(c+1)*step
	max_timestamp = t- c   *step

	conn = httplib.HTTPSConnection("api.instagram.com")
	conn.request("GET",          
		"/v1/media/search"       
		"?distance="     +distance               +
		"&client_id="    +client_id              +
		"&lat="          +lat                    +           
		"&lng="          +lng                    +           
		"&min_timestamp="+str(int(min_timestamp))+
		"&max_timestamp="+str(int(max_timestamp)))
	resp = conn.getresponse()

	print resp.status, resp.reason
	print time.ctime(min_timestamp), ">>>", time.ctime(max_timestamp)

	d = json.loads(resp.read())
	for i in d['data']: 
		print time.ctime(int(i['created_time'])), i['id'], i['location']['latitude'], i['location']['longitude'] 
		f.write(
			str(time.ctime(int(i['created_time'])))+", "+
			str(i['id']                           )+", "+
			str(i['location']['latitude']         )+", "+
			str(i['location']['longitude']        )+"\n")


