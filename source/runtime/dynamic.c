#include "switch/result.h"
#include "switch/kernel/svc.h"
#include <elf.h>

void __nx_dynamic(uintptr_t base, const Elf32_Dyn* dyn)
{
	const Elf32_Rela* rela = NULL;
	u64 relasz = 0;

	for (; dyn->d_tag != DT_NULL; dyn++)
	{
		switch (dyn->d_tag)
		{
			case DT_RELA:
				rela = (const Elf32_Rela*)(base + dyn->d_un.d_ptr);
				break;
			case DT_RELASZ:
				relasz = dyn->d_un.d_val / sizeof(Elf32_Rela);
				break;
		}
	}

	if (rela == NULL)
        svcExitProcess();
		//fatalSimple(MAKERESULT(Module_Libnx, LibnxError_BadReloc));

	for (; relasz--; rela++)
	{
		switch (ELF32_R_TYPE(rela->r_info))
		{
			case R_ARM_RELATIVE:
			{
				u32* ptr = (u32*)(base + rela->r_offset);
				*ptr = base + rela->r_addend;
				break;
			}
		}
	}
}
