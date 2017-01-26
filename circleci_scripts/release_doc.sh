#! bin/bash

git clone https://$GH_TOKEN_FOR_HTTPS@github.com/KiiPlatform/thing-if-ThingSDK.git
cd thing-if-ThingSDK
git checkout gh-pages && git config user.email 'satoshi.kumano@kii.com' && git config user.name 'satoshi kumano'
git rm -r --ignore-unmatch api-doc && mkdir -p api-doc
cp -r ../doc/html/ api-doc
git add api-doc && git commit -m 'updated doc' && git push origin gh-pages

