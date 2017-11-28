
#include "Statistics/Application/ApplicationStatistics.h"
<<<<<<< HEAD
#include "Statistics/Application/ProcessInfoTree.h"
=======
#include "Statistics/Application/ApplicationInfo.h"
>>>>>>> parent of 6c61b94... cnt

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