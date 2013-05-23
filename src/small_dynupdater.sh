#!/bin/bash

# My Dynupdater Script; twodns is the dyndns provider. 
# To do: Failure checks at file handling


IP=0.0.0.0

if [ -f ../lastip ]
then
    while read line    
    do    
        IP=${line}
    done <../lastip
    
else
    echo the file does not exist
    touch ../lastip
fi


DYNIP_ADDR=$(expr "$(curl -qs http://checkip.twodns.de)" : '.*[^0-9]\([0-9]*\.[0-9]*\.[0-9]*\.[0-9]*\)')

if [ $DYNIP_ADDR != $IP ]
then

curl -k -v -X PUT -u "YOUREMAIL:YOURAPI-TOKEN"\
  -d '{"ip_address": "auto"}' https://api.twodns.de/hosts/YOURDYNIP.de

echo  "$DYNIP_ADDR"  > ../lastip

if [ IP == 0.0.0.0 ] 
then
echo "[+] Unable to get new IP!"
fi

date +"%c"
echo "[+] Success!"

else

date +"%c"
echo "[+] Didn't have to change the IP!"

fi

