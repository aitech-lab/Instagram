import time
import thread
import httplib
import simplejson as json

# -*- coding: utf-8 -*-

# Moscow   
# NE 55.92578046   37.33204993 
#    -0.37421069    0.57128906
# SW 55.55701887   37.89647254

def main () :

	N = 55.92578046   
	E = 37.33204993 
	S = 55.55701887   
	W = 37.89647254

	step = 20.0;
	dlng = (N - S) / step
	dlat = (W - E) / step

	ll            = "55.727833,37.606804"
	client_id     = "FPYUMI2QZ5LQHGCV3JR3BL0MLD04M4OTDKVQSFBL5EX5N0RH"
	client_secret = "KQW5NWN5QF1R0YU4JALI3S2TTPV50NZZHTPWLJXA2AIZO4US"

	f = open('out.csv', 'w')
	f.write("latitude, longitude, checkins, id, name\n")

	for lat in drange(S, N, dlat) :
		for lng in drange (E, W, dlng) :
			print lat, lng

			# DOC: https://developer.foursquare.com/docs/venues/search
			#https://api.foursquare.com/v2/venues/search?client_id=FPYUMI2QZ5LQHGCV3JR3BL0MLD04M4OTDKVQSFBL5EX5N0RH&client_secret=KQW5NWN5QF1R0YU4JALI3S2TTPV50NZZHTPWLJXA2AIZO4US&ll=55.727833,37.606804&intent=checkin
			conn = httplib.HTTPSConnection("api.foursquare.com")
			conn.set_debuglevel(3)
			conn.request("GET",          
				"/v2/venues/search" +
				"?client_id="       + client_id     + 
				"&client_secret="   + client_secret +	
				"&v=20120601"       +
				"&limit=100"        +
				"&intent=browse"    +
				"&ne="              +str(lat     )+ "," +str(lng     )+
				"&sw="              +str(lat+dlat)+ "," +str(lng+dlng))

			resp = conn.getresponse()
			d = json.loads(resp.read())

			for v in d["response"]["venues"]:
				print v["id"], v["stats"]["checkinsCount"] 
				f.write(
					str(v['location']['lat'])       +", "+
					str(v['location']['lng'])       +", "+
					str(v["stats"]["checkinsCount"])+", "+
					str(v['id'])                    +", "+
					    v['name'].encode('utf-8')   +"\n")
					

def drange(start, stop, step):
	r = start
	while r < stop:
		yield r
		r += step

main()