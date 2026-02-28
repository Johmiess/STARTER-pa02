#include "movies.h"

movies::movies(string input_movie_name, double input_movie_rating){
    movie_name = input_movie_name;
    movie_rating = input_movie_rating;
}

string movies::get_movie_name(){
    return movie_name;
}

double movies::get_movie_rating(){
    return movie_rating;
}