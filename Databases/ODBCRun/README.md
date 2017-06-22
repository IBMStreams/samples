The following document describes how to use the ODBCRun operator from Streams toolkit.db to:

- Connect to a database

- Create a table

- Insert some data into table

- Update the data in a table

- Select records from the table.


These software needs to be installed to run this demo application:
- IBM Streams 4.1.x.x
- unixODBC 2.3.4
- Database client for example DB2

**1- Installation of database Client**

   Download the latest ODBC Driver and database client for your database
   and install it
 

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

Adapt the odbcinst.ini and odbc.ini files in $UNIXODBC_HOME/etc/

The following sample is a configuration for DB2. 

odbcinst.ini
-------------------------------------------------------------------------------------------------------------
	[DB2]
	Description=DB2 driver for Linux
	Driver=/home/db2inst1/sqllib/lib64/libdb2o.so
	fileusage=1
	dontdlclose=1
	UsageCount=1

-------------------------------------------------------------------------------------------------------------

odbc.ini
-------------------------------------------------------------------------------------------------------------

	[ODBC]
	[TESTDB2]
	Description=DB2 database
	Driver=DB2
-------------------------------------------------------------------------------------------------------------

**4- Check the odbc configuration**

	isql -v your-db-name your-db-user your-db-password

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
	
	    <connection_specification name="DBConnection" >
	    <! -- change the database name, database user and password with your database credentials -->
				<ODBC database="your-database-name" user="your-db-user" password="your-db-password" />
	    </connection_specification>
	    
	  </connection_specifications>
	  
	  <access_specifications>
	
	    <access_specification name="PersonCreate">
	      <statement statement="CREATE TABLE ODBCRUNSAMPLE (id INTEGER , fname VARCHAR(15), lname VARCHAR(20), age INTEGER, gender CHAR(1), salary FLOAT)" transaction_batchsize="1" />
	      <uses_connection connection="DBConnection" />
	      <native_schema>
	        <column name="result" type="VARCHAR" length="200" />
	      </native_schema>
	    </access_specification>

       .....
		</access_specifications>
	</st:connections>
-------------------------------------------------------------------------------------------------------------

**6- Configuration of environment variables**

The following environment variables have to be set correctly in .bashrc file.

	export DB2HOME=/home/db2inst1/sqllib
	export UNIXODBC_HOME=/usr/local/unixODBC
	export ODBCINI=$UNIXODBC_HOME/etc/odbc.ini
	export PATH=$UNIXODBC_HOME/bin:$PATH 
	# Uncomment the variable pertaining to the DB you are using
	export STREAMS_ADAPTERS_ODBC_DB2=1
	#export STREAMS_ADAPTERS_ODBC_IDS=1
	#export STREAMS_ADAPTERS_ODBC_ORACLE=1
	#export STREAMS_ADAPTERS_ODBC_SQLSERVER=1
	#export STREAMS_ADAPTERS_ODBC_MYSQL=1
	#export STREAMS_ADAPTERS_ODBC_UNIX_OTHER=1
	
	# Uncomment and set these variables to the C++ include and library
	# file directories pertaining to the ODBC client you are using
	# In this sample the database is DB2
	export STREAMS_ADAPTERS_ODBC_LIBPATH=$DB2HOME/lib
	export STREAMS_ADAPTERS_ODBC_INCPATH=$DB2HOME/include
	


**7- Make Main.spl and start the SPL application**

This SPL application (Main.spl) use the database credentials in etc/connections.xml and in odbc.ini  
connect via ODBCRun to a database and create a table, insert some records into table and update these records.

To create the application you can use the **make** or you can import this SPL project in your **Streams Studio**.
