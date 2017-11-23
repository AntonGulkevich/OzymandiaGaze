
#include "Statistics/Application/ApplicationStatistics.h"
#include "Statistics/Application/ApplicationInfo.h"

#include <io.h>
#include <fcntl.h>

int main(void)
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	ApplicationStatistics applStat;
	applStat.InitializeProcessList();
	applStat.TEST_showProcess();
	system("pause");
	return 0;
}