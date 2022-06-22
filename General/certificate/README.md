# Add Signed Certificate to IBM Streams
This document describes how to create a signed certificate and add it to the IBM Streams.

## System requirements

A installed and running IBM Streams on RedHat linux

Procedure
Please complete the following steps on your RedHat IBM Streams Server.

## openssl
Login as root in your Streams server und check if is **openssll** package already installed.

```
which openssl
/usr/local/bin/openssl
```

If not install openssl via yum
```
yum install openssl
```


## Create an OpenSSL client configuration file

Change user to streamsadmin ann create a directory for your certificates.

```
su – streamsadmin
mkdir $HOME/ca-certificates
cd $HOME/ca-certificates
## create an OpenSSL client configuration file
vi myca.cnf
```

Edit myca.cnf and adapt your countryName, stateOrProvinceName, localityName and ... in openssl configuration file in section req_distinguished_name.

```
[ req ]
prompt                 = no
days                   = 365
distinguished_name     = req_distinguished_name
req_extensions         = v3_req


[ req_distinguished_name ]
countryName            = US
stateOrProvinceName    = Florida
localityName           = Miami
organizationName       = MyCompany
organizationalUnitName = MyOrgUnit
commonName             = mydomain.com
emailAddress           = emailaddress@mmycompany.com

[ v3_req ]
basicConstraints       = CA:false
extendedKeyUsage       = serverAuth
subjectAltName         = @sans

[ sans ]
DNS.0 = localhost
DNS.1 = mydomain.com
```
**days** = days for expiration date for self signed certificate.

**distinguished_name** A section to use as a set of name value pairs for user. 

**countryName** = The Country Name is 2-letter code for example US for USA and DE for Germany.

**basicConstraints** = This is a multi valued extension which indicates whether a certificate is a CA certificate. The first (mandatory) name is CA followed by TRUE or FALSE. If CA is TRUE then an optional pathlen name followed by an non-negative value can be included.

**Extended Key Usage** = This extensions consists of a list of usages indicating purposes for which the certificate public key can be used for.

**subjectAltName** = The subject alternative name extension allows various literal values to be included in the configuration file. These include email (an email address) URI a uniform resource indicator, DNS (a DNS domain name), RID (a registered ID: OBJECT IDENTIFIER), IP (an IP address), dirName (a distinguished name) and otherName.

For more details about openssl configuration please refer to:

https://www.ibm.com/docs/en/hpvs/1.2.x?topic=reference-openssl-configuration-examples
and


## Create certificate file
**Self-Signed Certificate**: A file that contains a public key and identifies who owns that key and its corresponding private key.

**CA-Signed Certificate**: A certificate authority (CA) electronically signs a certificate to affirm that a public key belongs to the owner named in the certificate. Someone receiving a signed certificate can verify that the signature does belong to the CA, and determine whether anyone tampered with the certificate after the CA signed it.

Now you can create a certificate.
```
openssl req -config $HOME/ca-certificates/myca.cnf -newkey rsa:4096  -x509  -sha256  -nodes -out streams.crt -keyout streams.key
```
Description of openssl req command
-config     OpenSll configuration file
rsa:4096    bit lengths of certificate
If you want extra security you could increase the bit lengths.
-x509       Output a x509 structure instead of a cert request (Required by some CA's)
-sha256     Certificate Request with SHA256 Signature
For more details about openssl req:

```
openssl req -help
```
                     
Check if the file streams.crt is created.
```
cat $HOME/ca-certificates/streams.crt
-----BEGIN CERTIFICATE-----
MIIFqTCCA5ECFEwOOc+E1QaGj8k0BAUQVYZCQTREMA0GCSqGSIb3DQEBCwUAMIGQ
MQswCQYDVQQGEwJVUzELMAkGA1UECAwCRkwxDjAMBgNVBAcMBU1pYW1pMQ4wDAYD
....
....
c7j52odc9oDLVGZBKw/leEDrlA/cCivH8nFDfDr6UT3UQF7xqjnioUiNWvT3+jPO
vKqrtV23MiP84OlaVw==
-----END CERTIFICATE-----
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
streamtool getdomainproperty -a | grep sws
sws.clientAuthenticationCertificateRequired=true
```



