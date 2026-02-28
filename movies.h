#ifndef MOVIES_H
#define MOVIES_H

#include <string>

using namespace std;

class movies {
    public:
        movies(string input_movie_name, double input_movie_rating);
        string get_movie_name();
        double get_movie_rating();
    private:
        string movie_name;
        double movie_rating;
};

#endif