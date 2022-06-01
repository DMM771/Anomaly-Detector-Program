#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    sad = new HybridAnomalyDetector;
    anomalyReport = new std::vector<AnomalyReport>;
    linesNumber = new int;
    arrayCommand.push_back(new uploadCommand(dio, sad, anomalyReport, linesNumber));
    arrayCommand.push_back(new settingsCommand(dio, sad, anomalyReport, linesNumber));
    arrayCommand.push_back(new detectCommand(dio, sad, anomalyReport, linesNumber));
    arrayCommand.push_back(new resultsCommand(dio, sad, anomalyReport, linesNumber));
    arrayCommand.push_back(new analyzeCommand(dio, sad, anomalyReport, linesNumber));
    arrayCommand.push_back(new exitCommand(dio, sad, anomalyReport, linesNumber));

}

void CLI::start() {
    string choose;
    int choice = -1;
    string str = "1.";
    while (choice != 5) {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        for (int i = 0; i < arrayCommand.size(); i++) {
            str[0] = ((char)(i + 1 + '0'));
            dio->write(str);
            dio->write(arrayCommand[i]->getDescription() + "\n");
        }
        choose=dio->read();
        choice = choose[0] - '0' - 1;
        if(choice >= 0 && choice <= 6)
            arrayCommand[choice]->execute(&)
    }

}


CLI::~CLI() {
    delete sad;
    delete anomalyReport;
    delete linesNumber;
    for (Command *c :arrayCommand){
        delete c;
    }

}




//#include "CLI.h"
//
//CLI::CLI(DefaultIO *dio1)
//{
//    float p = 7;
//    option = &p;
//    dio = dio1;
//    this->cli = new cliSet();
//    commands[0] = new Upload(dio, cli);
//    commands[1] = new Settings(dio, cli);
//    commands[2] = new Detect(dio, cli);
//    commands[3] = new Display(dio, cli);
//    commands[4] = new Analyze(dio, cli);
//    commands[5] = new Exit(dio, cli);
//}
//
////activate the menu and execute every command
//void CLI::start()
//{
//    while (*option != 6)
//    {
//        dio->write("Welcome to the Anomaly Detection Server.\n");
//        dio->write("Please choose an option:\n");
//        for (int i = 0; i < 6; i++)
//        {
//            string ss = commands[i]->getName();
//            dio->write(ss);
//        }
//        dio->read(option);
//        int x = *option - 1;
//        commands[x]->execute();
//    }
//}
//
//CLI::~CLI()
//{
//    for (int i = 0; i < 6; i++)
//    {
//        delete commands[i];
//    }
//    delete commands;
//}