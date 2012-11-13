import time
import thread
import httplib
import simplejson as json
import sys

if len(sys.argv)<= 1 : 
    startTime = time.time() - 60*60*24*7
else : 
    startTime = float(sys.argv[1])

if len(sys.argv) <= 2 : 
    endTime = startTime + 60*60*24*7
else : 
    endTime = float(sys.argv[2])

id1 = "6f3a4f4174774f5da4380a481299a995"
id2 = "aff8d45d9e3644839000588e5ac4778b"
id3 = "ed5e6ed4df0349e584dcae51a46e42c5"
id4 = "5a7e84c2f0ad4082a2cdfcecaec1e180"
id5 = "dd341ab36cf04f36af9849113b6d1365"

# MOSCOW BOUNDS
N = 55.92578046
E = 37.33204993
S = 55.55701887
W = 37.89647254

# x -> 37.0 + 0.080 (5km)
# y -> 55.0 + 0.045 (5km)
dlat = 0.02000
dlng = 0.01125

def drange(start, stop, step):
    r = start
    while r < stop:
        yield r
        r += step

sectors = []
threadLock = thread.allocate_lock()

for lat in drange(S, N, dlng) :
    for lng in drange (E, W, dlat) :
        sectors.append([lat, lng])

# 5km radius
distance  = "1250"

f = open('posts.'+str(int(startTime))+'-'+str(int(endTime))+'.csv', 'w')
f.write("id, timestamp, latitude, longitude, url \n")
f.flush();
t = round(time.time()/60)*60
timeStep = 60*60

def grabPiceOfInstagram(client_id) :
    global sectors
    while(len(sectors)):
        s = sectors.pop()
        threadLock.acquire()  
        print "Sector", s
        threadLock.release()

        for timestamp in drange(startTime, endTime, timeStep) :
            for trycounter in range(0, 5):
                try:
                    conn = httplib.HTTPSConnection("api.instagram.com")
                    conn.request("GET",          
                        "/v1/media/search"       
                        "?distance="     +distance               +
                        "&client_id="    +client_id              +
                        "&lat="          +str(s[0])              +
                        "&lng="          +str(s[1])              +
                        "&min_timestamp="+str(int(timestamp))+
                        "&max_timestamp="+str(int(timestamp+timeStep)))
                    resp = conn.getresponse()
                except Exception, e:
                    time.sleep(60)
                    print "try ", trycounter
                    continue
           
                if (resp.status != 200) :
                    print resp.status
                    if(resp.status == 503) : 
                        time.sleep(3600)
                        print "try ", trycounter
                        continue
                    if(resp.status == 400) :
                        time.sleep(60)
                        print "try ", trycounter
                        continue
                break;

            d = json.loads(resp.read())
            threadLock.acquire()  
            c=0
            for i in d['data']:
                c+=1 
                f.write(
                    str(i['id']                        )+", "+
                    str(int(i['created_time'])         )+", "+
                    str(i['location']['latitude'  ]    )+", "+
                    str(i['location']['longitude']     )+", "+
                    str(i['images']['thumbnail']['url'])+"\n")
            f.flush()
            print c, time.ctime(int(timestamp)), timeStep

            threadLock.release()
    print "exit thread"
    
try:
    thread.start_new_thread( grabPiceOfInstagram, (id1,) )
    thread.start_new_thread( grabPiceOfInstagram, (id2,) )
    thread.start_new_thread( grabPiceOfInstagram, (id3,) )
    thread.start_new_thread( grabPiceOfInstagram, (id4,) )
    thread.start_new_thread( grabPiceOfInstagram, (id5,) )
    
except:
    print "Error: unable to start thread"
while 1 :
    pass