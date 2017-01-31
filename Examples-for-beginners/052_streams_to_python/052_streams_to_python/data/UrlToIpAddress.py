# =============================================================
# This simple python script has two functions that perform 
# URL to IP Address conversion and then a simple regex match to
# get the company name from the given URL.
#
# For example, this script can be tested from a shell window
# as shown below.
#
# python UrlToIpAddress.py www.cnn.com
#
#
# First created on: Jan/18/2013
# Last modified on: Feb/12/2013
# =============================================================
import re, sys, socket

def getCompanyNameFromUrl(url):
    # Do a regex match to get just the company/business part in the URL.
    # Example: In "www.ibm.com", it will return "ibm".
    escapedUrl = re.escape(url)
    m = re.match(r'www\.(.*)\..{3}', url)
    x = m.group(1)
    return (x)

def getIpAddressFromUrl(url):
    # The following python API will Return a triple
    # (hostname, aliaslist, ipaddrlist)
    # hostname is the primary host name for the given URL
    # aliaslist is a (possibly empty) list of alternative host names for the same URL
    # ipaddrlist is a list of IPv4 addresses for the same interface on the same host
    #
    # aliaslist and ipaddrlist may have multiple values separated by
    # comma. We will remove such comma characters in those two lists.
    # Then, return back to the caller with the three comma separated
    # fields inside a string. This can be done using the Python 
    # list comprehension.
    return(",".join([str(i).replace(",", "") for i in socket.gethostbyname_ex(url)])) 

if ((__name__ == "__main__") and (len(sys.argv) >= 2)):
    url = sys.argv[1]
    # print("url=%s" % (url, ))
    print "IP address of %s=%s" % (url, getIpAddressFromUrl(url))
    print "Company name in the URL=%s" % repr(getCompanyNameFromUrl(url))
elif ((__name__ == "__main__") and (len(sys.argv) < 2)):
    sys.exit("Usage: python UrlToIpAddress.py www.watson.ibm.com")
