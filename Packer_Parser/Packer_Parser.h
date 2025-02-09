//
// Created by kubam on 26.01.2025.
//

#ifndef BOXES_PARSER_H
#define BOXES_PARSER_H

#include "Packer.h"

/** Creates packer on base of provided file
     * \param filepath path to file with packer parameters
     * \return packer with boxes and container
     */
Packer generate_packer(const std::string& filepath);

#endif //BOXES_PARSER_H
