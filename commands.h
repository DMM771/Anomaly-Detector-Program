#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"


class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(std::string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() = default;

    // you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command {
protected:
    std::string description;
    DefaultIO *dio;
    SimpleAnomalyDetector *sad;
    std::vector<AnomalyReport> *anomalyReport;
    int *linesNumber;


public:
    Command(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport, int *linesNumber) :
            dio(dio), sad(sad), anomalyReport(anomalyReport), linesNumber(linesNumber) {}

    virtual void execute() = 0;

    virtual ~Command() {}

    std::string getDescription() {
        return description;
    };
};

class uploadCommand : public Command {
    void getFile(std::string readType) {
        dio->write("Please upload your local " + readType + " CSV file.\n");
        std::ifstream fin;
        std::string line;
        ofstream csv(readType + ".csv");
        line = dio->read();
        while (line != "done\n") {
            csv << line;
            line = dio->read();
        }
        dio->write("Upload complete.\n");
        csv.close();
    }

public:
    uploadCommand(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport,
                  int *linesNumber) : Command(dio, sad, anomalyReport, linesNumber) {
        description = "1.upload a time series csv file";
    };

    void execute() override {
        getFile("train");
        getFile("test");
    }
};

class settingsCommand : public Command {
public:
    settingsCommand(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport,
                    int *linesNumber) : Command(dio, sad, anomalyReport, linesNumber) {
        description = "2.algorithm settings";

    };

    void execute() override {
        float newThreshold;
        dio->write("The current correlation threshold is ");
        dio->write(sad->threshold);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        cout << "read1" << endl;
        newThreshold = atof(dio->read().c_str());
        cout << "read";
        // dio->read(&newThreshold);

        while (newThreshold < 0 || newThreshold > 1) {
            dio->write("please choose a value between 0 and 1\n");
            dio->write("The current correlation threshold is ");
            dio->write(sad->getThreshold());
            dio->write("\n");
            newThreshold = atof(dio->read().c_str());
//            dio->read(&newThreshold);
        }
        sad->setThreshold(newThreshold);
    }
};

class detectCommand : public Command {
public:
    detectCommand(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport,
                  int *linesNumber) : Command(dio, sad, anomalyReport, linesNumber) {
        description = "3.detect anomalies";
    }


    virtual void execute() {
        TimeSeries train("train.csv");
        TimeSeries test("test.csv");
        *linesNumber = test.getNumOfValues();
        sad->learnNormal(train);
        *anomalyReport = sad->detect(test);
        dio->write("anomaly detection complete.\n");
    }
};

class resultsCommand : public Command {
public:
    resultsCommand(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport,
                   int *linesNumber) : Command(dio, sad, anomalyReport, linesNumber) {
        description = "4.display results";
    }


    void execute() override {
        for (correlatedFeatures c: sad->getNormalModel()) {
            for (const AnomalyReport &report: *anomalyReport) {
                if (report.description == c.feature1 + "-" + c.feature2) {
                    dio->write(report.timeStep);
                    dio->write("\t" + report.description + "\n");
                }
            }
        }
        dio->write("Done.\n");
    }
};

class analyzeCommand : public Command {
    int getTimeFrames() {
        int count = 1;
        int numReports = anomalyReport->size();
        int i;
        for (i = 2; i < numReports; i++) {
            if (anomalyReport->at(i - 1).timeStep + 1 != anomalyReport->at(i).timeStep) {
                count++;
            }
        }
        if (anomalyReport->at(0).timeStep + 1 != anomalyReport->at(1).timeStep) {
            count++;
        }
        return count;
    }

public:
    analyzeCommand(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport,
                   int *linesNumber) : Command(dio, sad, anomalyReport, linesNumber) {
        description = "5.upload anomalies and analyze results";
    };

    void execute() override {
        dio->write("Please upload your local anomalies file.\n");
        int clientReporting = 0;
        int positive = 0;
        std::string line = dio->read();
        size_t pos;
        std::string delim = ",";

        // the match reporting
        float TP = 0;
        float FP = getTimeFrames();

        while (line != "done\n") {
            // the amount of reporting
            positive++;
            pos = line.find(delim);
            int start = stoi(line.substr(0, pos));
            line = line.erase(0, pos + 1);
            int end = stoi(line);
            clientReporting += end - start + 1;
            int count = 0;
            int numReport = anomalyReport->size();
            for (int i = 0; i < numReport; i++) {
                if (anomalyReport->at(i).timeStep >= start && anomalyReport->at(i).timeStep <= end) {
                    count++;
                }
            }
            if (count) {
                TP++;
                FP--;
            }

            line = dio->read();
        }
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write(TP / positive);
        dio->write("\nFalse Positive Rate: ");

        dio->write((int) (1000.0 * FP / (float) (*linesNumber - clientReporting)) / 1000.0f);
        dio->write("\n");
    }
};

class exitCommand : public Command {
public:
    exitCommand(DefaultIO *dio, SimpleAnomalyDetector *sad, std::vector<AnomalyReport> *anomalyReport, int *linesNumber)
            : Command(dio, sad, anomalyReport, linesNumber) {
        description = "6.exit";
    }

    void execute() override {}
};




// open the reading file





#endif /* COMMANDS_H_ */




//
//
//#ifndef COMMANDS_H_
//#define COMMANDS_H_
//
//#include<iostream>
//#include <string.h>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include "HybridAnomalyDetector.h"
//#include <stdlib.h>
//#include <unistd.h>
//#include <stdio.h>
//
//using namespace std;
//
//class DefaultIO{
//public:
//	virtual string read()=0;
//	virtual void write(string text)=0;
//	virtual void write(float f)=0;
//	virtual void read(float* f)=0;
//	virtual ~DefaultIO(){}
//
//	// you may add additional methods here
//};
//
//// you may add here helper classes
//
//class StandardIO : public DefaultIO
//{
//    ifstream input;
//    ofstream output;
//
//public:
//    StandardIO(string inputFile, string outputFile)
//    {
//        input.open(inputFile);
//        output.open(outputFile);
//    }
//
//    virtual void write(string str)
//    {
//        cout << str << endl;
//        output << str;
//    }
//
//    virtual void write(float num)
//    {
//        cout << num << endl;
//        output << num;
//    }
//    virtual string read()
//    {
//        string str;
//        input >> str;
//        return str;
//    }
//    virtual void read(float *num)
//    {
//        input >> *num;
//    }
//    void close()
//    {
//        if (input.is_open())
//            input.close();
//        if (output.is_open())
//            output.close();
//    }
//
//    ~StandardIO()
//    {
//        close();
//    }
//};
//
//class socketIo : public DefaultIO{
//    int fd;
//public:
//    socketIo(int fd){
//        this->fd = fd;
//    }
//    virtual void write(string str){
//        ::write(fd, str.c_str(), str.length());
//    }
//    virtual void write(float num){
//        ::write(fd, &num, sizeof(num));
//    }
//    virtual string read(){
//        string input = "";
//        char c = 0;
//        while (c != '\n'){
//            input += c;
//            ::read(fd, &c, sizeof(c));
//        }
//        return input;
//    }
//    virtual  void read(float *num){
//        string input = "";
//        char c = 0;
//        while (c != '\n') {
//            input += c;
//            ::read(fd, &c, sizeof(c));
//        }
//        *num = stof(input);
//    }
//};
//class cliSet
//{
//public:
//    cliSet()
//    {
//        this->csvTest = nullptr;
//        this->csvTrain = nullptr;
//        HybridAnomalyDetector *hybDetector = new HybridAnomalyDetector();
//        this->detector = hybDetector;
//    }
//    TimeSeries *csvTrain;
//    TimeSeries *csvTest;
//    HybridAnomalyDetector *detector;
//    void setCsvTrain(const char* name){
//        TimeSeries *ts = new TimeSeries(name);
//        csvTrain = ts;
//    }
//    void setCsvTest(const char* name){
//        TimeSeries *ts = new TimeSeries(name);
//        csvTest = ts;
//    }
//    void setDetector(float threashold){
//        this->detector->setThreashold(threashold);
//    }
//    ~cliSet()
//    {
//        delete csvTest;
//        delete csvTrain;
//        delete detector;
//    }
//};
//
//// you may edit this class
//class Command{
//protected:
//	DefaultIO* dio;
//    string name;
//    cliSet* cli;
//public:
//	Command(DefaultIO* dio,cliSet *cli) : dio(dio), cli(cli){}
//    Command() {}
//    virtual string getName(){
//        return name;
//    }
//	virtual void execute()=0;
//	virtual ~Command(){}
//};
//
//// implement here your command classes
//
//class Upload : public Command
//{
//public:
//    Upload(DefaultIO* dio,cliSet* cliSet) : Command(dio, cliSet){
//        Command::name = "1.upload a time series csv file\n";
//    }
//    string read(){
//        string str1, str2;
//        while(str1 != "done"){
//            str1 = dio->read();
//            str2 += str1 + "\n";
//        }
//        return str2;
//    }
//    void execute(){
//        ofstream file1;
//        ofstream file2;
//        file1.open("Train.csv");
//        file2.open("Test.csv");
//        string trainFile;
//        string testFile;
//        dio->write("Please upload your local train CSV file.\n");
//        trainFile = read();
//        file1 << trainFile;
//        dio->write("Upload complete.\n");
//        dio->write("Please upload your local test CSV file.\n");
//        testFile = read();
//        file2 << testFile;
//        dio->write("Upload complete.\n");
//        file1.close();
//        file2.close();
//        this->cli->setCsvTrain("Train.csv");
//        this->cli->setCsvTest("Test.csv");
//    }
//};
//
//class Settings : public Command{
//public:
//    Settings(DefaultIO *dio,cliSet *cliSet) : Command(dio, cliSet)
//    {
//        Command::name = "2.algorithm settings\n";
//    }
//    void execute(){
//        float f = 7;
//        float* newThreashold = &f;
//        dio->write("The current correlation threshold is 0.9\n");
//        dio->read(newThreashold);
//        while (*newThreashold < 0 || *newThreashold > 1){
//            dio->write("Please choose a value between 0 and 1.\n");
//            dio->write("Please insert new threashold.");
//            dio->read(newThreashold);
//        }
//        this->cli->setDetector(*newThreashold);
//    }
//};
//
//class Detect : public Command{
//public:
//    Detect(DefaultIO *dio, cliSet *cliSet) : Command(dio, cliSet)
//    {
//        Command::name = "3.detect anomalies.\n";
//    }
//    void execute(){
//        this->cli->detector->learnNormal(*this->cli->csvTrain);
//        dio->write("Anomaly detection complete.\\n");
//    }
//};
//class Display : public Command{
//public:
//    Display(DefaultIO *dio,cliSet *cliSet) : Command(dio,cliSet){
//        Command::name = "4.display results\n";
//    }
//    void execute(){
//        vector<AnomalyReport> reports = this->cli->detector->detect(*this->cli->csvTest);
//        for(AnomalyReport ar : reports){
//            dio->write(to_string(ar.timeStep) + "\t" + ar.description + "\n");
//        }
//        dio->write("Done.\n");
//    }
//};
//
//class Analyze : public Command{
//public:
//    Analyze(DefaultIO *dio,cliSet *cliSet) : Command(dio, cliSet)
//    {
//        Command::name = "5.upload anomalies and analyze results\n";
//    }
//    void execute(){
//        dio->write("Please upload your local anomalies file.\n");
//        map<string, vector<pair<long, long>>> anomalies = uniteAnomalies();
//        dio->write("Upload complete.\n");
//        long positive = 0;
//        long num1, num2;
//        string str = dio->read();
//        vector<pair<long, long>> timeSteps;
//        while (str != "done"){
//            positive++;
//            stringstream line(str);
//            string num;
//            getline(line, num,',');
//            num1 = stol(num);
//            getline(line, num, ',');
//            num2 = stol(num);
//            pair<long, long> p;
//            p.first = num1;
//            p.second = num2;
//            timeSteps.push_back(p);
//            str = dio->read();
//        }
//        long negative = this->cli->csvTest->result[0].second.size();
//        long reportsSum;
//        for(int i = 0; i < timeSteps.size(); i++){
//            reportsSum += timeSteps.at(i).second - timeSteps.at(i).first + 1;
//        }
//        negative -= reportsSum;
//        double TP = 0, FP = 0;
//        long FPCount = 0;
//        for(map<string, vector<pair<long, long>>>::iterator it = anomalies.begin(); it != anomalies.end(); ++it){
//            for(int i = 0;i < it->second.size(); i++){
//                FP++;
//                for(int j = 0; j < timeSteps.size(); j++){
//                    if(!(it->second.at(i).first > timeSteps.at(j).second || timeSteps.at(j).first > it->second.at(i).second)){
//                        TP++;
//                        FPCount++;
//                    }
//                }
//            }
//        }
//        double TPR = TP / positive;
//        TPR = floor(TPR * 1000);
//        TPR = TPR / 1000;
//        FP -= FPCount;
//        double AFR = FP / negative;
//        AFR = floor(AFR * 1000);
//        AFR = AFR / 1000;
//        stringstream ss;
//        ss << TPR;
//        string str1 = ss.str();
//        this->dio->write("True Positive Rate: " + str1 + '\n');
//        stringstream ss1;
//        ss1 << AFR;
//        string str2 = ss1.str();
//        this->dio->write("False Positive Rate: " + str2 + '\n');
//    }
//    map<string, vector<pair<long,long>>> uniteAnomalies(){
//        map<string, vector<pair<long, long>>> anomalies;
//        vector<pair<long, long>> timeSteps;
//        vector<AnomalyReport> reports = this->cli->detector->detect(*cli->csvTest);
//        long index1, index2, final;
//        string report1, report2;
//        index1 = reports.at(0).timeStep;
//        report1 = reports.at(0).description;
//        final = index1;
//        for(int i = 0; i < reports.size(); i++){
//            if(i != reports.size() - 1){
//                index2 = reports.at(i + 1).timeStep;
//                report2 = reports.at(i + 1).description;
//            }
//            if(report1 == report2 && (final + 1 == index2)){
//                final++;
//                continue;
//            }
//            else
//            {
//                pair<long, long> pair(index1, final);
//                timeSteps.push_back(pair);
//                anomalies.insert({report1, timeSteps});
//                report1 = report2;
//                index1 = index2;
//                final = index1;
//                timeSteps.clear();
//            }
//        }
//        return anomalies;
//    }
//};
//
//class Exit : public Command{
//public:
//    Exit(DefaultIO *dio, class cliSet *cliSet) : Command(dio, cliSet){
//        Command::name = "6.exit\n";
//    }
//    void execute()
//    {
//        return;
//    }
//};
//
//#endif /* COMMANDS_H_ */
