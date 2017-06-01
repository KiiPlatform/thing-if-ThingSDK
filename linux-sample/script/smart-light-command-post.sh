#!/bin/sh

curl -v -X POST \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/vnd.kii.CommandCreationRequest+json" \
  "https://${APP_HOST}/thing-if/apps/${APP_ID}/targets/thing:${THING_ID}/commands" \
  -d "{\"issuer\":\"user:${USER_ID}\",\"actions\":[{\"AirConditionerAlias\":[{\"turnPower\":true},{\"setPresetTemperature\":25}]}]}"
