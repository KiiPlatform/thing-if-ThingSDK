#!/bin/sh

# This is a curl script to get state from server.

APP_ID=00959619
APP_KEY=1ae0bcded44365e4f83c2daa2f4ca237
APP_HOST=api-development-jp.internal.kii.com
THING_ID=th.53ae324be5a0-f5e8-5e11-fdd5-000ee263
ACCESS_TOKEN=7H2sc6UYvC2mKCFihYhA52xh1b--G3rga_OYz_gbCg4

curl -v -X GET \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/iot-api/apps/${APP_ID}/targets/THING:${THING_ID}/states"



