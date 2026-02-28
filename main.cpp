// Winter'24
// Instructor: Diba Mirza
// Student name: 
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <string>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

bool compare_movie_ratings_then_alphabet(movies& movie1, movies& movie2){
    if(movie1.get_movie_rating() != movie2.get_movie_rating()){
        return movie1.get_movie_rating() > movie2.get_movie_rating();
    }
    return movie1.get_movie_name() < movie2.get_movie_name();
}

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    struct compare_movie_alphabet_order {
        string alpahbet = "abcdefghijklmnopqrstuvwxyz";
        bool operator()(movies& movie1, movies& movie2){
            string title_of_movie1 = movie1.get_movie_name();
            string title_of_movie2 = movie2.get_movie_name();
            return title_of_movie1 > title_of_movie2;
        }
    };
    // Create an object of a STL data-structure to store all the movies
    priority_queue<movies, vector<movies>, compare_movie_alphabet_order> alphabetical_movie_list;
    unordered_map<string, vector<movies>> prefix_based_list;
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            movies current_movie_in_line(movieName, movieRating);
            if(argc == 2){
                alphabetical_movie_list.push(current_movie_in_line);
            }
            if(argc == 3){
                string first_prefix = current_movie_in_line.get_movie_name().substr(0,1);
                prefix_based_list[first_prefix].push_back(current_movie_in_line);
            }
            
    }

    movieFile.close();

    if (argc == 2){
            while(!(alphabetical_movie_list.empty())){
                movies top = alphabetical_movie_list.top();
                string top_movie_name = top.get_movie_name();
                double top_movie_score = top.get_movie_rating();
                cout << top_movie_name << ", " << top_movie_score << endl;
                alphabetical_movie_list.pop();
            }
            return 0;
    }

    ifstream prefixFile (argv[2]);



    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    vector<pair<movies, string>> highest;
    for(int i = 0; i < prefixes.size(); i ++){
        string first_letter_of_prefix = prefixes[i].substr(0,1);
        int number_of_movies_with_prefix = 0;
        bool found_movie = false;
        movies current_highest("", -1.0); 
        vector<movies> matching_movies_for_prefix;
        for(int j = 0; j < prefix_based_list[first_letter_of_prefix].size(); j ++){
            movies& current_movie = prefix_based_list[first_letter_of_prefix][j];
            string name_of_current_movie = current_movie.get_movie_name();
            if(prefixes[i] == name_of_current_movie.substr(0,prefixes[i].size())){
                matching_movies_for_prefix.push_back(current_movie);
                if(!found_movie || current_movie.get_movie_rating() > current_highest.get_movie_rating() || (current_movie.get_movie_rating() == current_highest.get_movie_rating() && current_movie.get_movie_name() > current_highest.get_movie_name())){
                    current_highest = current_movie;  
                    found_movie = true;
                }
                number_of_movies_with_prefix += 1;
                double movie_score = prefix_based_list[first_letter_of_prefix][j].get_movie_rating();
            }
        }
            sort(matching_movies_for_prefix.begin(),matching_movies_for_prefix.end(),compare_movie_ratings_then_alphabet);

            for(movies&m : matching_movies_for_prefix){
                    cout << m.get_movie_name() << ", " << m.get_movie_rating() << endl;
            }

            if(number_of_movies_with_prefix == 0){
                cout << "No movies found with prefix "<< prefixes[i] << endl;
            }

            if(found_movie) {
                highest.push_back(make_pair(current_highest, prefixes[i]));
            }
        }
    

    
    cout << "\n";
    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    for(int i = 0; i < highest.size(); i ++){
        cout << "Best movie with prefix " << highest[i].second << " is: " << highest[i].first.get_movie_name() << " with rating " << std::fixed << std::setprecision(1) << highest[i].first.get_movie_rating()<< endl;
    }
    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.

    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}