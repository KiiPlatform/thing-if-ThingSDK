#!/bin/sh

curl -v -X POST \
  -H "Authorization: Bearer ${THING_ACCESS_TOKEN}" \
  "https://${APP_HOST}/api/apps/${APP_ID}/things/${THING_ID}/buckets/${BUCKET_ID}/filters/all/push/subscriptions/things"
