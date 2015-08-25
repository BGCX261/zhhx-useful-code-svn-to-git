#!/usr/bin/perl -w
#
# connect_multi.pl
# Chapter 2
# Listing 2
use strict;
use DBI;
my $dbh_m = 
       DBI->connect("dBi:mysql:test",
       "root", 
       "123456") 
     or die("Cannot connect: $DBI::errstr\n");
my $sql = qq(select * from pet);
my $sth_m = $dbh_m->prepare($sql);

$sth_m->execute;
my $p;
while($p = $sth_m->fetch){
   print "@$p\n";
}
print "\n";

$dbh_m->disconnect;
