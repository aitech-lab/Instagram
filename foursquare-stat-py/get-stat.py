import time
import thread
import httplib
import simplejson as json

# -*- coding: utf-8 -*-

sectors = []
threadLock = thread.allocate_lock()

# MOSCOW BOUNDS
N = 55.92578046   
E = 37.33204993 
S = 55.55701887   
W = 37.89647254

step = 20.0;
dlng = (N - S) / step
dlat = (W - E) / step

f = open('out.csv', 'w')
f.write("latitude, longitude, checkins, id, name\n")

def drange(start, stop, step):
	r = start
	while r < stop:
		yield r
		r += step


keys1 = ["FPYUMI2QZ5LQHGCV3JR3BL0MLD04M4OTDKVQSFBL5EX5N0RH", 
         "KQW5NWN5QF1R0YU4JALI3S2TTPV50NZZHTPWLJXA2AIZO4US"] 
keys2 = ["QZRFEYV0YJXKFVOGQSWXPVKFBVKMEJCEACR1QP0X4R4QZTPA", 
         "FOUZ0GH5FWDYJTCZPE0BPYWQGRDO43Y1UBTFS1HYYA0MMG1T"] 
keys3 = ["EACQ5MTS1QUZ0GXUE2BFVVUKGLNHVIRHIPIJIYMCJOTQZ0QW", 
         "GT2HMTHP4ELPIF0QHCN5TJFDGTWAJFWJZY3ONJAQKQ55XDQ3"] 
keys4 = ["FQ1PZRBSHQK300SYVS21DHWQOGPRT2PVDUTRZBPK50LVZ2VE", 
         "TNIJVELGSU0DKQ4HWZNUSHB1N5TYNAAKTSZEN3PIZ2LS4ZKE"] 


for lat in drange(S, N, dlat) :
	for lng in drange (E, W, dlng) :
		sectors.append([lat, lng])


def foursquareThread(keys) :
	# DOC: https://developer.foursquare.com/docs/venues/search
	# https://api.foursquare.com/v2/venues/search?client_id=FPYUMI2QZ5LQHGCV3JR3BL0MLD04M4OTDKVQSFBL5EX5N0RH&client_secret=KQW5NWN5QF1R0YU4JALI3S2TTPV50NZZHTPWLJXA2AIZO4US&ll=55.727833,37.606804&intent=checkin
	# https://api.foursquare.com/v2/venues/search?client_id=FPYUMI2QZ5LQHGCV3JR3BL0MLD04M4OTDKVQSFBL5EX5N0RH&client_secret=KQW5NWN5QF1R0YU4JALI3S2TTPV50NZZHTPWLJXA2AIZO4US&v=20120601&limit=100&intent=browse&ne=55.9238935665,37.885192315&sw=55.952114697,37.9036303945
	
	while(len(sectors)):
		
		s = sectors.pop()
		
		lat = s[0]
		lng = s[1]
		conn = httplib.HTTPSConnection("api.foursquare.com")
		conn.set_debuglevel(3)
		conn.request("GET",          
			"/v2/venues/search" +
			"?client_id="       + str(keys[0]) + 
			"&client_secret="   + str(keys[1]) +	
			"&v=20120601"       +
			"&limit=100"        +
			"&intent=browse"    +
			"&ne="              +str(lat     )+ "," +str(lng     )+
			"&sw="              +str(lat+dlat)+ "," +str(lng+dlng))
		resp = conn.getresponse()
		print resp.status
		if(resp.status == 500) : break
		d = json.loads(resp.read())
		
		threadLock.acquire()
		for v in d["response"]["venues"]:
			print v["id"], v["stats"]["checkinsCount"] 
			f.write(
				str(v['location']['lat'])       +", "+
				str(v['location']['lng'])       +", "+
				str(v["stats"]["checkinsCount"])+", "+
				str(v['id'])                    +", "+
				    v['name'].encode('utf-8')   +"\n")
		threadLock.release()				

try:
	#foursquareThread(keys2)	
	thread.start_new_thread( foursquareThread, (keys1,))
	thread.start_new_thread( foursquareThread, (keys2,))
	thread.start_new_thread( foursquareThread, (keys3,))
	thread.start_new_thread( foursquareThread, (keys4,))
except:
	print "Error: unable to start thread"
while 1 :
	pass	
	
	