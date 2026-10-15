# HC32F401B-Board-Files-and-examples-


I just pulled this straight from their website but it is blocked anywhere but China. The CMSIS pack also barely works. 

I will be adding more in a while once I figure things out.

[WORKING]
- code upload and flash erase via a STLINK (it appears)

[BROKEN]
- The cmsis header file is wonky
- cannot use USB DFU
- any code, even to get a LED flashing there is trouble.
- debugging is impossible with the VSCode CMSIS extension. I will try to get a CMakeLists.txt wrote for this and see if cortex debug works. 

# examples

These are again pulled right from their website. I have not tested. 

# boards

I have designed a simple demo board for this MCU (qfn32 one) if you want the files see [LINK TO OSHWLAB] for the design and BoM 
it uses my personal standard cheapo debugger header that just uses a 10 pin IDC connector to a clone STLink. Considering switching to a 10pin samtec header used by my jlink mini but the connector is 10x the cost of the MCU!



