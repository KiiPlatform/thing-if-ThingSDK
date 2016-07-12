#!/bin/sh

curl -v -X PUT \
  -H "Authorization: Bearer ${ACCESS_TOKEN}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/api/apps/${APP_ID}/things/${THING_ID}/buckets/${BUCKET_ID}/objects/test_data_id" \
  -d "{}"
