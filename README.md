#small_dynupdater
------------------

## General 
small_dynupdater is a small tool to update your ip to two-dns, a free dynamic dns provider.
 
## Intention
* Being client at [twodns.de](http://twodns.de/ "twodns") I had of course to update my ip
* First I used a small bashscript with curl commands; but I tried to do the same with a c-programm

## making
1.	Just set your username and password (the API-token) in the small_dynupdater.h file. 
2.	Adjust the time the program will check your IP
3. 	make install will build the program in the bin folder. 


## To do
*	Use always SSL (power on the ssl check)
*	Documentation
*	Licence

## Debug
	:	Run make debug to create a verbose version in ./bin/
	


 

