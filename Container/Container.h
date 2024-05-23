//
// Created by kubam on 01.03.2024.
//

#ifndef MASTERS_WORK_CONTAINER_H
#define MASTERS_WORK_CONTAINER_H

#include <list>
#include "Element_3D.h"
#include "Free_Space.h"
#include "Insertable_Element.h"
#include "Insertion_Coordinates.h"

typedef Free_Space External_Free_Space;

class Container:public Element_3D {
    enum anchor_place_type{
        start, wider, deeper, end
    };

    class Free_Space:public External_Free_Space{
        std::list<std::shared_ptr<Free_Space>> related_free_spaces;
        Container* owner;
    protected:

    public:
        Container* get_owner();

        Free_Space(Point_3D start, unsigned int width, unsigned int depth, unsigned int height, Container* owner);
        virtual Point_3D get_anchor_corner();

        [[nodiscard]] virtual std::array<unsigned int,3>get_anchor_lengths() const;
        std::list<std::shared_ptr<Free_Space>> get_related_free_spaces();
        void add_related_free_space(const std::shared_ptr<Free_Space>& free_space);
        void remove_related_free_space(const std::shared_ptr<Free_Space>& free_space);

        virtual Insertion_Coordinates get_insertion_coordinates_for_element(Insertable_Element *element);
        virtual std::shared_ptr<Free_Space> get_slice_on_left_from(unsigned int point_x_value);
        virtual std::shared_ptr<Free_Space> get_slice_on_right_from(unsigned int point_x_value);
        virtual std::shared_ptr<Free_Space> get_slice_on_front_from(unsigned int point_z_value);
        virtual std::shared_ptr<Free_Space> get_slice_on_back_from(unsigned int point_z_value);

        bool do_point_affects(const Point_2D &point);

        virtual std::shared_ptr<Free_Space> create_free_space_above_inserted_element(const Insertion_Coordinates &inserted_element_coordinates);
        virtual std::list<std::shared_ptr<Free_Space>> add_free_spaces_on_sides_of_inserted_element(Insertion_Coordinates &inserted_element_coordinates);
        virtual std::list<std::shared_ptr<Free_Space>> create_free_space_from_related_free_spaces(Insertion_Coordinates inserted_element_coordinates);

        [[nodiscard]] std::string get_properties() const override;
    };

    class Anchored_Free_Space:public Free_Space{
        class Anchor{
            anchor_place_type place;
            Point_3D corner;
            std::array<unsigned int,3> lengths;
        public:
            Anchor(Point_3D corner,std::array<unsigned int,3> lengths,anchor_place_type place);
        };
        anchor_place_type anchor_place;
        Point_3D anchor_corner;
        std::array<unsigned int,3>anchor_lengths;
    public:
        Anchored_Free_Space(Point_3D start, unsigned int width, unsigned int depth, unsigned int height, Container* owner);
        anchor_place_type get_anchor_corner_type();
        Point_3D get_anchor_corner() override;
        [[nodiscard]] std::array<unsigned int,3>get_anchor_lengths() const override;
        Insertion_Coordinates get_insertion_coordinates_for_element(Insertable_Element *element) override;

        std::shared_ptr<Free_Space> get_slice_on_left_from(unsigned int point_x_value) override;
        std::shared_ptr<Free_Space> get_slice_on_right_from(unsigned int point_x_value) override;
        std::shared_ptr<Free_Space> get_slice_on_front_from(unsigned int point_z_value) override;
        std::shared_ptr<Free_Space> get_slice_on_back_from(unsigned int point_z_value) override;

        std::shared_ptr<Free_Space> create_free_space_above_inserted_element(const Insertion_Coordinates &inserted_element_coordinates) override;
        std::list<std::shared_ptr<Free_Space>> add_free_spaces_on_sides_of_inserted_element(Insertion_Coordinates &inserted_element_coordinates) override;
        std::list<std::shared_ptr<Free_Space>> create_free_space_from_related_free_spaces(Insertion_Coordinates inserted_element_coordinates) override;


    };

    std::list<std::shared_ptr<Free_Space>>free_spaces;
    Element_3D element_max_sizes;

    static void add_relation_between_free_spaces(const std::shared_ptr<Free_Space>& first, const std::shared_ptr<Free_Space>& second);

    static bool compare_free_spaces_anchors_lengths(std::shared_ptr<Container::Free_Space>& first,std::shared_ptr<Container::Free_Space> &last);
    static bool compare_free_spaces_by_y_coordinate(std::shared_ptr<Container::Free_Space>& first, std::shared_ptr<Container::Free_Space> &last);
    static void mark_relations_between_free_spaces_in_lists(std::list<std::shared_ptr<Free_Space>> &first, std::list<std::shared_ptr<Free_Space>>& second);

    void remove_inserted_spaces_from_list(std::list<std::shared_ptr<Free_Space>> &free_spaces);
    std::list<std::shared_ptr<Free_Space>>::iterator get_first_element_with_y_coordinate(unsigned int y);
    std::list<std::shared_ptr<Free_Space>>::iterator get_last_element_with_same_y_after(std::list<std::shared_ptr<Free_Space>>::iterator first);
    ///Removes spaces which are inserted in another
    void remove_inserted_spaces();

public:
    Container(unsigned int width,unsigned int depth,unsigned int height);
    Container(const Container& other);
    bool have_free_space_aveable();
    std::list<std::shared_ptr<Free_Space>>::iterator select_free_space();
    Insertion_Coordinates insert_element_into_free_space(std::list<std::shared_ptr<Free_Space>>::iterator free_space,Insertable_Element* element);
    ///Checks if element can't be definitely inserted into container BUT it doesn't check can element be inserted
    bool cant_element_be_inserted(Insertable_Element* element);
    void remove_free_space(std::shared_ptr<Free_Space> &space);
    std::string get_text_list_of_free_spaces();
    void make_merges_for_new(std::shared_ptr<Free_Space> &space);
    std::shared_ptr<Free_Space> create_merge_in_x(const std::shared_ptr<Free_Space>& first,const std::shared_ptr<Free_Space>& second);
    std::shared_ptr<Free_Space> create_merge_in_z(const std::shared_ptr<Free_Space>& first,const std::shared_ptr<Free_Space>& second);
};

bool is_first_anchor_smaller_than_second(std::array<unsigned int,3> first, std::array<unsigned int,3> second);

#endif //MASTERS_WORK_CONTAINER_H
