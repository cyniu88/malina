#!/bin/sh
echo "Start programu przez start.sh"

cp /etc/config/iDom_SERVER/iDom_server-CMAKE .
sleep 15
exec /app/iDom_server-CMAKE 