# Sample application on CC3200

## Required tools

We built and tested this sample appliation on Windows 7 and 10.
Requilred tools are followings:

  * CC3200 SDK
  * Code Composer Studio IDE (CCS)

Getting started guide for CC3200 is [this
document](http://www.tij.co.jp/jp/lit/ug/swru376d/swru376d.pdf). How
to install these two tools are described in the document.

## Build

If you are a beginner of CC3200 Simple link development, you should
learn CC3200 SimpleLink development with [CC3200 SimpleLink Wi-Fi and
IoT Solution With MCU LaunchPad Getting Started
Guide](http://www.tij.co.jp/jp/lit/ug/swru376d/swru376d.pdf) before
using this sample application.

### Create a Kii Application

Before you use this sample application, you need to create a Kii
application.

A way to create kii application is described in [this
page](http://docs.kii.com/en/guides/thingifsdk/android/quickstart/create-app/)

### Build sample application.

1. Unzip freertos\_thingsdk\_demo.zip.
1. Copy unzipped directory to CC3200 SDK example folder(C:/TI/CC3200SDK\_1.2.0/cc3200-sdk/example as default).
1. Lunch CCS IDE.
1. Import project in freertos_thingsdk_demo/ccs as CCS Project on CCS.
  1. Choose File -> Import
  1. In Import window, Select Code Compose Studio -> CCS Projects.
1. Edit SSID\_NAME, SECURITY\_TYPE and SECURITY\_KEY in the file of cc3200-sdk/example/common/common.h. SSID\_NAME, SECURITY\_TYPE and SECURITY\_KEY are access point information
  * SSID\_NAME : Access point name.
  * SECURITY\_TYPE : Security type of you access point Such as WEP or WPA.
  * SECURITY\_KEY : Password of the your secured access point.
1. Edit APP_ID, APP_KEY and APP_SITE in main.c with your created Kii application information.
1. Connect SimpleLink board with USB to your PC.
1. Lunch tera term. A way to use tera term is described in [CC3200 SimpleLink Wi-Fi and IoT Solution With MCU LaunchPad Getting Started Guide](http://www.tij.co.jp/jp/lit/ug/swru376d/swru376d.pdf) at section 3.1. Option 1: Code Composer Studio (CCS).
1. Press Debug button (it is a bug icon). Building and transfering this sample application to board are started.
1. Type 'o' key and return, onboarding starts on CC3200.

