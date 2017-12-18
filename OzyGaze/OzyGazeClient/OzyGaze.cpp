
#include "Statistics/Application/ApplicationStatistics.h"
#include "Statistics/Application/ProcessInfo.h"

#include <io.h>
#include <fcntl.h>

int main(void)
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	

	system("pause");
	return 0;
}