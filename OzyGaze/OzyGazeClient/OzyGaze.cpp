
#include "Statistics/Application/ApplicationStatistics.h"
#include "Statistics/Application/ProcessInfoTree.h"

#include <io.h>
#include <fcntl.h>

int main(void)
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	ApplicationStatistics applStat;
	applStat.InitializeApplInfo();
	applStat.TEST_showProcess();
	system("pause");
	return 0;
}