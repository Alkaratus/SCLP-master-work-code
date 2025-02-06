//
// Created by kubam on 02.03.2024.
//

#ifndef MASTERS_WORK_POINT_2D_H
#define MASTERS_WORK_POINT_2D_H


class Point_2D {
    unsigned int x;
    unsigned int z;
public:
    explicit Point_2D(unsigned int x=0,unsigned int z=0);

    [[nodiscard]] unsigned int get_x() const;
    [[nodiscard]] unsigned int get_z() const;

    void set_x(unsigned int x);
    void set_z(unsigned int z);

    Point_2D& operator=(const Point_2D& other);
    bool operator==(const Point_2D& other) const;
};


#endif //MASTERS_WORK_POINT_2D_H
