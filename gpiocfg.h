#define MAX_PIN_NAME_WIDTH 	    32
#define MAX_PINS_IN_COMMUNITY	    128
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
