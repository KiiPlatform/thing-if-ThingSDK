#!/bin/sh

# This is a curl script to get state from server.

APP_ID=00959619
APP_KEY=1ae0bcded44365e4f83c2daa2f4ca237
APP_HOST=api-development-jp.internal.kii.com
THING_ID=th.53ae324be5a0-4908-5e11-a427-010b4f79
ACCESS_TOKEN=4JPctYftnTzFgCwKWMZFuzcZ5v-JMEckI1_ynb6eFEg

curl -v -X GET \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/thing-if/apps/${APP_ID}/targets/THING:${THING_ID}/states"



