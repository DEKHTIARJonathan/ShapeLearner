

#ifdef __cplusplus
extern "C" {
#endif

//__declspec(dllexport) bool openDataBase(string initFile = "")
__declspec(dllexport) void openDataBase(char* _dbUser, char* _dbPass, char* _dbName, char* _dbHost, unsigned int _dbPort, char* _dbInit = "") ;

//__declspec(dllexport) void initMatcher() ;

__declspec(dllexport) void signBinaryImage(char* _imgPath, char* _imgClass, unsigned int _jobID) ;

__declspec(dllexport) unsigned int getActiveThread() ;

__declspec(dllexport) void waitBeforeClosing() ;

#ifdef __cplusplus
}
#endif

