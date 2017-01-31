#!/usr/bin/perl -w

###################################################################################################
#  Copyright (C) 2013-2014, International Business Machines Corporation  
#  All Rights Reserved                                	                 
#  Edit configuration parameters for DbLoader config.cfg file.
# 
#  If you get the error "Can't locate Term/ReadKey.pm" 
#  you need to install the Term::ReadKey module
#  yum install perl-TermReadKey.x86_64
###################################################################################################

use warnings;
use Term::ANSIColor;
use Term::ANSIColor qw(:constants);
use Term::ReadKey;
$Term::ANSIColor::AUTORESET = 1;

START:

executeCmd("cat config.cfg");
 
print color 'blue';
print ("***************************************************************************\n");	
print ("**                                                                       **\n");
print ("** DbLoader configuration                                                **\n");
print ("** This program help you to edit the DbLoader configuration parameters.  **\n");
print ("** The Text above shows the current configuration.                       **\n");
print ("** At all times it is possible to exit the program by entering \"quit\" .**\n");
print ("***************************************************************************\n\n");	
print color 'reset';

printColor('blue' , "Enter \"quit\" to exit.\nPress Enter to start\n");
my $input= "";

chomp($input = <STDIN>);
checkInput($input);

#choiceDatabaseType();
setParameter("DATABASETYPE");
setParameter("DBHOST");
setParameter("DBNAME");
setParameter("DBUSER");
setParameter("DBPWD");
setParameter("INPUT_DIR");
setParameter("PATTERN");
setParameter("LOG_DIR");
setParameter("ARCHIVE_DIR");
setParameter("ERROR_DIR");
setParameter("CONTROLFILES_DIR");
setParameter("PARALLEL_LOADERS");
checkDatabaseConnection();

## subroutines
###################################################################################################
sub checkInput
{	
	my $input = $_[0];
	my $ret = 0;
	if((uc $input) eq "QUIT" )
	{
		exit;
	}
	
	if ($input =~ m/[^a-zA-Z0-9\/\\\*.:,-_]/)
	{
		printColor('red', "The input is not valid.\n");
	}
	else
	{
		$ret = 1;
	}	
	return $ret;
}

###################################################################################################
sub printColor
{
	my $coulor = $_[0];
	my $txt = $_[1];
	print color $coulor;	
	print( "$txt" );
 	print color 'reset';
}

###################################################################################################
sub choiceDatabaseType
{
	my $invalidInput = 1;
	my $input = "";
	my $configName = "DATABASETYPE";
	my $currentValue = get_config($configName);
	my $question = "Please choose the database type!";

	while($invalidInput) 
	{ 
		printColor('blue', "Please select the database vendor!\n1 DB2 \n2 Oracle \n3 Netezza\n4 Informix\n");
		print "$question [$currentValue] :";
		chomp($input = <STDIN>);
		if(checkInput($input))
		{
		$input = uc $input;
		# trim remove white space from both ends of a string:
		$input =~ s/^\s+|\s+$//g;
	
		if($input eq "" ) 
		{
			$input = $currentValue;
		}
		 
		if(($input eq "1") or($input eq "DB2" )) {
			change_config($configName, "DB2");
			$invalidInput = 0;
		} elsif(($input eq "2") or($input eq "ORACLE" )) {
			change_config($configName, "ORACLE");
			$invalidInput = 0;
		} elsif(($input eq "3") or($input eq "NETEZZA" )) {
			change_config($configName, "NETEZZA");
			$invalidInput = 0;
		} elsif(($input eq "4") or($input eq "INFORMIX" )) {
			change_config($configName, "INFORMIX");
			$invalidInput = 0;
		} else {
			printColor('red', "Your answer  $input  is wrong!\n");
			print ("Please enter 1 or 2 or 3 or 4\n");
		}	
		}
	}
	printColor('blue', "$configName=".get_config($configName)."\n");

}

###################################################################################################
sub executeCmd1
{
	my $cmd = $_[0];
	printColor('blue' ,"$cmd\n");
	my $result = `$cmd`;
	$result = "resssssult. ="."$result";	
	printColor('blue' ,"$result\n");
	return $result;
}

###################################################################################################
sub executeCmd
{
	my $cmd = $_[0];
	my $results = "";
	printColor('blue' ,"$cmd\n");
	my $result = `$cmd 2>&1`;
	printColor('blue' ,"$result\n");
	return $result;
}


###################################################################################################
sub pingIp
{
	my $ip = $_[0];
	my $ret = 0;
	my $result = executeCmd("ping -c 2 $ip");
	
	if (index($result, "0 received") != -1) 
	{
		printColor('red' ,"$ip is unreachable \n$result");
	} 
	else
	{
		printColor('green' ,"$ip\n$result");
		$ret = 1;
	}
	return $ret;
}

###################################################################################################
sub validIp
{
	my $ip = $_[0];
	my $ret = 1;

	if (length($ip) > 60)
	{
		printColor('red', "Hostname or IP address is too long! \n");		
		$ret = 0;
		return $ret;		
	}

	if ($ip =~ m/[^0-9.]/)
	{
	}
	else
	{
		if ( $ip =~ /(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})(:(\d{1,5}))?/ )
		{
			if( $1 > 255 || $2 > 255 || $3 > 255 || $4 > 255)
			{
				$ret = 0;
				printColor('red', "Invalid IP address! \n");
			}
			else
			{
				$ret = 1;
				printColor('green', "Valid IP\n");
			}
		}
	
	}	
	return $ret;
}


###################################################################################################
sub getComment
{
	my $configName = $_[0];
	my $comment = "";
	if($configName eq "DATABASETYPE") 
	{
		$comment = "1 DB2 \n2 Oracle \n3 Netezza \n4 Informix \nPlease select one of above database vendors";
	} 
	elsif($configName eq "DBHOST") 
	{
		$comment = "Please enter the database server IP address";
	}
	elsif($configName eq "DBNAME") 
	{
		$comment = "Please enter the database name";
	}
	elsif($configName eq "DBUSER") 
	{
		$comment = "Please enter the database user name";
	}
	elsif($configName eq "DBPWD") 
	{
		$comment = "Please enter the database password";
	}
	elsif($configName eq "PATTERN") 
	{
		$comment = "Please enter the pattern for input files";
	}
	elsif($configName eq "INPUT_DIR") 
	{
		$comment = "Please enter the path of input directory";
	}
	elsif($configName eq "ARCHIVE_DIR") 
	{
		$comment = "Please enter the path of archive directory";
	}
	elsif($configName eq "ERROR_DIR") 
	{
		$comment = "Please enter the path of error directory";
	}
	elsif($configName eq "LOG_DIR") 
	{
		$comment = "Please enter the path of log directory";
	}
	elsif($configName eq "CONTROLFILES_DIR") 
	{
		$comment = "Please enter the path of control files directory";
	}
	elsif($configName eq "PARALLEL_LOADERS") 
	{
		$comment = "Please enter a number between 1 and 10 for paralell loaders";
	}

	else
	{
		$comment = "Please enter the value of $configName:";
	}
 
}

###################################################################################################
sub setParameter
{
	my $configName = $_[0];
	my $invalidInput = 1;
	my $input = "";
	my $currentValue = get_config($configName);
	my $comment = getComment($configName);

	while($invalidInput) 
	{ 
		print ("\n---------------------------------------------------------------------\n\n");
		print("$comment [$currentValue] :");
		chomp($input = <STDIN>);
		# trim remove white space from both ends of a string:
		$input =~ s/^\s+|\s+$//g;
		if((uc $input) eq "QUIT" )
		{
			exit;
		}
		
		if($input eq "" ) 
		{
			$input = $currentValue;
		}

		if(checkInput($input)) 
		{
			if (index($configName, "DIR") != -1) 
			{
				if (checkDirectory($input))
				{
					$invalidInput = 0;
				}
				else
				{
					$invalidInput = 1;
				}
			
			}
			elsif (index($configName, "DATABASETYPE") != -1) 
			{
				if(($input eq "1") or($input eq "DB2" )) {
					$input="DB2";
					$invalidInput = 0;
				} elsif(($input eq "2") or($input eq "ORACLE" )) {
					$input="ORACLE";
					$invalidInput = 0;
				} elsif(($input eq "3") or($input eq "NETEZZA" )) {
					$input="NETEZZA";
					$invalidInput = 0;
				} elsif(($input eq "4") or($input eq "INFORMIX" )) {
					$input="INFORMIX";
					$invalidInput = 0;
				} else {
					printColor('red', "Your answer  $input  is wrong!\n");
					print ("Please enter 1 or 2 or 3 \n");
				
				}
			}
			elsif (index($configName, "PARALLEL_LOADERS") != -1) 
			{
				if ($input =~ m/[^0-9]/)
				{
					$invalidInput = 1;
					printColor('red', "Please enter a number between 1 and 10\n" );
				}
				elsif (($input < 1) or ($input > 10))
				{
					$invalidInput = 1;
					printColor('red', "Please enter a number between 1 and 10\n" );
				}
				else
				{
					$invalidInput = 0;
				}
			
			}
			elsif (index($configName, "DBHOST") != -1) 
			{
				if(validIp($input)) 
				{
					$invalidInput = 0;
				} 
				else 
				{
					printColor('red',"Your answer  $input  is wrong!\n");
					printColor ('red' , "Please enter a valid IP address or a valid hostname\n");
				}	

			}


			else
			{
				$invalidInput = 0;			
			}
		 
		}
	}		
	
	change_config($configName, $input);
	printColor('blue', "$configName=".get_config($configName)."\n");
	$ret = 1;
	return $ret;
}

###################################################################################################
sub change_config
{
	my $fileName = 'config.cfg';
	my $configName = $_[0];
	my $configValue = $_[1];
	my $line;
	my $foundConfigName  =  0;


	open(FILE,$fileName) || die("Cannot Open File");
	my(@fcont) = <FILE>;
	close FILE;
	
	open(FOUT,">$fileName") || die("Cannot Open File");
	foreach $line (@fcont) 
	{
		if ($line =~ m/$configName/) 
		{
			my $position = index($line, "=");
			my $configName2 = substr($line, 0, $position);	
			if ($configName2 eq  $configName)
			{
				$line="$configName=$configValue\n";
				# printColor('blue', "Config parameter $line has been changed in $fileName");
				$foundConfigName = 1;
			}
		}
		print FOUT $line;
	}
	if ($foundConfigName == 0)
	{
		$line="$configName=$configValue\n";
		print FOUT $line;
	}	
	close FOUT;
}


###################################################################################################
sub get_config
{
	my $fileName = 'config.cfg';
	my $configName = $_[0];
	my $configValue = "";
	my $line;
	my $foundConfigName = 0;
	
	open(FILE,$fileName) || die("Cannot Open File");
	my(@fcont) = <FILE>;
	foreach $line (@fcont) 
	{
		if ($line =~ m/$configName/) 
		{
			my $position = index($line, "=");
			my $configName2 = substr($line, 0, $position);	
			if ($configName2 eq  $configName)
			{
				$configValue= substr($line,$position+1);	
				$foundConfigName = 1;
			}
		}
	}
	close FILE;
	chomp($configValue);
	#print ("get_config2: $configName = $configValue\n");
	return ($configValue);
}

###################################################################################################
sub checkDirectory
{
	my $dir = $_[0];
	my $ret = 0;
	if (-d $dir) 
	{
		printColor('green',  "The directory $dir exists\n");
		$ret = 1;
	}
	elsif (-e "cgi-bin") 
	{
		printColor('red',  "$dir exists but is not a directory\n");
	}
	else 
	{
		printColor('red',  "$dir does not exist\n");
	}
	return $ret;
}

###################################################################################################
sub checkDatabaseConnection
{
	my $databaseType = get_config(DATABASETYPE);
	my $dbHost = get_config(DBHOST);
	my $dbName = get_config(DBNAME);
	my $dbUser = get_config(DBUSER);
	my $dbPwd = get_config(DBPWD);	
	print ("\n---------------------------------------------------------------------\n\n");
	print ("Current database settings: $databaseType host=$dbHost db name=$dbName user=$dbUser password=$dbPwd \n"); 
	pingIp($dbHost);
	
	my $connectionCmd = "";
	my $result = "";
	my $dbReady = 0;
	
	$databaseType = uc $databaseType;
	if ( $databaseType eq "DB2")
	{
		$connectionCmd = "db2 connect to $dbName user $dbUser using $dbPwd" ;
		$result = executeCmd($connectionCmd);
		if (index($result, "authorization") != -1) 
		{
			$dbReady = 1;
			executeCmd("db2 terminate");
		}	
	}

	if ( $databaseType eq "NETEZZA")
	{
		$connectionCmd = "nzsql -host $dbHost -d $dbName -u $dbUser -pw $dbPwd -l ";
		$result = executeCmd($connectionCmd);
		if (index($result, $dbName) != -1) 
		{
			$dbReady = 1;
		}	
	}

	if ( $databaseType eq "ORACLE")
	{
		$connectionCmd = "echo 'select status from v\$instance;' | sqlplus -s $dbUser/$dbPwd\@$dbName";
		$result = executeCmd($connectionCmd);
		if (index($result, "OPEN") != -1) 
		{
			$dbReady = 1;
		}	
	}			


	if ( $databaseType eq "INFORMIX")
	{
		$connectionCmd = "echo 'connect to \"$dbName\" user \"$dbUser\" using \"$dbPwd\";'> informix-connect.sql";
		$result = executeCmd($connectionCmd);
		
		$connectionCmd = "dbaccess - informix-connect.sql";
		$result = executeCmd($connectionCmd);

		if (index($result, "Connected") != -1) 
		{
			$dbReady = 1;
		}	
		$connectionCmd = "rm informix-connect.sql";
		$result = executeCmd($connectionCmd);
	}



	if ($dbReady)
		{
			printColor('green', "The $databaseType database $dbName is available! \n ");
		}	
		else
		{
			printColor('red', "The $databaseType database $dbName is not available!\n ");				
			printColor('red', "Please check the database name, database user and database password \n ");				
		}
	
}
###################################################################################################


