# Sample application on CC3200

## Requirement tools

### CC3200 SDK
Download and install SDK from [SimpleLink Wi-Fi CC3200 SDK](http://www.ti.com/tool/cc3200sdk).

### Code Composer Studio IDE(CCS)
1.Download and install IDE from [Code Composer Studio IDE](http://www.tij.co.jp/lsds/ti_ja/dsp/support/dev_tool/ccs_overview.page).
2.Start IDE and check all updates.

## Set up target project
1. Copy freertos\_thingsdk\_demo to CC3200 SDK example folder(C:/TI/CC3200SDK\_1.2.0/cc3200-sdk/example as default).
2. Cloning thing-if-ThingSDK git repository in freertos\_thingsdk\_demo, check out develop branch and update all sub modules.
3. Use ccs.patch in freertos\_thingsdk\_demo/thing-if-ThingSDK.

## Build and Run
1. Import project in freertos_thingsdk_demo/ccs on CCS.
2. Set SSID\_NAME, SECURITY\_TYPE and SECURITY\_KEY in the file of common.h, this file is in cc3200-sdk/example/common.
3. Build and run debug configuration `freertos_demo`.
4. Start your terminal application for CC3200 when stopping first break point of main.
5. Type any key and return, onboarding starts on CC3200.

