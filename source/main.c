#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "switch.h"
#include "twili.h"

#define GET_INFO(a, b, c, d) do { \
	rc = svcGetInfo(a, b, c, d); \
	if (rc) { \
		printf("Error: %" PRIx32 "\n", rc); \
		return 1; \
	} \
	} while (0)


int main(int argc, char **argv)
{
	size_t addr = 0;
	MemoryInfo info;
	u32 pageinfo;
	u64 alias_region_addr;
	u64 alias_region_size;
	u64 heap_region_addr;
	u64 heap_region_size;
	u64 addr_space_addr;
	u64 addr_space_size;
	u64 stack_region_addr;
	u64 stack_region_size;
	u32 rc;

	smInitialize();
	twiliInitialize();

	GET_INFO(&alias_region_addr, 2, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&alias_region_size, 3, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&heap_region_addr, 4, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&heap_region_size, 5, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&addr_space_addr, 12, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&addr_space_size, 13, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&stack_region_addr, 14, CUR_PROCESS_HANDLE, 0);
	GET_INFO(&stack_region_size, 15, CUR_PROCESS_HANDLE, 0);
	printf("INFOS:\n");
	printf("alias_region = %016" PRIx64 "-%016" PRIx64 "\n", alias_region_addr, alias_region_addr + alias_region_size);
	printf("heap_region = %016" PRIx64 "-%016" PRIx64 "\n", heap_region_addr, heap_region_addr + heap_region_size);
	printf("stack_region = %016" PRIx64 "-%016" PRIx64 "\n", stack_region_addr, stack_region_addr + stack_region_size);
	printf("addr_space = %016" PRIx64 "-%016" PRIx64 "\n", addr_space_addr, addr_space_addr + addr_space_size);
	printf("\nMEMORY LAYOUT\n");
	do {
		rc = svcQueryMemory(&info, &pageinfo, addr);
		if (rc) {
			printf("Error: %" PRIx32 "\n", rc);
			return 1;
		}
		printf("%016" PRIx64 "-%016" PRIx64 ": %" PRIx32 " %" PRIx32 " %" PRIo32 " %" PRId32 " %" PRId32 "\n", info.addr, info.addr + info.size, info.type, info.attr, info.perm, info.device_refcount, info.ipc_refcount);
		addr = info.addr + info.size;
	} while(addr != 0);

	twiliExit();
	smExit();

	return 0;
}
