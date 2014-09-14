## Level
#### A node for cognitive radio networks
---
Level is a small, low power and low bandwidth radio for creating and researching cognitive radio networks. It is still in a prototype phase. See the platform paper for a more detailed overview.

###Building
There are two methods to build the firmware. You can use TI's [Code Composer Studio](http://www.ti.com/tool/ccstudio), which is an Eclipse based IDE. Just create a new project and import the files in the firmware/levelfirmware directory. You'll also need to set the target chip to be the cc430f3157.

 You can also use [msp430-gcc](https://github.com/sergiocampama/Launchpad/blob/master/README.md), which has a more libre license than Code Composer Studio. Some of the source files must be modified slightly since interrupt service routine syntax is slightly different. To build, cd into the firmware/levelfirmware/msp430-gcc directory and run "make". To reset the build, run "make clean". 

To save time, I've also been using SimpliciTI as a mesh networking stack. This isn't as free-as-in-speech as I'd like, and I plan on moving away from it. Until then, the license for that code is below.

###License 
The MIT License (MIT)

Copyright (c) [2014] [Hunter Scott]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

All SimpliciTI code is distributed under TI's license agreement, which can be found in "SimpliciTI License Agreement.pdf". 