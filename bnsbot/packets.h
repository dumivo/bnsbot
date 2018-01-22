#pragma once
namespace packets {
	const char mushin_f9[0x80] =
	{
		0xC8, 0x9B, 0x13, 0x41, 0x01, 0x00, 0x00, 0x00, 0x91, 0x04, 0xBD, 0x44, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x75, 0x00, 0x73, 0x00, 0x68, 0x00, 0x14, 0x00, 0xBE, 0x61, 0x02, 0x00, 0x73, 0x00,
		0x01, 0x00, 0x00, 0x00, 0xF9, 0x72, 0x04, 0x00, 0x00, 0x00, 0x72, 0x00, 0x20, 0x00, 0x2D, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x60, 0x5B, 0x89, 0xD7, 0x00, 0x00, 0x00, 0x00, 0xF4, 0x63, 0x1A, 0x6B, 0x30, 0xAF, 0x04, 0x89,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x8F, 0xBD, 0x44, 0x01, 0x00, 0x00, 0x00,
		0xC0, 0x84, 0x60, 0x1F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x42, 0x00, 0x00, 0xC8, 0x41, 0xFE, 0x41, 0xC0, 0x3D, 0xEE, 0xCB, 0x7D, 0x3F
	};

	const char buy_orb[0x80] =
	{
		0x50, 0xE0, 0x12, 0x41, 0x01, 0x00, 0x00, 0x00, 0x33, 0x04, 0x79, 0x09, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x85, 0xA4, 0x04, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x07, 0x02, 0x02, 0x02,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x40, 0x15, 0x6A, 0x90, 0x00, 0x00, 0x00, 0x00, 0x78, 0xE1, 0x3E, 0x19, 0x00, 0xE7, 0x03, 0x95,
		0x30, 0x79, 0x79, 0x09, 0x01, 0x00, 0x00, 0x00, 0x70, 0x79, 0x79, 0x09, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F,
		0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x07, 0x02, 0x02, 0x02
	};

	/*const char exit[0x80] =
	{
		0x80, 0x88, 0x23, 0x41, 0x01, 0x00, 0x00, 0x00, 0xA0, 0x01, 0x24, 0x11, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x02, 0x00, 0x4D, 0x00, 0x19, 0x00, 0x00, 0x00,
		0x3E, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x01, 0xE8,
		0xFF, 0x83, 0x3F, 0x74, 0x7A, 0xEA, 0xAA, 0xAA, 0x53, 0xFD, 0x6F, 0x78, 0x78, 0x4F, 0x00, 0x80,
		0xB0, 0x3B, 0x5A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x60, 0x65, 0x24, 0x11, 0x01, 0x00, 0x00, 0x00,
		0x31, 0x00, 0x2E, 0x00, 0x31, 0x00, 0x30, 0x00, 0x20, 0x00, 0x73, 0x00, 0x65, 0x00, 0x63, 0x00,
		0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0xC9, 0x00, 0x00, 0x00, 0x00,
		0x1F, 0x84, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x5F, 0xFD, 0x6B, 0x78, 0xE0, 0x50, 0x00, 0x80
	};

	{
		0xC0, 0x39, 0x12, 0x41, 0x01, 0x00, 0x00, 0x00, 0xE6, 0x03, 0xC0, 0x12, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0xF0, 0x8F, 0x20, 0x00,
			0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x53, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x02, 0x02, 0x3E, 0x99, 0xBA, 0x63, 0x02, 0x6E, 0x01, 0x80,
			0xC0, 0x26, 0x02, 0xCA, 0x00, 0x00, 0x00, 0x00, 0x90, 0x23, 0x02, 0xCA, 0x00, 0x00, 0x00, 0x00,
			0x30, 0x00, 0x2F, 0x00, 0x31, 0x00, 0x00, 0x00, 0x20, 0x00, 0x61, 0x00, 0x70, 0x00, 0x70, 0x00,
			0x6C, 0x00, 0x79, 0x00, 0x20, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x6D, 0x00, 0x65, 0x00
	};*/
}