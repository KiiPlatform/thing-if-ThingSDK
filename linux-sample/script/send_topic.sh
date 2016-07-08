#!/bin/sh

curl -v -X POST \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "X-Kii-AppID: ${APP_ID}" \
  -H "X-Kii-AppKey: ${APP_KEY}" \
  -H "Content-Type: application/vnd.kii.SendPushMessageRequest+json" \
  -H "Accept: application/vnd.kii.SendPushMessageResponse+json" \
  "https://${APP_HOST}/api/apps/${APP_ID}/things/${THING_ID}/topics/${TOPIC_ID}/push/messages" \
  -d '{"data": {"Item": "Do something", "Done": 0}, "sendToDevelopment": true, "sendToProduction": true, "gcm": {"enabled": true}, "apns": {"enabled": true},  "jpush": {"enabled": true}, "mqtt": {"enabled": true}}'
