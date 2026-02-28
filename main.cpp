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
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // strip Windows-style \r from CRLF line endings
        }
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
                if(!found_movie || current_movie.get_movie_rating() > current_highest.get_movie_rating() || (current_movie.get_movie_rating() == current_highest.get_movie_rating() && current_movie.get_movie_name() < current_highest.get_movie_name())){
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
            if(!matching_movies_for_prefix.empty()){
                    cout << endl;
            }

            if(number_of_movies_with_prefix == 0){
                cout << "No movies found with prefix "<< prefixes[i] << endl;
            }

            if(found_movie) {
                highest.push_back(make_pair(current_highest, prefixes[i]));
            }
        }
    

    
    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    for(int i = 0; i < highest.size(); i ++){
        cout << "Best movie with prefix " << highest[i].second << " is: " << highest[i].first.get_movie_name() << " with rating " << std::fixed << std::setprecision(1) << highest[i].first.get_movie_rating()<< endl;
    }
    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.

    return 0;
}

/*
 * Part 3: Time Complexity Analysis
 *
 * The parameters that affect how long my algorithm takes to run are:
 *   n = total number of movies
 *   m = total number of prefixes we're searching for
 *   k = how many movies actually match a given prefix (at most n)
 *   l = the length of the prefix
 * Data structure I used:
 *   I used an unordered_map where the key is the first letter of a movie's
 *   title, and the value is a vector of all movies starting with
 *   that letter. So I split all the movies into 26 buckets,
 *   one for each letter of the alphabet.
 *
 * Loading the movies :
 *   For each of the n movies, I just grab the first letter and throw
 *   the movie into the right bucket. Each of those steps is O(1). Thus,
 *   so loading all n movies is  O(n) .
 *
 *   For each prefix, the m parameter I do two things:
 *
 *   1. Scan the right bucket:I look at the first letter of the prefix, grab that bucket,
 *      and go through every movie in it one by one to check if the
 *      movie's name starts with the prefix. In the worst case, all
 *      n movies are in one bucket, so this scan is O(n * l) per prefix
 *      L is because checking if a string starts with the prefix
 *      takes time proportional to the length of the prefix.
 *   2. Sort the matching movies: 
 *      Once I have all the movies that match, I sort them by rating
 *      and alphabetically for ties. If k movies matched, sorting
 *      takes O(k * log(k) * l). In the worst case k = n, so this
 *      is O(n * log(n) * l).
 *
 *   The sort step is slower than the scan step, so the total cost
 *   per prefix is O(n * log(n) * l).
 *
 *   Since I do this for all m prefixes, the total running time is:
 *   O(m * n * log(n) * l)
 *   since l is a constant: O(m * n * log n)
 *
 * Measured runtimes using prefix_large.txt:
 *
 *   input_20_random.csv    (n =     20) -->   ~12 ms
 *   input_100_random.csv   (n =    100) -->   ~17 ms
 *   input_1000_random.csv  (n =  1,000) -->   ~59 ms
 *   input_76920_random.csv (n = 76,920) --> ~3,958 ms
 *
 * 
 *   Looking at the jump from n=1,000 to n=76,920, the runtime went
 *   from ~59 ms to ~3,958 ms, which is about 67x slower(haha). The n*log(n)
 *   factor predicts roughly a 265x increase, so the measured growth is
 *   in the right ballpark . The runtimes
 *   do clearly grow faster than linearly as n increases, which matches
 *   the O(m * n * log n) prediction.
 */

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}