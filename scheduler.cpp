#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<charconv>

using namespace std;

//////////////// CLASSES ////////////////
class process {
    string name;
    int arrivalTime, processingTime, rank, originalIndex, response, turnAround;

public:
    void setName(string name) {
        this->name = name;
    }

    void setArrival(int arrival) {
        this->arrivalTime = arrival;
    }

    void setProcessing(int processTime) {
        this->processingTime = processTime;
    }

    void setRank(int rank) {
        this->rank = rank;
    }

    void setOriginalIndex(int index) {
        this->originalIndex = index;
    }

    void setResponse(int response) {
        this->response = response;
    }

    void setTurnAround(int turnAround) {
        this->turnAround = turnAround;
    }

    string getName() {
        return this->name;
    }

    int getArrival() {
        return this->arrivalTime;
    }

    int getProcessing() {
        return this->processingTime;
    }

    int getRank() {
        return this->rank;
    }

    int getOriginalIndex() {
        return this->originalIndex;
    }

    int getResponse() {
        return this->response;
    }

    int getTurnAround() {
        return this->turnAround;
    }

    process copy(process& aProcess) {
        aProcess.setName(this->getName());
        aProcess.setArrival(this->getArrival());
        aProcess.setProcessing(this->getProcessing());
        aProcess.setRank(this->getRank());
        aProcess.setOriginalIndex(this->getOriginalIndex());

        return aProcess;
    }

};

//////////////// GLOBAL VECTORS AND VARIABLES ////////////////
vector<process> processesVec;
vector<process> ready;
vector<process> waiting;
int totalTime = 0;

//////////////// FUNCTIONS ////////////////
void sortVectorByArrivalTime() {

    for (int pass = 1; pass < processesVec.size(); pass++) {
        for (int i = 1; i < processesVec.size(); i++) {
            if (processesVec[i].getArrival() < processesVec[i - 1].getArrival()) {
                process temp = processesVec[i - 1];
                processesVec[i - 1] = processesVec[i];
                processesVec[i] = temp;
            }
        }

    }

}

void copyProcessesVector(vector<process> target, vector<process>& copyVec) {

    for (int i = 0; i < target.size(); i++) {
        process aProcess;
        copyVec.push_back(target[i].copy(aProcess));
    }

}

void scheduler() {

    vector<process> processesCopy;
    copyProcessesVector(processesVec, processesCopy);

    for (int i = 0; i < processesVec.size(); i++) {
        bool isWaiting = false;
        for (int j = 0; j < processesCopy.size(); j++) {

            if (processesCopy[j].getArrival() <= totalTime) {
                isWaiting = true;
                waiting.push_back(processesCopy[j]);
            }

            else {
                if (j == processesCopy.size() - 1 && isWaiting != true) {
                    int lowestIndex = 0;
                    for (int k = 1; k < processesCopy.size(); k++) {
                        if (processesCopy[k].getArrival() == processesCopy[lowestIndex].getArrival()) {
                            if (processesCopy[k].getRank() < processesCopy[lowestIndex].getRank()) lowestIndex = k;

                        }
                        else break;
                    }
                
                    totalTime = processesCopy[lowestIndex].getArrival();
                    processesCopy[lowestIndex].setResponse(totalTime - processesCopy[lowestIndex].getArrival());
                    totalTime += processesCopy[lowestIndex].getProcessing();
                    processesCopy[lowestIndex].setTurnAround(totalTime - processesCopy[lowestIndex].getArrival());
                    ready.push_back(processesCopy[lowestIndex]);
                    processesCopy.erase(processesCopy.begin() + lowestIndex);

                }

            }

        }

        if (waiting.size() != 0) {

            int lowestIndex = 0;
            int lowestRank = waiting[0].getRank();
            for (int j = 1; j < waiting.size(); j++) {
                if (waiting[j].getRank() < lowestRank) {
                    lowestRank = waiting[j].getRank();
                    lowestIndex = j;
                }
            }

            waiting[lowestIndex].setResponse(totalTime - waiting[lowestIndex].getArrival());
            totalTime += waiting[lowestIndex].getProcessing();
            waiting[lowestIndex].setTurnAround(totalTime - waiting[lowestIndex].getArrival());
            ready.push_back(waiting[lowestIndex]);

            for (int k = 0; k < processesCopy.size(); k++) {
                if (waiting[lowestIndex].getName() == processesCopy[k].getName()) processesCopy.erase(processesCopy.begin() + k);
            }

            waiting.clear();

        }

    }

}

void readDataFromFile(string fileName) {

    string line;
    ifstream file(fileName);

    if (file.is_open()) {

        int iterations = 0; // to get rid off the first line in the file which is the number of processes
        while (getline(file, line)) {

            // geting the values of each line(process)
            int infoCnt = 0, maxCnt = 4;
            string info[4];
            string data = "";

            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '\t' && data == "") continue;
                if (line[i] == '\t' && data != "") {
                    info[infoCnt] = data;
                    infoCnt++;
                    data = "";
                }
                if (line[i] != '\t') data += line[i];

                if (i == line.size() - 1) info[infoCnt++] = data;

                if (infoCnt == maxCnt) break;
            }

            // storing each process data in an object and adding the object to a vector
            if (iterations != 0) {
                process aProcess;
                int i[3];
                from_chars(info[1].data(), info[1].data() + info[1].size(), i[0]);
                from_chars(info[2].data(), info[2].data() + info[2].size(), i[1]);
                from_chars(info[3].data(), info[3].data() + info[3].size(), i[2]);
                aProcess.setName(info[0]);
                aProcess.setArrival(i[0]);
                aProcess.setProcessing(i[1]);
                aProcess.setRank(i[2]);
                aProcess.setOriginalIndex(iterations - 1);

                processesVec.push_back(aProcess);

            }
            iterations++;
        }


        file.close();
    }

    else {
        cout << "file is not open";
    }

}

void writeDataOnAFile(string fileName) {

    ofstream file(fileName);

    if (file.is_open()) {

        for (int i = 0; i < ready.size(); i++) {
            file << ready[i].getName() << ": " << "(response= " << ready[i].getResponse() << ", turnaround= " << ready[i].getTurnAround() << ", delay= " << ready[i].getTurnAround() - ready[i].getProcessing() << ")" << endl;
        }

        for (int i = 0; i < ready.size(); i++)
        {
            file << ready[i].getName();
        }

        file.close();
        cout << "A file with the name (" << fileName << ") has been created." << endl;
    }
    else {
        cout << "file is not open";
    }



}

//////////////// MAIN ////////////////
int main() {

    readDataFromFile("in.txt");
    sortVectorByArrivalTime();
    scheduler();
    writeDataOnAFile("out.txt");

    return 0;
}
