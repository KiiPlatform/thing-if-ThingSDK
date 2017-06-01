## How to use

### Setup thing and owner.
1. Edit setup.sh
Go to https://developer.kii.com and create app for you.
replace APP_ID, APP_KEY, APP_HOST value with yours.
Create thing in develoer console and replace THING_ID value with yours.
Remember your thing password. it will be used later.

2. execute setup.sh
```sh
source setup.sh
```
To export environment variables, use source command instead of sh for later script execution.

3. execute owner.sh
Go to https://developer.kii.com and create user for test.
```sh
source owner.sh -u {username} -p {user-password} -t {thing-password}
```
To export environment variables, use source command instead of sh as well.

### Execute Command, Get Status.
After finished Setup, you can execute following scripts.

#### Execute Command
After finished Setup, you can execute following scripts.
```sh
sh air-conditioner-command-post.sh
```

#### List Command
```sh
sh air-conditioner-command-get.sh
```

#### Get Command with ID
```sh
sh air-conditioner-command-get.sh -c {command-id}
```

#### Get Thing State
```sh
sh air-conditioner-state-get.sh
```

### Notifications

If you want to receive notifications. You can execute followings after
*Setup thing and owner*

#### Push to App Notifications

Procedure to receive Push to App Notifications are followings:

  1. Subscribe a bucket.
  1. Create, modify, or delete a object in the bucket.

The commands are followings:

```sh
sh subscribe_bucket.sh
sh create_object.sh
```

After `sh create_object.sh`, this sample application will receive
notification.

Once you execute `sh subscribe_bucket.sh`, You does not need to
execute the command twice. Notifications are sent by only `sh
create_object.sh` command.

#### Push to User Notifications

Procedure to receive Push to User Notifications are followings:

  1. Create a topic.
  1. Subscribe the topic.
  1. Send message to the topic.

The commands are followings:

```sh
sh create_topic.sh
sh subscribe_topic.sh
sh send_topic.sh
```

After `sh send_topic.sh`, this sample application will receive
notification.

Once you execute `sh create_topic.sh` and `sh subscribe_topic.sh`, You
does not need to execute these command twice. Notifications are sent
by only `sh send_topic.sh` command.
