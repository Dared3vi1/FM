//
// Created by Максим Черкасов on 17.04.2021.
//

#include "hg.h"


HyperGraph::HyperGraph(const string &fileName, ostream &log_):
        log{log_}
{
    fstream graph_file;
    graph_file.open(fileName);
    if (!graph_file) {
        throw runtime_error("Could not open hg file");
    }
    ofstream console_log;
    console_log.open("../logs/console.log", ios::out);
    if (!console_log) {
        throw runtime_error("Could not open console log file");
    }

    console_log << "--------------------------------" << std::endl;
    console_log << "HG file have been already opened!" << std::endl;
    console_log << "--------------------------------" << std::endl;

    std::string reading_string;
    std::getline(graph_file, reading_string);
    istringstream line(reading_string);
    line >> edge_num >> vertNum;
    console_log << "HyperGraph contains " << edge_num << " edges and " << vertNum << " vertices" << std::endl;

    verts = vector<vector<int>>(vertNum);
    edges = vector<vector<int>>(edge_num);

    ofstream hyperedges;
    hyperedges.open("../logs/hyperedges.txt", ios::out);
    if(!hyperedges) {
        throw ("Could not open testing output file");
    }
    ofstream vertices;
    vertices.open("../logs/vertices.txt", ios::out);
    if(!vertices) {
        throw ("Could not open testing output file");
    }

    int i = 0;
    int tmp_vert = 0;
    while (std::getline(graph_file, reading_string)) {
        istringstream line(reading_string);
        while (line >> tmp_vert) {
            verts[tmp_vert-1].push_back(i);
            edges[i].push_back(tmp_vert-1);
//            console_log << "Vert: " << i << std::endl;
//            console_log << "Edge:" << tmp_vert << std::endl;
        }
//        console_log << "--------------------------------" << std::endl;
        i++;
    }

    console_log << "--------------------------------" << std::endl;
    console_log << i << " edges have been read" << std::endl;

//     Test output
    for (const auto& edge_: edges) {
        for (const auto& edge: edge_) {
            hyperedges << edge << " ";
        }
        hyperedges << std::endl;
    }
    for (const auto& vert_: verts) {
        for (const auto& vert: vert_) {
            vertices << vert << " ";
        }
        vertices << std::endl;
    }

    graph_file.close();
    hyperedges.close();
    vertices.close();

}