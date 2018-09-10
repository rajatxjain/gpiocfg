#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#define MAX_PIN_NAME_WIDTH 	    16
#define MAX_PINS_IN_COMMUNITY	    72
#define MAX_COMMUNITIES		    8

struct community {
	char *name;
	unsigned port_id;
	unsigned num_pins;
	char pin_names[MAX_PINS_IN_COMMUNITY][MAX_PIN_NAME_WIDTH];
};

struct platform {
	char *name;

	/* SBREG_BAR or whatever is the base address for the platform */
	unsigned long base_addr;

	/* How much address space in each port */
	unsigned long port_addr_space_size;

	/* Register offset from where the PAD_CFG_* registers start */
	unsigned long padbar;

	/* How many bytes of PAD_CFG registers for each PAD */
	unsigned long pad_cfg_size;

	/* List of GPIO communities */
	unsigned num_communities;
	struct community *communities;
};

struct community icelake_communities[] = {
	{
		.name = "community-0",
		.port_id = 0x6e,
		.num_pins = 58,
		.pin_names = {
			/* GPP_G0 - GPP_G7 */
			"GPP_G0",   
			"GPP_G1",
			"GPP_G2",
			"GPP_G3",
			"GPP_G4",
			"GPP_G5",
			"GPP_G6",
			"GPP_G7",
			/* GPP_B0 - GPP_B23 + 2 "reserved pins" */
			"GPP_B0",
			"GPP_B1",
			"GPP_B2",
			"GPP_B3",
			"GPP_B4",
			"GPP_B5",
			"GPP_B6",
			"GPP_B7",
			"GPP_B8",
			"GPP_B9",
			"GPP_B10",
			"GPP_B11",
			"GPP_B12",
			"GPP_B13",
			"GPP_B14",
			"GPP_B15",
			"GPP_B16",
			"GPP_B17",
			"GPP_B18",
			"GPP_B19",
			"GPP_B20",
			"GPP_B21",
			"GPP_B22",
			"GPP_B23",
			"GPP_B_RSRVD0", /* To skip over holes in address space */
			"GPP_B_RSRVD1", /* To skip over holes in address space */
			/* GPP_A0 - GPP_A23 */
			"GPP_A0",
			"GPP_A1",
			"GPP_A2",
			"GPP_A3",
			"GPP_A4",
			"GPP_A5",
			"GPP_A6",
			"GPP_A7",
			"GPP_A8",
			"GPP_A9",
			"GPP_A10",
			"GPP_A11",
			"GPP_A12",
			"GPP_A13",
			"GPP_A14",
			"GPP_A15",
			"GPP_A16",
			"GPP_A17",
			"GPP_A18",
			"GPP_A19",
			"GPP_A20",
			"GPP_A21",
			"GPP_A22",
			"GPP_A23"
		}
	},
	{
		.name = "community-1",
		.port_id = 0x6d,
		.num_pins = 65,
		.pin_names = {
			/* GPP_H0 - GPP_H23 */
			"GPP_H0",
			"GPP_H1",
			"GPP_H2",
			"GPP_H3",
			"GPP_H4",
			"GPP_H5",
			"GPP_H6",
			"GPP_H7",
			"GPP_H8",
			"GPP_H9",
			"GPP_H10",
			"GPP_H11",
			"GPP_H12",
			"GPP_H13",
			"GPP_H14",
			"GPP_H15",
			"GPP_H16",
			"GPP_H17",
			"GPP_H18",
			"GPP_H19",
			"GPP_H20",
			"GPP_H21",
			"GPP_H22",
			"GPP_H23",
			/* GPP_D0 - GPP_D19 + 1 "reserved pin" */
			"GPP_D0",
			"GPP_D1",
			"GPP_D2",
			"GPP_D3",
			"GPP_D4",
			"GPP_D5",
			"GPP_D6",
			"GPP_D7",
			"GPP_D8",
			"GPP_D9",
			"GPP_D10",
			"GPP_D11",
			"GPP_D12",
			"GPP_D13",
			"GPP_D14",
			"GPP_D15",
			"GPP_D16",
			"GPP_D17",
			"GPP_D18",
			"GPP_D19",
			"GPP_D_RSRVD0", /* To skip over holes in address space */
			/* GPP_F0 - GPP_F19 */
			"GPP_F0",
			"GPP_F1",
			"GPP_F2",
			"GPP_F3",
			"GPP_F4",
			"GPP_F5",
			"GPP_F6",
			"GPP_F7",
			"GPP_F8",
			"GPP_F9",
			"GPP_F10",
			"GPP_F11",
			"GPP_F12",
			"GPP_F13",
			"GPP_F14",
			"GPP_F15",
			"GPP_F16",
			"GPP_F17",
			"GPP_F18",
			"GPP_F19",
		}
	},
	{
		.name = "community-2",
		.port_id = 0x6c,
		.num_pins = 12,
		.pin_names = {
			/* GPD0 - GPD11 */
			"GPD0",
			"GPD1",
			"GPD2",
			"GPD3",
			"GPD4",
			"GPD5",
			"GPD6",
			"GPD7",
			"GPD8",
			"GPD9",
			"GPD10",
			"GPD11",
		}
	},
	{
		.name = "community-3",
		.port_id = 0x6b,
	},
	{
		.name = "community-4",
		.port_id = 0x6a,
		.num_pins = 54,
		.pin_names = {
			"GPP_C0",
			"GPP_C1",
			"GPP_C2",
			"GPP_C3",
			"GPP_C4",
			"GPP_C5",
			"GPP_C6",
			"GPP_C7",
			"GPP_C8",
			"GPP_C9",
			"GPP_C10",
			"GPP_C11",
			"GPP_C12",
			"GPP_C13",
			"GPP_C14",
			"GPP_C15",
			"GPP_C16",
			"GPP_C17",
			"GPP_C18",
			"GPP_C19",
			"GPP_C20",
			"GPP_C21",
			"GPP_C22",
			"GPP_C23",
			"GPP_C_RSRVD0", /* To skip over holes in address space */
			"GPP_C_RSRVD1", /* To skip over holes in address space */
			"GPP_C_RSRVD2", /* To skip over holes in address space */
			"GPP_C_RSRVD3", /* To skip over holes in address space */
			"GPP_C_RSRVD4", /* To skip over holes in address space */
			"GPP_C_RSRVD5", /* To skip over holes in address space */
			"GPP_E0",
			"GPP_E1",
			"GPP_E2",
			"GPP_E3",
			"GPP_E4",
			"GPP_E5",
			"GPP_E6",
			"GPP_E7",
			"GPP_E8",
			"GPP_E9",
			"GPP_E10",
			"GPP_E11",
			"GPP_E12",
			"GPP_E13",
			"GPP_E14",
			"GPP_E15",
			"GPP_E16",
			"GPP_E17",
			"GPP_E18",
			"GPP_E19",
			"GPP_E20",
			"GPP_E21",
			"GPP_E22",
			"GPP_E23",
		}
	},
	{
		.name = "community-5",
		.port_id = 0x69, /* Need to verify with Intel, could not find this in EDS */
		.num_pins = 16,
		.pin_names = {
			"GPP_R0",
			"GPP_R1",
			"GPP_R2",
			"GPP_R3",
			"GPP_R4",
			"GPP_R5",
			"GPP_R6",
			"GPP_R7",
			"GPP_S0",
			"GPP_S1",
			"GPP_S2",
			"GPP_S3",
			"GPP_S4",
			"GPP_S5",
			"GPP_S6",
			"GPP_S7",
		}
	},
};

struct platform icelake = {
	.name = "icelake",
	.base_addr = 0xFD000000,
	.port_addr_space_size = 0x10000,
	.padbar = 0x600,
	.pad_cfg_size = 0x10,
	.num_communities = 6,
	.communities = icelake_communities,
};

struct platform *platforms[] = {
	&icelake,
	NULL
};

struct platform *platform = &icelake;

#define DW0_ADDR(cmty, pin_no_in_cmty)			 \
	(platform->base_addr +                           \
	 cmty->port_id * platform->port_addr_space_size +\
	 platform->padbar +                              \
	 pin_no_in_cmty * platform->pad_cfg_size)

#define DW1_ADDR(cmty, pin_no_in_cmty)			\
	(DW0_ADDR(cmty, pin_no_in_cmty) + 4)

static struct platform *lookup_platform(char *name)
{
	unsigned int i;
	
	for (i = 0; platforms[i]; i++)		
		if (!strcmp(platforms[i]->name, name))
			return platforms[i];

	printf("Unknown platform: %s\n", name);
	exit(-1);
}

static int lookup_pin(char *pin_name, struct community **community, unsigned *pin_no_in_cmty)
{
	unsigned cmty_num, pin;
	struct community *cmty;

	for (cmty_num = 0; cmty_num < platform->num_communities; cmty_num++) {		
		cmty = &platform->communities[cmty_num];
		for (pin = 0; pin < cmty->num_pins; pin++)
			if (!strcmp(cmty->pin_names[pin], pin_name)) {
				*community = cmty;
				*pin_no_in_cmty = pin;
				return 1;
			}
	}
	return 0;
}

static u_int32_t mmio_read32(unsigned long addr)
{
	int fd;
	volatile u_int32_t *ptr;
	u_int32_t val;
	unsigned int pgsize = getpagesize();
	unsigned long aligned_addr = addr & ~(pgsize - 1);
	unsigned long offset = addr & (pgsize - 1);
	
	fd = open("/dev/mem", O_RDONLY | O_SYNC);
	if (fd < 0) {
		printf("Error opening /dev/mem!!!\n");
		return -1;
	}
	ptr = mmap(NULL, pgsize, PROT_READ, MAP_SHARED, fd, aligned_addr);
	if (!ptr || ptr == MAP_FAILED) {
		printf("Error in mmap for addr 0x%X!!!\n", addr);
		return -1;
	}
	val = *(volatile u_int32_t *)((unsigned long)ptr + offset);
	munmap((void*) ptr, pgsize);
	close(fd);
	return val;
}

/* Assumes start <= end, also both are within 0-31, end bit is included */
#define BITS(x, start, end)	\
	(((unsigned long)x & ((1ULL << (end + 1)) - 1)) >> start)

#define PARSE_1_BIT_FIELD(regval, bit_num, field_text, val0_text, val1_text)   \
	do {                                                                   \
		printf("[bit %5d]%-30s = 0x%X ", bit_num, field_text,        \
		       BITS(regval, bit_num, bit_num));                        \
		switch (BITS(regval, bit_num, bit_num)) {                      \
		case 0:                                                        \
			printf("(%s)\n", val0_text);                           \
			break;                                                 \
		case 1:                                                        \
			printf("(%s)\n", val1_text);                           \
			break;                                                 \
		}                                                              \
	} while (0)

#define PARSE_2_BIT_FIELD(regval, bit_begin, bit_end, field_text,              \
			  val0_text, val1_text, val2_text, val3_text)          \
	do {                                                                   \
		printf("[bit %2d-%2d]%-30s = 0x%X ", bit_end, bit_begin,       \
		       field_text, BITS(regval, bit_begin, bit_end));          \
		switch (BITS(regval, bit_begin, bit_end)) {                    \
		case 0:                                                        \
			printf("(%s)\n", val0_text);                           \
			break;                                                 \
		case 1:                                                        \
			printf("(%s)\n", val1_text);                           \
			break;                                                 \
		case 2:                                                        \
			printf("(%s)\n", val2_text);                           \
			break;                                                 \
		case 3:                                                        \
			printf("(%s)\n", val3_text);                           \
			break;                                                 \
		}                                                              \
	} while (0)

static void parse_dw0(u_int32_t dw0)
{
	printf("------------- DW0 -------------\n");
	PARSE_2_BIT_FIELD(dw0, 30, 31, "[Pad Reset Config]",
			  "RSMRST#", 
			  "Host deep reset", 
			  "PLTRST#",
			  "Reserved");

	PARSE_1_BIT_FIELD(dw0, 29, "[RX Pad State Select]",
			  "Raw RX pad state directly from RX buffer",
			  "Internal RX pad state");

	PARSE_1_BIT_FIELD(dw0, 28, "[RX Raw Override to '1']",
			  "No Override",
			  "RX drive 1 internally");

	PARSE_2_BIT_FIELD(dw0, 25, 26, "[RX Level/Edge Configuration]",
			  "Level",
			  "Edge (RxInv=0 for rising edge; 1 for falling edge)",
			  "Disable",
			  "Either rising edge or falling edge");

	PARSE_1_BIT_FIELD(dw0, 24, "[Pre Glitch Filter Stage RX Pad State Select]",
			  "Select synchronized, non filtered RX pad state",
			  "Select synchronized, filtered RX pad state");

	PARSE_1_BIT_FIELD(dw0, 23, "[RX Invert]",
			  "No inversion",
			  "Inversion");

	PARSE_2_BIT_FIELD(dw0, 21, 22, "[RX/TX Enable Config]",
			  "Function defined in Pad Mode controls TX and RX Enables",
			  "Function controls TX Enable and RX Disabled with RX drive 0 internally",
			  "Function controls TX Enable and RX Disabled with RX drive 1 internally",
			  "Function controls TX Enabled and RX is always enabled");

	PARSE_1_BIT_FIELD(dw0, 20, "[GPIO Input Route IOxAPIC]",
			  "Routing does not cause peripheral IRQ",
			  "Routing can cause peripheral IRQ");

	PARSE_1_BIT_FIELD(dw0, 19, "[GPIO Input Route SCI]",
			  "Routing Routing does not cause SCI",
			  "Routing can cause SCI");

	PARSE_1_BIT_FIELD(dw0, 18, "[GPIO Input Route SMI]",
			  "Routing does not cause SMI",
			  "Routing can cause SMI");

	PARSE_1_BIT_FIELD(dw0, 17, "[GPIO Input Route NMI]",
			  "Routing does not cause NMI",
			  "Routing can cause SMI");

	printf("[bit 12-10][Pad Mode]                     = 0x%X ", BITS(dw0, 10, 12));
	switch (BITS(dw0, 10, 12)) {
	case 0:
		printf("(GPIO controls the Pad)\n");
		break;
	case 1:
		printf("(native function 1, if applicable, controls the Pad)\n");
		break;
	case 2:
		printf("(native function 2, if applicable, controls the Pad)\n");
		break;
	case 3:
		printf("(native function 3, if applicable, controls the Pad)\n");
		break;
	case 4:
		printf("(native function 4, if applicable, controls the Pad)\n");
		break;
	case 5:
		printf("(native function 5, if applicable, controls the Pad)\n");
		break;
	default:
		printf("(WTF? ? ? ?)\n");
		break;
	}

	PARSE_1_BIT_FIELD(dw0, 9, "[GPIO RX Disable]",
			  "Enable the input buffer (active low enable) of the pad",
			  "Disable the input buffer of the pad");

	PARSE_1_BIT_FIELD(dw0, 8, "[GPIO TX Disable]",
			  "Enable the output buffer (active low enable) of the pad",
			  "Disable the output buffer of the pad");

	PARSE_1_BIT_FIELD(dw0, 1, "[GPIO RX State]",
			  "Current internal RX pad state",
			  "Current internal RX pad state");

	PARSE_1_BIT_FIELD(dw0, 0, "[GPIO TX State]",
			  "Drive a level '0' to the TX output pad",
			  "Drive a level '1' to the TX output pad");
	printf("-------------------------------\n");
}

static void usage(char *prog)
{
	printf("\nUsage: %s [options] <command>\n\n", prog);
	printf("Options:\n");
	printf("  -p, --platform=<name>    Platform to use (Default: icelake)\n\n");
	printf("Commands:\n");
	printf("  -r, --read=<gpio pin>    Read and parse DW0/DW1 config for given pin\n");
	printf("  -v, --value=<DW0 value>  Parse the given DW0 register value\n");
	printf("  -l, --list               List the GPIO pins for selected platform\n");
	printf("  -h, --help               Print this help.\n\n");
}

static void list_gpios()
{
	unsigned cmty_num, pin;
	struct community *cmty;

	printf("Platform : %s\n", platform->name);
	printf("Available GPIO names:\n");
	for (cmty_num = 0; cmty_num < platform->num_communities; cmty_num++) {		
		cmty = &platform->communities[cmty_num];
		printf("\t%s:\n", cmty->name);
		for (pin = 0; pin < cmty->num_pins; pin++)
			printf("\t\t%s\n", cmty->pin_names[pin]);
	}
}

static void read_pin_pad(char *pin_name, u_int32_t *dw0, u_int32_t *dw1)
{
	struct community *cmty;
	unsigned int pin_no_in_cmty;
	unsigned long dw0_addr, dw1_addr;

	if (!lookup_pin(pin_name, &cmty, &pin_no_in_cmty)) {
		printf("Pin lookup failure: %s\n", pin_name);
		exit (-1);
	}

	printf("Found pin          : %s\n", pin_name);
	printf("Community Name     : %s\n", cmty->name);
	printf("Community Port ID  : 0x%X\n", cmty->port_id);
	printf("Pin no in community: 0x%X (%u)\n\n",
	       pin_no_in_cmty, pin_no_in_cmty);

	dw0_addr = DW0_ADDR(cmty, pin_no_in_cmty);
	dw1_addr = DW1_ADDR(cmty, pin_no_in_cmty);
	printf("PAD_CFG_DW0 MMIO address : 0x%08X\n", dw0_addr); 
	printf("PAD_CFG_DW1 MMIO address : 0x%08X\n\n", dw1_addr);

	*dw0 = mmio_read32(dw0_addr);
	*dw1 = mmio_read32(dw1_addr);
	printf("PAD_CFG_DW0 value : 0x%08X\n", *dw0); 
	printf("PAD_CFG_DW1 value : 0x%08X\n\n", *dw1);
}

int main(int argc, char **argv)
{
	int c;
        struct community *cmty = NULL;
        unsigned int pin_no_in_cmty;
	u_int32_t dw0 = 0, dw1 = 0;
	char pin_name[MAX_PIN_NAME_WIDTH] = "";

	if (argc <= 1) {
		usage(argv[0]);
		return -1;
	}

	while (1) {
		static struct option long_options[] = {
			{"platform", required_argument, 0, 'p'},
			{"read", required_argument, 0, 'r'},
			{"value", required_argument, 0, 'v'},
			{"list", no_argument, 0, 'l'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "p:r:v:lh", long_options,
				 &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'p':
			printf("Platform requested            : %s\n", optarg);
			platform = lookup_platform(optarg);
			break;
		case 'r':
			printf("Pin lookup requested for pin  : %s\n", optarg);
			if (!lookup_pin(optarg, &cmty, &pin_no_in_cmty)) {
				printf("Pin lookup failure: %s\n", optarg);
				return -1;
			}
			strcpy(pin_name, optarg);
			break;
		case 'v':
			dw0 = (u_int32_t) strtoul(optarg, NULL, 0);
			printf("DW0 Value parse requested for : 0x%08X\n", dw0);
			break;
		case 'l':
			list_gpios();
			return 0;
		case 'h':
		case '?':
			/* getopt_long already printed an error message. */
		default:
			usage(argv[0]);
			return -1;
		}
	}
	if (!cmty && !dw0) {
		usage(argv[0]);
		return -1;
	}
		

	printf("Platform : %s\n", platform->name);

	if (cmty)
		read_pin_pad(pin_name, &dw0, &dw1);

	parse_dw0(dw0);
	
	return 0;
}
