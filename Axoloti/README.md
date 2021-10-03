# Axowl FAQ

## Can you actually breed avians and amphibians?

At least it's worth trying! Axowl is a port of [OWL/OpenWare firmware](https://github.com/pingdynasty/OpenWare) to [Axoloti](http://www.axoloti.com/) hardware. [Axocontrol](https://musicthing.co.uk/pages/axo.html) support is included.

## What if I don't have an Axocontrol? How about my own frankenboard?

Axowl bootloader might be usable on a bare Axoloti board. A copy of Axowl firmware with simplified version of UI could be made to use it with only MIDI control. The same is true about custom controller boards.

## Is it an official firmware supported by Johannes Taelman or Rebel Technology?

It is not, so use at your own risk.

## Which peripherals from Axoloti are supported? Anything broken?

There's no support for SD cards in the OWL firmware itself, so it can't be used for the time being. It's planned to be added eventually.

USB device on OWL is configured as a composite audio device, this occasionally makes some versions of Windows unhappy. USB audio is generating 2 channels that contain patch output.

USB host in OWL currently doesn't handle reconnect correctly, this issue is waiting to be resolved in upstream firmware.

## How much flash and memory can my patches use?

Bootloader uses 64 kb of flash. Firmware takes next 320 kb. Remaining 640 kb are available for user patches and resources.

As for memory, firmware uses first 48kb of SRAM. Patches code is loaded to SRAM and can use up to 144 kb of SRAM, unused memory would be available as dynamically allocated memory when patch runs along with 64 kb CCMRAM and 8 Mb of SDRAM.

## How can I run it?

1. Install the OWL bootloader (MidiBoot-Axoloti.bin file) in DFU mode. DFU mode is entered by pressing the tiny S1 switch when device is booting. After that you can use dfu-util or [WebDFU](https://devanlai.github.io/webdfu/dfu-util/) to flash bootloader binary to address 0x08000000

2. After flashing bootloader and rebooting you should be able to see a new USB MIDI device name named ``OWL-BOOT``

3. Flash firmware by playing MIDI SySex file (Axoloti.syx). Specific utility to play MIDI depends on OS used (i.e. amidi on Linux), but you can use official [Rebel Technlology flash page](https://www.openwarelab.org/Tools/firmware.html) in Chromium-based browsers.

4. Wait for device LED to stop flashing (firmware loading should take 5-10 seconds)

5. It's recommended to erase patch if you're flashing Axowl for the first time. This can be done using [OpenWare tool](https://pingdynasty.github.io/OwlWebControl/extended.html).

If all went well, you should end up with a working firmware after rebooting Axoloti. USB device name would be ``OWL-AXOLOTI``

## Where can I get binaries?

https://github.com/antisvin/OpenWare/releases

MidiBoot-Axoloty-*.bin is the bootloader file. There's also a sysex version which can be used to flash bootloader when firmware is running.

Axoloty-*.syx is firmware binary converted to SySex format.

## How can I build firmware from source?

You will need to use the following branches from Github:
* MIDI bootloader - https://github.com/antisvin/OpenWare/tree/axoloti/AxolotiBoot
* Firmware - https://github.com/antisvin/OpenWare/tree/axoloti/Axoloti

## This firmware sucks, how can I uninstall it?

There's nothing preventing you from going flashing original Axoloti bootloader in DFU mode and then the old firmware

## How do I build patches?

You can use upstream [OwlProgram](https://github.com/pingdynasty/OwlProgram) repo to build patches locally or online in the [patcher page](https://www.rebeltech.org/patch-library/patches/latest). Existing patch binaries are expected to be work as is, however note that there's no guarantee that patch was written with exactly the same controls in mind (i.e. could require more inputs or generate CV outputs).

[OWL docs](https://www.openwarelab.org/) should still apply.

## How are patches controlled?

OWL firmware supports using up to 40 parameters. The following mappings to hardware are used on Axoloti with Axocontrol:

* Parameters A - F: controlled by the 6 knobs
* Parameters G - H: controlled by joystick axis
* Buttons 1-3: controlled by buttons
* Button 4 (also aliased as pushbutton): joystick click
* Remaining parameters are virtual, so can be set by MIDI or from a running patch
* LEDs are used to show current button state (and can be set from patches)

This is the [full list](https://github.com/pingdynasty/OpenWare/blob/master/Source/OpenWareMidiControl.h#L9-L54) of parameter names and MIDI info.

## How does patch selection in Axowl UI work?

Switches S1/S2 are used for selecting patches. There are 2 ways to do it:

1. A single click loads previous or next patch when button is released

2. Keeping a button pressed and clicking the opposite button allows you to select previous/next patch that only gets loaded when first button is released. You can press second button multiple times, thus you can skip patches instead of loading them one by one.

When you're in patch selection mode, LEDs are used to display currently selected patch number as binary numbers. Patch number is displayed with most significant bit on the left (LED for button 4). For example patch number 3 is displayed as two unlit LEDs followed by 2 lit LEDs.

## I have more questions!

This port should be discussed on Axoloti forum in Axowl announcement thread.

Questions about writing patches (unrelated to Axowl) can be asked on [RebelTech forum](https://community.rebeltech.org/) (and might be answered already).

# Changelog

## V22.0

Initial release
