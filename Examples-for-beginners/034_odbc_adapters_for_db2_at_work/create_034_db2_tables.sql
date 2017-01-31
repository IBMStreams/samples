-- =========================================================================================
-- Nov/10/2012, Saturday
-- =====================
-- This SQL file creates the tables required for testing an SPL example that 
-- shows the odbc adapters connectivity to DB2 tables.
--
-- If you are using the IBM internal use only "RHEL+Streams" VM, then DB2 Express-C V10.1 is
-- already installed and configured for you in the VM.
--
-- If you are running this example on your own Linux servers, then you have to find out
-- from your system administrators about where DB2 Express-C v10.1 is installed and which machine
-- you can use a DB2 client.
-- 
-- Following are the details for using DB2 Express-C v10.1 installed on the 
-- virtual machine (with 64 bit RHEL and Streams) distributed for IBM internal use.
--
-- node:           streams
-- port:           50000
-- DB2 instance:   db2inst1
-- password:       db2expr1
--
-- If you are running this example inside the "RHEL + Streams" VM distributed for IBM internal use,
-- then everything is already configured for you to use the Streams ODBC operators with DB2.
--
-- If you are trying to run this example on your own server, then you have to work with your
-- system administrator and install and configure DB2.
--
-- Whether it is the RHEL VM (for IBM internal use) or your own Linux server, the following are the
-- general steps you have to follow.
--
-- 1) Ensure that from your Linux userid, you can run the following commands inside a terminal window.
--    If these commands give meaningful messages, then you have access to a correctly installed DB2 instance.
--   
--    db2 "get dbm cfg"
--    db2 "list db directory" [If there is no database created in your DB2, the error you may get for this command is just fine.]
--
-- 2) By logging in as the DB2 instance user, you can create a new database called "MYDB". After creating
--    the database, you can log out of the DB2 instance userid and return back to your regular userid.
--    In a properly configured DB2 environment, regular userids typically source the profile of
--    the DB2 instance user (e-g: source ~db2inst1/sqllib/db2profile)
--    (In the IBM internal use only VM, DB2 instance userid and password are as shown at the top of this file.)
--
-- 3) Finally, substitute the correct values for <MYDB>, <USERID>, and <PASSWD>.
--    Then, run this command to create the tables required to test this SPL example project.
--    db2  -tvf ./create_034_db2_tables.sql
-- =========================================================================================
-- Let us first create the Order table.
--

-- Substitute the correct values for <MYDB>, <USERID>, and <PASSWD>.
connect to <MYDB> user <USERID> using <PASSWD>;
DROP TABLE Order;
DROP TABLE OrderResults;

create table Order (OrderId integer, CustomerName varchar(50), CustomerId integer, ProductName varchar(50), ProductId integer, Price varchar(10));
describe table order;

insert into order values(34567, 'John Price', 453234, 'iPod Nano', 53234223, '231.56');
insert into order values(633623, 'Joe Hansen', 64334, 'Samsung Droid', 8324233, '183.25');
insert into order values(6576323, 'Mary Keller', 934213, 'Dell Inspiron', 4534234, '389.46');
insert into order values(7344234, 'Peter Parker', 234126, 'iomega USB Hard Drive', 1355234, '89.47');
insert into order values(3452454, 'Jeff Boros', 4352344, 'Lexmark Laser Printer', 3452346, '102.56');
insert into order values(4563456, 'Mark Zeltman', 3452361, 'Tennis Equipment', 262345, '289.52');
insert into order values(5463454, 'Carly Ferona', 453433, 'Beauty Set', 345232, '154.34');
insert into order values(84564534, 'Wendy Valkerno', 341534, 'Party Dress', 435224, '187.34');
insert into order values(56564567, 'Matt Troutin', 6743236, 'Apple iPad', 2343443, '574.72');
insert into order values(64536345, 'David Kerner', 7345743, 'Tax Software', 3423462, '33.78');

select count(*) from order;

create table OrderResults (OrderId integer, CustomerName varchar(50), CustomerId integer, ProductName varchar(50), ProductId integer, Price varchar(10), ShipmentStatus integer, ShippingDate varchar(50));

-- ======================================================
-- After writing the application that uses the ODBC adapter operators (ODBCSource, ODBCAppend, and ODBCEnrich), set the
-- following environment variables on the terminal in order to compile the application correctly.
-- [In the IBM internal use only RHEL VM, this step is already performed inside the .bashrc file.]
--
-- export STREAMS_ADAPTERS_ODBC_DB2=1
-- export STREAMS_ADAPTERS_ODBC_INCPATH=/opt/ibm/db2/V10.1/include/
-- export STREAMS_ADAPTERS_ODBC_LIBPATH=/opt/ibm/db2/V10.1/lib64/
--
-- Now, you can run the SPL application (034_odbc_adaters_at_work).
-- It is a simple application that puts all the three ODBC operators to use.
-- Read the SPL file in this project to learn more details about running this application.
--
--
-- After running the application, ensure that all the rows in the OrderResults table are deleted. 
-- Otherwise, the table will keep growing whenever the application is run. 
-- You can give the following command in a DB2 CLP to delete all the rows in the OrderResults table..
--
-- delete from OrderResults
-- ======================================================
