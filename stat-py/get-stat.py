import time
from instagram.client import InstagramAPI

api = InstagramAPI(client_id='91d827904e3c452aab4936d9658b88fd', client_secret='980e61f2aa7949d5a83e599e5d628863')
f = open('out.csv', 'w')
f.write("latitude, longitude\n")

t = time.time()
step = 60*60
for i in range(0, 24*7) :
	min_time = t-(i+1)*step
	max_time = t-i*step
	print "from", min_time, time.ctime(min_time), "to", max_time, time.ctime(max_time)
	popular_media = api.media_search(55.751943, 37.617123, min_time , max_time)
	
	for media in popular_media:
		p = media.location.point
		print media.created_time, p.latitude, p.longitude 
		f.write(str(p.latitude)+", "+str(p.longitude)+"\n")
		#for property, value in vars(media.location.point).iteritems():
		#	print(str(property)+": "+str(value)+"\n")