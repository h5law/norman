/*
 * Copyright (c) 2025 h5law <dev@h5law.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/*
 * crtbrand.S - ELF branding for norman libc on AION OS
 *
 * This file creates the ELF note sections to identify ELF binaries as
 * being built for the AION Operating System (with version) using norman.
 *
 * Follows the ELF note format specification with proper 4-byte padding.
 */

#include <sys/param.h>

	.section ".note.aion.ident", "a"
	.p2align 2

	.long	ELF_NOTE_AION_NAMESZ
	.long	ELF_NOTE_AION_DESCSZ
	.long	ELF_NOTE_TYPE_AION_TAG
	.ascii	"AION\0\0\0\0"	/* ELF_NOTE_AION_NAME */
	.long	__AION_Version__
