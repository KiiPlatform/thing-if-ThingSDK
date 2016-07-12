#!/bin/sh

usage_exit() {
  echo "Usage: $0 [-u username] [-p password] [-t thing-password]" 1>&2
  return 1 2>/dev/null || exit 1
}

while getopts u:p:t:h OPT
do
  case $OPT in
    u) USERNAME=$OPTARG
      ;;
    p) PASSWORD=$OPTARG
      ;;
    t) THING_PASSWORD=$OPTARG
      ;;
    h) usage_exit
      ;;
    \?) usage_exit
      ;;
  esac
done

if [ -z "$USERNAME" ]; then
  echo No username specified
  usage_exit
fi
if [ -z "$PASSWORD" ]; then
  echo No password specified
  usage_exit
fi
if [ -z "$THING_PASSWORD" ]; then
  echo No thing password specified
  usage_exit
fi

curl -v -X POST \
  -H "content-type: application/json" \
  -H "x-kii-appid: ${APP_ID}" \
  -H "x-kii-appkey: ${APP_KEY}" \
  "https://${APP_HOST}/api/oauth2/token" \
  -d "{\"username\" : \"${USERNAME}\", \"password\" : \"${PASSWORD}\"}" | tee .result.txt

# extract USER_ID and ACCESS_TOKEN
export ACCESS_TOKEN=`grep 'access_token' .result.txt | sed -e 's/^ *\"access_token\" *: *\"\([a-zA-Z0-9_-]*\)\",/\1/'`
export USER_ID=`grep 'id' .result.txt | sed -e 's/^ *\"id\" *: *\"\([a-zA-Z0-9_-]*\)\",/\1/'`

rm .result.txt

curl -v -X POST \
  -H "content-type: application/vnd.kii.onboardingWithThingIDByOwner+json" \
  -H "authorization: bearer ${ACCESS_TOKEN}" \
  "https://${APP_HOST}/thing-if/apps/${APP_ID}/onboardings" \
  -d "{\"thingPassword\" : \"${THING_PASSWORD}\", \"thingID\" : \"${THING_ID}\", \"owner\" : \"user:${USER_ID}\"}" | tee .result.txt

export THING_ACCESS_TOKEN=`grep 'accessToken' .result.txt | sed -e 's/^ *\"accessToken\" *: *\"\([a-zA-Z0-9_-]*\)\",/\1/'`

rm .result.txt

echo ' '
echo ' '
echo ==================
echo OWNER INFORMATION:
echo ==================
echo ACCESS_TOKEN=$ACCESS_TOKEN
echo USER_ID=$USER_ID
echo THING_ACCESS_TOKEN=$THING_ACCESS_TOKEN

