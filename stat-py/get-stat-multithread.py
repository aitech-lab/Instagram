import time
import thread
import httplib
import simplejson as json

# lat 00.034576416 lng 00.06354856  

id1 = "6f3a4f4174774f5da4380a481299a995"
id2 = "aff8d45d9e3644839000588e5ac4778b"
id3 = "ed5e6ed4df0349e584dcae51a46e42c5"
id4 = "5a7e84c2f0ad4082a2cdfcecaec1e180"
id5 = "dd341ab36cf04f36af9849113b6d1365"

distance  = "5000"

f = open('out.csv', 'w')
f.write("latitude, longitude,timestamp, id\n")
t = round(time.time()/60)*60
step = 60*5

threadLock = thread.allocate_lock()

def grabPiceOfInstagram(lat, lng, client_id, thr) :
	c=0
	while (c<288) : # 144 = 1 day step 10 min 
		min_timestamp = t-(c+1)*step
		max_timestamp = t- c   *step

		conn = httplib.HTTPSConnection("api.instagram.com")
		conn.request("GET",          
			"/v1/media/search"       
			"?distance="	 +distance               +
			"&client_id="    +client_id              +
			"&lat="    	     +lat                    +           
			"&lng="          +lng                    +           
			"&min_timestamp="+str(int(min_timestamp))+
			"&max_timestamp="+str(int(max_timestamp)))
		resp = conn.getresponse()

		print resp.status, resp.reason, thr
		print time.ctime(min_timestamp), ">>>", time.ctime(max_timestamp), thr

		d = json.loads(resp.read())
		threadLock.acquire()  
		for i in d['data']: 
			print time.ctime(int(i['created_time'])), i['id'], i['location']['latitude'], i['location']['longitude'], thr 
			
			f.write(
				str(i['location']['latitude'  ])+", "+
				str(i['location']['longitude'] )+", "+
				str(int(i['created_time'])     )+", "+
				str(i['id']                    )+"\n"
			)
		threadLock.release()
		c+=1

try:
	thread.start_new_thread( grabPiceOfInstagram, ("55.7173666", "37.5535744",id1, "thr1" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("55.7519430", "37.5535744",id1, "thr2" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("55.7865194", "37.5535744",id1, "thr3" ) )
	
	thread.start_new_thread( grabPiceOfInstagram, ("55.7173666", "37.6171230",id2, "thr4" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("55.7519430", "37.6171230",id2, "thr5" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("55.7865194", "37.6171230",id2, "thr1" ) )
	
	thread.start_new_thread( grabPiceOfInstagram, ("55.7173666", "37.6806716",id3, "thr2" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("55.7519430", "37.6806716",id3, "thr3" ) )
	thread.start_new_thread( grabPiceOfInstagram, ("55.7865194", "37.6806716",id3, "thr4" ) )
except:
	print "Error: unable to start thread"
while 1 :
	pass