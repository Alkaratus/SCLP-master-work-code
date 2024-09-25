//
// Created by kubam on 07.06.2024.
//

#ifndef MASTERS_WORK_VISITOR_H
#define MASTERS_WORK_VISITOR_H

class Box;
class Simple_Block;
class Complex_Block;


class Visitor {
public:
    virtual void visit(Box *box){};

    virtual void visit(Simple_Block *block){};

    virtual void visit(Complex_Block *block){};
};


#endif //MASTERS_WORK_VISITOR_H
