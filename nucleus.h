#ifndef NUCLEUS_H
#define NUCLEUS_H

#include <QColor>
#include <QVector>

class Nucleus
{
public:
    Nucleus();
    void inc_green();
    void inc_red();
    void inc_black();

    void calculate_perimeter();
    void calculate_square();
    void calculate_form_factor();

    void add_color(QColor color);

    void calc_aver_and_range();
    void calc_std_deviation();

    double return_perimeter();
    int return_square();
    double return_form_factor();

    int return_r_average();
    int return_g_average();
    int return_b_average();

    int return_r_range();
    int return_g_range();
    int return_b_range();

    void reset_all();

private:
    int green_border_;
    int red_border_;
    int black_body_;

    double perim_factor_;
    double perimeter_;
    int square_;
    double form_factor_;

    QVector<int> r_vect_;
    QVector<int> g_vect_;
    QVector<int> b_vect_;
    int r_max_, r_min_, g_max_, g_min_, b_max_, b_min_;
    int r_range_, g_range_, b_range_;
    int r_average_, g_average_, b_average_, r_std_dev_, g_std_dev_, b_std_dev_;
    int r_temp_to_aver_, g_temp_to_aver_, b_temp_to_aver_;

};

#endif // NUCLEUS_H
