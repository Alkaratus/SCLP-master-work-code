#ifndef TRIALS_H
#define TRIALS_H

#include <vector>
#include "Box.h"
#include "Container.h"

std::vector<std::pair<Container,std::list<Box>>> read_file(const std::string& filename);

void simple_packer_trial();
void tree_packer_trial();

#endif //TRIALS_H
