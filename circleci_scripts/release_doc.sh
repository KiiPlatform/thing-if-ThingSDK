#! bin/bash

declare -a uploadhosts=($DOC_HOST1 $DOC_HOST2)

# path TBD
basedir="/ext/ebs/references/c/thing-if"
version=$(grep -o "version.*" sdk-info.txt | grep -o '[0-9]\+\.[0-9]\+\.[0-9]\+')
docFolderPath="~/thing-if-ThingSDK/doc/"

updir="$basedir/$version"
latestdir="$basedir/latest"
echo ""
for host in "${uploadhosts[@]}"; do
  uptarget="$host:$updir"
  echo "Uploading to : $host"
  rsync -rlptD --chmod=u+rw,g+r,o+r --chmod=Da+x --delete-after "$docFolderPath" "$uptarget" > /dev/null 2>&1

  # check command exit code
  exitCode=$?
  if [ $exitCode -ne 0 ]; then
    echo "Faild when uploading doc to : $uptarget"
    exit $exitCode
  fi

  ssh "$host" "rm $latestdir" > /dev/null 2>&1
  # check command result
  exitCode=$?
  if [ $exitCode -ne 0 ]; then
    echo "Faild when removing older doc to : $uptarget"
    exit $exitCode
  fi

  ssh "$host" "ln -s $updir $latestdir" > /dev/null 2>&1
  # check command result
  exitCode=$?
  if [ $exitCode -ne 0 ]; then
    echo "Faild when releasing new doc to : $uptarget"
    exit $exitCode
  fi

done

echo "All uploads have completed!"
