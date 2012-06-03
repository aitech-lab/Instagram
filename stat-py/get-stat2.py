import httplib
import simplejson as json

client_id = "91d827904e3c452aab4936d9658b88fd"
lat       = "55.751943"
lng       = "37.617123"
distance  = "5000"

conn = httplib.HTTPSConnection("api.instagram.com")
conn.request("GET", "/v1/media/search?distance="+distance+"&client_id="+client_id+"&lat="+lat+"&lng="+lng)
resp = conn.getresponse()

print resp.status, resp.reason
d = json.loads(resp.read())
for i in d['data']: 
	print i['location']['latitude'], i['location']['longitude'] 



