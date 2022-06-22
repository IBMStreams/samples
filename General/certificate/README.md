# Add Signed Certificate to IBM Streams
This document describes how to create a signed certificate and add it to the IBM Streams.

System requirements

Linux RedHat
A installed and running IBM Streams

Procedure
Complete the following steps on your RedHat Streams Server.

## openssl
Login as root in your Streams server
Check if installed openssl
check if is openssll already installed.
```
which openssl
/usr/local/bin/openssl
```
If not install openssl via yum

```
yum install openssl

```
Change user to streamsadmin
```
su – streamsadmin
```

## Create an OpenSSL client configuration file
```
mkdir $HOME/ ca-certificates

cd $HOME/ ca-certificates

vi myca.cnf
```

Edit myca.cnf and put your countryName, countryName, localityName ... in openssl configuration file

```
[ req ]
prompt                 = no
days                   = 365
distinguished_name     = req_distinguished_name
req_extensions         = v3_req


[ req_distinguished_name ]
countryName            = US
countryName    = FL
localityName           = Miami
organizationName       = MyOrg
organizationalUnitName = MyOrgUnit
commonName             = mycommname.com
emailAddress           = emailaddress@myemail.com

[ v3_req ]
basicConstraints       = CA:false
extendedKeyUsage       = serverAuth
subjectAltName         = @sans

[ sans ]
DNS.0 = localhost
DNS.1 = myexampleserver.com

```

For more details about openssl configuration please refer to:

https://www.ibm.com/docs/en/hpvs/1.2.x?topic=reference-openssl-configuration-examples

## Create certificate file
Now you can create a certificate.
```
openssl req -config $HOME/ca-certificates/myca.cnf -newkey rsa:4096  -x509  -sha256  -nodes -out streams.crt -keyout streams.key
```
Check if the file streams.crt is created.
```
ls $HOME/ca-certificates/streams.crt
/home/streamsadmin/ca-certificates/streams.crt

```
## Add certificate to IBM Streams

The streamtool addcertificate command adds a client certificate into the web management service (SWS) truststore or the HTTP server 

For more details about add a certificate to IBM Streams please refer to:

https://www.ibm.com/docs/en/streams/4.3.0?topic=commands-streamtool-addcertificate


```
streamtool addcertificate -f $HOME/ca-certificates/streams.crt --clientid streams-ca
Trusted client certificate for streams-ca imported successfully for domain StreamsDomain.
```

## Check domain property
```
getdomainproperty -a | grep cer
```



