#/bin/sh

APP_ID=00959619
APP_KEY=1ae0bcded44365e4f83c2daa2f4ca237
APP_HOST=api-development-jp.internal.kii.com
THING_ID=th.53ae324be5a0-bea9-5e11-96a3-0721cbd1
ACCESS_TOKEN=UdBA-8JhO2BlTsdx-IN5Gpsr4vcQf2um63S7-qeMKcw
USER_ID=53ae324be5a0-bea9-5e11-86a3-0e9cbafc
COMMAND_ID=$1

if test $# -ne 1
    then
    echo "command id is required."
    exit
fi

curl -v -X GET \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/iot-api/apps/${APP_ID}/targets/THING:${THING_ID}/commands/${COMMAND_ID}"
