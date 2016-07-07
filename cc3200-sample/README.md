# Sample application on CC3200

## Requirement tools

We used these tools at Windows 10 only.

### CC3200 SDK
Download and install SDK from [SimpleLink Wi-Fi CC3200 SDK](http://www.ti.com/tool/cc3200sdk).

### Code Composer Studio IDE(CCS)
1. Download and install IDE from [Code Composer Studio IDE](http://www.tij.co.jp/lsds/ti_ja/dsp/support/dev_tool/ccs_overview.page).
1. Start IDE and check all updates.

## Set up, build and run
1. unzip freertos\_thingsdk\_demo.zip to CC3200 SDK example folder(C:/TI/CC3200SDK\_1.2.0/cc3200-sdk/example as default).
1. Import project in freertos_thingsdk_demo/ccs on CCS.
1. Set SSID\_NAME, SECURITY\_TYPE and SECURITY\_KEY in the file of common.h, this file is in cc3200-sdk/example/common. See [CC3200 SimpleLink Wi-Fi and IoT Solution w/ MCU LaunchPad Getting Started Guide](http://www.ti.com/jp/lit/pdf/swru376).
1. Build and run debug configuration `freertos_demo`.
1. Start your terminal application for CC3200 when stopping first break point of main.
1. Type 'o' key and return, onboarding starts on CC3200.

