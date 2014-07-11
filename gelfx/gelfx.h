/* Generic ELF wrapper for libelf which does not support gelf_ API.
   Copyright (C) 2001 Red Hat, Inc.
   Written by Jakub Jelinek <jakub@redhat.com>, 2001.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef GELFX_H
#define	GELFX_H

#include <libelf.h>
#include <gelf.h>
#include <stdlib.h>

#ifndef HAVE_GELFX_GETSHDR

#define gelfx_getshdr(elf,scn,shdr) gelf_getshdr(scn,shdr)
#define gelfx_update_shdr(elf,scn,shdr) gelf_update_shdr(scn,shdr)
#define gelfx_getsym(elf,data,ndx,x) gelf_getsym(data,ndx,x)
#define gelfx_update_sym(elf,data,ndx,x) gelf_update_sym(data,ndx,x)
#define gelfx_getdyn(elf,data,ndx,x) gelf_getdyn(data,ndx,x)
#define gelfx_update_dyn(elf,data,ndx,x) gelf_update_dyn(data,ndx,x)
#define gelfx_getrel(elf,data,ndx,x) gelf_getrel(data,ndx,x)
#define gelfx_update_rel(elf,data,ndx,x) gelf_update_rel(data,ndx,x)
#define gelfx_getrela(elf,data,ndx,x) gelf_getrela(data,ndx,x)
#define gelfx_update_rela(elf,data,ndx,x) gelf_update_rela(data,ndx,x)

#endif

#if defined(mips) || defined(_mips) || defined(__mips) || defined(__mips__)
/* In little-endian objects, r_info isn't really a
 64-bit little-endian value: it has a 32-bit
 little-endian symbol index followed by four
 individual byte fields. */

#define ELF64_MIPSEL_INFO_DECODE(info) \
	((((info) & 0xffffffff) << 32) \
	 | (((info) >> 56) & 0xff) \
	 | (((info) >> 40) & 0xff00) \
	 | (((info) >> 24) & 0xff0000) \
	 | (((info) >> 8 ) & 0xff000000))

#define ELF64_MIPSEL_INFO_ENCODE(sym,type) \
	(((uint64_t)(sym) & 0xffffffffUL) \
	 | ((uint64_t)(type) << 8  & 0xff00000000UL) \
	 | ((uint64_t)(type) << 24 & 0xff0000000000UL) \
	 | ((uint64_t)(type) << 40 & 0xff000000000000UL) \
	 | ((uint64_t)(type) << 56 & 0xff00000000000000UL))

#define ELF64_MIPS_R_SYM(info) ((info) & 0xffffffff)
#define ELF64_MIPS_R_TYPE(info) (ELF64_MIPSEL_INFO_DECODE(info) & 0xffffff)
#define ELF64_MIPS_R_INFO(sym,type) (ELF64_MIPSEL_INFO_ENCODE(sym,type))

#define IS_ELF64_MIPSEL(elf) \
	({ \
	   GElf_Ehdr _ehdr; \
	   if (gelf_getehdr (elf, &_ehdr) == NULL) \
	    abort(); \
	   (gelf_getclass (elf) == ELFCLASS64) \
	   && (_ehdr.e_machine == EM_MIPS) && (_ehdr.e_ident[EI_DATA] == ELFDATA2LSB); \
	 })

static inline GElf_Word
gelfx_r_sym (Elf *elf, GElf_Xword info)
{
  return IS_ELF64_MIPSEL(elf) ? ELF64_MIPS_R_SYM(info) : GELF_R_SYM(info);
}

static inline GElf_Word
gelfx_r_type (Elf *elf, GElf_Xword info)
{
  return IS_ELF64_MIPSEL(elf) ? ELF64_MIPS_R_TYPE(info) : GELF_R_TYPE(info);
}

static inline GElf_Xword
gelfx_r_info (Elf *elf, GElf_Word sym, GElf_Word type)
{
  return IS_ELF64_MIPSEL(elf) ? ELF64_MIPS_R_INFO(sym, type) : GELF_R_INFO(sym, type);
}
#else
#define gelfx_r_sym(elf,info) GELF_R_SYM(info)
#define gelfx_r_type(elf,info) GELF_R_TYPE(info)
#define gelfx_r_info(elf,sym,type) GELF_R_INFO(sym,type)
#endif

#endif	/* GELFX_H */
