# IBM Confidential
# OCO Source Materials
# 
# (C) Copyright IBM Corp. 2011, 2013
# The source code for this program is not published or otherwise divested of its trade secrets, irrespective of what has been deposited with the U.S. Copyright Office.

package Configuration;

use strict;
use warnings;
use Carp;
use Cwd;
use File::Spec::Functions qw(canonpath rel2abs catfile) ;
use Net::Domain qw(hostdomain);
use Digest::MD5;
use File::Path;
use Data::Dumper;
#use File::Path qw(make_path);

sub new($;$)
{
  my ($class,$cwd,$cfgfile) = @_;
  $cwd or $cwd=Cwd::getcwd();
  $cfgfile or $cfgfile='config.cfg';
  my $url=canonpath(catfile($cwd,'config',$cfgfile)) ;

  my $hostDomain = undef ;

  my $num_core = 0;
  if (open(PROC,catfile('/proc','cpuinfo')))
  {
      while(<PROC>)
      {
          /^process/ and $num_core++;
      }
      close PROC ;
  }
  $num_core or $num_core++;

  my $db2home = undef ;

  open(IN,$url) or croak "cannot open $url:$!\n";
  my %cfg = (
    '*' => {
        'CWD'     => [ $cwd ],
        'NCORE'   => [ $num_core ],
     },
  );
  $cfg{'*'}->{'DB2HOME'} = [ $db2home ] if defined $db2home;

  my $dom = '*' ;
  my $def = undef ;
  my $md5 = Digest::MD5->new();
  my $domains = "";
  while(<IN>)
  {
      $md5->add($_);
      chomp ;
      /^\s*$/ || /^\s*#/ and next;
      #my @domMatch = (/^\s*domain\s+(\w+|\*){1}(?:\s+(default))?(?:\s+(host=)(\S+))?/i);
      my @domMatch = (/^\s*domain\s+(\w+|\*)(?:\s+(default)|\s+(host=)(\S+))*/ig);
      if (@domMatch)
      {
          $domains .= " " . $domMatch[0]; 
          $dom=$domMatch[0];
          for(my $i = 1; $i < @domMatch; ++$i)
          {
            if ( lc($domMatch[$i]) eq 'default' && !defined $def)
            {
                $def=$domMatch[0] ;
            }
            elsif (lc($domMatch[$i]) eq 'host=')
            {
                ++$i ;
                my $x = lc($domMatch[$i]);
                $x =~ s/([^*?0-9a-zA-Z_-])/\\$1/g ;
                $x =~ s/\*/[0-9a-zA-Z_-]\*/g ;
                $x =~ s/\?/\./g ;
                defined $hostDomain or $hostDomain = `hostname -d`;
                chomp($hostDomain);
                defined $hostDomain or $hostDomain=lc(hostdomain());
                $hostDomain = lc($hostDomain);
                $hostDomain=~/^$x$/ and $def=$domMatch[0];
            }
          }
          next;
      }
      my @kv = split (/\s*=\s*/);
      my @kr = map {
          $_ =~ s/\&(\w+)\&/($cfg{$dom}->{$1}&&"@{$cfg{$dom}->{$1}}")||($cfg{'*'}->{$1}&&"@{$cfg{'*'}->{$1}}")||$ENV{$1}||"\&$1\&"/ge;
          $_ =~ s/\&((\w+)\[(\d+)\.\.(\d+)\])\&/($cfg{$dom}->{$2}&&"@{$cfg{$dom}->{$2}}[$3..$4]")||($cfg{'*'}->{$2}&&"@{$cfg{'*'}->{$2}}[$3..$4]")||$ENV{$1}||"\&$1\&"/ge;
          $_ =~ s/\&((\w+)\[(\d+)\])\&/$cfg{$dom}->{$2}->[$3]||$cfg{'*'}->{$2}->[$3]||$ENV{$1}||"\&$1\&"/ge;
          $_ =~ s/\s*$//;
          $_;
      } split ',', $kv[1];
      $cfg{$dom}->{$kv[0]} = \@kr;
  }
  close(IN);
  $cfg{'*'}->{MD5} = $md5->hexdigest();
  $domains =~ s/^ //;
  $domains =~ s/\*/dummy/;
  my $self = {
    _url    => $url,
    _config => \%cfg,
    _igncse => 0,
    _domain => $ENV{'CONFIG_DOMAIN'}||$def,
    _domains => $domains,
  };
  bless $self, $class;
  return $self;
}

sub domain($;$)
{
    my ($self,$dom) = @_;
    $dom or return $self->{_domain};
    $self->{_igncse} and $dom = uc $dom;
    $self->{_domain} = $dom;
    return $self->{_domain};
}

sub get_domain()
{
    my ($self) = @_;
    return $self->{_domain};
}

sub get_domains()
{
    my ($self) = @_;
    return $self->{_domains};
}

sub ignore_case($)
{
    my ($self) = @_;
    return if $self->{_igncse};
    my %cfg = undef;
    while (my ($k,$v) = each %{$self->{_config}})
    {
        while (my ($kk,$vv) = each %{$v})
        {
            $cfg{uc $k}->{uc $kk} = $vv ;
        }
    }
    $self->{_config} = \%cfg ;
    $self->{_igncse} = 1 ;
    $self->{_domain} = uc $self->{_domain}
}

sub hasParam($$)
{
    my ($self,$param) = @_;
    my $res = undef;
    $res =  $self->{_config}->{$self->{_domain}}->{$param} if defined $self->{_domain};
    defined $res or $res = $self->{_config}->{'*'}->{$param};
    defined $res or return undef;
    return 1;
}

sub get($$;$$)
{
    return unless defined wantarray ;
    my ($self,$param,$a,$b) = @_;
    $self->{_igncse} and $param = uc $param ;
    my $name = $param ;
    defined $a or ($name,$a,$b) = ($param =~ m{^\s*(\w+)(?:\[\s*(\d+)\s*(?:\.\.\s*(\d+))?\s*\])?\s*$});
    defined $name or croak "invalid query '".$param."'\n";
    my $res = undef;
    $res =  $self->{_config}->{$self->{_domain}}->{$name} if defined $self->{_domain};
    defined $res or $res = $self->{_config}->{'*'}->{$name};
    defined $res or croak "undefined configuration parameter '".$name."'\n";
    if(defined $b)
    {
        $res = [@{$res}[$a .. $b]];
        ($_ or croak "range out of bound '".$param."'\n") for @{$res};
    }
    elsif(defined $a)
    {
        $res = $res->[$a];
        defined $res or croak "index out of bound '".$param."'\n";
    }
    return $res;
}

sub getv($$;$$)
{
    my $self = shift ;
    my $r = undef;
    $r = eval { $self->get(@_); };
    $r = [ $ENV{$_[0]} ] unless defined $r ;
    return $r ;
}

sub gets($$;$$)
{
    my $self = shift ;
    my $r = undef;
    $r = $self->get(@_);
    return "@{$r}" if ref $r eq 'ARRAY';
    return $r;
}

sub getsv($$;$$)
{
    my $self = shift ;
    my $r = undef;
    $r = eval { $self->gets(@_); } ;
    $r = $ENV{$_[0]} unless defined $r ;
    return $r ;
}

sub set($$$;$)
{
    my ($self,$param,$values,$dom) = @_;
    ref $values eq 'ARRAY' or croak "values to Configuration::set method must be an ARRAY reference.\n";
    $dom = $dom || $self->get_domain() || '*' ;
    $self->{_igncse} and $param = uc $param ;
    $self->{_config}->{$dom}->{$param} = $values ;
}

sub keys($;$)
{
    return unless defined wantarray ;
    my ($self,$grep) = @_;
    my @keys = undef;
    @keys = keys %{$self->{_config}->{'*'}} ;
    defined $self->{_domain} and push @keys, keys %{$self->{_config}->{$self->{_domain}}} ;
    $self->{_igncse} and $grep = uc $grep ;
    $grep and @keys = grep {/$grep/} @keys ;
    return sort { $a cmp $b } @keys ;
}

sub printCfgFile($$)
{
   my ($self,$inFile) = @_;
   open(DATA, '<', $inFile);
   print <DATA>;
   close (DATA);
}

sub getArgValue() {
	my ($self,$argsHashRef,$attrvalue) = @_;
	my @splitvalue = split (/=/, $attrvalue);
	$argsHashRef->{$splitvalue[0]}=$splitvalue[1];
}

sub getStreamsVersion() {
	my $cmd = "streamtool version |grep Version";
	my $result = `$cmd`;
	my $version = substr($result,8,1,);
	return $version;
}

sub getFileSinkCloseMode() {
	my $version = getStreamsVersion();
	my $fileSinkCloseMode = "dynamic";
	$fileSinkCloseMode = "name" if (2 == $version);
	return $fileSinkCloseMode;
}

###################################################################################################
sub convertToAbsolutePath($$$)
{
	my ($self,$home, $myJobName,$cfgpath) = @_;
	$myJobName =~ s/Streams//;
	if (("$cfgpath" !~ /^\//) && ("$cfgpath" !~ /^\$/))
	{
		if ($cfgpath) {
			if ($cfgpath =~ /^\.\.\//) { # upper directory
				while ($cfgpath =~ /^\.\.\//)
				{
					$cfgpath =~ s/^\.\.\///;
				}
				$cfgpath = "$home/".$cfgpath;
			}
			else { # local directory
				$cfgpath =~ s/^\.\/// if ($cfgpath);
				$cfgpath = $home."/$myJobName/".$cfgpath;
			}
		}
	}
	if ("$cfgpath" =~ /^\$/) {
		my $tmp = $cfgpath;
		$tmp = `ls -d $tmp`;
		chomp($tmp);
		$cfgpath = $tmp if ("" ne "$tmp")
	}
	$cfgpath =~ s/\/\//\//g;
	return "$cfgpath";
}
###################################################################################################
sub makePath($$)
{
	my ($self,$path) = @_;
	if (! -d $path) {
		File::Path::mkpath("$path");
	}
}

1
;
