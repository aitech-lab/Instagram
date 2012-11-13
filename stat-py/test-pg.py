import psycopg2
import time
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

try:
    conn = psycopg2.connect("dbname='%s' user='%s' host='%s' password='%s'" % (DB_NAME, DB_USER, DB_HOST, DB_PASS))
except:
    print "I am unable to connect to the database"

cur = conn.cursor()

def grabPiceOfInstagram(lat, lng, client_id) :
    t         = round(time.time()/60)*60
    distance  = "5000"
    step      = 60*6
    c=0
    while (c<288) : # 144 = 1 day step 10 min 
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
        
        if resp.status == 200 :
            d = json.loads(resp.read())
            for i in d['data']: 
                print time.ctime(int(i['created_time'])), i['id'], i['location']['latitude'], i['location']['longitude']
                
                print(
                    str(i['location']['latitude'  ])+", "+
                    str(i['location']['longitude'] )+", "+
                    str(int(i['created_time'])     )+", "+
                    str(i['id']                    )+"\n"
                )
            c+=1
        else :
            time.sleep(1)


id1 = "6f3a4f4174774f5da4380a481299a995"
grabPiceOfInstagram("55.7173666", "37.5535744",id1 ) 
