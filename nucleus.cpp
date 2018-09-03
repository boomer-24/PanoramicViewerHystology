#include "nucleus.h"

Nucleus::Nucleus()
{
    this->green_border_ = 0;
    this->red_border_ = 0;
    this->black_body_ = 0;
    this->perim_factor_ = 0.41421; // 2 - sqrt(2)
    this->perimeter_ = 0.0;
    this->square_ = 0;
    this->form_factor_ = 0.0;

    r_max_ = g_max_ = b_max_ = 0;
    r_min_ = g_min_ = b_min_ = 250;
    r_range_ = g_range_ = b_range_ = 0;
    r_temp_to_aver_ = g_temp_to_aver_ = b_temp_to_aver_ = 0;
    r_average_ = g_average_ = b_average_ = 0;
}

void Nucleus::inc_green()
{
    this->green_border_++;
}

void Nucleus::inc_red()
{
    this->red_border_++;
}

void Nucleus::inc_black()
{
    this->black_body_++;
}

void Nucleus::calculate_perimeter()
{
//    perimeter = green_border + red_border * perim_factor;
//    perimeter = (green_border + red_border) + (green_border - (red_border + green_border)) * (sqrt(2) / 3);
    this->perimeter_ = green_border_ + red_border_ * this->perim_factor_;
}

void Nucleus::calculate_square()
{
    this->square_ = green_border_ + red_border_ + black_body_;
}

void Nucleus::calculate_form_factor()
{
    this->form_factor_ = (perimeter_ / sqrt(square_)) / (sqrt(3.14) * 2);
}

double Nucleus::return_perimeter()
{
    return this->perimeter_;
}

int Nucleus::return_square()
{
    return this->square_;
}

double Nucleus::return_form_factor()
{
    return this->form_factor_;
}

void Nucleus::add_color(QColor color)
{
    this->r_vect_.push_back(color.red());
    this->g_vect_.push_back(color.green());
    this->b_vect_.push_back(color.blue());

    this->r_temp_to_aver_ += color.red();
    this->g_temp_to_aver_ += color.green();
    this->b_temp_to_aver_ += color.blue();

    if (color.red() > r_max_)
        r_max_ = color.red();
    if (color.red() < r_min_)
        r_min_ = color.red();
    if (color.green() > g_max_)
        g_max_ = color.green();
    if (color.green() < g_min_)
        g_min_ = color.green();
    if (color.blue() > b_max_)
        b_max_ = color.blue();
    if (color.blue() < b_min_)
        b_min_ = color.blue();
}

void Nucleus::calc_aver_and_range()
{
    this->r_average_ = r_temp_to_aver_ / r_vect_.size();
    this->g_average_ = g_temp_to_aver_ / g_vect_.size();
    this->b_average_ = b_temp_to_aver_ / b_vect_.size();
    r_range_ = r_max_ - r_min_;
    g_range_ = g_max_ - g_min_;
    b_range_ = b_max_ - b_min_;
}

void Nucleus::calc_std_deviation()
{
    int r_temp_sum = 0;
    int g_temp_sum = 0;
    int b_temp_sum = 0;
    int size_vect = r_vect_.size();
    for (int i = 0; i < size_vect; i++)
    {
        r_temp_sum += pow(r_vect_.at(i) - r_average_, 2);
        g_temp_sum += pow(g_vect_.at(i) - g_average_, 2);
        b_temp_sum += pow(b_vect_.at(i) - b_average_, 2);
    }
    r_std_dev_ = sqrt(r_temp_sum / (size_vect - 1));  // maybe size_vect - 1 ???
    g_std_dev_ = sqrt(g_temp_sum / (size_vect - 1));
    b_std_dev_ = sqrt(b_temp_sum / (size_vect - 1));
}

int Nucleus::return_r_average()
{
    return this->r_average_;
}

int Nucleus::return_g_average()
{
    return this->g_average_;
}

int Nucleus::return_b_average()
{
    return this->b_average_;
}

int Nucleus::return_r_range()
{
    return this->r_range_;
}

int Nucleus::return_g_range()
{
    return this->g_range_;
}

int Nucleus::return_b_range()
{
    return this->b_range_;
}

void Nucleus::reset_all()
{
    this->green_border_ = 0;
    this->red_border_ = 0;
    this->black_body_ = 0;
    this->perim_factor_ = 0.41421;
    this->perimeter_ = 0.0;
    this->square_ = 0;
    this->form_factor_ = 0.0;
}
