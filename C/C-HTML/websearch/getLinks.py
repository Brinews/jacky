from BeautifulSoup import BeautifulSoup
import sys
import re
import socket
import errno

doc = sys.stdin.read()
soup = BeautifulSoup(doc)

# this version excludes anchor links, but includes relative links
#links = soup.findAll('a', href=re.compile("^[^#]"))

# this version only includes absolute http addresses
links = soup.findAll('a', href=re.compile("^http://"))


try:
    print len(links)

    for l in links:
        linkAddresses = [attr[1] for attr in l.attrs if attr[0] == u'href']

        for addr in linkAddresses:
            print addr

    # We close these in the "try" block to avoid
    #   broken pipe errors when the program quits
    sys.stdout.close()
    sys.stderr.close()
    sys.stdin.close()

except socket.error, e:
    # A socket error: that's okay
    x=7;
except IOError, e:
    if e.errno == errno.EPIPE:
        x=7;
    else:
        print "IOError"
