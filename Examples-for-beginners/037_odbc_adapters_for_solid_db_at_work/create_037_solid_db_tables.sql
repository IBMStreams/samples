-- =========================================================================================
-- This SQL file creates the tables required for testing an SPL example that 
-- shows the odbc adapters connectivity to the SolidDB tables.
--
-- If you are using the IBM internal use only "RHEL+Streams" VM, then Solid-DB 7.0 is
-- already installed and configured for you in the VM.
--
-- If you are running this example on your own Linux servers, then you have to find out
-- from your system administrators about where Solid-DB 7.0 is installed and which machine
-- you can use a Solid-DB client.
-- 
-- Before you can create the tables, it is necessary to start the SolidDB server.
-- Following instructions show you how to start and stop the SolidDB server on the
-- IBM internal use only RHEL VM. You can use a similar procedure, if you use your own Linux server.
-- ======
-- Starting and stopping the SolidDB server in Linux:
--
-- Starting SolidDB:
--
-- 1) su root
--
-- 2) export SOLIDDIR=/opt/solidDB/soliddb-7.0/full_kit
--   (Set SOLIDDIR to the directory, where the SolidDB license file is present.)
--
-- 3) cd $SOLIDDIR
--
-- 4) Run this command to start Solid-DB server: 
--    solid -f
--
--    If you are prompted to create a database, press y and then press Enter.
--    For the System Catalog, enter mydb
--    For user name, enter solid
--    For password, enter solid
--
-- 5) Press CTRL+Z to suspend the process and then enter bg to force the process to the background.
--
-- 6) In a virtual machine, it may take a few minutes for the server to start.
--
-- 7) Logout as root and return back to the regular userid (streamsadmin).
--
--
-- Stopping SolidDB:
--
-- 1) su root
--
-- 2) solsql "tcp 2315" solid solid
-- 
-- 3) In the solsql shell, enter this command:
--    admin command 'shutdown force';
--
-- 4) Enter exit; to leave from the solsql shell.
--
-- 5) Logout as root and return back to the regular userid (streamsadmin).
-- ======
--
-- You can run the following command (from within the IBM internal use only VM or
-- from your own Solid-DB client node if you are using your own Linux infrastructure).
-- 
-- If you are using the IBM internal use only VM, give this command.
-- solsql "tcp localhost 2315" solid solid create_037_solid_db_tables.sql
-- 
-- If you are using your own Linux server, give this command with proper values for the arguments.
-- solsql "tcp YOUR_SOLID_DB_SERVER_NAME YOUR_SOLID_DB_PORT" USERID PASSWD create_037_solid_db_tables.sql
--
-- First created on: Mar/11/2011
-- Last modified on: April/30/2012
-- =========================================================================================
-- Let us first create the Order table.
--
DROP TABLE Order;
COMMIT WORK;

create table Order (OrderId bigint, CustomerName varchar(50), CustomerId bigint, ProductName varchar(50), ProductId bigint, Price varchar(10), Primary Key(OrderId)) STORE MEMORY;

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
COMMIT WORK;

-- Let us create an empty OrderResults table now.
DROP TABLE OrderResults;
COMMIT WORK;

create table OrderResults (OrderId bigint, CustomerName varchar(50), CustomerId bigint, ProductName varchar(50), ProductId bigint, Price varchar(10), ShipmentStatus bigint, ShippingDate varchar(50)) STORE MEMORY;
COMMIT WORK;

-- ======================================================
-- It is required to create the following files in your home directory before you can run the
-- 037_odbc_adapters_for_solid_db_at_work SPL example.
-- Please continue with the instructions specified at the top of the Main.spl file about creating these
-- files.
-- 
-- a) ~/.odbcinst.ini
-- b) ~/.odbc.ini
-- c) ~/solid.ini
-- d) ~/solid-db-odbc.sh
-- ======================================================
-- After running the application, ensure that all the rows in the OrderResults table are deleted.
-- Otherwise, the table will keep growing whenever the application is run.
-- You can give the following command in a Solid DB solsql shell to delete all the rows in the OrderResults table..

-- delete from OrderResults
-- ======================================================

exit;

