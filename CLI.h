#ifndef CLI_H_
#define CLI_H_

#include <string>
#include "commands.h"


class CLI {
    DefaultIO *dio;
    vector<Command*> arrayCommand;
    HybridAnomalyDetector* sad;
    vector<AnomalyReport> *anomalyReport;
    int *linesNumber;


public:
    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();
};

#endif /* CLI_H_ */








//
//
//#ifndef CLI_H_
//#define CLI_H_
//
//#include <string.h>
//#include "commands.h"
//#include <array>
//#include "minCircle.h"
//
//using namespace std;
//
//class CLI {
//public:
//	DefaultIO* dio;
//    Command **commands = new Command*[6];
//    cliSet* cli;
//    float* option;
//	CLI(DefaultIO* dio);
//	void start();
//	virtual ~CLI();
//};
//
//#endif /* CLI_H_ */
