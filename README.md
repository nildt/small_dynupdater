#small_dynupdater

## General 
small_dynupdater is a small tool to update your ip to two-dns, a free dynamic dns provider.

## How
It uses the [libcur](http://curl.haxx.se/libcurl/ "libcurl") library and the Two Dns [API](https://twodns.de/de/api "API")
to change your IP if it has changed after a certain time. To check your current IP it uses http://checkip.twodns.de/. 

## Intention
* Being client at [twodns.de](http://twodns.de/ "twodns") I had of course to update my ip
* First I used a small bashscript with curl commands; but I tried to do the same with a c-program
* Look at the *old* small_dynupdater.sh

## Making
1.	Just set your username and password (the API-token) in the small_dynupdater.h file. 
2.	Adjust the time the program will check your IP
3. 	make install will build the program in the bin folder. 


## To do
*	Use *always SSL* (power on the ssl check)
*	Documentation
*	Licence
*	Improve IP extraction :P
*	Add some funktions out of the two dns API e.g. to show all hosts... (get request)

## Debug
:	Run make debug to create a verbose version in ./bin/
	


 

