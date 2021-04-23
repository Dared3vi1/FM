#include "src/hg.h"
#include "src/partition.h"
#include "src/gc.h"
#include "src/fm.h"

#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    bool modified {false};
    if (argc < 2) {
        cout << "File not found" << endl;
        return -1;
    }
    int tolerance = 0;
    if (argc == 3) {
        tolerance = atoi (argv[2]);
    }
    if (argc == 4) {
        modified = true;
    }
    string inFileName = string(argv[1]);
    string outFileName = inFileName + ".part.2";
    string logFileName= inFileName + ".log";
    ofstream log;
    log.open(logFileName);
    HyperGraph hg(inFileName, log);
    Partition partitions(hg, false, tolerance);
    auto start = high_resolution_clock::now();
    int epoch = FM(partitions, modified);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    partitions.print(outFileName);
    log << "Time of execution is " << duration.count() << " milliseconds" << endl;
    log.close();
    cout << "Test: "              << inFileName                          << endl;
    cout << " Num of vertecies: " << hg.vertNum                         << endl;
    cout << " Num of edges: "     << hg.edge_num                         << endl;
    cout << " balance: "          << partitions.balance                  << endl;
    cout << " final cut cost: "   << partitions.solutionCost            << endl;
    cout << " execution time: "   << duration.count() << " milliseconds" << endl;
    cout << " iterations: "       << epoch                               << endl;
    return 0;


    return 0;
}

