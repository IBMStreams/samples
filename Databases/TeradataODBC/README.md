The following document describes how to connect to a Teradata database via Streams database toolkit (com.ibm.streams.db)


These software needs to be installed to run this demo application:
- IBM Streams 4.1.x.x
- unixODBC 2.3.4
- Teradata client 16.00

**1- Installation of Teradata Client**

   Download the latest Teradata ODBC Driver for Linux from:
   
    http://downloads.teradata.com/download/connectivity/odbc-driver/linux

Login as root and perform the following commands to install the Teradata client.

	tar –xzvf tdodbc1600__linux_indep.16.00.00.03-1.tar.gz 
	cd tdicu1600
	rpm -ivh tdicu1600-16.00.00.01-1.noarch.rpm
	rpm -ivh TeraGSS_linux_x64-16.00.00.05-1.noarch.rpm
	rpm -ivh tdodbc1600-16.00.00.03-1.noarch.rpm

It installs all Teradata client packages in /opt/teradata/client/16.00


**2- Installation and configuration of unixODBC**

Download the unixODBC software unixODBC-2.3.2.tar.gz from unixodbc.org:  

    http://www.unixodbc.org/unixODBC-2.3.4.tar.gz

Login as root and perform the following commands to install the unixODBC.

	tar xzvf unixODBC-2.3.4.tar.gz
	mkdir /usr/local/unixODBC
	chmod +r /usr/local/unixODBC
	cd unixODBC-2.3.4
	./configure --prefix=/usr/local/unixODBC
	make
	make install
	cd /usr/local/unixODBC/lib
	ln -s libodbcinst.so.2.0.0 libodbcinst.so.1
	

**3- ODBC configuration for Teradata**

Adapt the odbcinst.ini and odbc.ini files in /opt/teradata/client/16.00/odbc_64/

odbcinst.ini
-------------------------------------------------------------------------------------------------------------
	[ODBC DRIVERS]
	Teradata Database ODBC Driver 16.00=Installed

	[Teradata Database ODBC Driver 16.00]
	Driver=/opt/teradata/client/16.00/odbc_64/lib/tdata.so
	APILevel=CORE
	ConnectFunctions=YYY
	DriverODBCVer=3.51
	SQLLevel=1
-------------------------------------------------------------------------------------------------------------

odbc.ini
-------------------------------------------------------------------------------------------------------------

	[ODBC]
	InstallDir=/opt/teradata/client/16.00/odbc_64
	Trace=0
	TraceDll=/opt/teradata/client/16.00/odbc_64/lib/odbctrac.so
	TraceFile=/tmp/odbc-trace.log
	TraceAutoStop=0

	[ODBC Data Sources]
	TeraDsn=tdata.so

	[TeraDsn]
	Driver=/opt/teradata/client/16.00/odbc_64/lib/tdata.so
	Description=Teradata database
	DBCName=your-teradabase-ip-address
	LastUser=
	Username=
	Password=
	Database=streamstest
	DefaultDatabase=
-------------------------------------------------------------------------------------------------------------

**4- Check the odbc configuration**

	isql -v TeraDsn your-db-user your-db-password

	+---------------------------------------+
	| Connected!                            |
	|                                       |
	| sql-statement                         |
	| help [tablename]                      |
	| quit                                  |
	|                                       |
	+---------------------------------------+
	SQL> 

If you get Connected your configuration is correct. 

**5- Configuration of connection document etc/connections.xml**

The connections.xml contains database configuration information that is required to access the external data source.
More details in:

https://www.ibm.com/support/knowledgecenter/SSCRJU_4.2.0/com.ibm.streams.toolkits.doc/spldoc/dita/tk$com.ibm.streams.db/op$com.ibm.streams.db$ODBCSource.html#spldoc_operator__parameter__connectionDocument

**etc/connections.xml**
-------------------------------------------------------------------------------------------------------------
	<st:connections xmlns:st="http://www.ibm.com/xmlns/prod/streams/adapters"
		xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
		<connection_specifications>
			<connection_specification name="dbConnection">
				<ODBC database="TeraDsn" user="your-db-user" password="your-db-password" />
			</connection_specification>
		</connection_specifications>

		<access_specifications>
			<!-- This access specification is used by the ODBCSource operator in the application. -->
			<access_specification name="SelectAll">
				<query query="SELECT TableName FROM dbc.tables" />
				<parameters></parameters>
				<uses_connection connection="dbConnection" />
				<native_schema>
					<column name="TableName" type="CHAR" length="20"/>
				</native_schema>
			</access_specification>
		</access_specifications>
	</st:connections>
-------------------------------------------------------------------------------------------------------------

**6- Configuration of environment variables**

The following environment variables have to be set correctly in .bashrc file.

	export TERA_HOME=/opt/teradata/client/16.00
	export UNIXODBC_HOME=/usr/local/unixODBC
	export PATH=$UNIXODBC_HOME/bin:$PATH 
	export STREAMS_ADAPTERS_ODBC_UNIX_OTHER=true
	export STREAMS_ADAPTERS_ODBC_LIBPATH=$TERA_HOME/lib
	export STREAMS_ADAPTERS_ODBC_INCPATH=$TERA_HOME/include
	export ODBCINI=$TERA_HOME/odbc_64/odbc.ini


**7- Make main.spl and start the SPL application**

The following SPL application (Main.spl) use the database credentials in etc/connections.xml and in odbc.ini  
connect via ODBCSource to a Teradata database and list table names of all available tables in database.  
(SELECT TableName FROM dbc.tables)

To create the application you can use the make or you can import this SPL project in your Streams Studio.

Main.spl
-------------------------------------------------------------------------------------------------------------
    namespace application;
    use com.ibm.streams.db::ODBCSource ;
 
    /** 
    * The Main composite is a sample application to read all table names from a Teradada database.
    *  
    */
    composite Main
    {
	    ////////////////////////////////////////////////                    
	    // Read from DB
	    ////////////////////////////////////////////////                    
	    stream<rstring TableName> myODBCstream = ODBCSource() {
	      param
		connectionDocument : "./etc/connections.xml" ;
		connection : "dbConnection";
		access     : "SelectAll";
		initDelay  : 3.0;
	    }


	    ////////////////////////////////////////////////                    
	    // Write to a file
	    ////////////////////////////////////////////////                    
	    () as mySink = FileSink(myODBCstream) {

		logic
		state :
		{
			mutable int64 counter = 0;
		}

		onTuple myODBCstream :
		{
			printStringLn((rstring)counter++ + " TableName = " +(rstring) TableName) ;
		}
		param
			file         : "output.dat";
			format       :  csv;
			quoteStrings : false;
		}
    }
-------------------------------------------------------------------------------------------------------------


