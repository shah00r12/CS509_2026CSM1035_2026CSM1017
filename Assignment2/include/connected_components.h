#ifndef CONNECTED_COMPONENTS_H
#define CONNECTED_COMPONENTS_H

#include "csr_graph.h"
#include <vector>

struct ConnectedComponentsResult {
    int num_components = 0;
    std::vector<int> component;
};

ConnectedComponentsResult find_connected_components(const CSRGraph &g);

#endif
