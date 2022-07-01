#include "gpiocfg.h"

static struct community alderlake_communities[] = {
	{
		.name = "community-0",
		.port_id = 0x6e,
		.num_pins = 67,
		.pin_names = {
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
			"GPP_B24",
			"GPP_B25",
			"GPP_T0",
			"GPP_T1",
			"GPP_T2",
			"GPP_T3",
			"GPP_T4",
			"GPP_T5",
			"GPP_T6",
			"GPP_T7",
			"GPP_T8",
			"GPP_T9",
			"GPP_T10",
			"GPP_T11",
			"GPP_T12",
			"GPP_T13",
			"GPP_T14",
			"GPP_T15",
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
			"GPP_A23",
			"GPP_ESPI_CLK_LOOPBK",
		}
	},
	{
		.name = "community-1",
		.port_id = 0x6d,
		.num_pins = 104,
		.pin_names = {
			"GPP_S0",
			"GPP_S1",
			"GPP_S2",
			"GPP_S3",
			"GPP_S4",
			"GPP_S5",
			"GPP_S6",
			"GPP_S7",
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
			"GPP_GSPI2_CLK_LOOPBK",
		}
	},
	{
		.name = "community-2",
		.port_id = 0x6c,
		.num_pins = 17,
		.pin_names = {
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
			"GPD_INPUT3VSEL",
			"GPD_SLP",
			"GPD_SLP",
			"GPD_WAKEB",
			"GPD_DRAM",
		}
	},
	{
		.name = "community-3",
		.port_id = 0x6b,
		.num_pins = 0,
		.pin_names = {
			"BAD_PIN",
		}
	},
	{
		.name = "community-4",
		.port_id = 0x6a,
		.num_pins = 89,
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
			"GPP_F20",
			"GPP_F21",
			"GPP_F22",
			"GPP_F23",
			"GPP_F_CLK_LOOPBK",
			"GPP_L_BKLTEN",
			"GPP_L_BKLTCTL",
			"GPP_L_VDDEN",
			"GPP_SYS_PWROK",
			"GPP_SYS_RESETB",
			"GPP_MLK_RSTB",
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
			"GPP_E_CLK_LOOPBK",
		}
	},
	{
		.name = "community-5",
		.port_id = 0x69,
		.num_pins = 17,
		.pin_names = {
			"GPP_R0",
			"GPP_R1",
			"GPP_R2",
			"GPP_R3",
			"GPP_R4",
			"GPP_R5",
			"GPP_R6",
			"GPP_R7",
			"GPP_SPI_IO_2",
			"GPP_SPI_IO_3",
			"GPP_SPI_MOSI_IO_0",
			"GPP_SPI_MOSI_IO_1",
			"GPP_SPI_TPM_CSB",
			"GPP_SPI_FLASH_0_CSB",
			"GPP_SPI_FLASH_1_CSB",
			"GPP_SPI_CLK",
			"GPP_CLK_LOOPBK",
		}
	},
};

struct platform alderlake = {
	.name = "alderlake",
	.base_addr = 0xFD000000,
	.port_addr_space_size = 0x10000,
	.padbar = 0x700,
	.pad_cfg_size = 0x10,
	.num_communities = 6,
	.communities = alderlake_communities,
};
