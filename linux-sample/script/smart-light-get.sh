#!/bin/sh

APP_ID=00959619
APP_KEY=1ae0bcded44365e4f83c2daa2f4ca237
APP_HOST=api-development-jp.internal.kii.com
THING_ID=th.53ae324be5a0-4908-5e11-a427-010b4f79
ACCESS_TOKEN=4JPctYftnTzFgCwKWMZFuzcZ5v-JMEckI1_ynb6eFEg
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
  "https://${APP_HOST}/thing-if/apps/${APP_ID}/targets/THING:${THING_ID}/commands/${COMMAND_ID}"
