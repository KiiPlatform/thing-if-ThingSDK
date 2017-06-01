#!/bin/sh

usage_exit() {
  echo "Usage: $0 [-c commandID] [-t thingID]" 1>&2
  exit 1
}

while getopts c:t:h OPT
do
  case $OPT in
    c) COMMAND_ID=$OPTARG
      ;;
    t) THING_ID=$OPTARG
      ;;
    h) usage_exit
      ;;
    \?) usage_exit
      ;;
  esac
done

curl -v -X GET \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/thing-if/apps/${APP_ID}/targets/THING:${THING_ID}/commands/${COMMAND_ID}"
