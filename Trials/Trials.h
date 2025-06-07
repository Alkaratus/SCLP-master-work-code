#ifndef TRIALS_H
#define TRIALS_H

#include <vector>
#include "Box.h"
#include "Container.h"

std::vector<std::pair<Container,std::list<Box>>> read_file(const std::string& filename);
std::pair<Container,std::list<Box>>read_pair(std::istream& stream);

void simple_packer_trial();
void tree_packer_trial();
void parallel_packer_trial();
void mock_up_trial();
void compare_packers(int file_index, int dataset);
void simple_packer_trial(int file_index, int dataset);
void tree_packer_trial(int file_index, int dataset);


#endif //TRIALS_H
