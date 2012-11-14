import psycopg2
import time, sys
import httplib
import simplejson as json
import ConfigParser,  StringIO

class SimpleConfig(ConfigParser.ConfigParser):
    def read(self, filename):
        try:
            text = open(filename).read()
        except IOError:
            pass
        else:
            file = StringIO.StringIO("[config]\n" + text)
            self.readfp(file, filename)

cfg = SimpleConfig()
cfg.read("database")

DB_HOST = cfg.get("config","DB_HOST")
DB_NAME = cfg.get("config","DB_NAME")
DB_USER = cfg.get("config","DB_USER")
DB_PASS = cfg.get("config","DB_PASSWORD")
DB_PORT = cfg.get("config","DB_PORT", 5432)

try:
    pgcon = psycopg2.connect("dbname='%s' user='%s' host='%s' password='%s' port='%s'" % (DB_NAME, DB_USER, DB_HOST, DB_PASS, DB_PORT))
except:
    print "I am unable to connect to the database"
    sys.exit()

pgcur = pgcon.cursor()

def grabPiceOfInstagram(lat, lng, client_id) :
    t         = round(time.time()/60)*60
    distance  = "50000"
    step      = 60*6
    c=0
    while (c<288) : # 144 = 1 day step 10 min 
        min_timestamp = t-(c+1)*step
        max_timestamp = t- c   *step

        conn = httplib.HTTPSConnection("api.instagram.com")
        url = "/v1/media/search?distance=%s&client_id=%s&lat=%s&lng=%s&min_timestamp=%s&max_timestamp=%s" % (distance, client_id, lat, lng, str(int(min_timestamp)), str(int(max_timestamp)))

        conn.request("GET",  url)        

        resp = conn.getresponse()

        print resp.status, resp.reason
        print time.ctime(min_timestamp), ">>>", time.ctime(max_timestamp)
        
        if resp.status == 200 :
            print "https://api.instagram.com%s" % url
            d = json.loads(resp.read())
            for i in d['data']: 
                print time.ctime(int(i['created_time'])), i['id'], i['location']['latitude'], i['location']['longitude']
                #print i
                SQL = "INSERT INTO insta(location,json,unixtime) VALUES(point(%s,%s),%s,%s)"
                DATA = (i['location']['latitude'],i['location']['longitude'], str(i), i['created_time'],)
                print SQL % DATA
                pgcur.execute(SQL, DATA)
            if len(d['data']) :
                pgcon.commit()
            c+=1
        else :
            print "https://api.instagram.com%s" % url
            time.sleep(1)


id1 = "6f3a4f4174774f5da4380a481299a995"
#https://maps.google.com/maps?hl=en&ll=55.747792,37.630234&spn=0.104926,0.338173&t=m&z=12
grabPiceOfInstagram("55.747792", "37.630234",id1 ) 
