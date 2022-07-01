#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include "gpiocfg.h"

extern struct platform icelake;
extern struct platform kabylake;
extern struct platform cometlake;
extern struct platform jasperlake;
extern struct platform tigerlake;
extern struct platform alderlake;

struct platform *platforms[] = {
	&icelake,
	&kabylake,
	&cometlake,
	&jasperlake,
	&tigerlake,
	&alderlake,
	NULL
};

struct platform *platform = &alderlake;

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
