
#include "Statistics/Application/ApplicationStatistics.h"
#include "Statistics/Application/ProcessInfo.h"

#include <io.h>
#include <fcntl.h>

int main(void)
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	

	ApplicationStatistics applStat;

	while (true)
	{
		applStat.GetForegroundWndInfo();
		Sleep(500);
	}

	/*applStat.InitializeApplInfo();
	applStat.TEST_showProcess();*/
	system("pause");
	return 0;
}