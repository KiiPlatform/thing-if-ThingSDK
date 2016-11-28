#! bin/bash

declare -a uploadhosts=($DOC_HOST1 $DOC_HOST2)

# path TBD
basedir="/ext/ebs/references/c/thing-if"
version=$(cd thing-if-ThingSDK; grep -o "version.*" sdk-info.txt | grep -o '[0-9]\+\.[0-9]\+\.[0-9]\+')
docFolderPath="~/thing-if-ThingSDK/doc/"

updir="$basedir/$version"
latestdir="$basedir/latest"
echo ""
for host in "${uploadhosts[@]}"; do
  uptarget="$host:$updir"
  echo "Uploading to : $host"
  rsync -rlptD --chmod=u+rw,g+r,o+r --chmod=Da+x --delete-after "$docFolderPath" "$uptarget"

  # check command exit code
  exitCode=$?
  if [ $exitCode -ne 0 ]; then
    echo "Faild when uploading doc to : $uptarget"
    exit $exitCode
  fi

  ssh "$host" "rm $latestdir"
  # check command result
  exitCode=$?
  if [ $exitCode -ne 0 ]; then
    echo "Faild when removing older doc to : $uptarget"
    exit $exitCode
  fi

  ssh "$host" "ln -s $updir $latestdir"
  # check command result
  exitCode=$?
  if [ $exitCode -ne 0 ]; then
    echo "Faild when releasing new doc to : $uptarget"
    exit $exitCode
  fi

done

echo "All uploads have completed!"
