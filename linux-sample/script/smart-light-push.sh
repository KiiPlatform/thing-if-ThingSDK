#!/bin/sh

APP_HOST=api-jp.kii.com
APP_ID=a4c6c6a7
APP_KEY=1154dcfd901bf1fdf5606d3b3d92e095
# for 4649
THING_ID=th.88eb20051321-7beb-5e11-c031-0096b1de
# for 46490
#THING_ID=th.88eb20051321-27da-5e11-d742-04089dbe
# for 46491
# THING_ID=th.dfa848a00022-8e09-5e11-1842-0d40c82b

USER_ID=88eb20051321-7beb-5e11-9bd0-067e4ff6

curl -v -X POST \
  -H "X-Kii-AppID: ${APP_ID}" \
  -H "X-Kii-AppKey: ${APP_KEY}" \
  -H "Content-Type: application/json" \
  "https://${APP_HOST}/api/apps/${APP_ID}/server-code/versions/current/postCommands" \
  -d "{\"bodyJson\":{\"target\": \"${THING_ID}\",\"targetType\":\"thing\",\"schema\":\"SmartLightDemo\",\"schemaVersion\":1,\"actions\":[{\"turnPower\":{\"power\":true}},{\"setBrightness\":{\"brightness\":3000}},{\"setColor\":{\"color\":[0,128,255]}},{\"setColorTemperature\":{\"colorTemperature\":-100}}],\"failIfOffline\":false,\"issuer\":\"${USER_ID}\", \"issuerType\":\"user\"},\"headersJson\":{\"authorization\":\"meh\"}}"
