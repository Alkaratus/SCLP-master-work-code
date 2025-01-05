//
// Created by kubam on 11.03.2024.
//

#ifndef MASTERS_WORK_BLOCK_ELEMENTS_NUMBERS_H
#define MASTERS_WORK_BLOCK_ELEMENTS_NUMBERS_H


class Block_Elements_Numbers {
    unsigned int elements_number_in_width;
    unsigned int elements_number_in_depth;
    unsigned int elements_number_in_height;
public:
    Block_Elements_Numbers(unsigned int elements_in_width, unsigned int elements_in_depth,
                           unsigned int elements_in_height);
    [[nodiscard]] unsigned int get_elements_number_in_block() const;
    [[nodiscard]] unsigned int get_elements_number_in_width() const;
    [[nodiscard]] unsigned int get_elements_number_in_depth() const;
    [[nodiscard]] unsigned int get_elements_number_in_height() const;

    void set_elements_number_in_width(unsigned int number);
    void set_elements_number_in_depth(unsigned int number);
    void set_elements_number_in_height(unsigned int number);

};


#endif //MASTERS_WORK_BLOCK_ELEMENTS_NUMBERS_H
