import time
import thread
import httplib
import simplejson as json

# lat 00.034576416 lng 00.06354856  

client_id = "91d827904e3c452aab4936d9658b88fd"
distance  = "5000"

f = open('out.csv', 'w')
f.write("timestamp, id, latitude, longitude\n")
t = round(time.time()/60)*60
step = 60

threadLock = thread.allocate_lock()

def grabPiceOfInstagram(lat, lng, thr) :
	c=0
	while (c<100) : 
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

		print resp.status, resp.reason, thr
		print time.ctime(min_timestamp), ">>>", time.ctime(max_timestamp), thr

		d = json.loads(resp.read())
		for i in d['data']: 
			print time.ctime(int(i['created_time'])), i['id'], i['location']['latitude'], i['location']['longitude'], thr 
			threadLock.acquire()
			f.write(
				str(time.ctime(int(i['created_time'])))+", "+
				str(i['id']                           )+", "+
				str(i['location']['latitude']         )+", "+
				str(i['location']['longitude']        )+"\n")
			threadLock.release()
		c+=1

try:
	thread.start_new_thread( grabPiceOfInstagram, ("55.751943", "37.617123", "thr1" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("57.751943", "39.617123", "thr2" ) )
except:
	print "Error: unable to start thread"
while 1 :
	pass