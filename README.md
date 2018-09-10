USAGE:
=====

localhost ~ # ./gpiocfg -h        

Usage: ./gpiocfg [options] <command>

Options:
  -p, --platform=<name>    Platform to use (Default: icelake)

Commands:
  -r, --read=<gpio pin>    Read and parse DW0/DW1 config for given pin
  -v, --value=<DW0 value>  Parse the given DW0 register value
  -l, --list               List the GPIO pins for selected platform
  -h, --help               Print this help.


EXAMPLE OUTPUT:
==============

localhost ~ # 
localhost ~ # ./gpiocfg -r GPP_D16
Pin lookup requested for pin  : GPP_D16
Platform : icelake
Found pin          : GPP_D16
Community Name     : community-1
Community Port ID  : 0x6D
Pin no in community: 0x28 (40)

PAD_CFG_DW0 MMIO address : 0xFD6D0880
PAD_CFG_DW1 MMIO address : 0xFD6D0884

PAD_CFG_DW0 value : 0x80900102
PAD_CFG_DW1 value : 0x00003038

------------- DW0 -------------
[bit 31-30][Pad Reset Config]             = 0x2 (PLTRST#)
[bit    29][RX Pad State Select]          = 0x0 (Raw RX pad state directly from RX buffer)
[bit    28][RX Raw Override to '1']       = 0x0 (No Override)
[bit 26-25][RX Level/Edge Configuration]  = 0x0 (Level)
[bit    24][Pre Glitch Filter Stage RX Pad State Select] = 0x0 (Select synchronized, non filtered RX pad state)
[bit    23][RX Invert]                    = 0x1 (Inversion)
[bit 22-21][RX/TX Enable Config]          = 0x0 (Function defined in Pad Mode controls TX and RX Enables)
[bit    20][GPIO Input Route IOxAPIC]     = 0x1 (Routing can cause peripheral IRQ)
[bit    19][GPIO Input Route SCI]         = 0x0 (Routing Routing does not cause SCI)
[bit    18][GPIO Input Route SMI]         = 0x0 (Routing does not cause SMI)
[bit    17][GPIO Input Route NMI]         = 0x0 (Routing does not cause NMI)
[bit 12-10][Pad Mode]                     = 0x0 (GPIO controls the Pad)
[bit     9][GPIO RX Disable]              = 0x0 (Enable the input buffer (active low enable) of the pad)
[bit     8][GPIO TX Disable]              = 0x1 (Disable the output buffer of the pad)
[bit     1][GPIO RX State]                = 0x1 (Current internal RX pad state)
[bit     0][GPIO TX State]                = 0x0 (Drive a level '0' to the TX output pad)
-------------------------------
localhost ~ # 
