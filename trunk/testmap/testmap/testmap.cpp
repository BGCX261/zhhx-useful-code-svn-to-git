// testmap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <map>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	map<int,int> test;
	map<int,int>::iterator it;

	test.insert( pair<int,int>(1,10) );

	it = test.begin();

	test.insert( pair<int,int>(10,100) );

	test.insert( pair<int,int>(5,50) );

	for( ; it != test.end(); it++ )
	{
		cout<<"map key:"<<it->first<<", value:"<<it->second<<endl; 
	}

	it = test.begin();

	it++;
	cout<<"map key:"<<it->first<<", value:"<<it->second<<endl; 

	test.insert(pair<int,int>(8, 80));	

	it++;

	cout<<"map key:"<<it->first<<", value:"<<it->second<<endl; 

	test.insert(pair<int,int>(2, 20));

	it++;
	cout<<"map key:"<<it->first<<", value:"<<it->second<<endl; 

	test.insert(pair<int,int>(7, 70));
	test.insert(pair<int,int>(6, 60));
	test.insert(pair<int,int>(9, 90));
	test.insert(pair<int,int>(3, 30));

	for( ; it != test.end(); it++ )
	{
		cout<<"map key:"<<it->first<<", value:"<<it->second<<endl; 
	}

	int a;
	cin>>a;

	return 0;
}

