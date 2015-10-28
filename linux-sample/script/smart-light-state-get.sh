#!/bin/sh

# This is a curl script to get state from server.
curl -v -X GET \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/thing-if/apps/${APP_ID}/targets/THING:${THING_ID}/states"
