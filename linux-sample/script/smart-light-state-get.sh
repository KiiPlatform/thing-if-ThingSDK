#!/bin/sh

# This is a curl script to get state from server.

APP_ID=00959619
APP_KEY=1ae0bcded44365e4f83c2daa2f4ca237
APP_HOST=api-development-jp.internal.kii.com
THING_ID=th.53ae324be5a0-bea9-5e11-96a3-0721cbd1
ACCESS_TOKEN=gf59UOSCuvVJNddsXaLBs2yq107TlOB9ZrwifPEzCo0

curl -v -X GET \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/iot-api/apps/${APP_ID}/targets/THING:${THING_ID}/states"



