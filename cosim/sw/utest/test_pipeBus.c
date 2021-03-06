#include <stdio.h>
#include "pipeBus.h"
#include "sanity.h"
#include "log.h"

static const char gModName[] = "Test_pipeBus";

void showUsage(const char* name)
{
	printf("Usage: ");
	printf("%s <pipe in file> <pipe out file> \n", name);
}

int main (int argc, const char*argv[])
{
	pPIPE_Obj_t pPipe;
	uint32_t Data;
	
	/* Verify number of entries */
	if(argc < 2)
	{
		showUsage(gModName);
		return -1;
	}

	printf("Allocating pipe objects...\n");
	pPipe = PIPE_Alloc();

	printf("Initializing pipe objects...\n");
	if (PIPE_Init(pPipe, argv[1], argv[2]))
	{
		Warning(gModName, __LINE__, "Failure initializing simulation PIPE");
	}

	/* Put PIPE in interactive mode */
	//PIPE_Interactive(pPipe);

	printf("Reading from bus...\n");
	for (int i = 0; i < 4; i++)
	{
		PIPE_ReadFromBus(pPipe, 4*i, &Data);
		printf("...read from bus DONE! \"%d time\"\n", i);
		printf("%X\n", Data);	
	}

	printf("Writing to bus...\n");
	for(int i = 0; i < 2; i++)
	{
		PIPE_WriteToBus(pPipe, 0x10 + 4*i, 0x11111111*i + 0x12345678);
		printf("...write from bus DONE! \"%d time\"\n", i);
	}

	printf("Reading from bus...\n");
	for(int i = 0; i < 2; i++)
	{
		PIPE_ReadFromBus(pPipe, 0x10 + 4*i, &Data);
		printf("...read from bus DONE! \"%d time\"\n", i);
		printf("%X\n", Data);
	}

	printf("Finishing simulation...");
	PIPE_EndSimulation(pPipe);

	PIPE_Free(pPipe);

	/* Success */
	return 0;

failure:
	/* failure */
	PIPE_Free(pPipe);
	return -1;
}
