//   Utils.h

#ifndef	__MFXC_UTILS
#define	__MFXC_UTILS

class CUtils
{
public:
     static void getargs (char *command_line, int *argc, char *argv[], int max_args);
     static int GetPointSize (const LOGFONT* lf, CDC* pDC = NULL);
     static int GetPointSize (const LOGFONT& lf, CDC* pDC = NULL);
};

#endif	// __MFXC_UTILS